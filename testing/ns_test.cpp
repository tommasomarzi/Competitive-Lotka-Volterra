#include "../numerical_simulation/lotka_volterra.cpp"
#include "catch.hpp"
#include <iostream>


SCENARIO("Vector operations","[vector]")
{
    GIVEN("a vector with a certain shape and a scalar value")
    {
        std::vector<double> initial_vec(10,20);
        std::vector<double> result_prod;
        double scalar = 1.2;
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
            std::vector<double> result_sum;
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