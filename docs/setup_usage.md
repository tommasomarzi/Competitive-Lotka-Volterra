# Usage of setup.h
This file aims to explain each parameter of the [setup.h](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/utilities/setup.h) file.

## Warning
Since at the moment I have not found a formal way to convert the parameters defined in the header file into a format usable by python, the conversion into a dictionary is done in the lines 6-13 of [plotter.py](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/utilities/plotter.py). 

As one can see, this method is based on regex matching. 
Therefore, it is required not to overturn the original structure of the file in order to avoid possible conflicts with extra characters or punctuation and compromise the correct operation of the program.

## Explanation of the parameters
*   **ITER_MAX**:                         number of iterations of the models.

*   **h_increment**:                      time increment used in the Runge-Kutta 4 algorithm to solve numerically the system of equations.

*   **LOCAL_ITER**:                       number of iterations of the agent-based model corresponding to one iteration in the numerical simulation.

*   **ENABLE_BIRTHS**:                    as the README.md explains, if the considered cell is not empty and it interacts with an empty cell the individual can move according to an uniform probability. This parameter complexifies this dynamics: if ENABLE_BIRTHS is set to true, it has the opportunity to generate another individual in an empty cell with a probability:

    <p  align="center">
    <img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\bg_white&space;p_i&space;=&space;\frac{r_ig_0}{r_ig_0&space;&plus;&space;(8-g_0)}" title="\bg_white p_i = \frac{r_ig_0}{r_ig_0 + (8-g_0)}" />
    </p>
    
    If this event does not occur, the individual can move according to an uniform probability as in the standard case.

*   **n_rows**:                           number of rows of the grid in the agent-based model.

*   **n_cols**:                           number of columns of the grid in the agent-based model.

*   **w_width**:                          width in pixels of the window which displays the graphical part of the agent-based model.

*   **w_height**:                         height in pixels of the window which displays the graphical part of the agent-based model.

*   **w_right, w_left, w_up, w_down**:    corners of the orthographic viewing region (**not recommended to change**).

*   **fps**:                              frame per second, i.e. the time (measured in *s<sup>-1</sup>*) to refresh and update the window of the graphical part.

*   **folder**:                           name of the folder in the [data folder](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/data) which contains the required parameters (rates.txt, values.txt, matrix.txt and capacity.txt) and that will contain the output files (output_ns.txt and output_abm.txt).

*   **N_SIM_TEST**:                       number of times that the agent-based model methods are tested.

*   **DEBUG_MODE**:                       if it is true, it will fix the seed of the random number generator to a constant value.

*   **ENABLE_GRAPHICS**:                  if it is true, it will display the evolution of the agent-based model.

*   **ENABLE_OUTPUT**:                    if it is true, it will compute and save the output of the evolution of the species in the models.

*   **ENABLE_PLOT**:                      if it is true, it executes the python script to plot the results.

*   **ENABLE_COMPARISON**:                if it is true, the python script (if it is executed) will plot the trend of the numerical solution and of the agent-based model with respect to the iterations.

*   **ENABLE_PLOT_NS**:                   if it is true, in the case of two, three or four species the python script (if it is executed) will plot the results of the numerical simulation. In particular, according to the dimensionality we can have x<sub>1</sub> vs x<sub>2</sub> (two species), x<sub>1</sub> vs x<sub>2</sub> vs x<sub>3</sub> (three species) or x<sub>1</sub> vs x<sub>2</sub> vs x<sub>3</sub> vs x<sub>4</sub> (four species).

*   **ENABLE_PLOT_ABM**:                  if it is true, in the case of two, three or four species the python script (if it is executed) will plot the results of the agent-based model. In particular, according to the dimensionality we can have x<sub>1</sub> vs x<sub>2</sub> (two species), x<sub>1</sub> vs x<sub>2</sub> vs x<sub>3</sub> (three species) or x<sub>1</sub> vs x<sub>2</sub> vs x<sub>3</sub> vs x<sub>4</sub> (four species).

*   **SAVE_PLOT**:                      if it is true, the python script will save the plot(s) in output/figure_comparison_#-of-species_species.png (for the plot in ENABLE_COMPARISON), output/figure_ns_#dim.png (for ENABLE_PLOT_NS) or output/figure_abm_#dim.png (for ENABLE_PLOT_AMB). If it is false, the plot(s) will be shown on the screen. 