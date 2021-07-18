#include "general.h"

void check_input(vector<double> values, vector<vector<double>> interaction, vector<double> rates, vector<double> capacity)
{
    int n_species = values.size();
    if(interaction.size() != n_species)
    {
        cout<<"INPUT ERROR: interaction matrix has an incorrect size. Aborted."<<endl;
        exit(1);
    }
    for(int i = 0; i < interaction.size(); i++)
    {
        if(interaction[i].size() != n_species)
        {
            cout<<"INPUT ERROR: interaction matrix has an incorrect size. Aborted."<<endl;
            exit(1);
        }
    }
    if(rates.size() != n_species)
    {
        cout<<"INPUT ERROR: rates vector has an incorrect size. Aborted."<<endl;
        exit(1);
    }
    if(capacity.size() != n_species)
    {
        cout<<"INPUT ERROR: capacity vector has an incorrect size. Aborted."<<endl;
        exit(1);
    }
}