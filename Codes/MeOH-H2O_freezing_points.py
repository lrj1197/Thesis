import matplotlib.pyplot as plt
import numpy as np
#from scipy import optimize


def T(R):
	a = -1.3661685
	b = 1.93908956501
	c = 0.625701423489
	return a + b*np.exp(1000.0*c/R)  - 273.15

# def my_func(x,m,b):
# 	return m * x + b

ratio = [0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.0]
resistance = [133.76, 135.38, 136.45, 136.72, 139.75, 137.64, 134.52]
temp = []
for i in range(len(ratio)):
	temp.append(T(resistance[i]))

ratio = np.array(ratio)
temp = np.array(temp)
# coef, err = optimize.curve_fit(my_func, ratio[:4], temp[:4], (-5,1))


plt.figure()
plt.scatter(ratio,temp,s = 10)
# plt.plot(ratio[:4], my_func(ratio[:4],*coef))
plt.title("MeOH_H2O Freezing Points")
plt.xlabel("MeOH/H2O")
plt.ylabel("Temperature (C)")
plt.savefig('/Users/lucas/Documents/Thesis/Plots/MeOH_H2O_Freezing_Points.png')
plt.show()

#print(temp)



