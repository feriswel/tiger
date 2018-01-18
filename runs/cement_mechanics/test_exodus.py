import sys
import netCDF4
import numpy as np

nc = netCDF4.Dataset('./cement_mechanics_tensor_out.e')

for key, value in nc.variables.iteritems():
    print key
print '------------'
print nc.variables['vals_elem_var1eb1']
print nc.variables['vals_nod_var1']
