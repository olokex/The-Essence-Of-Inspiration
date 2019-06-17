#!/usr/bin/env python3

class Log():
	"""Comment"""
	def __init__(self, filename):
		"""Comment"""
		self.output=open(filename, "w")

	def __enter__(self):
		"""Comment"""
		self.output.write("Begin\n")
		return self

	def __exit__(self, exc_type, exc_val, exc_tb):
		"""Comment"""
		self.output.write("End\n")
		self.output.close()

	def logging(self, log):
		"""Comment"""
		self.output.write(log + "\n")

class TooManyCallsError(Exception):
	"""Comment"""
	def __init__(self, msg):
		"""Comment"""
		super(TooManyCallsError, self).__init__(msg)

def limit_calls(max_calls = 2, error_message_tail = "called too often"):
	"""Comment"""
	def decoration(fun):
		"""Comment"""
		fun.prom = 0
		def name_for_my_function(*args, **kwargs):
			"""Comment"""
			fun.prom += 1
			if fun.prom > max_calls:
				raise TooManyCallsError("function " + '"' + fun.__name__ + '"' + " - " + error_message_tail)
			return fun(*args, **kwargs)
		return name_for_my_function
	return decoration


def ordered_merge(*args, **kwargs):
	"""Comment"""
	ret = list()
	arg = list()

	if kwargs.__contains__("selector"):
		selector = kwargs["selector"]
	else:
		return ret

	for i in args:
		arg.append(list(i))

	for index in selector:
		ret.append(arg[index][0])
		arg[index].pop(0)

	return ret

if __name__ == "__main__":
	"""Comment"""
	import math

	print(list(ordered_merge('abcde', [1, 2, 3], (3.0, 3.14, 3.141), range(11, 44, 11), selector = [2,3,0,1,3,1])))

	@limit_calls(1, 'that is too much')
	def pyth(a,b):
		c = math.sqrt(a**2 + b ** 2)
		return c
		
	print(pyth(3,4))
	print(pyth(6,8))
	
	with Log('mylog.txt') as logfile:
		logfile.logging('Test1')
		logfile.logging('Test2')
		a = 1/0
		logfile.logging('Test3')