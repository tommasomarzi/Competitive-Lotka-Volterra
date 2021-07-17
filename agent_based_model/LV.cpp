#include "LV.h"
#include <iostream>
#include <stdlib.h>

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


float* LV::get_specie_color(int x, int y)
{
    return colors[grid[x][y] + 1];
}


int LV::neighborhood(int x, int y)
{
    int result;
    vector<int> presences(values_zero.size() + 1, 0);
    vector<bool> empty_cells(9, false);

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
                if((grid[nb_x[i]][nb_y[j]] + 1) == 0)
                {
                    empty_cells[i*3 + j] = true;
                }
            }
		}
	}

    if(grid[x][y] == -1)
    {
        result = filler(presences);
    }
    else
    {
        result = normalizer(presences, empty_cells, grid[x][y]);
    }

    return result;
}


int LV::filler(vector<int> presences)
{
    int result;
    double den = 0.0;
    double rv = (double)rand() / RAND_MAX;
    double prob = 0.0;
    den += presences[0];

    for(int i = 0; i < rates.size(); i++)  
    {
        den += presences[i+1]*rates[i];
    }
    
    for(int i = 0; i < (rates.size() + 1); i++)
    {
        if(i == 0)
        {
            prob += presences[i]/den;
        }
        else
        {
            prob += presences[i]*rates[i-1]/den;
        }

        if(rv < prob)
        {
            result = i - 1;
            break;
        }
    }
    return result;
}


int LV::normalizer()
{
    int result = -1;
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


int LV::normalizer(vector<int> presences)
{
    int result;
    double den = 0.0;
    double rv = (double)rand() / RAND_MAX;
    double prob = 0.0;
    vector<double> rates_neighborhood;

    for(int i = 0; i < presences.size(); i++)  
    {
        rates_neighborhood.push_back(rates[i]*presences[i]);
    }

    for(int i = 0; i < rates_neighborhood.size(); i++)  
    {
        den += rates_neighborhood[i];
    }

    for(int i = 0; i < rates_neighborhood.size(); i++)
    {
        prob += rates_neighborhood[i]/den;
        if(rv < prob)
        {
            result = i;
            break;
        }
    }
    return result;
}


int LV::normalizer(vector<int> presences, vector<bool> empty_cells, int specie)
{    
    int result;
    double rv = (double)rand() / RAND_MAX;
    double prob = 0.0;
 
    for(int i = 0; i < (rates.size() + 1); i++)
    {    
        prob += presences[i]/8.0;
        if(rv < prob)
        {
            if((i - 1) == specie)               //iteration[i][i] == 1
            {
               result = -1;                    // A + A -> 0 + A
            }
            else if(i == 0)                     // A + 0 -> move/birth
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
                if(ENABLE_BIRTHS)
                {
                    double den  = count_empty*rates[specie] + (8-count_empty);
                    rv = (double)rand() / RAND_MAX;

                    if(rv < (count_empty*rates[specie]/den))
                    {
                        if(cell == 9)
                        {
                            result = specie;                              // A + 0 -> A + 0
                        }
                        else
                        {
                            result = values_zero.size() + cell + 101;     // A + 0 -> A + A
                        }
                    }
                    else
                    {
                        if(cell == 9)
                        {
                            result = specie;                            // A + 0 -> A + 0
                        }
                        else
                        {
                            result = values_zero.size() + cell + 1;     // A + 0 -> 0 + A
                        }
                    }
                    }
                else
                {
                    if(cell == 9)
                    {
                        result = specie;                            // A + 0 -> A + 0
                    }
                    else
                    {
                        result = values_zero.size() + cell + 1;     // A + 0 -> 0 + A
                    }
                }
            }
            else
            {
                rv = (double)rand() / RAND_MAX;
                if(rv < interaction[specie][i-1])
                {
                    result = -1;                 //A + X -> 0 + X
                }
                else
                {
                    result = specie;             //A + X -> A + X
                }
            }
            break;
        }
    }
    return result;
}


int LV::random_walk(vector<bool> empty_cells, int count_empty)
{
    int result = 9;
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


void LV::evolve() 
{
    int result, shift, destination;
    int x = rand() % rows;
    int y = rand() % columns;
    result = neighborhood(x, y);
    if((int(values_zero.size()) + 1) > result)
    {
        grid[x][y] = result;
    }
    else 
    {
        shift = result - (int(values_zero.size()) + 1);
        bool birth;
        if(shift > 99)
        {
            birth = true;
            shift -= 100;
        }
        else
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
            grid[x][y] = -1;
        }
        if(destination != -1)
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


void LV::print_output()
{
    double species[values_zero.size()] = {0};
    for(int x = 0; x < rows; x++)
    {
        for(int y = 0; y < columns; y++)
        {
            if(grid[x][y] != -1)
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


int LV::get_iter()
{
    return iter;
}


void LV::get_stats()
{
    double species[values_zero.size()] = {0};
    for(int x = 0; x < rows; x++)
    {
        for(int y = 0; y < columns; y++)
        {
            if(grid[x][y] != -1)
            {
                species[grid[x][y]] += 1;
            }
        }
    }
    for(int i = 0; i < values_zero.size(); i++)
    {   
        cout<<endl;
        cout<<"SPECIE: "<<i+1<<"    INDIVIDUALS: "<<species[i]/capacity[i]<<endl;
    }
    cout<<endl;
}