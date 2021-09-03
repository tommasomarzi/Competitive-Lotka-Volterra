# Usage of setup.h
This file aims to explain each parameter of the [setup.h](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/utilities/setup.h) file.

## Warning
Since at the moment I have not found a formal way to convert the parameters defined in the header file into a format usable by python, the conversion into a dictionary is done in the lines 6-13 of [plotter.py](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/utilities/plotter.py). 

As one can see, this method is based on regex matching. 
Therefore, it is required not to overturn the original structure of the file in order to avoid possible conflicts with extra characters or punctuation and compromise the correct operation of the program.

## Explanation of the parameters
The parameters of the [setup.h](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/utilities/setup.h) file are the following:
*   **ITER_MAX**: number of iterations of the models.
*   **h_increment**: time increment used in the Runge-Kutta 4 algorithm to solve numerically the system of equations.
*   **LOCAL_ITER**: number of iterations of the agent-based model corresponding to one iteration in the numerical simulation.
*   **ENABLE_BIRTHS**: as the README.md explains, if it is set to true, when interacting with an empty cell the individual can generate another individual; otherwise, it can only move to that cell.
*   **n_rows**: number of rows of the grid in the agent-based model.
*   **n_cols**: number of columns of the grid in the agent-based model.
*   **w_width**: width in pixels of the window which displays the graphical part of the agent-based model.
*   **w_height**: height in pixels of the window which displays the graphical part of the agent-based model.
*   **w_right, w_left, w_up, w_down**: corners of the orthographic viewing region (**not recommended to change**).
*   **fps**: frame per second, i.e. the time (measured in *s<sup>-1</sup>*) to refresh and update the window of the graphical part.
*   **folder**: name of the folder in the [data folder](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/data) which contains the required parameters (rates.txt, values.txt, matrix.txt and capacity.txt) and that will contain the output files (output_ns.txt and output_abm.txt).
*   **N_ABM_TEST**: number of times that the agent-based model methods are tested.
*   **N_NS_TEST**: number of iterations in the testing of the numerical simulation algorithm.
*   **DEBUG_MODE**: if it is true, it will fix the seed of the random number generator to a constant value.
*   **ENABLE_GRAPHICS**: if it is true, it will display the evolution of the agent-based model.
*   **ENABLE_OUTPUT**: if it is true, it will compute and save the output of the evolution of the species in the models.
*   **ENABLE_PLOT**: if it is true, it executes the python script to plot the results.
*   **ENABLE_COMPARISON**: if it is true, the python script (if it is executed) will plot the trend of the numerical solution and of the agent-based model with respect to the iterations.
*   **ENABLE_PLOT_NS**: if it is true, in the case of two, three or four species the python script (if it is executed) will plot the results of the numerical simulation. In particular, according to the dimensionality we can have x<sub>1</sub> vs x<sub>2</sub> (two species), x<sub>1</sub> vs x<sub>2</sub> vs x<sub>3</sub> (three species) or x<sub>1</sub> vs x<sub>2</sub> vs x<sub>3</sub> vs x<sub>4</sub> (four species).
*   **ENABLE_PLOT_ABM**: if it is true, in the case of two, three or four species the python script (if it is executed) will plot the results of the agent-based model. In particular, according to the dimensionality we can have x<sub>1</sub> vs x<sub>2</sub> (two species), x<sub>1</sub> vs x<sub>2</sub> vs x<sub>3</sub> (three species) or x<sub>1</sub> vs x<sub>2</sub> vs x<sub>3</sub> vs x<sub>4</sub> (four species).
*   **SAVE_PLOT**: if it is true, the python script will save the plot(s) in output/figure_comparison_#-of-species_species.png (for the plot in ENABLE_COMPARISON), output/figure_ns_#dim.png (for ENABLE_PLOT_NS) or output/figure_abm_#dim.png (for ENABLE_PLOT_AMB). If it is false, the plot(s) will be shown on the screen. 