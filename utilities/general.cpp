#include "general.h"


/*
Check that the vectors containing the parameters obtained from the data folder are built properly.
In particular, verify that:
1) the files are read properly, i.e. the number of species is greater than zero
2) the files are built properly, i.e. there is correspondence between the dimensions of the vectors
and of the matrix
*/
void check_input(vector<double> values, vector<vector<double>> interaction, vector<double> rates, vector<double> capacity)
{
    int n_species = values.size();
    if(n_species == 0)
    {
        cout<<"INPUT ERROR: please check the input file or its path (0 species found). Aborted."<<endl;
        exit(1);
    }
    if(interaction.size() != n_species)
    {
        cout<<"INPUT ERROR: the interaction matrix has a different size ("<<interaction.size();
        cout<<") from the vector of the initial conditions ("<<n_species<<"). Aborted."<<endl;
        exit(1);
    }
    for(int i = 0; i < interaction.size(); i++)
    {
        if(interaction[i].size() != n_species)
        {
            cout<<"INPUT ERROR: the row "<<i<<" of interaction matrix has a different size ("<<interaction[i].size();
            cout<<") from the vector of the initial conditions ("<<n_species<<"). Aborted."<<endl;
            exit(1);
        }
    }
    if(rates.size() != n_species)
    {
        cout<<"INPUT ERROR: the growth rates vector has a different size ("<<rates.size();
        cout<<") from the vector of the initial conditions ("<<n_species<<"). Aborted."<<endl;
        exit(1);
    }
    if(capacity.size() != n_species)
    {
        cout<<"INPUT ERROR: the capacities vector has a different size ("<<capacity.size();
        cout<<") from the vector of the initial conditions ("<<n_species<<"). Aborted."<<endl;
        exit(1);
    }
}