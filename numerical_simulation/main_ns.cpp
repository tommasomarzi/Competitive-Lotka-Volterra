/*! \file main_ns.cpp
    \brief It contains the main function for the numerical simulation. 
*/

#include "lotka_volterra.cpp"
#include "../utilities/reader.cpp"
#include "../utilities/general.cpp"


/**
 * Hanlde the main cycle of the numerical simulation, reading the parameters from the files and managing the output.
 * If the plots are enabled, it calls plotter.py.
 * @return 0 if the simulation was successful
 */
int main()
{
    ofstream toplot;
    vector<vector<double>> interaction;
    vector<double> values_zero, values, rates;
    if(ENABLE_OUTPUT)
    {
        toplot.open("data/" + string(folder) + "/output_ns.txt");
    }

    MatrixReader(interaction, "data/" + string(folder) + "/matrix.txt");
    VectorReader(values_zero, "data/" + string(folder) + "/values.txt");
    VectorReader(rates,       "data/" + string(folder) + "/rates.txt");

    check_input(values_zero, interaction, rates, values_zero);

    for ( int n = 0; n < ITER_MAX; ++n)	
    {
    	values = runge_kutta(values_zero, rates, interaction);   
        values_zero = values;
        if(ENABLE_OUTPUT)
        {
            for (auto el : values)  
            {
                toplot <<el<< ' ';
            }
            toplot<<endl;
        }
    }

    if(ENABLE_OUTPUT)
    {
        toplot.close();
    }

    if(ENABLE_PLOT)
    {
        system("/usr/bin/python3 utilities/plotter.py");
    }

    return 0;
}
