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

#include "solution.hpp"
#include "pfspinstance.hpp"
#include "localsearch.hpp"

using namespace std;

#define NBEXEC 10

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
    long int tot;
    long bestval = strtol(argv[2], NULL, 10);
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
        tot = 0;
        for(i = 0; i<NBEXEC; ++i) {
            sol.randomPermutation();
            search.descent(instance, sol);
            tot += instance.computeWCT(sol);
        }

        cout<<(double)(tot/i - bestval)/(double)(bestval)<<":"<<flush;
    //random, transpose PPD
        tot = 0;
        search.changePPD(true);        
        for(i = 0; i<NBEXEC; ++i) {
            sol.randomPermutation();
            search.descent(instance, sol);
            tot += instance.computeWCT(sol);
        }
        cout<<(double)(tot/i - bestval)/(double)(bestval)<<":"<<flush;

    //random, exchange
        tot = 0;
        search.changechoix(2);
        search.changePPD(false);
        for(i = 0; i<NBEXEC; ++i) {
            sol.randomPermutation();
            search.descent(instance, sol);
            tot += instance.computeWCT(sol);
        }
        cout<<(double)(tot/i - bestval)/(double)(bestval)<<":"<<flush;

    //random, exchange PPD
        tot = 0;
        search.changePPD(true);
        for(i = 0; i<NBEXEC; ++i) {
            sol.randomPermutation();
            search.descent(instance, sol);
            tot += instance.computeWCT(sol);
        }
        cout<<(double)(tot/i - bestval)/(double)(bestval)<<":"<<flush;

    //random, insert
        tot = 0;
        search.changechoix(3);
        search.changePPD(false);
        for(i = 0; i<NBEXEC; ++i) {
            sol.randomPermutation();
            search.descent(instance, sol);
            tot += instance.computeWCT(sol);
        }
        cout<<(double)(tot/i - bestval)/(double)(bestval)<<":"<<flush;

    //random, insert PPD
        tot = 0;
        search.changePPD(true);
        for(i = 0; i<NBEXEC; ++i) {
            sol.randomPermutation();
            search.descent(instance, sol);
            tot += instance.computeWCT(sol);
        }
        cout<<(double)(tot/i - bestval)/(double)(bestval)<<":"<<flush;

    //next are deterministic, so one run is enought
    //rz, transpose
        sol.constructRZ(instance);
        search.changechoix(1);
        search.changePPD(false);
        search.descent(instance, sol);
        cout<<(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;

    //rz, transpose PPD
        sol.constructRZ(instance);
        search.changePPD(true);
        search.descent(instance, sol);
        cout<<(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;

    //rz, exchange
        sol.constructRZ(instance);
        search.changechoix(2);
        search.changePPD(false);
        search.descent(instance, sol);
        cout<<(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;

    //rz, exchange PPD
        sol.constructRZ(instance);
        search.changePPD(true);
        search.descent(instance, sol);
        cout<<(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;

    //rz, insert
        sol.constructRZ(instance);
        search.changechoix(3);
        search.changePPD(false);
        search.descent(instance, sol);
        cout<<(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;

    //rz, insert PPD
        sol.constructRZ(instance);
        search.changePPD(true);
        search.descent(instance, sol);
        cout<<(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;

    //tests of VND
    //transpose->exchange->insert random
        neighborhoud[0].changechoix(1); neighborhoud[0].changePPD(false); neighborhoud[0].changedofor(true);
        neighborhoud[1].changechoix(2); neighborhoud[1].changePPD(false); neighborhoud[1].changedofor(true);
        neighborhoud[2].changechoix(3); neighborhoud[2].changePPD(false); neighborhoud[2].changedofor(true);
        tot = 0;
        for(i = 0; i<NBEXEC; ++i) {
            sol.randomPermutation();
            vnd(sol, instance, neighborhoud, 3);
            tot += instance.computeWCT(sol);
        }
        cout<<(double)(tot/i - bestval)/(double)(bestval)<<":"<<flush;
    //transpose->exchange->insert PPD random
        neighborhoud[0].changePPD(true); neighborhoud[1].changePPD(true); neighborhoud[2].changePPD(true);
        tot = 0;
        for(i = 0; i<NBEXEC; ++i) {
            sol.randomPermutation();
            vnd(sol, instance, neighborhoud, 3);
            tot += instance.computeWCT(sol);
        }
        cout<<(double)(tot/i - bestval)/(double)(bestval)<<":"<<flush;
    //transpose->exchange->insert rz
        neighborhoud[0].changePPD(false); neighborhoud[1].changePPD(false); neighborhoud[2].changePPD(false);
        sol.constructRZ(instance);
        vnd(sol, instance, neighborhoud, 3);
        cout<<(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
    //transpose->exchange->insert PPD rz
        neighborhoud[0].changePPD(true); neighborhoud[1].changePPD(true); neighborhoud[2].changePPD(true);
        sol.constructRZ(instance);
        vnd(sol, instance, neighborhoud, 3);
        cout<<(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
    //transpose->insert->exhange random
        neighborhoud[0].changePPD(false);
        neighborhoud[1].changechoix(3); neighborhoud[1].changePPD(false);
        neighborhoud[2].changechoix(2); neighborhoud[2].changePPD(false);
        tot = 0;
        for(i = 0; i<NBEXEC; ++i) {
            sol.randomPermutation();
            vnd(sol, instance, neighborhoud, 3);
            tot += instance.computeWCT(sol);
        }
        cout<<(double)(tot/i - bestval)/(double)(bestval)<<":"<<flush;
    //transpose->insert->exhange PPD random
        neighborhoud[0].changePPD(true); neighborhoud[1].changePPD(true); neighborhoud[2].changePPD(true);
        tot = 0;
        for(i = 0; i<NBEXEC; ++i) {
            sol.randomPermutation();
            vnd(sol, instance, neighborhoud, 3);
            tot += instance.computeWCT(sol);
        }
        cout<<(double)(tot/i - bestval)/(double)(bestval)<<":"<<flush;
    //transpose->insert->exhange rz
        neighborhoud[0].changePPD(false); neighborhoud[1].changePPD(false); neighborhoud[2].changePPD(false);
        sol.constructRZ(instance);
        vnd(sol, instance, neighborhoud, 3);
        cout<<(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<flush;
    //transpose->insert->exhange PPD rz
        neighborhoud[0].changePPD(true); neighborhoud[1].changePPD(true); neighborhoud[2].changePPD(true);
        sol.constructRZ(instance);
        vnd(sol, instance, neighborhoud, 3);
        cout<<(double)(instance.computeWCT(sol) - bestval)/(double)bestval<<":"<<endl;

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