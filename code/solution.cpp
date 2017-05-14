#include <vector>
#include <cstdlib>
#include <iostream>

#include "solution.hpp"
#include "pfspinstance.hpp"

using namespace std;

Solution::Solution(const PfspInstance & instance) {
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

Solution::Solution(const Solution & other, const bool copyEndDateTable) {
    this->nbJobs = other.nbJobs;

    solution.resize(nbJobs);
    enddate.resize(nbJobs+1);
    for(int cpt = 0; cpt<nbJobs; ++cpt) {
        solution[cpt] = other.solution[cpt];
        enddate[cpt].resize(other.enddate.size());
        if (copyEndDateTable) {
            for(int cpt2 = other.enddate.size()-1; cpt2>=0; --cpt2) {
                enddate[cpt][cpt2] = other.enddate[cpt][cpt2];
            }
        }
    }

    enddate[nbJobs].resize(other.enddate.size());
    if (copyEndDateTable) {
        for(int cpt2 = other.enddate.size()-1; cpt2>=0; --cpt2) {
            enddate[nbJobs][cpt2] = other.enddate[nbJobs][cpt2];
        }
    }

}

Solution::Solution() {}

Solution::~Solution() {}

void Solution::initialize(const PfspInstance & instance) {
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

int Solution::getJ(const int j) const {
    return solution[j];
}

void Solution::setJ(const int i, const int j) {
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

void Solution::constructRZ(const PfspInstance & instance) {
  //variable
    vector<double> T (nbJobs); //to calculate the weighted sum of processing time
    vector<int> jobtoplace (nbJobs); //to order jobs
    int i,j, best;
    long int tmp, valpos; //these two sometime store the score
    vector<int> timeprec (instance.getNbMac()); //end date on this machine of the previous job

  //start
    //calculate weighted sum of processing time
    for (i = 0; i<nbJobs; ++i) { //i on jobs
        T[i] = 0;
        jobtoplace[i] = i+1; //initialize this array
        for (j = 0; j<instance.getNbMac(); ++j) { //j on machines
            T[i] += (double) instance.getTime(i+1, j+1);
        }
        T[i] /= (double) (instance.getPriority(i+1));
    }

    //order job
    for(i = 0; i<nbJobs; ++i) {
        for(j = 0; j<nbJobs-1; ++j) {
            if (T[jobtoplace[j]-1] > T[jobtoplace[j+1]-1]) {
                tmp = jobtoplace[j];
                jobtoplace[j] = jobtoplace[j+1];
                jobtoplace[j+1] = tmp;
            }
        }
    }

    solution[0] = jobtoplace[0]; //the first job

    //place other jobs
    for (i = 1; i<nbJobs; ++i) { //index of the considered job
        solution[i] = jobtoplace[i]; //to start we place it at the end
        valpos = instance.computeWCTpartial(*this, i+1);
        best = i;
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

int Solution::generateRndPosition(const int min, const int max) const {
    return ( rand() % max + min );
}

void Solution::print() const {
    for(int i = 0; i<nbJobs; ++i) {
        cout<<solution.at(i)<<" ";
    }
    cout<<endl;
}