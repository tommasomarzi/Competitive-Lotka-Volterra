#include "../agent_based_model/LV.cpp"
#include "catch.hpp"
#include <iostream>

class LV_Fixture
{
    private:
    LV* simulation;
    int neighborhood(const int, const int);

    protected:
    int test_normalizer()
    {
        int result = simulation->normalizer();
        return result;
    }
    int test_normalizer(vector<int> presences, vector<bool> empty_cells, int species)
    {
        int result = simulation->normalizer(presences, empty_cells, species);
        return result;
    }
    int test_random_walk(vector<bool> empty_cells)
    {
        int result = simulation->random_walk(empty_cells);
        return result;
    }
    int test_filler(vector<int> presences)
    {
        int result = simulation->filler(presences);
        return result;        
    }

    public:
    LV_Fixture() 
    {
        simulation = new LV(10,10);
    }

    void setup() 
    {
        simulation->initializer_fill();
    }

    ~LV_Fixture( )  
    {}

};

TEST_CASE_METHOD(LV_Fixture, "Test LV class methods") 
{
    int result = test_normalizer();
    REQUIRE(result > -2);
}