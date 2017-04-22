/***************************************************************************
 *   Copyright (C) 2012 by Jérémie Dubois-Lacoste   *
 *   jeremie.dl@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <time.h>

#include "solution.hpp"
#include "pfspinstance.hpp"
#include "localsearch.hpp"

using namespace std;

#define NBEXEC 1

//#define RELATIVE_DEVIATION
//#define SCORE
#define EXECUTION_TIME

/**
* \brief improve the solution with the vnd indea with the 3 local search
*
* \param[in, out] sol the solution to improve
* \param[in] instance the context of hte solution
* \param[in] first the first local search to use
* \param[in] second the second neighborhoud to use
* \param[in] third the last neighborhourd to use
*/
void vnd(Solution & sol, PfspInstance & instance, LocalSearch neighborhoud [], int nbneighborhoud);

int main(int argc, char *argv[]) {
  //variable
    int i;
    PfspInstance instance; // Create instance object
    LocalSearch search (1 , false, true);
    #ifdef RELATIVE_DEVIATION
        long int tot;
        long bestval = strtol(argv[2], NULL, 10);
    #endif
    #ifdef SCORE
        long int tot;
    #endif
    #ifdef EXECUTION_TIME
        clock_t t, tot;
    #endif
    LocalSearch neighborhoud [3];
  //start
    //initialize random seed as constant
    srand (0);

    if (argc == 1) {
        cout << "Usage: ./flowshopWCT <instance_file>" << endl;
        return 0;
    }

    // Read data from file
    if (! instance.readDataFromFile(argv[1]) ) {
        return 1;
    }

    Solution sol (instance); // Create a Solution to represent the permutation

    //random, transpose    
        #ifdef RELATIVE_DEVIATION
            tot = 0;
        #endif
        #ifdef SCORE
            tot = 0;
        #endif
        #ifdef EXECUTION_TIME
            tot = 0;
        #endif
        for(i = 0; i<NBEXEC; ++i) {
            #ifdef EXECUTION_TIME
                t = clock();
            #endif
            sol.randomPermutation();
            search.descent(instance, sol);
            #ifdef RELATIVE_DEVIATION
                tot += instance.computeWCT(sol);
            #endif
            #ifdef SCORE
                tot += instance.computeWCT(sol);
            #endif
            #ifdef EXECUTION_TIME
                tot += clock() - t;
            #endif
        }
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(tot/NBEXEC - bestval)/(double)(bestval)<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<tot/NBEXEC<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            cout<<(double)((double)tot/(double)(CLOCKS_PER_SEC*NBEXEC))<<":"<<flush;
        #endif

    //random, transpose PPD
        #ifdef RELATIVE_DEVIATION
            tot = 0;
        #endif
        #ifdef SCORE
            tot = 0;
        #endif
        #ifdef EXECUTION_TIME
            tot = 0;
        #endif
        search.changePPD(true);        
        for(i = 0; i<NBEXEC; ++i) {
            #ifdef EXECUTION_TIME
                t = clock();
            #endif
            sol.randomPermutation();
            search.descent(instance, sol);
            #ifdef RELATIVE_DEVIATION
                tot += instance.computeWCT(sol);
            #endif
            #ifdef SCORE
                tot += instance.computeWCT(sol);
            #endif
            #ifdef EXECUTION_TIME
                tot += clock() - t;
            #endif
        }
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(tot/NBEXEC - bestval)/(double)(bestval)<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<tot/NBEXEC<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            cout<<(double)((double)tot/(double)(CLOCKS_PER_SEC*NBEXEC))<<":"<<flush;
        #endif

    //random, exchange
        #ifdef RELATIVE_DEVIATION
            tot = 0;
        #endif
        #ifdef SCORE
            tot = 0;
        #endif
        #ifdef EXECUTION_TIME
            tot = 0;
        #endif
        search.changechoix(2);
        search.changePPD(false);
        for(i = 0; i<NBEXEC; ++i) {
            #ifdef EXECUTION_TIME
                t = clock();
            #endif
            sol.randomPermutation();
            search.descent(instance, sol);
            #ifdef RELATIVE_DEVIATION
                tot += instance.computeWCT(sol);
            #endif
            #ifdef SCORE
                tot += instance.computeWCT(sol);
            #endif
            #ifdef EXECUTION_TIME
                tot += clock() - t;
            #endif
        }
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(tot/NBEXEC - bestval)/(double)(bestval)<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<tot/NBEXEC<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            cout<<(double)((double)tot/(double)(CLOCKS_PER_SEC*NBEXEC))<<":"<<flush;
        #endif

    //random, exchange PPD
        #ifdef RELATIVE_DEVIATION
            tot = 0;
        #endif
        #ifdef SCORE
            tot = 0;
        #endif
        #ifdef EXECUTION_TIME
            tot = 0;
        #endif
        search.changePPD(true);
        for(i = 0; i<NBEXEC; ++i) {
            #ifdef EXECUTION_TIME
                t = clock();
            #endif
            sol.randomPermutation();
            search.descent(instance, sol);
            #ifdef RELATIVE_DEVIATION
                tot += instance.computeWCT(sol);
            #endif
            #ifdef SCORE
                tot += instance.computeWCT(sol);
            #endif
            #ifdef EXECUTION_TIME
                tot += clock() - t;
            #endif
        }
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(tot/NBEXEC - bestval)/(double)(bestval)<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<tot/NBEXEC<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            cout<<(double)((double)tot/(double)(CLOCKS_PER_SEC*NBEXEC))<<":"<<flush;
        #endif

    //random, insert
        #ifdef RELATIVE_DEVIATION
            tot = 0;
        #endif
        #ifdef SCORE
            tot = 0;
        #endif
        #ifdef EXECUTION_TIME
            tot = 0;
        #endif
        search.changechoix(3);
        search.changePPD(false);
        for(i = 0; i<NBEXEC; ++i) {
            #ifdef EXECUTION_TIME
                t = clock();
            #endif
            sol.randomPermutation();
            search.descent(instance, sol);
            #ifdef RELATIVE_DEVIATION
                tot += instance.computeWCT(sol);
            #endif
            #ifdef SCORE
                tot += instance.computeWCT(sol);
            #endif
            #ifdef EXECUTION_TIME
                tot += clock() - t;
            #endif
        }
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(tot/NBEXEC - bestval)/(double)(bestval)<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<tot/NBEXEC<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            cout<<(double)((double)tot/(double)(CLOCKS_PER_SEC*NBEXEC))<<":"<<flush;
        #endif

    //random, insert PPD
        #ifdef RELATIVE_DEVIATION
            tot = 0;
        #endif
        #ifdef SCORE
            tot = 0;
        #endif
        #ifdef EXECUTION_TIME
            tot = 0;
        #endif
        search.changePPD(true);
        for(i = 0; i<NBEXEC; ++i) {
            #ifdef EXECUTION_TIME
                t = clock();
            #endif
            sol.randomPermutation();
            search.descent(instance, sol);
            #ifdef RELATIVE_DEVIATION
                tot += instance.computeWCT(sol);
            #endif
            #ifdef SCORE
                tot += instance.computeWCT(sol);
            #endif
            #ifdef EXECUTION_TIME
                tot += clock() - t;
            #endif
        }
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(tot/NBEXEC - bestval)/(double)(bestval)<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<tot/NBEXEC<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            cout<<(double)((double)tot/(double)(CLOCKS_PER_SEC*NBEXEC))<<":"<<flush;
        #endif

    //next are deterministic, so one run is enought
    //rz, transpose
        search.changechoix(1);
        search.changePPD(false);
        #ifdef EXECUTION_TIME
            t = clock();
        #endif
        sol.constructRZ(instance);
        search.descent(instance, sol);
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<instance.computeWCT(sol)<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            t = clock() - t;
            cout<<(double)((double)t/(double)(CLOCKS_PER_SEC))<<":"<<flush;
        #endif

    //rz, transpose PPD
        search.changePPD(true);
        #ifdef EXECUTION_TIME
            t = clock();
        #endif
        sol.constructRZ(instance);
        search.descent(instance, sol);
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<instance.computeWCT(sol)<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            t = clock() - t;
            cout<<(double)((double)t/(double)(CLOCKS_PER_SEC))<<":"<<flush;
        #endif

    //rz, exchange
        search.changechoix(2);
        search.changePPD(false);
        #ifdef EXECUTION_TIME
            t = clock();
        #endif
        sol.constructRZ(instance);
        search.descent(instance, sol);
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<instance.computeWCT(sol)<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            t = clock() - t;
            cout<<(double)((double)t/(double)(CLOCKS_PER_SEC))<<":"<<flush;
        #endif

    //rz, exchange PPD
        search.changePPD(true);
        #ifdef EXECUTION_TIME
            t = clock();
        #endif
        sol.constructRZ(instance);
        search.descent(instance, sol);
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<instance.computeWCT(sol)<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            t = clock() - t;
            cout<<(double)((double)t/(double)(CLOCKS_PER_SEC))<<":"<<flush;
        #endif

    //rz, insert
        search.changechoix(3);
        search.changePPD(false);
        #ifdef EXECUTION_TIME
            t = clock();
        #endif
        sol.constructRZ(instance);
        search.descent(instance, sol);
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<instance.computeWCT(sol)<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            t = clock() - t;
            cout<<(double)((double)t/(double)(CLOCKS_PER_SEC))<<":"<<flush;
        #endif

    //rz, insert PPD
        search.changePPD(true);
        #ifdef EXECUTION_TIME
            t = clock();
        #endif
        sol.constructRZ(instance);
        search.descent(instance, sol);
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<instance.computeWCT(sol)<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            t = clock() - t;
            cout<<(double)((double)t/(double)(CLOCKS_PER_SEC))<<":"<<flush;
        #endif

    //tests of VND
    //transpose->exchange->insert random
        neighborhoud[0].changechoix(1); neighborhoud[0].changePPD(false); neighborhoud[0].changedofor(true);
        neighborhoud[1].changechoix(2); neighborhoud[1].changePPD(false); neighborhoud[1].changedofor(true);
        neighborhoud[2].changechoix(3); neighborhoud[2].changePPD(false); neighborhoud[2].changedofor(true);
        #ifdef RELATIVE_DEVIATION
            tot = 0;
        #endif
        #ifdef SCORE
            tot = 0;
        #endif
        #ifdef EXECUTION_TIME
            tot = 0;
        #endif
        for(i = 0; i<NBEXEC; ++i) {
            #ifdef EXECUTION_TIME
                t = clock();
            #endif
            sol.randomPermutation();
            vnd(sol, instance, neighborhoud, 3);
            #ifdef RELATIVE_DEVIATION
                tot += instance.computeWCT(sol);
            #endif
            #ifdef SCORE
                tot += instance.computeWCT(sol);
            #endif
            #ifdef EXECUTION_TIME
                tot += clock() - t;
            #endif
        }
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(tot/NBEXEC - bestval)/(double)(bestval)<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<tot/NBEXEC<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            cout<<(double)((double)tot/(double)(CLOCKS_PER_SEC*NBEXEC))<<":"<<flush;
        #endif

    //transpose->exchange->insert PPD random
        neighborhoud[0].changePPD(true); neighborhoud[1].changePPD(true); neighborhoud[2].changePPD(true);
        #ifdef RELATIVE_DEVIATION
            tot = 0;
        #endif
        #ifdef SCORE
            tot = 0;
        #endif
        #ifdef EXECUTION_TIME
            tot = 0;
        #endif
        for(i = 0; i<NBEXEC; ++i) {
            #ifdef EXECUTION_TIME
                t = clock();
            #endif
            sol.randomPermutation();
            vnd(sol, instance, neighborhoud, 3);
            #ifdef RELATIVE_DEVIATION
                tot += instance.computeWCT(sol);
            #endif
            #ifdef SCORE
                tot += instance.computeWCT(sol);
            #endif
            #ifdef EXECUTION_TIME
                tot += clock() - t;
            #endif
        }
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(tot/NBEXEC - bestval)/(double)(bestval)<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<tot/NBEXEC<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            cout<<(double)((double)tot/(double)(CLOCKS_PER_SEC*NBEXEC))<<":"<<flush;
        #endif

    //transpose->exchange->insert rz
        neighborhoud[0].changePPD(false); neighborhoud[1].changePPD(false); neighborhoud[2].changePPD(false);
        #ifdef EXECUTION_TIME
            t = clock();
        #endif
        sol.constructRZ(instance);
        vnd(sol, instance, neighborhoud, 3);
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<instance.computeWCT(sol)<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            t = clock() - t;
            cout<<(double)((double)t/(double)(CLOCKS_PER_SEC))<<":"<<flush;
        #endif

    //transpose->exchange->insert PPD rz
        neighborhoud[0].changePPD(true); neighborhoud[1].changePPD(true); neighborhoud[2].changePPD(true);
        #ifdef EXECUTION_TIME
            t = clock();
        #endif
        sol.constructRZ(instance);
        vnd(sol, instance, neighborhoud, 3);
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<instance.computeWCT(sol)<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            t = clock() - t;
            cout<<(double)((double)t/(double)(CLOCKS_PER_SEC))<<":"<<flush;
        #endif

    //transpose->insert->exhange random
        neighborhoud[0].changePPD(false);
        neighborhoud[1].changechoix(3); neighborhoud[1].changePPD(false);
        neighborhoud[2].changechoix(2); neighborhoud[2].changePPD(false);
        #ifdef RELATIVE_DEVIATION
            tot = 0;
        #endif
        #ifdef SCORE
            tot = 0;
        #endif
        #ifdef EXECUTION_TIME
            tot = 0;
        #endif
        for(i = 0; i<NBEXEC; ++i) {
            #ifdef EXECUTION_TIME
                t = clock();
            #endif
            sol.randomPermutation();
            vnd(sol, instance, neighborhoud, 3);
            #ifdef RELATIVE_DEVIATION
                tot += instance.computeWCT(sol);
            #endif
            #ifdef SCORE
                tot += instance.computeWCT(sol);
            #endif
            #ifdef EXECUTION_TIME
                tot += clock() - t;
            #endif
        }
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(tot/NBEXEC - bestval)/(double)(bestval)<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<tot/NBEXEC<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            cout<<(double)((double)tot/(double)(CLOCKS_PER_SEC*NBEXEC))<<":"<<flush;
        #endif

    //transpose->insert->exhange PPD random
        neighborhoud[0].changePPD(true); neighborhoud[1].changePPD(true); neighborhoud[2].changePPD(true);
        #ifdef RELATIVE_DEVIATION
            tot = 0;
        #endif
        #ifdef SCORE
            tot = 0;
        #endif
        #ifdef EXECUTION_TIME
            tot = 0;
        #endif
        for(i = 0; i<NBEXEC; ++i) {
            #ifdef EXECUTION_TIME
                t = clock();
            #endif
            sol.randomPermutation();
            vnd(sol, instance, neighborhoud, 3);
            #ifdef RELATIVE_DEVIATION
                tot += instance.computeWCT(sol);
            #endif
            #ifdef SCORE
                tot += instance.computeWCT(sol);
            #endif
            #ifdef EXECUTION_TIME
                tot += clock() - t;
            #endif
        }
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(tot/NBEXEC - bestval)/(double)(bestval)<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<tot/NBEXEC<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            cout<<(double)((double)tot/(double)(CLOCKS_PER_SEC*NBEXEC))<<":"<<flush;
        #endif

    //transpose->insert->exhange rz
        neighborhoud[0].changePPD(false); neighborhoud[1].changePPD(false); neighborhoud[2].changePPD(false);
        #ifdef EXECUTION_TIME
            t = clock();
        #endif
        sol.constructRZ(instance);
        vnd(sol, instance, neighborhoud, 3);
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
        #endif
        #ifdef SCORE
            cout<<instance.computeWCT(sol)<<":"<<flush;
        #endif
        #ifdef EXECUTION_TIME
            t = clock() - t;
            cout<<(double)((double)t/(double)(CLOCKS_PER_SEC))<<":"<<flush;
        #endif

    //transpose->insert->exhange PPD rz
        neighborhoud[0].changePPD(true); neighborhoud[1].changePPD(true); neighborhoud[2].changePPD(true);
        #ifdef EXECUTION_TIME
            t = clock();
        #endif
        sol.constructRZ(instance);
        vnd(sol, instance, neighborhoud, 3);
        #ifdef RELATIVE_DEVIATION
            cout<<100*(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<endl;
        #endif
        #ifdef SCORE
            cout<<instance.computeWCT(sol)<<endl;
        #endif
        #ifdef EXECUTION_TIME
            t = clock() - t;
            cout<<(double)((double)t/(double)(CLOCKS_PER_SEC))<<endl;
        #endif

  //end
  return 0;
}

void vnd(Solution & sol, PfspInstance & instance, LocalSearch neighborhoud [], int nbneighborhoud) {
    int i = 0;
    while (i < nbneighborhoud) {
        if (0 == i) {
            neighborhoud[i].descent(instance,sol); //enable the dofor if he is activated on the neighborhoud
            ++i; //now we are in a local minima of the first neighborhoud
        }else if (neighborhoud[i].search(instance,sol)) {
            i = 0;
        }  else {
            ++i;
        }
    }
}