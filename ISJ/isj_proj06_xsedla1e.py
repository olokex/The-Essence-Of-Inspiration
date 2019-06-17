#!/usr/bin/env python3
import itertools
import operator
import itertools
from fractions import Fraction

def first_nonrepeating(ahoj):
	"""return first non repeating char"""
	if ahoj == '\t' or ahoj == ' ' or not(type(ahoj) == str):
		return None
	for i in range(len(ahoj)):
		novy = list(ahoj)
		novy.pop(i)
		if ahoj[i] not in novy:
			return ahoj[i]

operations = dict()
operations['+'] = operator.add
operations['-'] = operator.sub
operations['/'] = operator.truediv
operations['*'] = operator.mul

def solve(numbers, target):
	"""List ways to make target from numbers."""
	numbers = [Fraction(x) for x in numbers]
	return solve_inner(target, numbers)

def solve_inner(target, numbers):
	"""comment"""
	if len(numbers) == 1:
		if numbers[0] == target:
			yield str(target)
		return

	# combine a pair of numbers with an operation, then recurse
	for a,b in itertools.permutations(numbers, 2):
		for symbol, operation in operations.items():
			try:
				product = operation(a,b)
			except ZeroDivisionError:
				continue

			subnumbers = list(numbers)
			subnumbers.remove(a)
			subnumbers.remove(b)
			subnumbers.append(product)

			for solution in solve_inner(target, subnumbers):
				# expand product (but only once)
				yield solution.replace(str(product), "({0}{1}{2})".format(a, symbol, b), 1)

def combine4(numbers, target):
	"""comment"""
	solutions = solve(numbers, target)
	b = []
	for each in solutions:
		if not (each[1:len(each) - 1] in b):
			b.append(each[1:len(each) - 1])
	return b