#include "../agent_based_model/LV.cpp"
#include "catch.hpp"
#include <iostream>

class LV_Fixture
{
    protected:
    vector<int> pres;
    LV* simulation;
    int n_species;
    int row,col;

    int test_neighborhood(int x, int y)
    {
        int result = simulation->neighborhood(x,y);
        return result;
    }

    int test_normalizer()
    {
        int result = simulation->normalizer();
        return result;
    }

    int test_filler(vector<int> presences)
    {
        int result = simulation->filler(presences);
        return result;        
    }

    int test_normalizer(vector<int> presences, vector<bool> empty_cells, int species)
    {
        int result = simulation->normalizer(presences, empty_cells, species);
        return result;
    }

    int test_random_walk(vector<bool> empty_cells, int count_empty)
    {
        int result = simulation->random_walk(empty_cells, count_empty);
        return result;
    }

    public:
    LV_Fixture() 
    {
        simulation = new LV(10,10);
        simulation->initializer_fill();
        n_species = simulation->values_zero.size();
        row = simulation->rows;
        col = simulation->columns;
    }

    vector<int> pres_fill() 
    {
        int tot = 8;
        vector<int> pres(n_species + 1,0);
        int rv;
        for(int i = 1; i < pres.size(); i++) 
        {
            rv = rand()% tot;
            pres[i] = rv;
            tot -= rv;     
        }
        pres[0] = tot;
        return pres;
    }

    vector<bool> neighborhood_is_empty() 
    {
        vector<bool> empty_cells(9,false);
        int rv;
        for(int i = 0; i < empty_cells.size(); i++) 
        {
            if(i == 4)
            {
                continue;
            }
            else
            {
                rv = (double)rand() / RAND_MAX;
                if(rv < 0.5)
                {
                    empty_cells[i] = true;
                }
            }
        }
        return empty_cells;
    }

    ~LV_Fixture( )  
    {}

};


TEST_CASE_METHOD(LV_Fixture, "Test LV class methods") 
{   
    int result;
    int x = rand() % row;
    int y = rand() % col;
    bool check;

    //LV::neighborhood(int,int)
    result = test_neighborhood(x,y);
    REQUIRE(result > -2);
    REQUIRE(result < n_species);
    
    //LV::normalizer()
    result = test_normalizer();
    REQUIRE(result > -2);
    REQUIRE(result < n_species);

    //LV::filler(vector<int>)
    vector<int> presences = pres_fill();
    result = test_filler(presences);
    check = false;
    for(int i = 0; i < presences.size(); i++)
    {
        if(presences[i] == 0)
        {
            continue;
        }
        else if(result == (i - 1))
        {
            check = true;
        }
    }
    REQUIRE(result > -2);
    REQUIRE(result < n_species);
    REQUIRE(check == true);

    //LV::normalizer(vector<int>, vector<int>, int)
    int species = rand() % n_species;
    vector<bool> empty_cells = neighborhood_is_empty();
    result = test_normalizer(presences, empty_cells, species);
    check = false;
    for(int i = 0; i < presences.size(); i++)
    {
        if(presences[i] == 0)
        {
            continue;
        }
        else if(result == (i - 1))
        {
            check = true;
        }
    }
    if(result == species)
    {
        check = true;
    }
    REQUIRE(result > -2);
    REQUIRE(result < n_species);
    REQUIRE(check == true);
}