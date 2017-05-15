#include <iostream>
#include <climits>
#include <ctime>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "ILS.hpp"
#include "localsearch.hpp"
#include "solution.hpp"
#include "pfspinstance.hpp"

using namespace std;

Ils::Ils() {}

Ils::~Ils() {}

void Ils::init(const int neighbours, const int neighboursPerturb, const bool DD, const int acceptanceCrit, const double perturbFrac, const double perturbRadius, PfspInstance * instance, const double T0, const double alpha, const long l, const double warmupThreshold, const double T1) {
	//initialize and save all parameter
	this->neighboursPerturb = neighboursPerturb;
	this->perturbFrac = perturbFrac;
	this->perturbRadius = perturbRadius;
	this->acceptanceCrit = acceptanceCrit;
	this->instance = instance;
	this->T = T0;
	this->alpha = alpha;
	this->l = l;
	this->warmupThreshold = warmupThreshold;
	this->T1 = T1;
	this->nbJob = instance->getNbJob();
	this->date = l;

	//initialization of the local search
	if (1 == neighbours) { //transpose
		nbneighborhoud = 1;
		neighborhoud = new LocalSearch*[1];
		neighborhoud[0] = new LocalSearch(1, DD, true);
	} else if (2 == neighbours) { //exchange
		nbneighborhoud = 1;
		neighborhoud = new LocalSearch*[1];
		neighborhoud[0] = new LocalSearch(2, DD, true);
	} else if (3 == neighbours) { //insert
		nbneighborhoud = 1;
		neighborhoud = new LocalSearch*[1];
		neighborhoud[0] = new LocalSearch(3, DD, true);
	} else if (4 == neighbours) { //VND1
		nbneighborhoud = 3;
		neighborhoud = new LocalSearch*[3];
		neighborhoud[0] = new LocalSearch(1, DD, true);
		neighborhoud[1] = new LocalSearch(2, DD, true);
		neighborhoud[2] = new LocalSearch(3, DD, true);
	} else /*if (5 == neighbours)*/ { //VND2
		nbneighborhoud  = 3;
		neighborhoud = new LocalSearch*[3];
		neighborhoud[0] = new LocalSearch(1, DD, true);
		neighborhoud[1] = new LocalSearch(3, DD, true);
		neighborhoud[2] = new LocalSearch(2, DD, true);
	}

	//initialization of the solution
	current = new Solution(*instance);
	current->constructRZ(*instance);
	Localsearch(current);
	bestval = instance->computeWCT(*current);
	best = new Solution(*current);
}

Solution * Ils::search(const clock_t tmax, const long valOpt) {
	//variable
	clock_t tstart = clock(); //for the stop criterion
	Solution * parc; //this will stock the pertubation of the current solution and his local search
	long valprev = bestval; //value of the curent solution (usefull for metropolis criterion)

	//start
	while (clock() - tstart < tmax*CLOCKS_PER_SEC) {
		//generation of a new solution by perturbation and local search
		parc = perturbe();
		Localsearch(parc);

		if (keep(valprev, parc)) { //test if this solution should be keep (become the new current) according to the acceptance criterion
			//update of the current solution and the best one if needed
			delete(current);
			valprev = instance->computeWCT(*parc);
			current = parc;
			if (valprev < bestval) {
				delete(best);
				best = new Solution(*parc, false);
				bestval = valprev;

				//for the corelation plot
				cout<<(double)((double)(clock() - tstart)/(double)(CLOCKS_PER_SEC))<<":"<<100*(double)(bestval - valOpt)/(double)(valOpt)<<endl;
			}
		} else {
			//we don't keep this solution so it's deleted
			delete(parc);
		}

		if (3 == acceptanceCrit) { //update (of the temperature) needed by the metropolis criterion
			--date;
			if (0 == date) {
				T = alpha * T;
				date = l;
				if (T < warmupThreshold) {
					T = T1;
				}
			}
		}

	}

	//end
	return best;
}

Solution * Ils::perturbe() {
	//variable
	long radius = (long)(nbJob * perturbRadius); //radius of perturbation
	Solution * res = new Solution(*current, false); //copy the current solution (because we won't be able to go back to it)

	//start
	if (radius > 0) {
		//variable
		long nbMove = (long)(nbJob * perturbFrac); //number of perturbation to be done
		int rnd1, rnd2; //to select a random move
		int tmp,j;

		//start
		for(int i = 0; i<nbMove; ++i) {
			//select an acceptable random move acording to the radius
			rnd1 = rand() % nbJob;
			if (rand() / RAND_MAX < 0.5) {
				rnd2 = (rnd1 + rand() % radius +1) %nbJob;
			} else {
				rnd2 = (rnd1 - rand() % radius -1) %nbJob;
			}

			//apply this move
			if (1 == neighboursPerturb) { //exchange
				//rnd1 th and rnd2 th job are exchange
				tmp = res->getJ(rnd1);
				res->setJ(rnd1, res->getJ(rnd2));
				res->setJ(rnd2, tmp);
			} else /*if (2 == neighboursPerturb)*/ { //insert
				//rnd1 go the rnd2 position
				if (rnd1 < rnd2) {
					for(j = 0; j<rnd2-rnd1; ++j) {
						//bring forward the job
						tmp = res->getJ( rnd1+j );
						res->setJ( rnd1+j, res->getJ(rnd1+j+1) );
						res->setJ( rnd1+j+1, tmp);
					}
				} else /*if (rnd1 > rnd2)*/ {
					for(j = 0; j>rnd2-rnd1; --j) {
						//bring backward the job
						tmp = res->getJ( rnd1+j );
						res->setJ( rnd1+j, res->getJ(rnd1+j-1) );
						res->setJ( rnd1+j-1, tmp);
					}
				}
			}
		}
		//end
	}

	//end
	return res;
}

bool Ils::keep(const long valprev, Solution * sol) const {
	if (1 == acceptanceCrit) { //only improvement
		return instance->computeWCT(*sol) < valprev;
	} else if (2 == acceptanceCrit) { //always
		return true;
	} else /*if ((3 == acceptanceCrit) || (4 == acceptanceCrit))*/ { //metropolis or the like one
		long delta = instance->computeWCT(*sol) - valprev;
		if (delta < 0 ) {
			return true;
		} else {
			/*double rnd = (double)((double)(rand()) / (double)(RAND_MAX));
			return rnd < exp(-delta / T);*/
			return (double)((double)(rand()) / (double)(RAND_MAX)) < exp(-delta / T );
		}
	}
}

void Ils::Localsearch(Solution * parc) {
	int i = 0;
    while (i < nbneighborhoud) {
        if (0 == i) {
            neighborhoud[0]->descent(*instance,*parc); //enable the dofor
            i = 1; //now we are in a local minima of the first neighborhoud
        }else if (neighborhoud[i]->search(*instance,*parc)) {
            i = 0;
        } else {
            ++i;
        }
    }
}