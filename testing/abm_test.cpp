#include "../agent_based_model/LV.cpp"
#include "catch.hpp"
#include <iostream>


/*
Friend class of LV.
Its methods allow to call the methods of LV.
*/
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

        if(!DEBUG_MODE)
        {
            cout<<"WARNING: the program is running in normal mode."<<endl;
            cout<<"Please set the DEBUG_MODE variable of the setup.h file to true in order to fix the srand seed."<<endl<<endl;
        }
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

    vector<bool> neighborhood_is_empty(vector<int> presences)
    {
        vector<bool> empty_cells(9,false);
        empty_cells[4] = false;
        int occupied_cells = 8 - presences[0];
        for(int i = 0; i < occupied_cells; i++)
        {
            if(i < 4)
            {
                empty_cells[i] = false;
            }
            else
            {
                empty_cells[i+1] = false;
            }
        }
        return empty_cells;
    }

    ~LV_Fixture( )  
    {}

};


/*
Test the methods of the LV class of the agent based model.

The parameter N_ABM_TEST (defined in the setup.h file) represents the number of times that the methods
of the LV class are tested.
*/
TEST_CASE_METHOD(LV_Fixture, "Test LV class methods") 
{
    for(int n_sim = 0; n_sim < N_ABM_TEST; n_sim++)
    {
        int result;   
        int x;
        int y;
        bool check;


        /*
        LV::neighborhood(int,int)

        verify that the function returns a valid value,
        i.e. it is a value greater than -1 (we have to take into account of the movements
        and possible births).
        */
        x = rand() % row;
        y = rand() % col;
        result = test_neighborhood(x,y);
        REQUIRE(result > -2);


        /*
        LV::normalizer()

        verify that the function returns a valid value,
        i.e. it is -1 (empty cell) or a value corresponding to a species (between 0 and n_species - 1).
        */
        result = test_normalizer();
        REQUIRE(result > -2);
        REQUIRE(result < n_species);


        /*
        LV::filler(vector<int>)

        once a vector representing the occupation of the neighborhood is defined,
        verify that the result (i.e. the value of the empty cell at the next iteration) is a valid value, i.e.:
        1) it is -1 (empty cell) or a value corresponding to a species (between 0 and n_species - 1)
        2) it can be generated from the neighborhood
        */
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


        /*
        LV::normalizer(vector<int>, vector<bool>, int)

        once a boolean vector representing the spatial occupation of the neighborhood is defined,
        verify that the result (i.e. the value of the empty cell at the next iteration) is a valid value, i.e.:
        1) it is -1 (empty cell) after the death of the individual
        2) it remains the same value (no interaction)
        3) it moves to an empty cell
        4) if the births are enabled, it generates a new individual in the neighrborhood
        */
        int species = rand() % n_species;
        vector<bool> empty_cells = neighborhood_is_empty(presences);
        check = false;

        result = test_normalizer(presences, empty_cells, species);

        if(result == -1)                                                //A + X -> 0 + X
        {
            check = true;
        }
        else if(result == species)                                      //A + 0 -> A + 0
        {
            check = true;
        }
        else if((result > n_species) && (result < (n_species + 10)))    //A + 0 -> 0 + A
        {
            check = true;
            int to_cell = result - (n_species + 1);
            REQUIRE(empty_cells[to_cell] == true);
        }
        else if(ENABLE_BIRTHS)                                          //A + 0 -> A + A
        {
            if((result > (n_species + 100)) && (result < (n_species + 110)))
            {
                check = true;
                int to_cell = result - (n_species + 1 + 100);
                REQUIRE(empty_cells[to_cell] == true);
            }
        }

        REQUIRE(result > -2);
        REQUIRE(check == true);


        /*
        LV::random_walk(vector<bool>, int)

        once we verified that there is at least one empty cell in the neighborhood,
        check that the result is a valid value, i.e.:
        1) it is a valid value (between 0 and 9, different from 4)
        2) if the individual moves to an empty cell (i.e. result != 9)
        in the case in which there are no empty cells, empty the cell 0 and (if the individual moves)
        verify that it moves to that cell
        */
        int count_empty = 0;
        for(int i = 0; i < empty_cells.size(); i++)
        {
            if(empty_cells[i] == true)
            {
                count_empty++;
            }
        }
        if(count_empty == 0)
        {
            empty_cells[0] = true;
        }

        result = test_random_walk(empty_cells, count_empty);
        REQUIRE(result > -1);
        REQUIRE(result < 10);
        if(result != 9)
        {
            REQUIRE(empty_cells[result] == true);
            if(count_empty == 0)
            {
                REQUIRE(result == 0);
            }
        }
    }
}