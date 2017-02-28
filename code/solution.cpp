#include <vector>
#include <cstdlib>
#include <iostream>

#include "solution.hpp"
#include "pfspinstance.hpp"

using namespace std;

Solution::Solution(PfspInstance & instance) {
    nbJobs = instance.getNbJob();
    solution.resize(nbJobs);

    enddate.resize(nbJobs+1); // 1st dimension
    for (int cpt = 0; cpt <= nbJobs; ++cpt) {
        enddate[cpt].resize(instance.getNbMac()+1); // 2nd dimension
        enddate[cpt][0] = 0; //all jobs are available from the start
    }
    for (int cpt = 0; cpt <= instance.getNbMac(); ++cpt) {
        enddate[0][cpt] = 0; //all machines are available from the start
    }
}

Solution::~Solution() {}

int Solution::getJ(int j) {
    return solution[j];
}

void Solution::setJ(int i, int j) {
    solution[i] = j;
}

void Solution::randomPermutation() {
    vector<bool> alreadyTaken(nbJobs, false); // nbJobs elements with value false

    int rnd, i, j, nbFalse;

    for (i = nbJobs; i > 0; --i) { //place the job i
        rnd = generateRndPosition(1, i);

        // find the rndth cell with value = false
        j = 0; nbFalse = 0;
        while (nbFalse < rnd) {
            if ( ! alreadyTaken[j] ) {
                ++nbFalse;
            }
            ++j;
        }
        --j;

        solution[j] = i;

        alreadyTaken[j] = true;
    }
}

void Solution::constructRZ(PfspInstance & instance) {
  //variable
    vector<double> T (nbJobs); //to calculate the weighted sum of processing time
    vector<int> jobtoplace (nbJobs); //to order jobs
    int i,j, best;
    long int tmp, valpos; //thes two sometime store the score
    vector<int> timeprec (instance.getNbMac()); //end date on this machine of the previous job

  //start
    //calculate weighted sum of processing time
    for (i = 0; i<nbJobs; ++i) { //i on jobs
        T[i] = 0;
        jobtoplace[i] = i+1; //initialize this array
        for (j = 0; j<instance.getNbMac(); ++j) { //j on machines
            T[i] += (double) instance.getTime(i+1, j+1);
        }
        T[i] = T[i] / (double) (instance.getPriority(i+1));
    }

    //order job
    for(i = 0; i<nbJobs-1; ++i) {
        for(j = 0; j<nbJobs-1-i; ++j) {
            if (T[jobtoplace[i]] < T[jobtoplace[i+1]]) {
                tmp = jobtoplace[i];
                jobtoplace[i] = jobtoplace[i+1];
                jobtoplace[i+1] = tmp;
            }
        }
    }

    //choose between the two first possibility
        // first possibility (jobtoplace[0], jobtoplace[1]) => tmp
    timeprec[0] = instance.getTime( jobtoplace[0] , 1); //the first machine always exist
    for(j = 1; j<instance.getNbMac(); ++j) { //procedure for other machine
        timeprec[j] = timeprec[j-1] + instance.getTime( jobtoplace[0] , j+1);
    }
    tmp = timeprec[instance.getNbMac()-1] * instance.getPriority(jobtoplace[0]);
    timeprec[0] = timeprec[0] + instance.getTime( jobtoplace[1] , 1);
    for(j = 1; j<instance.getNbMac(); ++j) {
        timeprec[j] = max(timeprec[j-1], timeprec[j]) + instance.getTime( jobtoplace[1] , j+1);
    }
    tmp += timeprec[instance.getNbMac()-1] * instance.getPriority(jobtoplace[1]);
        //second possibility (jobtoplace[1], jobtoplace[0]) => valpos
    timeprec[1] = instance.getTime( jobtoplace[1] , 1); //the first machine always exist
    for(j = 1; j<instance.getNbMac(); ++j) { //procedure for other machine
        timeprec[j] = timeprec[j-1] + instance.getTime( jobtoplace[1] , j+1);
    }
    valpos = timeprec[instance.getNbMac()-1] * instance.getPriority(jobtoplace[1]);
    timeprec[0] = timeprec[0] + instance.getTime( jobtoplace[0] , 1);
    for(j = 1; j<instance.getNbMac(); ++j) {
        timeprec[j] = max(timeprec[j-1], timeprec[j]) + instance.getTime( jobtoplace[0] , j+1);
    }
    valpos += timeprec[instance.getNbMac()-1] * instance.getPriority(jobtoplace[0]);

    if (valpos < tmp) {
        solution[0] = jobtoplace[0]; solution[1] = jobtoplace[1];
    } else {
        solution[0] = jobtoplace[1]; solution[1] = jobtoplace[0];
    }

    //place other jobs
    for (i = 2; i<nbJobs; ++i) { //index of the considered job
        solution[i] = jobtoplace[i]; //to start we place it at the end
        valpos = instance.computeWCTpartial(*this, i+1);
        best = 0;
        for (j = i; j>0; --j) { //place to put it +1
            tmp = solution[j-1];
            solution[j-1] = solution[j];
            solution[j] = tmp; //now it at the place j-1
            if ( (tmp = instance.computeWCTpartial(*this, i+1)) < valpos) {
                valpos = tmp;
                best = j-1;
            }
        } //now it is at 0
        //put the job in the best place
        for (j = 0; j<best; ++j) {
            tmp = solution[j];
            solution[j] = solution[j+1];
            solution[j+1] = tmp;
        } //now it is at best
    }

  //end
}

int Solution::generateRndPosition(int min, int max) {
    return ( rand() % max + min );
}