#include "LV.h"

LV::LV(const int w, const int h): width(w), height(h) 
{
    grid = new int*[width];
    next_grid = new int*[width];
    for (int i = 0; i < width; ++i)
    {
        grid[i] = new int[height];
        next_grid[i] = new int[height];
    }
    
    nb_x = new int[3];
    nb_y = new int[3];
    
    iter = 0;

    MatrixReader(interaction, file_matrix);
    VectorReader(values_zero, file_values);
    VectorReader(rates, file_rates);
    VectorReader(capacity, file_capacity);

    to_plot.open(file_output);

    colors = new float*[values_zero.size() + 1];

    if((values_zero.size() + 1) > n_colors)
    {
        std::cout<<"Color palette error: colors are not enough for the number of species."<<std::endl;
        exit(1);
    }

    for(int i = 0; i < values_zero.size() + 1; i++)
    {
        colors[i] = color_wheel[i];
    }

    configuration();

    if(true)    
    {
        srand((unsigned)time(NULL));
    }
    else srand(1);
};


LV::~LV() 
{
    for(int i = 0; i < width; i++) 
    {
        delete [] grid[i];
        delete [] next_grid[i];
    } 
    delete [] grid;
    delete [] next_grid;
    delete [] nb_x;
    delete [] nb_y; 
    for(int i = 0; i < values_zero.size(); i++)
    {
        delete [] colors[i];
    }
}


void LV::configuration()
{
    std::cout<<std::endl;
    std::cout<<"::--------------------------- COMPETITIVE LOTKA-VOLTERRA ---------------------------:: "<<std::endl<<std::endl<<std::endl;
    std::cout<<":: BACKGROUND COLOR:                        ( "<<n_rows<<" x "<<n_cols<<") cells (GREEN)"<<std::endl<<std::endl;
    std::cout<<":: NUMBER OF SPECIES:                         "<<values_zero.size()<<std::endl<<std::endl;
    std::cout<<":: COLORS:                                  ( ";
    for(int i = 0; i < values_zero.size(); i++)
    {
        switch (i) 
        {
            case 0: std::cout<<"SENAPE"; break;
            case 1: std::cout<<","<<'\t'<<"BLACK"; break;
            case 2: std::cout<<","<<'\t'<<"RED"; break;
            case 3: std::cout<<","<<'\t'<<"BLUE"; break;
            case 4: std::cout<<","<<'\t'<<"LIGHT GRAY"; break;
            case 5: std::cout<<","<<'\t'<<"CYAN"; break;
            case 6: std::cout<<","<<'\t'<<"PURPLE"; break;
        }
    }
    std::cout<<" )"<<std::endl<<std::endl;
    std::cout<<":: INITIAL VALUES (not normalized):         ( ";
    for(int i = 0; i < (values_zero.size() - 1); i++)
    {
        std::cout<<values_zero[i]<<","<<'\t';
    }
    std::cout<<values_zero[values_zero.size() - 1]<<" )"<<std::endl<<std::endl;
    std::cout<<":: RATES (not normalized):                  ( ";
    for(int i = 0; i < (rates.size() - 1); i++)
    {
        std::cout<<rates[i]<<","<<'\t';
    }
    std::cout<<rates[rates.size() - 1]<<" )"<<std::endl<<std::endl;
    std::cout<<":: INTERACTION MATRIX (not normalized):       ";
    for (auto row : interaction) 
    {
        for (auto el : row) 
        {
            cout<<el<<","<<'\t';
        }
        cout<<std::endl;
        std::cout<<"                                              ";
    }
    std::cout<<std::endl<<std::endl;
    std::cout<<"Press 'p' to pause/resume, 'i' to stamp the current iteration, 'e' to exit."<<std::endl<<std::endl;
}


float* LV::get_specie_color(int x, int y)
{
    return colors[grid[x][y] + 1];
}


int LV::neighborhood(int x, int y)
{
    vector<int> presences(values_zero.size() + 1, 0);
    int result;
	nb_x[0] = (x == 0 ? width - 1: x - 1);
	nb_x[1] = x;
	nb_x[2] = (x == width - 1 ? 0 : x + 1);
	nb_y[0] = (y == 0 ? height - 1: y - 1);
	nb_y[1] = y;
	nb_y[2] = (y == height - 1 ? 0 : y + 1);
    for(int i = 0; i < 3; ++i) 
    {
		for(int j = 0; j < 3; ++j) 
        {
            if (!(nb_x[i] == x && nb_y[j] == y)) 
            {
                presences[grid[nb_x[i]][nb_y[j]] + 1]++;
            }
		}
	}
    result = normalizer(presences, grid[x][y]);
    return result;
}


int LV::normalizer()
{
    int result = -1;
    int cells = n_rows*n_cols;
    double rv = (double)rand() / RAND_MAX;
    double prob = 0;
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
    double den = 0;
    double rv = (double)rand() / RAND_MAX;
    double prob = 0;
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


int LV::normalizer(vector<int> presences, int specie)
{    
    int result;
    double den = 0.0;
    double rv = (double)rand() / RAND_MAX;
    double prob = 0.0;
    for(int i = 0; i < presences.size(); i++)  
    {
        den += presences[i]*interaction[specie][i];
    }
    if(den == 0)
    {
        result = specie;
    }
    else
    {
        for(int i = 0; i < presences.size(); i++)
        {
            prob += presences[i]*interaction[specie][i]/den;
            if(rv < prob)
            {
                if(i == specie)
                {
                    result = normalizer(presences);
                    if(result == -1)
                    {
                        result = specie;
                    }
                }
                else    
                {
                    result = i;
                }
                break;
            }
        }
    }
    return result;
}


void LV::initializer_fill() 
{      
	for(int i = 0; i < width; ++i)
    {
		for(int j = 0; j < height; ++j) 
        {
            grid[i][j] = normalizer();
		}
	}
}


void LV::evolve() 
{
    ++iter;
    for (int x = 0; x < width; ++x) 
    {
		for(int y = 0; y < height; ++y) 
        {
            next_grid[x][y] = neighborhood(x, y);
        }
    }
    int** swap = grid;
    grid = next_grid;
    next_grid = swap;
    if(iter == ITER_MAX)
    {
        to_plot.close();
        exit(0);
    }
    print_output();
}


void LV::print_output()
{
    double species[values_zero.size()] = {0};
    for(int x = 0; x < n_rows; x++)
    {
        for(int y = 0; y < n_cols; y++)
        {
            species[grid[x][y]] += 1;
        }
    }
    for(int i = 0; i < values_zero.size(); i++)
    {   
        to_plot<<species[i]/(n_cols*n_rows)<< ' ';
    }
    to_plot<<std::endl;
}


int LV::get_iter()
{
    return iter;
}
