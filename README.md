# Competitive Lotka-Volterra 
The aim of this project is to calibrate an Agent-Based model representing a competitive N-species Lotka-Volterra model with its numerical solution (performed with a [Runge-Kutta 4 algorithm](https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods#The_Runge%E2%80%93Kutta_method)).

Jump to a section:
*   [Theoretical model](#The-theoretical-model)
*   [Agent-Based model](#The-Agent-Based-model)
*   [Structure of the code](#Structure-of-the-code)
*   [Requirements](#Requirements)
*   [Usage](#Usage)
*   [Documentation](#Documentation)

## The theoretical model
The model describes the evolution of N species competing for the same resources.
In particular, the evolution of each species *i* normalized with respect to its own carrying capacity is described by the following first-order nonlinear differential equation:

<p>
<CENTER>
<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\bg_white&space;\frac{dx_i}{dt}&space;=&space;r_ix_i\left&space;(&space;1-\sum_{j=1}^{N}\alpha_{ij}x_j&space;\right&space;)" title="\bg_white \frac{dx_i}{dt} = r_ix_i\left ( 1-\sum_{j=1}^{N}\alpha_{ij}x_j \right )" />
</CENTER>
</p>

where *r<sub>i</sub>* > 0 is the growth rate of the species *i* and *&alpha;<sub>ij</sub>* >= 0 is the element of the interaction matrix *&alpha;* which quantifies the effect of the species *j* on the species *i* (conventionally *&alpha;<sub>ii</sub>* = 1).
The evolution of each species is bounded between 0 and 1 with respect to its own carrying capacity.

For references regarding the chaotic dynamics or attractor phenomena that this model can exhibit, check the [Wikipedia page](https://en.wikipedia.org/wiki/Competitive_Lotka%E2%80%93Volterra_equations#Possible_dynamics).

## The Agent-Based model
An Agent-Based model is realized by considering a *n*x*n* grid in which the individuals move and interact according to stochastic events based on the values of the parameters of the model. The rules are the following:

1.  at the beginning, each cell of the grid is initialized with an individual belonging to a species *i* with a probability:

    <p>
    <CENTER>
    <img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\bg_white&space;p_i&space;=&space;\frac{K_ix_i^0}{n^2}" title="\bg_white p_i = \frac{K_i\,x_i^0}{n^2}" />
    </CENTER>
    </p>

where *K<sub>i</sub>* is the carrying capacity of the species *i* (i.e. maximum number that the population can reach).

2.  then for each iteration we pick a cell:
    *   if it is empty, an individual of a certain species is created with a probability given by:

        <p>
        <CENTER>
        <img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\bg_white&space;p_i&space;=&space;\frac{r_ig_i}{Z}" title="\bg_white p_i = \frac{r_ig_i}{Z}" />
        </CENTER>
        </p>

    where *g<sub>i</sub>* is the occupation number in the eight-cells neighborhood and *Z* is a normalization constant (the rate associated to the empty cells *r<sub>0</sub>* is fixed to one and it leaves the cell unoccupied).

    *   if it is occupied, it interact with one kind of the neighborhood cells according to the occupation number:
    
        <p>
        <CENTER>
        <img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\bg_white&space;p_j&space;=&space;\frac{g_j}{8}" title="\bg_white p_j = \frac{g_j}{8}" />
        </CENTER>
        </p>

    If *j* = 0 then the individual can move to an empty cell (or it stays in the current one) with a uniform probability.
    If *j* = 1, ..., N then the individual in the considered cell dies with a probability *&alpha;<sub>ij</sub>*. Therefore, we require that *&alpha;<sub>ij</sub>* <= 1.

The simulation evolves up to the chosen number of iterations (check the [setup_usage](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/docs/setup_usage.md) file).

## Structure of the code
The code is structured in the following folders:
*   [Numerical simulation](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/numerical_simulation): in this folder the numerical solution of the competitive Lotka-Volterra model is computed through the RK4 algorithm. In particular, main_ns.cpp handles the main loop and it calls the algorithm and the model that are present in lotka_volterra.cpp/h files.
*   [Agent-based model](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/agent_based_model): in this folder three blocks of files are present, i.e. main_abm.cpp, which handles the glutMainLoop (if the graphical part is enabled), graphics.cpp/h in which the rules concerning the graphical part are defined, and LV.cpp/h, in which the dynamics of the agent-based model is specified.
*   [Testing](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/testing): in this folder the main functions of the program are tested.
*   [Utilites](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/utilities): this folder contains several files that are required both for the numerical simulation and the agent-based model. In particular, we distinguish the reader.cpp/h files, in which the functions to read the data are defined, plotter.py, which handles the visualization of the trends, and utilities.h, which contains the parameter for the simulation.
*   [Data](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/data): this folder contains some examples of data regarding the dynamics of the model (growth rates in rates.txt, initial conditions in values.txt, interaction matrix in matrix.txt and carrying capacities in capacity.txt) with different number of species.
*   [Output](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/output): this folder will contain the plots if SAVE_PLOT in utilies.h is set to true.

## Requirements
### C++
The C++ version required is the C++11 standard.

The graphical part of the agent-based model is made with [OpenGL](https://www.opengl.org//) using the [FreeGlut library](http://freeglut.sourceforge.net/). 
To install it, type (Ubuntu or Debian Linux):
```bash
sudo apt-get install freeglut3 freeglut3-dev
```
To install the [GNU binutils](https://www.gnu.org/software/binutils/) tools, type:
```bash
sudo apt-get install binutils-gold
```

### Python
At the moment to run the program the python3 version is required.
For the python script the following libraries are required:

*   numpy
*   matplotlib 

If they are not already installed, type:
```bash
pip3 install numpy matplotlib
```

## Usage
Clone the repository:
```bash
git clone https://github.com/tommasomarzi/Competitive-Lotka-Volterra.git
```
Then type:
```bash
cd Competitive-Lotka-Volterra 
```

### Run the model
Once the configuration in the [setup.h](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/utilities/setup.h) file has been chosen (**please refer to the [setup_usage](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/docs/setup_usage.md) file**), in order to build and run the simulation type (the flag '-s' silences the output of make, but it is not strictly necessary):
```bash
make model -s
```
Once the simulation has been performed, the output files of the two models can be found in the folder with the parameters chosen in setup.h. If you want to run the plots directly from those files, type:
```bash
/usr/bin/python3 utilities/plotter.py
```

### Testing
The testing is performed with the [Catch2](https://github.com/catchorg/Catch2/tree/v2.x) framework: you do not have to install anything since it is a [single-header file](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/testing/catch.hpp) that is already present in the [testing folder](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/testing) to avoid path conflicts.
The testing can be done by typing (the flag '-s' silences the output of make, but it is not strictly necessary):
```bash
make test -s
```

## Documentation
The documentation can be realized with [doxygen](https://www.doxygen.nl/index.html). 
As [this issue](https://github.com/doxygen/doxygen/issues/4851) explains, in the doxygen versions before the 1.8.3 there are problems with the TOC page which lead to an incorrect hierarchy of headers.
Therefore you should install one of the latest versions of doxygen by following this [guide](https://www.doxygen.nl/manual/install.html).
However, since these problems are layout problems and they do not change the content of the documentation, you can install the version 1.8.17 by simply typing (Ubuntu or Debian Linux):
```bash
sudo apt-get install doxygen
```
which will also avoid problems related to possible missing GNU tools as flex, bison and libiconv.
In this case, please remember that the TOC page will not be displayed correctly.

In order to create the html documentation file, from the Competitive-Lotka-Volterra folder type:
```bash
doxygen docs/doxygen/Doxyfile
```
After some processing time, a folder named html which contains a bunch of files will be created in docs/: the documentation can be viewed in the file index.html and you can open it in your browser directly from the shell by typing (in my case I'm using firefox):
```bash
firefox docs/html/index.html 
```
