import numpy as np



import datetime as dt
#test number
test_id = 2

#Take in R from AB9, convert to Temp in C
def T(R):
	R = R - 1.96 #offset due to longer cables and poor connections
	a = -1.3661685
	b = 1.93908956501
	c = 0.625701423489
	return a + b*np.exp(1000.0*c/R) - 273.15 - 20
def TvP(P):
    a = -1.66226818
    b = 34.90949649
    c = -192.76644538
    return (b/(a-np.log(P)) - c)
def main_1():
    #open file and make header
    filename = "/Users/lucas/Documents/Thesis/Data/Test_{}_Coldfinger_1_2.txt".format(test_id)
    print("Writing file to Test_{}_Coldfinger_1_2.txt".format(test_id))
    file = open(filename,'w')
    file.write('Time\tTemperature (C)\tResistance (Ohms)\t time difference (s)\n')
    print("time, Temp (K), Resistance (Ohms)")
    #start time of DAQ
    t0 = dt.datetime.now()
    while True:
        cmd = input("Enter Resistance or q to exit:\n")
        try:
            R = float(cmd)
            temp  = T(R)
            time = dt.datetime.now()
            print(time-t0, temp, R)
            file.write(str(time) + '\t')
            file.write(str(temp) + '\t')
            file.write(str(R) + '\t')
            file.write(str(time-t0) + '\n')
        except ValueError:
            if cmd == 'q':
                print('Closing file...')
                file.close()
                print('Exiting...')
                break
            else:
                pass
def main_2():
    #open file and make header
    filename = "/Users/lucas/Documents/Thesis/Data/Test_{}_Coldfinger_1_2.txt".format(test_id)
    print("Writing file to Test_{}_Coldfinger_1_2.txt".format(test_id))
    file = open(filename,'w')
    file.write('Time\tTemperature (C)\tVapor Pressure (mTorr)\t time difference (s)\n')
    print("time, Temp (K), Pressure (Torr)")
    #start time of DAQ
    t0 = dt.datetime.now()
    while True:
        cmd = input("Enter Pressure or q to exit:\n")
        try:
            P = float(cmd)
            temp  = TvP(P)
            time = dt.datetime.now()

            print(time-t0, temp, P)
            file.write(str(time) + '\t')
            file.write(str(temp) + '\t')
            file.write(str(P) + '\t')
            file.write(str(time-t0) + '\n')
        except ValueError:
            if cmd == 'q':
                print('Closing file...')
                file.close()
                print('Exiting...')
                break
            else:
                pass
if __name__ == "__main__":
    print("If using vapor pressure, Enter v, Otherwise Enter r")
    control = input()
    while True:
        if control == 'v':
            main_2()
            break
        elif control == 'r':
            main_1()
            break
        else:
            print("Invalid, Try again.")
            control = input()











#2-5-19
#-> saw rapid change at around 1/2 submerged, 15 min in.

# ID_NUM += 1
# id = open("id.txt", 'a')
# id.write(str(ID_NUM))
# id.close()

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
