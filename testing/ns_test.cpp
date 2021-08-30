#include "../numerical_simulation/lotka_volterra.cpp"
#include "../utilities/reader.cpp"
#include "catch.hpp"
#include <iostream>
#include <cmath>


/*
Test vector operations (addition and multiplication by a scalar).
*/
SCENARIO("Vector operations","[vector]")
{
    srand(1);
    GIVEN("a vector with a certain shape and a scalar value")
    {
        int size = (rand() % 10) + 1;
        double value = (double)rand() / RAND_MAX;
        vector<double> initial_vec(size,value);
        double scalar = (double)rand() / RAND_MAX;
        vector<double> result_prod;

        WHEN("the vector is multiplied by the scalar")
        {
            result_prod = VectorTimesScalar(initial_vec, scalar);
            THEN("the resulting vector has the same shape and its values are the result of the product")
            {
                REQUIRE(result_prod.size() == initial_vec.size());
                for(int i = 0; i < result_prod.size(); i++)
                {
                    REQUIRE(result_prod[i] == initial_vec[i]*scalar);
                }

            }
        }
        WHEN("the vector is added to itself")
        {
            vector<double> result_sum;
            result_sum = VectorPlusVector(initial_vec, initial_vec);
            THEN("the resulting vector has the same shape and its values are the result of the sum")
            {
                REQUIRE(result_sum.size() == initial_vec.size());
                for(int i = 0; i < result_sum.size(); i++)
                {
                    REQUIRE(result_sum[i] == (initial_vec[i] + initial_vec[i]));
                }
            }
        }
    }
}


/*
Test the Runge-Kutta 4 algorithm for Lotka-Volterra in the one-species case.

As the theory regarding the RK4 algorithm explains, the local truncation error is of the order 
of O(h^5) and it generates a global truncation error of the order of O(h^4).
Therefore, it is required that those error are less than h^4 and h^3 respectively.

The parameter N_NS_TEST (defined in the setup.h file) represents the number of iteration of the model.
*/
TEST_CASE("Runge-Kutta for Lotka-Volterra", "[numerical]")
{
    vector<vector<double>> interaction;
    vector<double> values_ns, values_exact;
    vector<double> values_zero, values_old;
    vector<double> rates;

    MatrixReader(interaction, "data/1-species/matrix.txt");
    VectorReader(values_zero, "data/1-species/values.txt");
    VectorReader(rates,       "data/1-species/rates.txt");

    values_old = values_zero;
    values_exact = values_zero;

    double global_difference = 0.0;

    double local_error = pow(h_increment,4);
    double global_error = pow(h_increment,3);

    for(int t = 1; t < N_NS_TEST; t++)
    {
        values_exact[0] = 1.0/(1+ (1/values_zero[0] -1)*exp(-h_increment*t*rates[0]));
        values_ns = runge_kutta(values_old, rates, interaction);

        double local_difference = abs(values_ns[0] - values_exact[0]);
        REQUIRE(local_difference < local_error);

        global_difference += local_difference;
        values_old = values_ns;
    }

    REQUIRE(global_difference < global_error);
}