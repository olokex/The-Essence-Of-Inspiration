#include "mmal.h"
#include <sys/mman.h> // mmap
#include <stdbool.h> // bool
#include <assert.h> // assert
 
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS 0x20
#endif
 
 
#ifdef NDEBUG
/**
 * The structure header encapsulates data of a single memory block.
 *   ---+------+----------------------------+---
 *      |Header|DDD not_free DDDDD...free...|
 *   ---+------+-----------------+----------+---
 *             |-- Header.asize -|
 *             |-- Header.size -------------|
 */
typedef struct header Header;
struct header {
 
    /**
     * Pointer to the next header. Cyclic list. If there is no other block,
     * points to itself.
     */
    Header *next;
 
    /// size of the block
    size_t size;
 
    /**
     * Size of block in bytes allocated for program. asize=0 means the block
     * is not used by a program.
     */
    size_t asize;
};
 
/**
 * The arena structure.
 *   /--- arena metadata
 *   |     /---- header of the first block
 *   v     v
 *   +-----+------+-----------------------------+
 *   |Arena|Header|.............................|
 *   +-----+------+-----------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
typedef struct arena Arena;
struct arena {
 
    /**
     * Pointer to the next arena. Single-linked list.
     */
    Arena *next;
 
    /// Arena size.
    size_t size;
};
 
#define PAGE_SIZE (128*1024)
 
#endif // NDEBUG
 
static Header *hdr_get_prev(Header *hdr);
static void hdr_ctor(Header *hdr, size_t size);
 
Arena *first_arena = NULL;
 
/**
 * Return size alligned to PAGE_SIZE
 */
static
size_t allign_page(size_t size)
{
    return ((size + (PAGE_SIZE)) / PAGE_SIZE)*PAGE_SIZE;
}
 
/**
 * Allocate a new arena using mmap.
 * @param req_size requested size in bytes. Should be alligned to PAGE_SIZE.
 * @return pointer to a new arena, if successfull. NULL if error.
 * @pre req_size > sizeof(Arena) + sizeof(Header)
 */
 
/**
 *   +-----+------------------------------------+
 *   |Arena|....................................|
 *   +-----+------------------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
static
Arena *arena_alloc(size_t req_size)
{
    assert(req_size > sizeof(Arena) + sizeof(Header));
    Arena *tmp = mmap(NULL, req_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (tmp == MAP_FAILED) return NULL;
    tmp->next = NULL;
    tmp->size = req_size;
    return tmp;
}
 
/**
 * Appends a new arena to the end of the arena list.
 * @param a     already allocated arena
 */
static
void arena_append(Arena *a)
{
    if (first_arena == NULL) {
        first_arena = a;
    } else {
        Arena *tmp = first_arena;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = a;
    }
}
 
/**
 * Header structure constructor (alone, not used block).
 * @param hdr       pointer to block metadata.
 * @param size      size of free block
 * @pre size > 0
 */
/**
 *   +-----+------+------------------------+----+
 *   | ... |Header|........................| ...|
 *   +-----+------+------------------------+----+
 *
 *                |-- Header.size ---------|
 */
static
void hdr_ctor(Header *hdr, size_t size)
{
    assert(size > 0);
    hdr->asize = 0;
    hdr->size = size;
    hdr->next = (Header*)(&first_arena[1]);
}
 
/**
 * Checks if the given free block should be split in two separate blocks.
 * @param hdr       header of the free block
 * @param size      requested size of data
 * @return true if the block should be split
 * @pre hdr->asize == 0
 * @pre size > 0
 */
static
bool hdr_should_split(Header *hdr, size_t size)
{
    assert(hdr->asize == 0);
    assert(size > 0);
    if (hdr->size > size + sizeof(Header)) {
        return true;
    }
    return false;
}
 
/**
 * Splits one block in two.
 * @param hdr       pointer to header of the big block
 * @param req_size  requested size of data in the (left) block.
 * @return pointer to the new (right) block header.
 * @pre   (hdr->size >= req_size + 2*sizeof(Header))
 */
/**
 * Before:        |---- hdr->size ---------|
 *
 *    -----+------+------------------------+----
 *         |Header|........................|
 *    -----+------+------------------------+----
 *            \----hdr->next---------------^
 */
/**
 * After:         |- req_size -|
 *
 *    -----+------+------------+------+----+----
 *     ... |Header|............|Header|....|
 *    -----+------+------------+------+----+----
 *             \---next--------^  \--next--^
 */
static
Header *hdr_split(Header *hdr, size_t req_size)
{
    assert((hdr->size >= req_size + 2*sizeof(Header)));
    if (hdr_should_split(hdr, req_size)) {
        char *tmp = &(((char *)hdr)[req_size + sizeof(Header)]);
        Header *new = (Header *)tmp;
        hdr_ctor(new, hdr->size - req_size - sizeof(Header));
        new->next = hdr->next;
        hdr->next = new;
        hdr->size = req_size;
        return new;
    }
    return hdr;
}
 
/**
 * Detect if two adjacent blocks could be merged.
 * @param left      left block
 * @param right     right block
 * @return true if two block are free and adjacent in the same arena.
 * @pre left->next == right
 * @pre left != right
 */
static
bool hdr_can_merge(Header *left, Header *right)
{
    assert(left->next == right);
    assert(left != right);
    char *tmp_left = &(((char *)left)[left->size]);
    if (left->next == right && left->asize == 0 &&
        right->asize == 0 && ((((Header*)tmp_left) + 1) == right)) {
        return true;
    }
    return false;
}
 
/**
 * Merge two adjacent free blocks.
 * @param left      left block
 * @param right     right block
 * @pre left->next == right
 * @pre left != right
 */
static
void hdr_merge(Header *left, Header *right)
{
    assert(left->next == right);
    assert(left != right);
    if (left == right)
        return;
    if (hdr_can_merge(left, right)) {
        left->next = right->next;
        left->size += right->size + sizeof(Header);
    }
}
 
/**
 * Finds the first free block that fits to the requested size.
 * @param size      requested size
 * @return pointer to the header of the block or NULL if no block is available.
 * @pre size > 0
 */
static
Header *first_fit(size_t size)
{
    assert(size > 0);
    Header *tmp = (Header*)(&first_arena[1]);
    if (tmp == NULL) return NULL;
    do {
        if (tmp->asize == 0 && tmp->size >= size) {
            if (hdr_should_split(tmp, size)){
                hdr_split(tmp, size);
                return tmp;
            }
        }
        tmp = tmp->next;
    } while ((tmp != (Header*)(&first_arena[1])) && (tmp != NULL));
    return NULL;
}
 
/**
 * Search the header which is the predecessor to the hdr. Note that if
 * @param hdr       successor of the search header
 * @return pointer to predecessor, hdr if there is just one header.
 * @pre first_arena != NULL
 * @post predecessor->next == hdr
 */
static
Header *hdr_get_prev(Header *hdr)
{
    assert(first_arena != NULL);
    if (hdr->next == hdr) return hdr;
    Header *tmp = hdr;
    while (tmp->next != hdr) {
        tmp = tmp->next;
    }
    return tmp;
}
 
/**
 * Allocate memory. Use first-fit search of available block.
 * @param size      requested size for program
 * @return pointer to allocated data or NULL if error or size = 0.
 */
void *mmalloc(size_t size)
{
    if (size == 0) return NULL;
    if (first_arena == NULL) {
        if ((first_arena = arena_alloc(allign_page(size))) == NULL) return NULL;
        hdr_ctor((Header*)(&first_arena[1]), (allign_page(size) - sizeof(Arena) - sizeof(Header)));
    }
    Header *tmp = first_fit(size);
    if (tmp == NULL) {
        Arena *tmp_arena = arena_alloc(allign_page(size));
        hdr_ctor((Header*)(&tmp_arena[1]), (allign_page(size) - sizeof(Arena) - sizeof(Header)));
        arena_append(tmp_arena);
 
        Header *last = hdr_get_prev((Header *)&first_arena[1]);
        last->next = (Header *)&tmp_arena[1];
        tmp = first_fit(size);
    }
    tmp->asize = size;
    return &tmp[1];
}
 
/**
 * Free memory block.
 * @param ptr       pointer to previously allocated data
 * @pre ptr != NULL
 */
void mfree(void *ptr)
{
    assert(ptr != NULL);
    if (ptr == NULL) return;
    Header *tmp = ((Header*)ptr) - 1;
    tmp->asize = 0;
    hdr_merge(tmp, tmp->next);
    hdr_merge(hdr_get_prev(tmp), tmp);
}
 
/**
 * Reallocate previously allocated block.
 * @param ptr       pointer to previously allocated data
 * @param size      a new requested size. Size can be greater, equal, or less
 * then size of previously allocated block.
 * @return pointer to reallocated space or NULL if size equals to 0.
 */
void *mrealloc(void *ptr, size_t size)
{
    if (size == 0) return NULL;
    if ((&((Header*)ptr)[-1])->size >= size) {
         (&((Header*)ptr)[-1])->asize = size;
    } else if ((&((Header*)ptr)[-1])->size < size) {
        void *tmp = mmalloc(size);
        for (size_t i = 0; i < ((Header *)ptr)->asize; i++){
            ((char *)tmp)[i] = ((char *)ptr)[i];
        }
        mfree(ptr);
        return tmp;
    }
    return ptr;
}