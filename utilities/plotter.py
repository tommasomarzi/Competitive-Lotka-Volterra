'''! @file plotter.py
    @brief Define the functions to plot the results of the simulation.
'''

import numpy as np
import matplotlib.pyplot as plt
import re
import logging
from mpl_toolkits.mplot3d import Axes3D


def setup_parser():
    '''
    Store the parameters of the setup.h header.

    Returns
    -------
        setup : dict
            Dictionary containing the parameters of the setup file.
    '''
    setup = {}
    with open("utilities/setup.h") as setup_file:
        next(setup_file)
        next(setup_file)
        for line in setup_file.readlines():
            if line.startswith("#define"):
                line.rstrip()
                matches = re.search('#define\s+([A-Za-z]\w+)\s+(.*\S+)', line)
                if matches:
                    setup[matches[1]] = matches[2]
    
    if setup['ENABLE_OUTPUT'] == "false":
        logging.warning('ENABLE_OUTPUT is set to false. If no output files are present in the chosen folder, an error will occur.')

    return setup


def models_vs_iter(path_ns, path_abm):
    '''
    Plot the trend of each species of the two models (y-axis) versus the iterations (x-axis).

    Parameters
    ----------
        path_ns : str
            Path containing the data of the numerical simulation.
        path_abm : str
            Path containing the data of the agent-based model.

    Returns
    -------
        fig : matplotlib.figure.Figure
            Plot of the species of the two models as a function of the iterations.
        n_species: int
            Number of species in the simulation.
    '''

    file_ns  = np.loadtxt(path_ns)
    file_abm = np.loadtxt(path_abm)
    iterations = np.arange(0, file_ns.shape[0], 1)

    cut_plot = False

    fig = plt.figure(figsize=(8, 6))
    ax = plt.subplot()

    if file_ns.ndim == 1:
        n_species = 1
        ax.plot(iterations, file_abm, label = 'agent-based model')
        ax.plot(iterations, file_ns,  label = 'numerical simulation')
    else:
        n_species = file_ns.shape[1]
        for species in range(n_species):
            ax.plot(iterations,file_ns[:,species],  label = 'species ' + str(species + 1) + ' (ns)')
            ax.plot(iterations,file_abm[:,species], label = 'species ' + str(species + 1) + ' (abm)')
            if (max(file_ns[:,species]) > 1.3 or max(file_abm[:,species]) > 1.3):
                cut_plot = True

    if cut_plot:
        logging.warning('The capacity of at least one species is not calibrated properly (i.e. the density is much greater than one).\nThe plot will be cut.')

    if n_species > 2:
        ax.legend(fontsize = 10)
    else:
        ax.legend(fontsize = 14)

    ax.set_xlabel("Iterations", fontsize=15)
    ax.set_ylabel("Population/Capacity", fontsize=15)
    ax.tick_params(axis='both', labelsize=14)
    ax.set_ylim([0,1.3])
    ax.grid(color = 'r', linestyle = '--', alpha = 0.4)

    return fig, n_species


def two_species(file_data, model):
    '''
    Two species case: plot the trend of the first species (x-axis)
    versus the trend of the second species (y-axis).
    
    Parameters
    ----------
        file_data : numpy.ndarray
            Array containing the data of the species for each iteration.
        model : str
            Model to plot. It can be ns (numerical simulation) or abm (agent-based model).

    Returns
    -------
        fig : matplotlib.figure.Figure
            Plot of the trajectory in the two-species space.
    '''

    fig = plt.figure(figsize=(8, 6))
    ax = plt.subplot()
    ax.plot(file_data[:, 0], file_data[:, 1])
    ax.set_xlabel('$x_1$', fontsize = 15)
    ax.set_ylabel('$x_2$', fontsize = 15)
    ax.tick_params(axis='both', labelsize=14)
    ax.set_xlim([0,1.1])
    ax.set_ylim([0,1.1])
    ax.grid(color = 'r', linestyle = '--', alpha = 0.4)

    return fig


def three_species(file_data, model):
    '''
    In the three species case, for each iteration plot a point in the three dimensional space where
    the x, y, z coordinates represent respectively the values of the first, second and third species
    at that iteration.

    Parameters
    ----------
        file_data : numpy.ndarray
            Array containing the data of the species for each iteration.
        model : str
            Model to plot. It can be "ns" (numerical simulation) or "abm" (agent-based model).

    Returns
    -------
        fig : matplotlib.figure.Figure
            Plot of the trajectory in the three-species space.
    '''

    fig = plt.figure(figsize=(8, 6))
    ax = fig.add_subplot(projection='3d')
    ax.plot(file_data[:, 0], file_data[:, 1], file_data[:, 2])
    ax.set_xlabel('$x_1$', fontsize = 15)
    ax.set_ylabel('$x_2$', fontsize = 15)
    ax.set_zlabel('$x_3$', fontsize = 15)
    ax.tick_params(axis='both', labelsize=14)
    ax.tick_params(axis='z', labelsize=14)
    ax.zaxis.set_rotate_label(False)
    ax.set_xlim([0,1])
    ax.set_ylim([0,1])
    ax.set_zlim([0,1])
    
    return fig


def four_species(file_data, model):
    '''
    In the three species case, for each iteration plot a point in the three dimensional space where
    the x, y, z coordinates represent respectively the values of the first, second and third species
    at that iteration, while the color represents the value of the fourth species at that iteration.

    Parameters
    ----------
        file_data : numpy.ndarray
            Array containing the data of the species for each iteration.
        model : str
            Model to plot. It can be ns (numerical simulation) or abm (agent-based model).

    Returns
    -------
        fig : matplotlib.figure.Figure
            Plot of the trajectory in the four-species space.
    '''
    
    fig = plt.figure(figsize=(8, 6))
    ax = fig.add_subplot(projection='3d')
    img = ax.scatter(file_data[:, 0], file_data[:, 1], file_data[:,2], s = 2, c=file_data[:, 3],
                     vmin = 0, vmax = 1, cmap=plt.viridis())
    cbar = fig.colorbar(img)
    cbar.set_label(label = '$x_4$', rotation = 0, fontsize = 15)
    ax.view_init(10,40)
    ax.set_xlabel('$x_1$', fontsize = 15)
    ax.set_ylabel('$x_2$', fontsize = 15)
    ax.set_zlabel('$x_3$', fontsize = 15)
    ax.zaxis.set_rotate_label(False)
    ax.set_xlim([0,1])
    ax.set_ylim([0,1])
    ax.set_zlim([0,1])
    
    return fig


def save_plot(figure, model, n_species, path = 'output/figure_'):
    '''
    Save the plot with a specific path according to the model and the type of plot.

    Parameters
    ----------
        figure : matplotlib.figure.Figure
            Plot to be saved.
        model : str
            Model to plot. It can be ns (numerical simulation) or abm (agent-based model).
        n_species : int
            Number of species in the simulation.
        path : str
            Path where to save the plot. Default is in "output/figure_".
    '''
    
    if model == "both":
        figure.savefig(path + 'comparison_' + str(n_species) + '_species.png', bbox_inches="tight")
    else:
        figure.savefig(path + model + '_' + str(n_species) + 'dim.png', bbox_inches="tight")


def plot_handler():
    '''
    Handle the creation and the saving of the plots according to the parameters in the setup.h file.
    '''

    setup = setup_parser()
    path_abm = "data/{}/output_abm.txt".format(setup['folder'].replace('"',''))
    path_ns  = "data/{}/output_ns.txt".format(setup['folder'].replace('"',''))
    
    if setup['ENABLE_COMPARISON'] == "true":
        figure, n_species = models_vs_iter(path_ns, path_abm)
        if setup['SAVE_PLOT'] == "true":
            save_plot(figure, "both", n_species)
        else:
            plt.show()
            
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
            else:
                n_species = 1
             
            if n_species == 2:
                figure = two_species(file_data, model)
            if n_species == 3:
                figure = three_species(file_data, model)
            if n_species == 4:
                figure = four_species(file_data, model)
            
            if (n_species > 1 and n_species < 5):
                if setup['SAVE_PLOT'] == "true":
                    save_plot(figure, model, n_species)
                else:
                    plt.show()
    
        if n_species > 4:
            logging.warning('A plot with more than four species is not supported.\nPlease set ENABLE_PLOT_NS and ENABLE_PLOT_ABM to false.')
        elif n_species == 1:
            logging.warning('A trajectory in the one-species space makes no sense.\nPlease set ENABLE_PLOT_NS and ENABLE_PLOT_ABM to false.')


if __name__ == "__main__":
    plot_handler()