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


def log_growth(t):
    return 1/(1+ (1/0.005 -1)*np.exp(-setup['h_increment']*t*3.5))

path_abm = "data/{}/output_abm.txt".format(setup['folder'].replace('"',''))
path_ns  = "data/{}/output_ns.txt".format(setup['folder'].replace('"',''))
path_list = [path_ns, path_abm]

path_to_save = "output/figure_"

if setup['ENABLE_COMPARISON'] == "true":
    plt.figure()
    file_ns  = np.loadtxt(path_ns)
    file_abm = np.loadtxt(path_abm)
    iterations = np.arange(0, file_ns.shape[0], 1)
    
    if file_ns.ndim == 1:
        n_species = 1
        plt.plot(iterations, file_ns,  label = 'numerical simulation')
        plt.plot(iterations, file_abm, label = 'agent-based model')
        plt.plot(iterations, log_growth(iterations), label = 'exact solution', marker = 'o', markersize=0.01)
        plt.legend()
    else:
        n_species = file_ns.shape[1]
        for species in range(n_species):
            plt.plot(iterations,file_ns[:,species],  label = 'species ' + str(species) + ' (ns)')
            plt.plot(iterations,file_abm[:,species], label = 'species ' + str(species) + ' (abm)')
        plt.legend()
    plt.title("Trend of the models with " + str(n_species) + " species")
    plt.xlabel("Iterations")
    plt.ylabel("Population/Capacity")
    plt.grid(color = 'r', linestyle = '--', alpha = 0.4)
    if setup['SAVE_PLOT'] == "true":
        plt.savefig(path_to_save  + 'comparison_' + str(n_species) + '_species.png')
    else:
        plt.show()
if setup['ENABLE_MODEL'] == "true":
    for path in path_list:
        if path == path_ns:
            model = "ns"
        else:
            model = "abm"
        print("Starting the plots for the {} model".format(model))

        file_data = np.loadtxt(path) 
        iterations = np.arange(0, file_data.shape[0], 1)

    #----------ALL THE SPECIES vs ITERATIONS---------#
        plt.figure()
        if file_data.ndim == 1:
            n_species = 1
            plt.plot(iterations, file_data, label = 'species ' + str(n_species))
        else:
            n_species = file_data.shape[1]
            for species in range(n_species):
                plt.plot(iterations,file_data[:,species], label = 'species ' + str(species))
            plt.legend()
        if setup['SAVE_PLOT'] == "true":
            plt.savefig(path_to_save + model + '_' + str(n_species) + '_tot.png')
        else:
            plt.show()

    #----------FOUR SPECIES---------#
        if(n_species == 4):
            fig1 = plt.figure()
            ax1 = fig1.gca(projection="3d")
            img = ax1.scatter(file_data[:, 1], file_data[:, 0], file_data[:,2], s = 2, c=file_data[:, 3], cmap=plt.viridis())
            fig1.colorbar(img)
            #ax1.set_xlim(0.8, 0) 
            #ax1.set_zlim(0.4, 0) 
            ax1.view_init(10,40)
            if setup['SAVE_PLOT'] == "true":
                plt.savefig(path_to_save + model + str(n_species) + '_4dim.png')
            else:
                plt.show()


    #----------THREE SPECIES---------#
        if n_species == 3:
            fig = plt.figure()
            ax = fig.gca(projection="3d")
            ax.plot(file_data[:, 2], file_data[:, 1], file_data[:, 0])
            plt.draw()
            if setup['SAVE_PLOT'] == "true":
                plt.savefig(path_to_save + model + '_' + str(n_species) + '_3dim.png')
            else:
                plt.show()


    #----------TWO SPECIES---------#
        if n_species == 2:
            fig = plt.figure()
            #ax = fig.gca(projection="3d")
            plt.plot(file_data[:, 0], file_data[:, 1])
            plt.draw()
            if setup['SAVE_PLOT'] == "true":
                plt.savefig(path_to_save + model + '_' + str(n_species) + '_2dim.png')
            else:
                plt.show()