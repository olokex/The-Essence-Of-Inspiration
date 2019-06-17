/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2017/18
 * a pro dokumentaci Javadoc.
 * 
 * @file
 * @mainpage
 *
 * @author: Adam Sedlacek
 * Login: xsedla1e
 *
 * Program provadi jednoduchou shlukovou analyzu
 * Metoda pouziti: Unweighted pair-group average
 *
 * @defgroup objekty_prace Vsechny funkce na praci s objekty 
 * @defgroup pole_shluku Vsechny funkce na praci se shluky objektu
 */

/**
 * @struct obj_t
 * @brief Jednotlive objekty
 * @param id - identifikator objektu
 * @param x - Xsova souradnice objektu
 * @param y - Ypsilonova souradnice objektu
 */
struct obj_t {
    /// @param id
    int id;
    /// @param x
    float x;
    /// @param y
    float y;
};

/**
 * @struct cluster_t
 * @brief Struktura pro shlukovani objektu
 * @param size - Aktualni pocet objektu
 * @param capacity - Maximalni pocet objektu v danem struct
 * @param obj_t - Ukazatel na dany objekt
 */
struct cluster_t {
    /// @param size
    int size;
    /// @param capacity
    int capacity;
    /// @param obj
    struct obj_t *obj;
};

/******************************************************************/
/************************* PRACE S OBJEKTY ************************/
/******************************************************************/

/**
 * @ingroup objekty_prace
 * @brief Pomocna funkce na inicializaci objektu
 * @param c Inicializace shluku
 * @param cap Alokace pro CAP objektu (velikost)
 * @pre c != NULL
 * @pre cap >= 0
 * @post c->obj != NULL
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @ingroup objekty_prace 
 * @brief Odstraneni vsech objektu ve shluku a nastaveni na 0 hodnoty
 * @param c Samotny shluk v kterem maji byt odstraneny objekty
 */
void clear_cluster(struct cluster_t *c);

/**
 * @brief Doporucena velikost pro realokaci
 */
extern const int CLUSTER_CHUNK;

/**
 * @ingroup objekty_prace 
 * @brief Zmena velikosti shluku z cap na new_cap
 * @param c Shluk ktery ma byt zvetsen
 * @param new_cap parametr o kolik ma byt novy shluk mit vice mista
 * @pre c->cappacity >= 0
 * @pre new_cap >= 0
 * @pre c
 * @return c ukazatel na shluk s novou velikosti
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @ingroup objekty_prace
 * @brief Pridani objektu na konec shluku, pokud je jiz plny rozsiri shluk
 *        o cluster_chunk (vychozi 10)
 * @param c Ukazatel na shluk do ktereho se ma pridat objekt
 * @param obj Dany objekt
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @ingroup objekty_prace
 * @brief Do prvniho shluku prida vsechny objekty z druheho shluku
 *        v pripade potreby jej rozsiri a na konci seradi
 * @param c1 Ukazatel na prvni shluk
 * @param c2 Ukazatel na druhy shluk
 * @pre c1 != NULL
 * @pre c2 != NULL
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/******************************************************************/
/********************* PRACE S POLI SHLUKU ************************/
/******************************************************************/

/**
 * @ingroup pole_shluku
 * @brief Odstrani dany idx shluku a vsechny shluky posune doleva
 * @param carr Ukazatel na shluky objektu
 * @param narr Pocet shluku v poli
 * @param idx Index daneho shluku k odstraneni
 * @pre idx < narr
 * @pre narr > 0
 * @post narr < 0
 * @return narr-1 Novy pocet prvku ve shluku
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @ingroup pole_shluku
 * @brief Pocita euklidovsou vzdalenost mezi dvema objekty
 * @param o1 Pole objektu v shluku jedna
 * @param o2 Pole objektu v shluku dva
 * @pre o1 != NULL
 * @pre o2 != NULL
 * @return sqrtf(x^2 + y^2) Vysledna vzdalenost mezi objekty
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @ingroup pole_shluku
 * @brief Pocita prumernou vzdalenost mezi shluky
 * @param c1 Ukazatel na prvni shluk
 * @param c2 Ukazatel na druhy shluk
 * @pre c1 != NULL
 * @pre c1->size > 0
 * @pre c2 != NULL
 * @pre c2->size > 0
 * @return vys Soucet vsech vzdalenosti objektu deleno pocet objektu v c1 a c2
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @ingroup pole_shluku
 * @brief Funkce najde dva nejblizsi shluky
 * @param carr Ukazatel na pole shluku
 * @param narr Velikost shluku
 * @param c1 Ulozeni do pameti jejich index X (pomoci dereference)
 * @param c2 Ulozeni do pameti jejich index Y (pomoci dereference)
 * @pre narr > 0
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @ingroup objekty_prace
 * @brief Razeni objektu ve shluku podle jejich id
 * @param c Ukazatel na dany shluk
 */
void sort_cluster(struct cluster_t *c);

/**
 * @ingroup objekty_prace
 * @brief Vytiskne jednotlive objekty pod shlukem
 * @param c Ukazatel na dany shluk ktery ma byt vytisknut
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief Nacte id, x, y ze souboru 
 *        kazdemu objektu inicializuje shluk a priradi jej k nemu
 * @param filename Nazev souboru, ktery ma byt nacten
 * @param arr Ukazatel na pole shluku
 * @pre arr != NULL 
 * @post size != 0 && size > count
 * @return size Pocet nactenych prvku (prvni radek v souboru count=X)
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @ingroup pole_shluku
 * @brief Vytiskne vsechny shluky
 * @param carr Ukazatel na dany shluk
 * @param narr Tiskne danych narr shluku
 */
void print_clusters(struct cluster_t *carr, int narr);
