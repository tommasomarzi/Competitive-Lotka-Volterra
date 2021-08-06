#ifndef READER_H
#define READER_H

#include <vector>
#include <string>

using namespace std;

void MatrixReader(vector<vector<double>> &, string);    //!< Read the values of the matrix from a file.
void VectorReader(vector<double> &, string);            //!< Read the values of the vector from a file.

#endif  // READER_H