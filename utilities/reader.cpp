#include "reader.h"
#include <sstream>
#include <fstream>


/**
 * Overwrite the interaction matrix with the coefficients of the file.
 * @param interaction interaction matrix where to store the parameters.
 * @param filename name of the file which contains the matrix.
 */
void MatrixReader(vector<vector<double>> &interaction, string filename)
{
    ifstream ifs(filename);       
    string tempstr;                   
    double tempd;                           
    char delimiter;                        
    while (getline(ifs, tempstr)) 
    {   
        istringstream iss(tempstr);   
        vector<double> tempv;            
        while (iss >> tempd) 
        {          
            tempv.push_back(tempd);      
            iss >> delimiter;              
        }
        interaction.push_back(tempv);                
    }
}


/**
 * Overwrite the vector with the coefficients of the file.
 * It can be used for the vector of initial values, growth rates or carrying capacities.
 * @param vector vector where to store the parameters.
 * @param filename name of the file which contains the vector.
 */
void VectorReader(vector<double> &vector, string filename)
{
    ifstream ifs(filename);      
    string tempstr;                  
    double tempd;                          
    while (ifs >> tempd) 
    {   
        vector.push_back(tempd);
    }
}
