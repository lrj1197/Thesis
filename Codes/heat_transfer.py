import numpy as np
from scipy.integrate import odeint
import matplotlib.pyplot as plt

N = 1024  # number of points to discretize
L = 12.0
h = L / (N - 1)
z = np.linspace(0,L,N)
r = np.linspace(2.0,2.5,N)
dr = r.max() - r.min()
m = 1000.0
C_p = 1 #need sp heat cap for steal
A = 2.0*np.pi*dr**2 #need cross sectional area 
k_st = 205.0
a = dr*m*C_p/(A*k_st)

def odefunc(u, t):
    dudt = np.zeros(X.shape)

    dudt[0:0] = 0 # constant at boundary condition
    dudt[-1:-1] = 0

    # now for the internal nodes
    for i in range(1, N-1):
        for j in range(1, N-1):
            dudt[i] = 1.0/a * (T[i + 1:j] - 2*T[i:j] + T[i - 1:j] + T[i:j + 1] - 2*T[i:j] + T[i:j - 1]) / 2.0*h**2

    return dudt

T_i = 150.0 * np.ones(z.shape) # initial temperature
T_i[0:0] = T_f1  # one boundary condition
T_i[-1:0] = T_f1 # the other boundary condition
T_i[-1:-1] = 253.0  # one boundary condition
T_i[0:-1] = 253.0 # the other boundary condition



tspan = np.linspace(0.0, 30.0*60.0, 100) # run for 30 min
sol = odeint(odefunc, T_i, tspan)


# for i in range(0, len(tspan), 5):
#     plt.plot(X, sol[i], label='t={0:1.2f}'.format(tspan[i]))

# # put legend outside the figure
# plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))
# plt.xlabel('X position')
# plt.ylabel('Temperature')

# # adjust figure edges so the legend is in the figure
# plt.subplots_adjust(top=0.89, right=0.77)
# plt.savefig('pde-transient-heat-1.png')


# Make a 3d figure
from mpl_toolkits.mplot3d import Axes3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

SX, ST = np.meshgrid(r, z)
ax.plot_surface(SX, ST, sol, cmap='jet')
ax.set_xlabel('X')
ax.set_ylabel('time')
ax.set_zlabel('T')
ax.view_init(elev=15, azim=-124) # adjust view so it is easy to see
plt.savefig('images/pde-transient-heat-3d.png')

# animated solution. We will use imagemagick for this

# we save each frame as an image, and use the imagemagick convert command to 
# make an animated gif
# for i in range(len(tspan)):
#     plt.clf()
#     plt.plot(X, sol[i])
#     plt.xlabel('X')
#     plt.ylabel('T(X)')
#     plt.title('t = {0}'.format(tspan[i]))
#     plt.savefig('___t{0:03d}.png'.format(i))

# import commands
# print commands.getoutput('convert -quality 100 ___t*.png images/transient_heat.gif')
# print commands.getoutput('rm ___t*.png') #remove temp files