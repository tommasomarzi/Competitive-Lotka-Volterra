# **Competitive Lotka-Volterra**
The aim of this project is to calibrate an Agent-Based model representing a competitive N-species Lotka-Volterra model with its numerical solution (performed with a Runge-Kutta 4 algorithm).

## The theoretical model
For more references on the model check the [Wikipedia page](https://en.wikipedia.org/wiki/Competitive_Lotka-Volterra_equations).

## The Agent-Based model
The graphical part of the Agent-Based model is made with OpenGL/GLUT. The rules are the following:

1. at first the grid is initialized in a probabilistic way: the probability to have an individual in a cell is given by the Hadamard (pointwise) product between the vector of the initial values and vector of the capacities divided by the total number of cells.  
2. then for each iteration we consider a cell:
    - if it is empty, an individual of a certain species is created with a probability given by the product between the rates and the number of individuals of each species in the eight adjacent cells (if an adjacent cell is empty the rate is one and it leads to an unoccupied cell) 
    - if it is occupied, there is competition according to the coefficients of the interaction matrix or (if it is possible) the individual moves to an adjacent empty cell.

## Usage
Clone the repository:
```bash
git clone https://github.com/tommasomarzi/Competitive-Lotka-Volterra.git
```
Then type:
```bash
cd Competitive-Lotka-Volterra 
```
Once the configuration in the setup.h file has been chosen, in order to build and run the simulation type:
```bash
make
```
