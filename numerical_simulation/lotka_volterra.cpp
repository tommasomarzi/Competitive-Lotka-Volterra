#include "lotka_volterra.h"
#include "../utilities/setup.h"

double h_half = h_increment/2;
double h_six = h_increment/6;


vector<double> VectorTimesScalar(vector<double> v, double k)
{
    vector<double> result(v.begin(), v.end());
	for(int i = 0; i < v.size(); ++i)
		result[i] *= k;
    return result;
}


vector<double> VectorPlusVector(vector<double> v1, vector<double> v2)
{
    vector<double> result(v1.begin(), v1.end());
	for(int i = 0; i < v1.size(); ++i)
		result[i] += v2[i];
    return result;
}


double lotka_volterra(int index, vector<double> values, double rate, vector<double> interaction)
{
    double sum = 0;
    for(int i = 0; i < values.size(); i++) sum += interaction[i]*values[i];
    double dxdt = rate*values[index]*(1-sum);
    return dxdt;
}


vector<double> runge_kutta(vector<double> values, vector<double> rate, vector<vector<double>> interaction)		
{
    int i;
    vector<vector<double>> fields;
    vector<double> tmp1, tmp2;
    vector<double> result;  
    for(i = 0; i < values.size(); i++)
    {
       tmp1.push_back(lotka_volterra(i, values, rate[i], interaction[i]));      //f(x(0),t)      
    }
    fields.push_back(tmp1);
    tmp2 = VectorPlusVector(values, VectorTimesScalar(tmp1,h_half));            //y = x(0) + f(x(0),t)*h/2
    tmp1.clear();

    for(i = 0; i < values.size(); i++)
        tmp1.push_back(lotka_volterra(i, tmp2, rate[i], interaction[i]));       //f(y,t) 
    fields.push_back(tmp1);  
    tmp2 = VectorPlusVector(values, VectorTimesScalar(tmp1,h_half));            //z = x(0) + f(y,t)*h/2
    tmp1.clear();

    for(i = 0; i < values.size(); i++)
        tmp1.push_back(lotka_volterra(i, tmp2, rate[i], interaction[i]));       //f(z,t) 

    fields.push_back(tmp1);  
    tmp2 = VectorPlusVector(values, VectorTimesScalar(tmp1,h_increment));       //w = x(0) + f(z,t)*h
    tmp1.clear();

    for(i = 0; i < values.size(); i++)
        tmp1.push_back(lotka_volterra(i, tmp2, rate[i], interaction[i]));       //f(w,t) 
    fields.push_back(tmp1);  
    
    fields[1] = VectorTimesScalar(fields[1], 2);           //f(y,t) -> 2*f(y,t)                            
    fields[2] = VectorTimesScalar(fields[2], 2);           //f(z,t) -> 2*f(z,t)
    fields[0] = VectorPlusVector(fields[0], fields[1]);    //f(x(0),t) + 2*f(y,t)                         
    fields[0] = VectorPlusVector(fields[0], fields[2]);    //f(x(0),t) + 2*f(y,t) + 2*f(z,t)
    fields[0] = VectorPlusVector(fields[0], fields[3]);    //f(x(0),t) + 2*f(y,t) + 2*f(z,t) + f(w,t)
    fields[0] = VectorTimesScalar(fields[0], h_six);       //[f(x(0),t) + 2*f(y,t) + 2*f(z,t) + f(w,t)]*h/6

    result = VectorPlusVector(values, fields[0]);          //x(0) + [f(x(0),t) + 2*f(y,t) + 2*f(z,t) + f(w,t)]*h/6                               
    return result;
}
