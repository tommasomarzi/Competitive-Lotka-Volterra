#ifndef LOTKA_VOLTERRA_H
#define LOTKA_VOLTERRA_H

#include <vector>
#include <string>

using namespace std;

vector<double> VectorTimesScalar(vector<double>, double);
vector<double> VectorPlusVector(vector<double>, vector<double>);
double lotka_volterra(int, vector<double>, double, vector<double>);
vector<double> runge_kutta(vector<double>, vector<double>, vector<vector<double>>);

#endif  // LOTKA_VOLTERRA_H