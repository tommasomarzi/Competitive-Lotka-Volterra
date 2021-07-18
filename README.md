# **Competitive Lotka-Volterra**
The aim of this project is to calibrate an Agent-Based model representing a competitive N-species Lotka-Volterra model with its numerical solution (performed with a Runge-Kutta 4 algorithm).

Jump to a section:
* [Theoretical model](#The-theoretical-model)
* [Agent-Based model](#The-Agent-Based-model)
* [Structure of the code](#Structure-of-the-code)
* [Requirements](#Requirements)
* [Usage](#Usage)


## The theoretical model
For more references on the model check the [Wikipedia page](https://en.wikipedia.org/wiki/Competitive_Lotka%E2%80%93Volterra_equations#N_species).


## The Agent-Based model
The graphical part of the Agent-Based model is made with [OpenGL](https://www.opengl.org//). The rules are the following:

1. at first the grid is initialized in a probabilistic way: the probability to have an individual in a cell is given by the Hadamard (pointwise) product between the vector of the initial values and vector of the capacities divided by the total number of cells.  
2. then for each iteration we pick a cell:
    - if it is empty, an individual of a certain species is created with a probability given by the product between the rates and the number of individuals of each species in the eight adjacent cells (if an adjacent cell is empty the rate is one and it leads to an unoccupied cell) 
    - if it is occupied, there is competition according to the coefficients of the interaction matrix or (if it is possible) the individual moves to an adjacent empty cell.


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

