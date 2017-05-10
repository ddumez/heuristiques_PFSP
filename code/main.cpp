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

/**
 * \file main.cpp
 * \author dorian dumez & Jérémie Dubois-Lacoste
 * \brief run diferent heuristic on the PFSP to make experiments
 */

// /home/dorian/R/x86_64-pc-linux-gnu-library/3.0/irace/bin/irace dans tunning
//./main --instance ./../instances/50_20_01 --tmax 45 --bestval 595260 --neighbourTabu 1 --tabuListLenght 10 --longTimeMemoryImpact 0.001 --restartThreshold 0.3 dans code pour tabu seul
//./main --instance ./../instances/50_20_01 --tmax 45 --bestval 595260 --neighbourILS 1 -- neighbourPerturb 2 --acceptanceCrit 2 --perturbFrac 0.05 --perturbRadius 0.05 dans code pour ILS crit 1 ou 2
//./main --instance ./../instances/50_20_01 --tmax 45 --bestval 595260 --neighbourILS 1 -- neighbourPerturb 2 --acceptanceCrit 3 --perturbFrac 0.05 --perturbRadius 0.05 --alpha 0.95 --T0 1000 --l 100 --warmupThreshold 80 --T1 500 dans code pour ILS crit 3
//./main --instance ./../instances/50_20_01 --tmax 45 --bestval 595260 --neighbourILS 1 -- neighbourPerturb 2 --acceptanceCrit 4 --perturbFrac 0.05 --perturbRadius 0.05 --lamda 0.5 dans code pour ILS crit 4

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <string.h>

#include "solution.hpp"
#include "pfspinstance.hpp"
#include "localsearch.hpp"
#include "tabu.hpp"
#include "ILS.hpp"

using namespace std;

#define NBEXEC 1

//#define RELATIVE_DEVIATION
#define SCORE
//#define EXECUTION_TIME

/**
* \brief improve the solution with the vnd indea with the 3 local search
*
* \param[in, out] sol the solution to improve
* \param[in] instance the context of hte solution
* \param[in] neighborhood table of diferent neighbohood relation to use, they should be ordered
* \param[in] nbneighborhood number of neighborhood to use
*/
void vnd(Solution & sol, PfspInstance & instance, LocalSearch neighborhoud [], int nbneighborhoud);

int main(int argc, char *argv[]) {
  //variable
    //mesurment variable
    #ifdef RELATIVE_DEVIATION
        long int tot;
    #endif
    #ifdef SCORE
        long int tot;
    #endif
    #ifdef EXECUTION_TIME
        clock_t t, tot;
    #endif
    PfspInstance instance; // Create instance object
    int i;
    Solution * solution;
    //parameter variable
    char *instanceFile; clock_t tmax; long bestval;
    int neighbourTabu; int tabuListLenght; double longTimeMemoryImpact; double restartThreshold;
    int neighbourILS; int neighboursPerturb; bool DD = false; int acceptanceCrit; double perturbFrac; double perturbRadius; double T0; double lambda; double alpha; long l; double warmupThreshold; double T1;

    //initialize random seed as constant
    //srand (0);
    //or as random
    srand(time(NULL));


  //start
    for(int i=1; i< argc ; i++){
        if(strcmp(argv[i], "--tabuListLenght") == 0){
           tabuListLenght = atoi(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--neighbourTabu") == 0){
           neighbourTabu = atoi(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--longTimeMemoryImpact") == 0){
           longTimeMemoryImpact = atof(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--restartThreshold") == 0){
           restartThreshold = atof(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--neighbourILS") == 0){
           neighbourILS = atoi(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--neighboursPerturb") == 0){
           neighboursPerturb = atoi(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--DD") == 0){
           DD = true;
        } else if(strcmp(argv[i], "--acceptanceCrit") == 0){
           acceptanceCrit = atoi(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--perturbFrac") == 0){
           perturbFrac = atof(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--perturbRadius") == 0){
           perturbRadius = atof(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--T0") == 0){
           T0 = atof(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--alpha") == 0){
           alpha = atof(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--lambda") == 0){
           lambda = atof(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--l") == 0){
           l = strtol(argv[i+1], NULL, 10);
           i++;
        } else if(strcmp(argv[i], "--warmupThreshold") == 0){
           warmupThreshold = atof(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--T1") == 0){
           T1 = atof(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--tmax") == 0) {
           tmax = atol(argv[i+1]);
           i++;
        } else if(strcmp(argv[i], "--instance") == 0) {
           instanceFile = argv[i+1];
           i++;
        } else if(strcmp(argv[i], "--bestval") == 0){
           bestval = strtol(argv[i+1], NULL, 10);
           i++;
        }
    }

    // Read data from file
    if (! instance.readDataFromFile(instanceFile) ) {
        return 1;
    }

    //experiment for the second part of the project
/*    #ifdef EXECUTION_TIME
        t = clock();
    #endif
    Tabu tabusearch(tabuListLenght, longTimeMemoryImpact, restartThreshold, &instance, neighbourTabu);
    solution = tabusearch.search(tmax);
    #ifdef RELATIVE_DEVIATION
        cout<<100*(double)(instance.computeWCT(*solution) - bestval)/(double)bestval<<":"<<flush;
    #endif
    #ifdef SCORE
        cout<<instance.computeWCT(*solution)<<":"<<flush;
    #endif
    #ifdef EXECUTION_TIME
        t = clock() - t;
        cout<<(double)((double)t/(double)(CLOCKS_PER_SEC))<<":"<<flush;
    #endif
    delete(solution);
*/
    #ifdef RELATIVE_DEVIATION
        tot = 0;
    #endif
    #ifdef SCORE
        tot = 0;
    #endif
    #ifdef EXECUTION_TIME
        tot = 0;
    #endif
    Ils iterativeLocalSearch;
    for(i = 0; i<NBEXEC; ++i) {
        #ifdef EXECUTION_TIME
            t = clock();
        #endif
        if (3 == acceptanceCrit) {
            iterativeLocalSearch.init(neighbourILS, neighboursPerturb, DD, 3, perturbFrac, perturbRadius, &instance, T0, alpha, l, warmupThreshold, T1);
        }else if (4 == acceptanceCrit) {
            for (int ii = instance.getNbJob(); ii>0; --ii) {
                for(int jj = instance.getNbMac(); jj>0; --jj) {
                    T0 += (double)(instance.getTime(ii, jj));
                }
            }
            T0 = lambda * (T0 / (10.0 * (double)(instance.getNbJob()) * (double)(instance.getNbMac())) );
            iterativeLocalSearch.init(neighbourILS, neighboursPerturb, DD, 4, perturbFrac, perturbRadius, &instance, T0);
        } else {
            iterativeLocalSearch.init(neighbourILS, neighboursPerturb, DD, acceptanceCrit, perturbFrac, perturbRadius, &instance);
        }
        solution = iterativeLocalSearch.search(tmax);
        solution->print();
        #ifdef RELATIVE_DEVIATION
            tot += instance.computeWCT(*solution);
        #endif
        #ifdef SCORE
            tot += instance.computeWCT(*solution);
        #endif
        #ifdef EXECUTION_TIME
            tot += clock() - t;
        #endif
        delete(solution);
    }
    #ifdef RELATIVE_DEVIATION
        cout<<100*(double)(tot/NBEXEC - bestval)/(double)(bestval)<<":"<<flush;
    #endif
    #ifdef SCORE
        cout<<tot/NBEXEC<<flush;
    #endif
    #ifdef EXECUTION_TIME
        cout<<(double)((double)tot/(double)(CLOCKS_PER_SEC*NBEXEC))<<":"<<flush;
    #endif
    cout<<endl;

/*
  //experiments for the first part of the project
    Solution sol (instance); // Create a Solution to represent the permutation
    LocalSearch search (1 , false, true);
    LocalSearch neighborhoud [3];

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
*/
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
