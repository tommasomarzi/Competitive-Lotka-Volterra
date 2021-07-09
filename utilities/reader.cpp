#include "reader.h"
#include <sstream>
#include <fstream>

void MatrixReader(vector<vector<double>> &interaction, string filename)
{
    ifstream ifs(filename);       // open the file
    string tempstr;                   // declare a temporary string
    double tempd;                           // declare a temporary integer
    char delimiter;                        // declare a temporary delimiter
    while (getline(ifs, tempstr)) 
    {   // read line by line from a file into a string
        istringstream iss(tempstr);   // initialize the stringstream with that string
        vector<double> tempv;            // declare a temporary vector for the row
        while (iss >> tempd) 
        {           // extract the numbers from a stringstream
            tempv.push_back(tempd);      // push it onto our temporary vector
            iss >> delimiter;              // read the , delimiter
        }
        interaction.push_back(tempv);                // push the vector onto vector of vectors
    }
}


void VectorReader(vector<double> &vector, string filename)
{
    ifstream ifs(filename);       // open the file
    string tempstr;                   // declare a temporary string
    double tempd;                           // declare a temporary integer
    while (ifs >> tempd) 
    {   
        vector.push_back(tempd);                // push the vector onto vector of vectors
    }
}
