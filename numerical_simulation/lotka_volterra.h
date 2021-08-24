/*! \file lotka_volterra.h
    \brief Header of the lotka_volterra.cpp file.
*/

#ifndef LOTKA_VOLTERRA_H
#define LOTKA_VOLTERRA_H

#include <vector>
#include <string>

using namespace std;

extern double h_half;
extern double h_six;

vector<double> VectorTimesScalar(vector<double>, double);                               //!< Multiply all the elements of a vector by a scalar.
vector<double> VectorPlusVector(vector<double>, vector<double>);                        //!< Sum the element-by-element of two vectors.
double lotka_volterra(int, vector<double>, double, vector<double>);                     //!< Competitive Lotka-Volterra equation.
vector<double> runge_kutta(vector<double>, vector<double>, vector<vector<double>>);     //!< Implementation of the Runge-Kutta 4 algorithm.
vector<double> compute_fields(vector<double>, vector<double>, vector<vector<double>>, vector<vector<double>> &, double);  //!< Compute the fields for the Runge-Kutta 4 algorithm.
void compute_fields(vector<double>, vector<double>, vector<vector<double>>, vector<vector<double>> &);  //!< Compute the last field for the Runge-Kutta 4 algorithm.

#endif  // LOTKA_VOLTERRA_H