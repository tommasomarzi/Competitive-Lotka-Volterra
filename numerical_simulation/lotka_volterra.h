#ifndef LOTKA_VOLTERRA_H
#define LOTKA_VOLTERRA_H

#include <vector>
#include <string>

using namespace std;

extern double h_half;
extern double h_six;

vector<double> VectorTimesScalar(vector<double>, double);                           //!< Multiply all the elements of a vector by a scalar.
vector<double> VectorPlusVector(vector<double>, vector<double>);                    //!< Sum the element-by-element of two vectors.
double lotka_volterra(int, vector<double>, double, vector<double>);                 //!< Competitive Lotka-Volterra equation.
vector<double> runge_kutta(vector<double>, vector<double>, vector<vector<double>>); //!< Implementation of the Runge-Kutta 4 algorithm.

#endif  // LOTKA_VOLTERRA_H