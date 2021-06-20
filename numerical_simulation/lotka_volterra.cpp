#include <iostream>
#include <cmath>
#include <vector>
#include <functional>
#include <sstream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "../utilities/reader.cpp"

using namespace std;


double h = 0.01;
int iter_max = 100000;
double h_half = h/2;
double h_six = h/6;



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
       tmp1.push_back(lotka_volterra(i, values, rate[i], interaction[i]));      //f(x,t)      
    }
    fields.push_back(tmp1);
    tmp2 = VectorPlusVector(values, VectorTimesScalar(tmp1,h_half));            //y 
    tmp1.clear();

    for(i = 0; i < values.size(); i++)
        tmp1.push_back(lotka_volterra(i, tmp2, rate[i], interaction[i]));       //f(y,t) 
    fields.push_back(tmp1);  
    tmp2 = VectorPlusVector(values, VectorTimesScalar(tmp1,h_half));            //z
    tmp1.clear();

    for(i = 0; i < values.size(); i++)
        tmp1.push_back(lotka_volterra(i, tmp2, rate[i], interaction[i]));       //f(z,t) 

    fields.push_back(tmp1);  
    tmp2 = VectorPlusVector(values, VectorTimesScalar(tmp1,h));                 //w
    tmp1.clear();

    for(i = 0; i < values.size(); i++)
        tmp1.push_back(lotka_volterra(i, tmp2, rate[i], interaction[i]));       //f(w,t) 
    fields.push_back(tmp1);  
    

    fields[1] = VectorTimesScalar(fields[1], 2);
    fields[2] = VectorTimesScalar(fields[2], 2);
    fields[0] = VectorPlusVector(fields[0], fields[1]);
    fields[0] = VectorPlusVector(fields[0], fields[2]);
    fields[0] = VectorPlusVector(fields[0], fields[3]);
    fields[0] = VectorTimesScalar(fields[0], h_six);

    result = VectorPlusVector(values, fields[0]);
    return result;
}



int main()
{
    ofstream toplot;
    vector<vector<double>> interaction;
    vector<double> values_zero, values, rates;
    string outfile = "../data/4-species-roques/output.txt";
    string filematrix = "../data/4-species-roques/matrix.txt";
    string filevalues = "../data/4-species-roques/values.txt";
    string filerates = "../data/4-species-roques/rates.txt";
    toplot.open (outfile);
    MatrixReader(interaction, filematrix);
    VectorReader(values_zero, filevalues);
    VectorReader(rates, filerates);
    /*
    for (auto el : values)  cout <<el<< ' ';
    cout<<endl;
    for (auto el : rates)  cout <<el<< ' ';
    cout<<endl;
    for (auto row : interaction) {
    for (auto el : row) {
        cout << el << ' ';
    }
    cout << "\n";
    }
    */
    for ( int n = 0; n < iter_max; ++n)	
    {
    	values = runge_kutta(values_zero, rates, interaction);    
        values_zero = values;
        for (auto el : values)  
        {
            toplot <<el<< ' ';
        }
        toplot<<endl;
    }
    toplot.close();
    system("/usr/bin/python3 macro.py");

}
