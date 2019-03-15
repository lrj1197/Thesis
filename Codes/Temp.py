# import stocker as s 
# import matplotlib.pyplot as plt
import numpy as np
#import datetime as dt
# import encryption_protocol

def T(R):
	R = R - 2.16 #offset due to longer cables and poor connections
	a = -1.3661685
	b = 1.93908956501
	c = 0.625701423489
	return a + b*np.exp(1000.0*c/R) - 273.15 - 20
# x = 135.4 == -82 C

while True:
        cmd = input("Enter Resistance or q to quit:\n")
        if cmd == "q":
                break
        else:
                y = float(cmd)
                print(T(y))




# # 	-> 86:14 MeOH:H20 gives a FP of -128 C or 145 K (not measured)
# # 	-> pure MeOH gives FP of -98 C or 175 K (not measured)
# # 	-> measured 90:10 FP of -103 C or 170 K
# # 	-> measured pure MeOH FP of -71 C or 202 K
# # 	-> FP of NH3 -77.73 C
#want R = ~ 132.8 Ohms 
#at 25 min with finger 1/2 sumbered, R = 130 and decresing, need better seal on top, key cryostat fully inside cold chamber. 


# r_86_nm = 145
# r_100_nm = 175
# r_100_m = 202
# r_90_m = 170


# print(r_100_m - r_100_nm)
# print(r_90_m - r_86_nm)
# 90200083094189252TCWTH9D


