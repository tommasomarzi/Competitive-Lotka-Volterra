import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

print("py start")
path = "../data/3-species_1/output.txt"
path= "../data/4-species-roques/output.txt"
path = "../output/output_abm.txt"
file_data = np.loadtxt(path) #usecols=(0,1),skiprows=1)
x = file_data[:,0]
t = np.arange(0, len(x), 1)

plt.figure()
nspecie = 0
for column in file_data.T:
    nspecie += 1
    plt.plot(t,column,label = 'specie ' + str(nspecie))

#plt.legend()
plt.savefig('../abm/figure.png')
print(nspecie)
if(nspecie > 3):
    fig1 = plt.figure()
    ax1 = fig1.gca(projection="3d")
    img = ax1.scatter(file_data[:, 1], file_data[:, 0], file_data[:,2], s = 2, c=file_data[:, 3], cmap=plt.viridis())
    fig1.colorbar(img)
    #ax1.set_xlim(0.8, 0) 
    #ax1.set_zlim(0.4, 0) 
    ax1.view_init(10,40)
    plt.savefig("../abm/fig1.png")

if True:
    fig = plt.figure()
    ax = fig.gca(projection="3d")
    ax.plot(file_data[:, 2], file_data[:, 1], file_data[:, 0])
    plt.draw()
    plt.savefig("../abm/fig21_a.png")

if True:
    fig = plt.figure()
    #ax = fig.gca(projection="3d")
    plt.plot(file_data[:, 0], file_data[:, 1])
    plt.draw()
    plt.savefig("../abm/fig2a.png")