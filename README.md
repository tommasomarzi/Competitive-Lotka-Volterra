# **Competitive Lotka-Volterra**
The aim of this project is to calibrate an Agent-Based model representing a competitive N-species Lotka-Volterra model with its numerical solution (performed with a Runge-Kutta 4 algorithm).

Jump to a section:
* [Theoretical model](#The-theoretical-model)
* [Agent-Based model](#The-Agent-Based-model)
* [Structure of the code](#Structure-of-the-code)
* [Requirements](#Requirements)
* [Usage](#Usage)


## The theoretical model
The model describes the evolution of N species competing for the same resources.
In particular, the evolution of each species *i* normalized with respect to its own carrying capacity is describes by the following first-order nonlinear differential equation:

![equation1](https://latex.codecogs.com/gif.latex?\frac{dx_i}{dt}&space;=&space;r_i&space;x_i\left(1-\sum_{j&space;=&space;1}^N\alpha_{ij}x_j\right))

where *r<sub>i</sub>* > 0 is the growth rate of the species *i* and *&alpha;<sub>ij</sub>* >= 0 is the element of the interaction matrix *&alpha;* which quantifies the effect of the species *j* on the species *i* (conventionally *&alpha;<sub>ii</sub>* = 1).
The evolution of each species is bounded between 0 and 1 with respect to its own carrying capacity. 

For references regarding the chaotic dynamics or attractor phenomena that this model can exhibit, check the [Wikipedia page](https://en.wikipedia.org/wiki/Competitive_Lotka%E2%80%93Volterra_equations#Possible_dynamics).


## The Agent-Based model
An Agent-Based model is realized by considering a *n*x*n* grid in which the individuals move and interact according to stochastic events based on the values of the parameters of the model. The rules are the following:

1. at the beginning, each cell of the grid is initialized with an individual belonging to a species *i* with a probability:

    ![equation_start](https://latex.codecogs.com/gif.latex?p_i&space;=&space;\frac{K_i\,x_i^0}{n^2})

where *K<sub>i</sub>* is the carrying capacity of the species *i* (i.e. maximum number that the population can reach).

2. then for each iteration we pick a cell:
    - if it is empty, an individual of a certain species is created with a probability given by:

        ![equation_empty](https://latex.codecogs.com/gif.latex?p_i&space;=&space;\frac{r_i\,g_i}{Z})

    where *g<sub>i</sub>* is the occupation number in the eight-cells neighborhood and *Z* is a normalization constant (the rate associated to the empty cells *r<sub>0</sub>* is fixed to one and it leaves the cell unoccupied).

    - if it is occupied, it interact with one kind of the neighborhood cells according to the occupation number:
    
        ![equation_int](https://latex.codecogs.com/gif.latex?p_j&space;=&space;\frac{g_j}{8})

    If *j* = 0 then the individual can move to an empty cell (or it stays in the current one) with a uniform probability.
    If *j* = 1, ..., N then the individual in the considered cell dies with a probability *&alpha;<sub>ij</sub>*. Therefore, we require that *&alpha;<sub>ij</sub>* <= 1.

The simulation evolves up to the chosen number of iterations.

The graphical part of the Agent-Based model is made with [OpenGL](https://www.opengl.org//).


## Structure of the code:
The code is structured in the following folders:
- [Numerical simulation](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/numerical_simulation): in this folder the numerical solution of the competitive Lotka-Volterra model is computed through the RK4 algorithm. In particular, main_ns.cpp handles the main loop and it calls the algorithm and the model that are present in lotka_volterra.cpp/h files.
- [Agent-based model](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/agent_based_model): in this folder three blocks of files are present, i.e. main_abm.cpp, which handles the glutMainLoop (if the graphical part is enabled), graphics.cpp/h in which the rules concerning the graphical part are defined, and LV.cpp/h, in which the dynamics of the agent-based model is specified.
- [Testing](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/testing): in this folder (still under development) the main functions of the program are tested.
- [Utilites](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/utilities): this folder contains several files that are required both for the numerical simulation and the agent-based model. In particular, we distinguish the reader.cpp/h files, in which the functions to read the data are defined, plotter.py, which handles the visualization of the trends, and utilities.h, which contains the parameter for the simulation.
- [Data](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/data): this folder contains some examples of data regarding the dynamics of the model (growth rates, initial conditions, interaction matrix and carrying capacities) with different number of species.
- [Output](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/output): this folder will contain the plots if SAVE_PLOT in utilies.h is set to true.


## Requirements 
### C++
The C++ version required is the C++11 standard.

In order to install the [freeGLUT library](http://freeglut.sourceforge.net/), type:
```bash
sudo apt-get install freeglut3 freeglut3-dev
```
and for the [GNU binutils](https://www.gnu.org/software/binutils/) tools:
```bash
sudo apt-get install binutils-gold
```

### Python
For the python script the following libraries are required:

- numpy
- matplotlib 

If they are not already installed, type:
```bash
pip install numpy matplotlib
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
Once the configuration in the [setup.h](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/utilities/setup.h) file has been chosen, in order to build and run the simulation type (the flag '-s' silences the output of make, but it is not strictly necessary):
```bash
make model -s
```

### Testing
The testing is performed with the [Catch2](https://github.com/catchorg/Catch2/tree/v2.x) framework: you do not have to install anything since it is a [single-header file](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/testing/catch.hpp) that is already present in the [testing folder](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/tree/master/testing) to avoid path conflicts.
The testing can be done by typing (the flag '-s' silences the output of make, but it is not strictly necessary):
```bash
make test -s
```

