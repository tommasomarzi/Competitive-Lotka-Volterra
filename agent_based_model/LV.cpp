/*! \file LV.cpp
    \brief Define the methods of the agent-based model class.

    The doxygen documentation related to these methods can be found in the Classes section.
*/

#include "LV.h"
#include <iostream>
#include <stdlib.h>


/**
 * Instantiate the agent-based model in a grid. 
 * @param rows_ The number of cells in the x-axis.
 * @param columns_ The number of cells in the y-axis.
 */
LV::LV(const int rows_, const int columns_): rows(rows_), columns(columns_) 
{
    grid = new int*[rows];
    
    for (int i = 0; i < rows; ++i)
    {
        grid[i] = new int[columns];
    }
    
    nb_x = new int[3];
    nb_y = new int[3];
    
    iter = 0;
    local_iter = 1;

    MatrixReader(interaction, "data/" + string(folder) + "/matrix.txt");
    VectorReader(values_zero, "data/" + string(folder) + "/values.txt");
    VectorReader(rates,       "data/" + string(folder) + "/rates.txt");
    VectorReader(capacity,    "data/" + string(folder) + "/capacity.txt");

    check_input(values_zero, interaction, rates, capacity);

    if(ENABLE_OUTPUT)
    {
        to_plot.open( "data/" + string(folder) + "/output_abm.txt");
    }

    colors = new float*[values_zero.size() + 1];

    if((values_zero.size() + 1) > n_colors)
    {
        cout<<"Color palette error: colors are not enough for the number of species."<<endl;
        exit(1);
    }

    for(int i = 0; i < (values_zero.size() + 1); i++)
    {
        colors[i] = color_wheel[i];
    }

    configuration();

    if(!DEBUG_MODE)    
    {
        srand((unsigned)time(NULL));
    }
    else 
    {
        cout<<"RUNNING THE PROGRAM IN DEBUG MODE"<<endl<<endl;
        srand(1);
    }
};


LV::~LV() 
{
    for(int i = 0; i < rows; i++) 
    {
        delete [] grid[i];
    } 
    delete [] grid;
    delete [] nb_x;
    delete [] nb_y; 
    for(int i = 0; i < values_zero.size(); i++)
    {
        delete [] colors[i];
    }
}


/**
 * Stamp in the shell the chosen parameters and the associated colors.
 * If the graphical part is enabled, display some useful commands.
 */
void LV::configuration()
{
    cout<<endl;
    cout<<"::--------------------------- COMPETITIVE LOTKA-VOLTERRA ---------------------------:: "<<endl<<endl<<endl;
    cout<<":: BACKGROUND:                ( "<<rows<<" x "<<columns<<" ) cells (GREEN)"<<endl<<endl;
    cout<<":: NUMBER OF SPECIES:           "<<values_zero.size()<<endl<<endl;
    cout<<":: COLORS:                    ( ";
    for(int i = 0; i < values_zero.size(); i++)
    {
        switch (i) 
        {
            case 0: cout<<"YELLOW"; break;
            case 1: cout<<","<<'\t'<<'\t'<<"BLACK"; break;
            case 2: cout<<","<<'\t'<<'\t'<<"RED"; break;
            case 3: cout<<","<<'\t'<<'\t'<<"BLUE"; break;
            case 4: cout<<","<<'\t'<<'\t'<<"LIGHT GRAY"; break;
            case 5: cout<<","<<'\t'<<'\t'<<"CYAN"; break;
            case 6: cout<<","<<'\t'<<'\t'<<"PURPLE"; break;
        }
    }
    cout<<" )"<<endl<<endl;
    cout<<":: INITIAL VALUES:            ( ";
    for(int i = 0; i < (values_zero.size() - 1); i++)
    {
        cout<<values_zero[i]<<","<<'\t'<<'\t';
    }
    cout<<values_zero[values_zero.size() - 1]<<" )"<<endl<<endl;
    cout<<":: CARRYING CAPACITIES:       ( ";
    for(int i = 0; i < (capacity.size() - 1); i++)
    {
        cout<<capacity[i]<<","<<'\t'<<'\t';
    }
    cout<<capacity[capacity.size() - 1]<<" )"<<endl<<endl;
    cout<<":: RATES:                     ( ";
    for(int i = 0; i < (rates.size() - 1); i++)
    {
        cout<<rates[i]<<","<<'\t'<<'\t';
    }
    cout<<rates[rates.size() - 1]<<" )"<<endl<<endl;
    cout<<":: INTERACTION MATRIX:          ";
    for(auto row : interaction) 
    {
        for(auto el : row) 
        {
            cout<<el<<","<<'\t'<<'\t';
        }
        cout<<endl;
        cout<<"                                ";
    }
    cout<<endl<<endl;
    if(ENABLE_GRAPHICS)
    {
        cout<<"Press 'p' to pause/resume, 'i' to stamp the current iteration, ";
        cout<<"'s' to stamp the current statistics, 'e' to exit."<<endl<<endl;
    }
}


/**
 * Get the color associated to the content of a cell (x,y).
 * @param x x position of the cell
 * @param y y position of the cell
 * @return 3-dimensional array with the corresponding RGB values.
 */
float* LV::get_species_color(int x, int y)
{    
    return colors[grid[x][y] + 1];
}


/**
 * Compute the content of a cell in the next iteration based on its neighborhood.
 * @param x x position of the cell
 * @param y y position of the cell
 * @return value of the cell at the next iteration
 */
int LV::neighborhood(int x, int y)
{
    int result;
    vector<int> presences(values_zero.size() + 1, 0);
    vector<bool> empty_cells(9, false);

    //BOUNDARY CONDITIONS
	nb_x[0] = (x == 0 ? rows - 1: x - 1);
	nb_x[1] = x;
	nb_x[2] = (x == rows - 1 ? 0 : x + 1);
	nb_y[0] = (y == 0 ? columns - 1: y - 1);
	nb_y[1] = y;
	nb_y[2] = (y == columns - 1 ? 0 : y + 1);

    for(int i = 0; i < 3; ++i) 
    {
		for(int j = 0; j < 3; ++j) 
        {
            if (!(nb_x[i] == x && nb_y[j] == y)) 
            {
                presences[grid[nb_x[i]][nb_y[j]] + 1]++;
                if((grid[nb_x[i]][nb_y[j]]) == EMPTY_CELL)
                {
                    empty_cells[i*3 + j] = true;
                }
            }
		}
	}

    if(grid[x][y] == EMPTY_CELL)
    {
        result = filler(presences);
    }
    else
    {
        result = normalizer(presences, empty_cells, grid[x][y]);
    }

    return result;
}


/**
 *  In the case in which a cell is empty, compute its value at the next iteration 
    based on the growth rates of the neighborhood.
 * @param presences occupation of the neighborhood.
 * @return value of the cell at the next iteration.
 */
int LV::filler(vector<int> presences)
{
    int result = -2;
    double normalization_factor = 0.0;
    double rv = (double)rand() / RAND_MAX;
    double prob = 0.0;
    normalization_factor += presences[0];

    for(int i = 0; i < rates.size(); i++)  
    {
        normalization_factor += presences[i+1]*rates[i];
    }
    
    for(int i = 0; i < (rates.size() + 1); i++)
    {
        if(i == 0)
        {
            prob += presences[i]/normalization_factor;
        }
        else
        {
            prob += presences[i]*rates[i-1]/normalization_factor;
        }

        if(rv < prob)
        {
            result = i - 1;
            break;
        }
    }

    if(result == -2)
    {
        cout<<"ERROR: an internal error has occurred (an incorrect value has been assigned). Aborted."<<endl;
        exit(1);
    }
    return result;
}


/**
 *  In the case in which a cell is occupied by an individual, compute its value at the next iteration 
    according to the interaction with the neighborhood.
 * @param presences occupation of the neighborhood.
 * @param empty_cells spatial information on the occupation of the neighborhood.
 * @param species species which occupies the cell. 
 * @return value of the cell at the next iteration.
 *
 * The returned value can be:
 *
 * 1) -1 (the indivual dies)
 *
 * 2) the current species (nothing happens)
 *
 * 3) the number of species + 1 + cell, where cell is the index of an empty cell (movement)
 *
 * 4) the number of species + 101 + cell, where cell is the index of an empty cell (birth)
 */
int LV::normalizer(vector<int> presences, vector<bool> empty_cells, int species)
{    
    int result;
    double rv = (double)rand() / RAND_MAX;
    double prob = 0.0;
 
    for(int i = 0; i < (rates.size() + 1); i++)
    {    
        prob += presences[i]/8.0;
        if(rv < prob)
        {
            if((i - 1) == species)              //interation with the same species A (-> death)
            {
               result = EMPTY_CELL;             // A + A -> 0 + A
            }
            else if(i == 0)                     //interaction with an empty cell
            {
                int count_empty = 1;
                for(int i = 0; i < empty_cells.size(); i++)
                {
                    if(empty_cells[i] == true)
                    {
                        count_empty++;
                    }
                }
                int cell = random_walk(empty_cells, count_empty);

                if(ENABLE_BIRTHS)                       //possible birth/movement
                {
                    double normalization_factor  = count_empty*rates[species] + (8-count_empty);
                    rv = (double)rand() / RAND_MAX;

                    if(rv < (count_empty*rates[species]/normalization_factor))             //possible birth
                    {
                        if(cell == NO_ACTIONS)                            //nothing happens
                        {
                            result = species;                             // A + 0 -> A + 0
                        }
                        else                                              //birth
                        {
                            result = values_zero.size() + cell + 101;     // A + 0 -> A + A
                        }
                    }
                    else                                                  //possible movement
                    {
                        if(cell == NO_ACTIONS)                            //nothing happens
                        {
                            result = species;                             // A + 0 -> A + 0
                        }
                        else                                              //movement
                        {
                            result = values_zero.size() + cell + 1;       // A + 0 -> 0 + A
                        }
                    }
                }
                else                                    //possible movement
                {
                    if(cell == NO_ACTIONS)                                //nothing happens
                    {
                        result = species;                                 // A + 0 -> A + 0
                    }
                    else                                                  //movement
                    {
                        result = values_zero.size() + cell + 1;           // A + 0 -> 0 + A
                    }
                }
            }
            else                                //interaction with another species B
            {
                rv = (double)rand() / RAND_MAX;
                if(rv < interaction[species][i-1])                        //death of the individual
                {
                    result = EMPTY_CELL;                                  //A + B -> 0 + B
                }
                else                                                      //nothing happens
                {
                    result = species;                                     //A + B -> A + B
                }
            }
            break;
        }
    }
    return result;
}


/**
 *  In the case in which a cell is occupied by an individual and it interacts with an empty cell,
    the individual has the opportunity to move towards that cell. 
 * @param empty_cells spatial information on the occupation of the neighborhood.
 * @param count_empty number of empty cells in the neighborhood.
 * @return the cell that will be occupied by the individual at the next iteration.
 */
int LV::random_walk(vector<bool> empty_cells, int count_empty)
{
    int result = NO_ACTIONS;
    double rv = (double)rand() / RAND_MAX;
    double prob = 1.0/count_empty;
    for(int i = 0; i < empty_cells.size(); i++)
    {
        if(rv < prob)
        {
            if(empty_cells[i] == true)
            {
                result = i;
                break;
            }
        }
        else
        {
            prob += 1.0/count_empty;
        }
    }
    return result;
}


/**
 *  At the beginning, generate an individual with a probability depending on the initial values, 
    the carrying capacities and the dimensions of the grid.
 * @return value of the cell.
 */
int LV::normalizer()
{
    int result = EMPTY_CELL;
    int cells = rows*columns;
    double rv = (double)rand() / RAND_MAX;
    double prob = 0.0;
    for(int i = 0; i < values_zero.size(); i++)
    {
        prob += values_zero[i]*capacity[i]/cells;
        if(rv < prob)
        {
            result = i;
            break;
        }
    }
    return result;
}


/**
 *  At the beginning, initialize each cell of the grid in a probabilistic way based on the initial values, 
    the carrying capacities and the dimensions of the grid.
 * @see normalizer()
 */
void LV::initializer_fill() 
{      
	for(int i = 0; i < rows; ++i)
    {
		for(int j = 0; j < columns; ++j) 
        {
            grid[i][j] = normalizer();
		}
	}
}


/**
 *  Pick randomly a cell of the grid and get its value at the next iteration.
 *  @see neighborhood(int x, int y)
 */
void LV::evolve() 
{
    int result;
    int x = rand() % rows;
    int y = rand() % columns;
    result = neighborhood(x, y);
    if(result < (int(values_zero.size()) + 1))              //no shift -> new value of the cell
    {
        grid[x][y] = result;
    }
    else                                                    //shift -> birth/movement
    {
        int destination;
        int shift = result - (int(values_zero.size()) + 1);
        bool birth;
        if(shift > 99)                                      //shift > 99 -> birth
        {
            birth = true;
            shift -= 100;
        }
        else                                                //shift between 0 and 8 -> movement
        {
            birth = false;
        }
        switch(shift)
        {
            case 0: 
                destination = grid[nb_x[0]][nb_y[0]];
                grid[nb_x[0]][nb_y[0]] = grid[x][y];
                break;
            case 1: 
                destination = grid[nb_x[0]][nb_y[1]];
                grid[nb_x[0]][nb_y[1]] = grid[x][y];
                break;
            case 2: 
                destination = grid[nb_x[0]][nb_y[2]];
                grid[nb_x[0]][nb_y[2]] = grid[x][y];
                break;
            case 3: 
                destination = grid[nb_x[1]][nb_y[0]];
                grid[nb_x[1]][nb_y[0]] = grid[x][y];
                break;
            case 4: 
                cout<<"ERROR: destination not allowed (it is not possible to move into the starting cell). Aborted."<<endl;
                exit(1);
                break;
            case 5: 
                destination = grid[nb_x[1]][nb_y[2]];
                grid[nb_x[1]][nb_y[2]] = grid[x][y];
                break;
            case 6: 
                destination = grid[nb_x[2]][nb_y[0]];
                grid[nb_x[2]][nb_y[0]] = grid[x][y];
                break;
            case 7: 
                destination = grid[nb_x[2]][nb_y[1]];
                grid[nb_x[2]][nb_y[1]] = grid[x][y];
                break;
            case 8: 
                destination = grid[nb_x[2]][nb_y[2]];
                grid[nb_x[2]][nb_y[2]] = grid[x][y];
                break;
        }
        if(!birth)
        {
            grid[x][y] = EMPTY_CELL;
        }
        if(destination != EMPTY_CELL)
        {            
            cout<<"ERROR: destination not allowed (it is not possible to move into an occupied cell). Aborted."<<endl;
            exit(1);
        }
    }
    if(local_iter%LOCAL_ITER == 0)
    {
        if(ENABLE_OUTPUT)
        {
            print_output();
        }
        ++iter;
        local_iter = 0;
    }
    if(iter == ITER_MAX)
    {
        if(ENABLE_OUTPUT)
        {
            to_plot.close();
        }
        exit(0);
    }
    ++local_iter;
}


/**
 *  Save the values of the species normalized on the carrying capacities in the output file.
 */
void LV::print_output()
{
    double species[values_zero.size()] = {0};
    for(int x = 0; x < rows; x++)
    {
        for(int y = 0; y < columns; y++)
        {
            if(grid[x][y] != EMPTY_CELL)
            {
                species[grid[x][y]] += 1;
            }
        }
    }
    for(int i = 0; i < values_zero.size(); i++)
    {   
        to_plot<<species[i]/capacity[i]<< ' ';
    }
    to_plot<<endl;
}


/**
 * @return the current iteration.
 */
int LV::get_iter()
{
    return iter;
}


/**
 * Display in the shell the values of the species normalized on the carrying capacities at the current iteration.
 */
void LV::get_stats()
{
    double species[values_zero.size()] = {0};
    for(int x = 0; x < rows; x++)
    {
        for(int y = 0; y < columns; y++)
        {
            if(grid[x][y] != EMPTY_CELL)
            {
                species[grid[x][y]] += 1;
            }
        }
    }
    for(int i = 0; i < values_zero.size(); i++)
    {   
        cout<<endl;
        cout<<"SPECIES: "<<i+1<<"    INDIVIDUALS: "<<species[i]/capacity[i]<<endl;
    }
    cout<<endl;
}