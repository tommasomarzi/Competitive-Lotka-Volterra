# **Competitive Lotka-Volterra**
The aim of this project is to calibrate an Agent-Based model representing a competitive N-species Lotka-Volterra model with its numerical solution (performed with a Runge-Kutta 4 algorithm).

## The theoretical model
For more references on the model check the [Wikipedia page](https://en.wikipedia.org/wiki/Competitive_Lotka%E2%80%93Volterra_equations#N_species).

## The Agent-Based model
The graphical part of the Agent-Based model is made with [OpenGL](https://www.opengl.org//). The rules are the following:

1. at first the grid is initialized in a probabilistic way: the probability to have an individual in a cell is given by the Hadamard (pointwise) product between the vector of the initial values and vector of the capacities divided by the total number of cells.  
2. then for each iteration we pick a cell:
    - if it is empty, an individual of a certain species is created with a probability given by the product between the rates and the number of individuals of each species in the eight adjacent cells (if an adjacent cell is empty the rate is one and it leads to an unoccupied cell) 
    - if it is occupied, there is competition according to the coefficients of the interaction matrix or (if it is possible) the individual moves to an adjacent empty cell.

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
Once the configuration in the [setup.h](https://github.com/tommasomarzi/Competitive-Lotka-Volterra/blob/master/utilities/setup.h) file has been chosen, in order to build and run the simulation type:
```bash
make
```
