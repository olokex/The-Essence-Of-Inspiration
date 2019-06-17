#!/usr/bin/env python3

"""
	comment section
"""

def can_be_a_set_member_or_frozenset(item):
	"""
	comment section
"""
	try: # try
		if int == type(item):
			return item # return
		if list == type(item):
			return frozenset(item) # another comment
		if dict == type(item):
			return frozenset(item)
		set(item)
		return item # return of item
	except:
		return frozenset(item) # return of frozenset
"""
	comment section
"""

assert can_be_a_set_member_or_frozenset(1) == 1
assert can_be_a_set_member_or_frozenset((1,2)) == (1,2)
assert can_be_a_set_member_or_frozenset([1,2]) == frozenset([1,2])
assert can_be_a_set_member_or_frozenset({'a':1,'b':2}) == frozenset(['a','b'])

"""
	comment section
"""

def all_subsets(lst):
	"""
	comment section
"""
	sets = [[]]
	for elem in lst:
	# iterate over whole list
		for sub_set in sets:
	# add a new subset consisting of the subset at hand added elem
			sets = sets + [list(sub_set) + [elem]]
	return sets
"""
	comment section
"""

assert all_subsets(['a', 'b', 'c']) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]

"""
	comment section
"""

def all_subsets_excl_empty(*lst, exclude_empty=True):
	"""
	comment section
"""
	sets = [[]]
	for elem in lst:
	# iterate over whole list
		for sub_set in sets:
	# add a new subset consisting of the subset at hand added elem
			sets = sets + [list(sub_set) + [elem]]

	if exclude_empty:
		sets.pop(0)
	return sets

	"""
	comment section
"""

assert all_subsets_excl_empty('a', 'b', 'c') == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = True) == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = False) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]