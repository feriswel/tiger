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

nc = netCDF4.Dataset('../../runs/cement_gelation/Test/cement_gelation_diff_out.e','r+')
nc_const = netCDF4.Dataset('../../runs/cement_gelation/Test/cement_gelation_diff_const_out.e','r+')

for ii in nc.variables:
    print ii

phi =  nc.variables['vals_nod_var1']
time = nc.variables['time_whole']
n_steps = np.size(time)

phi_const =  nc_const.variables['vals_nod_var1']
time_const = nc_const.variables['time_whole']
n_steps_const = np.size(time_const)

dim = int(np.sqrt(np.size(phi[0])))
xx = nc.variables['coordx']
yy = nc.variables['coordy']

interface = np.zeros((n_steps,))

for ii in np.arange(n_steps, step = 1):
    print ii
    grad_bin = np.gradient(np.reshape(phi[ii]>0.5, (dim,dim)))
    interface[ii] = np.mean(np.sqrt(grad_bin[0]**2 + grad_bin[1]**2))

interface_const = np.zeros((n_steps_const,))

for ii in np.arange(n_steps_const):
    print ii
    grad_bin = np.gradient(np.reshape(phi_const[ii]>0.5, (dim,dim)))
    interface_const[ii] = np.mean(np.sqrt(grad_bin[0]**2 + grad_bin[1]**2))

plt.semilogx(time, interface)
plt.semilogx(time_const, interface_const)
plt.ylim((0,0.15))
plt.savefig( './interface.png', format = 'png')
plt.close()

# print time[-1], time_const[161]
# grad_bin = np.gradient(np.reshape(phi[-1]>0.5, (dim,dim)))
# # xx = np.reshape(xx, (dim,dim))
# fig = plt.figure(facecolor='white',figsize=(7,7))
# cax = plt.pcolormesh(
#     np.reshape(xx, (dim,dim)),
#     np.reshape(yy, (dim,dim)),
#     np.reshape(phi_const[161], (dim,dim)),
#     vmin = 0.0, vmax = 1.0, cmap=plt.get_cmap('jet'))
# ax = plt.gca()
# # plt.tight_layout()
# plt.savefig( './test_04.pdf', format = 'pdf')
# plt.close()
# #
# fig = plt.figure(facecolor='white',figsize=(7,7))
# cax = plt.pcolormesh(
#     np.reshape(xx, (dim,dim)),
#     np.reshape(yy, (dim,dim)),
#     np.reshape(phi_const[161], (dim,dim)),
#     vmin = 0.0, vmax = 1.0, cmap=plt.get_cmap('jet'))
# ax = plt.gca()
# # plt.tight_layout()
# plt.savefig( './test_const_04.pdf', format = 'pdf')
# plt.close()
