"""comment"""
class Polynomial():
	"""comment"""
	def __init__(self, *vstup, **kwargs):
		"""comment"""	
		self.lst = []
		self.b = []
		self.dict = {'x0':0, 'x1':0, 'x2':0, 'x3':0, 'x4':0, 'x5':0, 'x6':0, 'x7':0, 'x8':0, 'x9':0}
		self.dict.update(kwargs)
		if not vstup:
			for key, value in sorted(self.dict.items()):
				self.lst.append(value)
			self.ahoj = self.lst
		elif len(vstup) > 1:
			for i in range(len(vstup)):
				self.b.append(vstup[i])
			self.ahoj = self.b
		else:
			self.ahoj = vstup[0]
		while self.lst and self.lst[-1] == 0:
			del self.lst[-1] 
	"""comment"""
	def __repr__(self):
		"""comment"""

		while self.ahoj and self.ahoj[-1] == 0:
			del self.ahoj[-1]
		if self.ahoj == []:
			return "0"
		a = ""
		for i in range(len(self.ahoj) - 1, -1, -1):
			if self.ahoj[i] < 0:
				if self.ahoj[i] != 0:
					if i > 1:
						if self.ahoj[i] == -1:
							a += " - " + "x^%d" % (i)
						else:
							a += " - " + str(abs(self.ahoj[i])) + "x^%d" % (i)
					elif i != 0 and self.ahoj[i] != 1:
						a += " - " + str(abs(self.ahoj[i])) + "x"
					else:
						a += " - " + str(abs(self.ahoj[i]))
			elif i > 1 and self.ahoj[i] != 0:
				if self.ahoj[i] == 1:
					a += " + " + "x^%d" % (i)
				else:
					a += " + " + str(abs(self.ahoj[i])) + "x^%d" % (i)
			elif self.ahoj[i] != 0 and i > 0:
				if self.ahoj[i] == 1:
					a += " + " + "x"
				else:
					a += " + " + str(abs(self.ahoj[i])) + "x"
			elif self.ahoj[i] != 0:
				a += " + " + str(abs(self.ahoj[i]))

		if a[1] == "+":
			return a[3:]
		else:
			return a[1:]
	"""comment"""
	def __eq__(self, other):
		"""comment"""
		return self.ahoj == other.ahoj
	"""comment"""
	def __add__(self, other):
		"""comment"""
		a = []
		if len(self.ahoj) > len(other.ahoj):
			a = self.ahoj[:]
			for i in range(len(other.ahoj)):
				a[i] += other.ahoj[i]
		else:
			a = other.ahoj[:]
			for i in range(len(self.ahoj)):
				a[i] += self.ahoj[i] 
		return Polynomial(a)
	"""comment"""
	def __pow__(self, number):
		"""comment"""
		vys = []
		#list pro vysledek
		mezi = []
		#list pro mezivysledky
		for _ in range(100):
			vys.append(0)
			mezi.append(0)
			#vytvoreni seznamu o 100 nulach, budou se k nemu pricitat casti nasobeni
		for i in range(len(self.ahoj)):
			vys[i] += self.ahoj[i]
		for _ in range(number-1):
			for inew in range(len(self.ahoj)):
				for iold in range(len(vys)):
					if vys[iold] != 0:
						mezi[inew + iold] += vys[iold] * self.ahoj[inew]
						#algoritmus podle pocitani jako clovek (kazdy prvek s kazdym) pro kazdou mocninu
			vys = mezi[:]
			#kopie mezivysledku do vysledneho listu
			for i in range(len(mezi)):
				mezi[i] = 0
		for i in range(len(vys) - 1,-1,-1):
			if vys[i] == 0:
				vys.pop(i)
			else:
				break
		return Polynomial(vys)
	"""comment"""
	def at_value(self, number, number2 = 0):
		"""comment"""
		sum1 = sum2 = 0
		for i in range(len(self.ahoj)):
			sum1 += self.ahoj[i] * (number ** i)
			if number2 != 0:
				sum2 += self.ahoj[i] * (number2 ** i)
		if number2 == 0:
			return sum1
		else:
			return sum2 - sum1
	"""comment"""
	def derivative(self):
		"""comment"""
		return Polynomial([self.ahoj[i] * i for i in range(1, len(self.ahoj))])
"""comment"""
def test():
	"""comment"""
	assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
	assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
	assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
	assert str(Polynomial(x2=0)) == "0"
	assert str(Polynomial(x0=0)) == "0"
	assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
	assert Polynomial(x2=0) == Polynomial(x0=0)
	assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
	assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
	pol1 = Polynomial(x2=3, x0=1)
	pol2 = Polynomial(x1=1, x3=0)
	assert str(pol1+pol2) == "3x^2 + x + 1"
	assert str(pol1+pol2) == "3x^2 + x + 1"
	assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
	assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
	pol3 = Polynomial(x0=-1,x1=1)
	assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
	assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
	assert str(Polynomial(x0=2).derivative()) == "0"
	assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
	assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
	pol4 = Polynomial(x3=2,x1=3,x0=2)
	assert str(pol4.derivative()) == "6x^2 + 3"
	assert str(pol4.derivative()) == "6x^2 + 3"
	assert Polynomial(-2,3,4,-5).at_value(0) == -2
	assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
	assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
	pol5 = Polynomial([1,0,-2])
	assert pol5.at_value(-2.4) == -10.52
	assert pol5.at_value(-2.4) == -10.52
	assert pol5.at_value(-1,3.6) == -23.92
	assert pol5.at_value(-1,3.6) == -23.92
"""comment"""
if __name__ == '__main__':
	"""comment"""
	test()