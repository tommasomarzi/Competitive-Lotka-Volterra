#ifndef LV_H
#define LV_H

#include <ctime>
#include "../utilities/setup.h"
#include "../utilities/reader.cpp"
#include "../testing/general.cpp"

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

    friend class LV_Fixture;

    int neighborhood(const int, const int);
    int normalizer();
    int normalizer(vector<int>);
    int normalizer(vector<int>, vector<bool>, int);
    int random_walk(vector<bool>, int);
    int filler(vector<int>);
    void configuration();
    void print_output();
    
public:
    LV(const int, const int);
	~LV();
	void initializer_fill();
	void evolve();
    float* get_specie_color(int, int);
    int get_iter();
    void get_stats();
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