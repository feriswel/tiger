import sys
import netCDF4
import numpy as np
import matplotlib
from matplotlib import pyplot as plt
import matplotlib as mpl

# font = {'family' : 'serif',
#         'size'   : 24}
# mpl.rc('font', **font)
# mpl.rc('text', usetex=True)

nc = netCDF4.Dataset('../../runs/cement_gelation/Test/cement_gelation_diff_1D_out.e','r+')
nc_const = netCDF4.Dataset('../../runs/cement_gelation/Test/cement_gelation_diff_const_1D_out.e','r+')

for ii in nc.variables:
    print ii
#
#
print nc.variables['time_whole'][-1]


# print '----'
# for ii in np.arange(1,10):
#
#     phi =  nc.variables['vals_elem_var' + str(ii) + 'eb1']
#     x = np.linspace(0,1,2048)
#
#     plt.plot(x,phi[40])
#     plt.savefig('test' + str(ii) + '.png')
#     plt.close()

phi =  nc.variables['vals_nod_var1']
phi_const =  nc_const.variables['vals_nod_var1']
x = np.linspace(0,1,2049)
dx = x[1] - x[0]

# int_energy = 0.0
# int_energy_const = 0.0
#
# for ii in np.arange(np.size(x)):
#     grad = (phi[ii] - phi[ii-1]) / dx
#     int_energy += (1./(phi[ii] * (1.0-phi[ii]))) * grad**2
#     int_energy_const += grad**2

# print int_energy, int_energy_const

plt.plot(phi[20])
plt.plot(phi[50])
# plt.plot(phi_const)
plt.savefig('testc.png')
plt.close()
