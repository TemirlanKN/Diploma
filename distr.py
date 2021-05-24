#!/usr/bin/env python3
import sys
from scipy.stats import norm

if len(sys.argv) == 1:
	a = float(input("м.о. заработка: "))
	sigma = float(input("дисперсия заработка: "))
	n = float(input("кол-во предприятий: "))
	total = float(input("общее кол-во денег: "))
	x = float(input("ставка: "))

elif len(sys.argv) == 2:
	with open(sys.argv[1], "r") as f:
		tmp = (f.readline()).split()
		n = float(tmp[0])
		t = float(tmp[1])
		total = float(tmp[2])
		tmp = (f.readline()).split()
		a = float(tmp[0])
		sigma = float(tmp[1])
		print("|n : ", n, "| t: ", t, "| total: ", total, "| a: ", a, " |sigma: ", sigma)
		x = float(input("Ставка: "))
x = 1.06
print("result:")
while x < a:
	tmp = norm(a, sigma)
	p = tmp.cdf(x)
	print("x = ", str(x)[:4], "| ", str(p)[:6], "сл. вел.", end="|")
	math_xi = (1 - p) / p
	if (math_xi > t):
		math_xi = t
	print(str(math_xi)[:4], "м.о.", end="|")
	tsum = total / t * (x - 1.05) * math_xi
	print(tsum)
	x += 0.01
