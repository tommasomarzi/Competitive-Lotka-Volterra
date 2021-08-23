/*! \file LV.h
    \brief Header of the LV.cpp file. 
*/

#ifndef LV_H
#define LV_H

#include <ctime>
#include "../utilities/setup.h"
#include "../utilities/reader.cpp"
#include "../testing/general.cpp"

/**
 * @brief Class to generate an agent-based model which evolves in a grid according to stochastic rules
 * based on the theoretical competitive Lotka-Volterra model.
 */
class LV 
{
private:
	int rows, columns, iter, local_iter;
    int **grid;
    int *nb_x, *nb_y;

    vector<vector<double>> interaction;
    vector<double> values_zero, rates, capacity;
    
    float **colors;

    ofstream to_plot;

    friend class LV_Fixture;                //!< Class for testing.

    int neighborhood(int, int);             //!< Update the value of a cell.
    int normalizer(vector<int>, vector<bool>, int);     //!< Evolve an occupied cell.
    int random_walk(vector<bool>, int);     //!< Possibility of movement.   
    int normalizer();                       //!< Initialize a cell.
    int filler(vector<int>);                //!< Evolve an empty cell.
    void configuration();                   //!< Display the configuration.
    void print_output();                    //!< Save the values of the populations.
    
public:
    LV(const int, const int);               //!< Parametrized constructor of the class.
	~LV();                                  //!< Destructor of the class.
	void initializer_fill();                //!< Initialize the grid.
	void evolve();                          //!< Evolve the simulation.
    float* get_species_color(int, int);     //!< Get the color associated to the content of a cell.
    int get_iter();                         //!< Get the current iteration.
    void get_stats();                       //!< Get the values of the populations.
};

const int n_colors = 8;
float color_wheel[n_colors][3] 
{             
    {0.16, 0.53, 0.01},                       //green
    {0.92, 0.82, 0.19},                      //yellow  
    {0.0,  0.0,  0.0},                       //black 
    {0.66, 0.13, 0.15},                       //red
    {0.24, 0.55, 0.87},                       //blue  
    {0.83, 0.83, 0.83},                      //Light Gray            
    {0.19, 0.92, 0.85},                      //Cyan             
    {0.82, 0.35, 0.91}                      //purple     	         
};

#endif  // LV_H