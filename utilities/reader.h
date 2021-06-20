#ifndef READER_H
#define READER_H

#include <vector>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;

void MatrixReader(vector<vector<double>> &, string);
void VectorReader(vector<double> &, string);

#endif  // READER_H