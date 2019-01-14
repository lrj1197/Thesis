'''
Flow rate of LN2 at 1 atm, exposed to room temp.
'''
m_i = 166.48 #g
m_f = 60.04 #85.09 #g
t_i = 0.00 #s
t_f = 52*60.0 + 46.0 #34.22 #s
rho = 0.807 #g/mL
def dVdt(m_i,m_f, t_i, t_f, rho):
	print((1.0/rho) * (m_f - m_i)/(t_f - t_i)) #mL/s
print('Rate:')
dVdt(m_i,m_f, t_i, t_f, rho)

'''
Flow rate of LN2 at 1 atm when in contact with room temp body
'''
m_i = 113.00 #g
m_f = 109.00 #g
t_i = 50.0 #s
t_f = 1*60.0 + 10.0 #s
rho = 0.807 #g/mL
def dVdt(m_i,m_f, t_i, t_f, rho):
	print((1.0/rho) * (m_f - m_i)/(t_f - t_i)) #mL/s
print('Rate when in contact with warm body:')
dVdt(m_i,m_f, t_i, t_f, rho)

'''
The mass of one bead of NH3 approx
817 kg/cu m rho_NH3 @ -80 C
1 cu m = 1e9 cu mm
1kg = 1e3 g
size of bead is ~2 cu mm 
'''
rho_NH3 = 817 * 10**(-6) #g/cu mm
mass_of_bead = rho_NH3 * 2.0 #g
print("mass of 1 NH3 bead:", mass_of_bead)







