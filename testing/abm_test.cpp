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
        for(int i = 1; i < pres.size(); i++) 
        {
            int rv;
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
    for(int n_sim = 0; n_sim < N_SIM_TEST; n_sim++)
    {
        int result;   
        int x;
        int y;
        bool check;

        
        //LV::neighborhood(int,int)
        x = rand() % row;
        y = rand() % col;
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


        //LV::normalizer(vector<int>, vector<bool>, int)
        int species = rand() % n_species;
        vector<bool> empty_cells = neighborhood_is_empty();
        check = false;

        result = test_normalizer(presences, empty_cells, species);
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
        if(check == false)
        {
            if((result > n_species) && (result < (n_species + 10)))
            {
                check = true;
            }
        }
        if(ENABLE_BIRTHS)
        {
            if(check == false)
            {
                if((result > (n_species + 100)) && (result < (n_species + 110)))
                {
                    check = true;
                }
            }
        }
        REQUIRE(result > -2);
        REQUIRE(check == true);


        //LV::random_walk(vector<bool>, int)
        int count_empty = 1;
        for(int i = 0; i < empty_cells.size(); i++)
        {
            if(empty_cells[i] == true)
            {
                count_empty++;
            }
        }
        result = test_random_walk(empty_cells, count_empty);
        REQUIRE(result > -1);
        REQUIRE(result < 10);
        if(result != 9)
        {
            REQUIRE(empty_cells[result] == true);
        }
    }
}