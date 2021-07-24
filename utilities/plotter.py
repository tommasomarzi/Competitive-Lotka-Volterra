import numpy as np
import matplotlib.pyplot as plt
import re
from mpl_toolkits.mplot3d import Axes3D

setup = {}
with open("utilities/setup.h") as setup_file:
    for line in setup_file.readlines():
        if line.startswith("#define"):
            line.rstrip()
            par = re.search('#define\s+([A-Za-z]\w+)\s+(.*)', line)
            if par:
                setup[par.group(1)] = par.group(2)

path_abm = "data/{}/output_abm.txt".format(setup['folder'].replace('"',''))
path_ns  = "data/{}/output_ns.txt".format(setup['folder'].replace('"',''))

path_to_save = "output/figure_"

#Comparison of the trends of the two models versus the iterations
def models_vs_iter():
    file_ns  = np.loadtxt(path_ns)
    file_abm = np.loadtxt(path_abm)
    iterations = np.arange(0, file_ns.shape[0], 1)
    plt.figure(figsize=(8, 6))

    if file_ns.ndim == 1:
        n_species = 1
        plt.plot(iterations, file_abm, label = 'agent-based model')
        plt.plot(iterations, file_ns,  label = 'numerical simulation')
    else:
        n_species = file_ns.shape[1]
        for species in range(n_species):
            plt.plot(iterations,file_ns[:,species],  label = 'species ' + str(species + 1) + ' (ns)')
            plt.plot(iterations,file_abm[:,species], label = 'species ' + str(species + 1) + ' (abm)')
    plt.legend(fontsize = 15)
    plt.tight_layout(rect=[0, 0.03, 1, 0.93])
    plt.xlabel("Iterations", fontsize=15)
    plt.ylabel("Population/Capacity", fontsize=15)
    plt.xticks(fontsize = 14)
    plt.yticks(fontsize = 14)
    plt.ylim([0,1.3])
    plt.grid(color = 'r', linestyle = '--', alpha = 0.4)
    if setup['SAVE_PLOT'] == "true":
        plt.savefig(path_to_save  + 'comparison_' + str(n_species) + '_species.png',bbox_inches="tight")
    else:
        plt.show()


#x1 vs x2
def two_species(file_data, model):
    iterations = np.arange(0, file_data.shape[0], 1) 
    fig = plt.figure()
    plt.plot(file_data[:, 0], file_data[:, 1])
    plt.xlabel('$x_1$', fontsize = 15)
    plt.ylabel('$x_2$', fontsize = 15)
    plt.xticks(fontsize = 14)
    plt.yticks(fontsize = 14)
    plt.xlim([0,1])
    plt.ylim([0,1])
    plt.grid(color = 'r', linestyle = '--', alpha = 0.4) 
    plt.draw()
    if setup['SAVE_PLOT'] == "true":
        plt.savefig(path_to_save + model + '_2dim.png')
    else:
        plt.show() 


#x1 vs x2 vs x3
def three_species(file_data, model):
    iterations = np.arange(0, file_data.shape[0], 1)  
    fig = plt.figure()
    ax = fig.gca(projection="3d")
    ax.plot(file_data[:, 0], file_data[:, 1], file_data[:, 2])
    ax.set_xlabel('$x_1$', fontsize = 15)
    ax.set_ylabel('$x_2$', fontsize = 15)
    ax.set_zlabel('$x_3$', fontsize = 15)
    ax.zaxis.set_rotate_label(False) 
    ax.set_xlim([0,1])
    ax.set_ylim([0,1])
    ax.set_zlim([0,1])
    if setup['SAVE_PLOT'] == "true":
        plt.savefig(path_to_save + model + '_3dim.png')
    else:
        plt.show() 


#x1 vs x2 vs x3 vs x4
def four_species(file_data, model):
    iterations = np.arange(0, file_data.shape[0], 1)  
    fig = plt.figure()
    ax = fig.gca(projection="3d")
    img = ax.scatter(file_data[:, 1], file_data[:, 0], file_data[:,2], s = 2, c=file_data[:, 3], vmin = 0, vmax = 1, cmap=plt.viridis())
    cbar = fig.colorbar(img)
    cbar.set_label(label = '$x_4$', rotation = 0, fontsize = 15) 
    ax.view_init(10,40)
    ax.set_xlabel('$x_2$', fontsize = 15)
    ax.set_ylabel('$x_1$', fontsize = 15)
    ax.set_zlabel('$x_3$', fontsize = 15)
    ax.zaxis.set_rotate_label(False) 
    ax.set_xlim([0,1])
    ax.set_ylim([0,1])
    ax.set_zlim([0,1])
    if setup['SAVE_PLOT'] == "true":
        plt.savefig(path_to_save + model + '_4dim.png')
    else:
        plt.show()  


if setup['ENABLE_COMPARISON'] == "true":
    models_vs_iter()

path_list = []

if setup['ENABLE_PLOT_NS'] == "true":
    path_list.append(path_ns)

if setup['ENABLE_PLOT_ABM'] == "true":
    path_list.append(path_abm)

if path_list:
    for path in path_list:
        if path == path_ns:
            model = "ns"
        else:
            model = "abm"

        file_data = np.loadtxt(path)

        if file_data.ndim > 1:
            n_species = file_data.shape[1]
            if n_species == 2:
                two_species(file_data, model)
            if n_species == 3:
                three_species(file_data, model)
            if n_species == 4:
                four_species(file_data, model)