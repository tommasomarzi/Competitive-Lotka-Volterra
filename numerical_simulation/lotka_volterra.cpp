/*! \file lotka_volterra.cpp
    \brief Compute the numerical solution of the model.
*/

#include "lotka_volterra.h"
#include "../utilities/setup.h"


double h_half = h_increment/2;
double h_six = h_increment/6;


/**
 * @param v original vector.
 * @param k scalar.
 * @return a vector composed of the elements of the original one multiplied by the scalar.
 */
vector<double> VectorTimesScalar(vector<double> v, double k)
{
    vector<double> result(v.begin(), v.end());
	for(int i = 0; i < v.size(); ++i)
		result[i] *= k;
    return result;
}


/**
 * @param v1 first vector.
 * @param v2 second vector.
 * @return a vector whose elements are the element-by-element sum of the vectors.
 */
vector<double> VectorPlusVector(vector<double> v1, vector<double> v2)
{
    vector<double> result(v1.begin(), v1.end());
	for(int i = 0; i < v1.size(); ++i)
		result[i] += v2[i];
    return result;
}


/**
 * Given the parameters associated to a species and the current values, get the time-derivative of the value of that species. 
 * @param index species identifier.
 * @param values vector containing the current values of all the species.
 * @param rate growth rate of the considered species.
 * @param interaction row of the interaction matrix associated to the considered species.
 * @return time-derivative of the considered species.
 */
double lotka_volterra(int index, vector<double> values, double rate, vector<double> interaction)
{
    double sum = 0;
    for(int i = 0; i < values.size(); i++) sum += interaction[i]*values[i];
    double dxdt = rate*values[index]*(1-sum);
    return dxdt;
}


/**
 * Compute the field for the Runge-Kutta 4 algorithm and allow to use iteratively the result.
 * @param values vector containing the starting values of all the species of the previous field.
 * @param rate  vector containing the growth rates.
 * @param interaction interaction matrix.
 * @param fields vector containing the auxiliary fields (check the README.md for an extendend explanation).
 * @param increment the time increment associated to the field.
 * @return vector containing the values of the computed field.
 */
vector<double> compute_fields(vector<double> values, vector<double> rate, vector<vector<double>> interaction, vector<vector<double>> &fields, double increment)		
{
    vector<double> tmp;
    for(int i = 0; i < values.size(); i++)
    {
       tmp.push_back(lotka_volterra(i, values, rate[i], interaction[i]));    
    }
    fields.push_back(tmp);
    tmp = VectorTimesScalar(tmp,increment);
    return tmp;
}


/**
 * Compute the last field for the Runge-Kutta 4 algorithm. 
 * @param values vector containing the starting values of all the species of the previous field.
 * @param rate  vector containing the growth rates.
 * @param interaction interaction matrix.
 * @param fields vector containing the auxiliary fields (check the README.md for an extendend explanation).
 */
void compute_fields(vector<double> values, vector<double> rate, vector<vector<double>> interaction, vector<vector<double>> &fields)		
{
    vector<double> tmp;
    for(int i = 0; i < values.size(); i++)
    {
       tmp.push_back(lotka_volterra(i, values, rate[i], interaction[i]));    
    }
    fields.push_back(tmp);
}


/**
 * Given the values x at the time t_0, the Runge-Kutta 4 algorithm allows to obtain the values at the time t_0 + h,
   where h is the time increment. This iterative algorithm commits a global error of the order of h^4.
 * @param values vector containing the current values of all the species x(t_0).
 * @param rate  vector containing the growth rates.
 * @param interaction interaction matrix.
 * @return vector containing the solution of the next iteration x(t_0 + h).
 */
vector<double> runge_kutta(vector<double> values, vector<double> rate, vector<vector<double>> interaction)		
{
    vector<vector<double>> fields;
    vector<double> tmp2;
    vector<double> result;  

    tmp2 = compute_fields(values, rate, interaction, fields, h_half);         //f(x(t_0),t)*h/2
    tmp2 = VectorPlusVector(values, tmp2);                                    //y = x(t_0) + f(x(t_0),t)*h/2
    tmp2 = compute_fields(tmp2, rate, interaction, fields, h_half);           //f(y,t)*h/2
    tmp2 = VectorPlusVector(values, tmp2);                                    //z = x(t_0) + f(y,t)*h/2
    tmp2 = compute_fields(tmp2, rate, interaction, fields, h_increment);      //f(z,t)*h
    tmp2 = VectorPlusVector(values, tmp2);                                    //w = x(t_0) + f(z,t)*h
    compute_fields(tmp2, rate, interaction, fields);                          //f(w,t)

    fields[1] = VectorTimesScalar(fields[1], 2.0);           //f(y,t) -> 2*f(y,t)                            
    fields[2] = VectorTimesScalar(fields[2], 2.0);           //f(z,t) -> 2*f(z,t)
    fields[0] = VectorPlusVector(fields[0], fields[1]);    //f(x(t_0),t) + 2*f(y,t)                         
    fields[0] = VectorPlusVector(fields[0], fields[2]);    //f(x(t_0),t) + 2*f(y,t) + 2*f(z,t)
    fields[0] = VectorPlusVector(fields[0], fields[3]);    //f(x(t_0),t) + 2*f(y,t) + 2*f(z,t) + f(w,t)
    fields[0] = VectorTimesScalar(fields[0], h_six);       //[f(x(t_0),t) + 2*f(y,t) + 2*f(z,t) + f(w,t)]*h/6

    result = VectorPlusVector(values, fields[0]);          //x(t_0) + [f(x(t_0),t) + 2*f(y,t) + 2*f(z,t) + f(w,t)]*h/6                               
    return result;
}