import sys
import netCDF4
import numpy as np

val_id = np.loadtxt('param.txt')
val_id = int(round(val_id))
vals = np.arange(0,4000,10)

E = np.loadtxt('./E' + str(vals[val_id]) +'.gz')
size = E.shape[0]
E = np.reshape(E,(size * size,))

nc = netCDF4.Dataset('./youngs_modulus_in.e','r+')

nc.variables['vals_elem_var2eb1'][0] = E
nc.variables['vals_elem_var2eb1'][1] = E


# with is like your try .. finally block in this case
with open('./cement_mechanics.i', 'r') as file:
    # read a list of lines into data
    data = file.readlines()

# now change the 2nd line, note that you have to add a newline
data[48] = '    mesh = /home/tapeter/phase_field/V2/2D_nucleation_growth_v1/Output/PL_Data/T1_512/temp1/' + str(val_id) + '/youngs_modulus_in.e \n'

# and write everything back
with open('./cement_mechanics.i', 'w') as file:
    file.writelines( data )
