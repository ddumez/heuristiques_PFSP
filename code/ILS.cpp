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

Ils::Ils(const int neighbours, const int neighboursPerturb, const bool DD, const int acceptanceCrit, const double perturbFrac, const double perturbRadius, PfspInstance * instance, const double T0, const double alpha, const long l, const double warmupThreshold, const double T1) {
	init(neighbours, neighboursPerturb, DD, acceptanceCrit, perturbFrac, perturbRadius, instance, T0, alpha, l, warmupThreshold, T1);
}

Ils::~Ils() {}

void Ils::init(const int neighbours, const int neighboursPerturb, const bool DD, const int acceptanceCrit, const double perturbFrac, const double perturbRadius, PfspInstance * instance, const double T0, const double alpha, const long l, const double warmupThreshold, const double T1) {
	this->neighborhoud = new LocalSearch(neighbours+1, DD, true);
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

	//initialization of the solution
	current = new Solution(*instance);
	current->constructRZ(*instance);
	neighborhoud->descent(*instance, *current);
	bestval = instance->computeWCT(*current);
	best = new Solution(*current);
}

Solution * Ils::search(const clock_t tmax) {
	//variable
	clock_t tstart = clock();
	Solution * parc;
	long valprev = bestval;

	//start
	while (clock() - tstart < tmax*CLOCKS_PER_SEC) {
		parc = perturbe();
		neighborhoud->descent(*instance, *current);
		if (keep(valprev, parc)) {
			delete(current);
			valprev = instance->computeWCT(*parc);
			current = parc;
			if (valprev < bestval) {
				delete(best);
				best = new Solution(*parc);
				bestval = valprev;
			}
		} else {
			delete(parc);
		}

		if (3 == acceptanceCrit) {
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
	Solution * res = new Solution(*current);
	long rest = (long)(nbJob * perturbFrac); //number of perturbation to be done
	long radius = (long)(nbJob * perturbRadius); //radius of perturbation
	int rnd1, rnd2;
	int tmp,j;

	//start
	if (radius > 0) {
		while (rest > 0) {
			rnd1 = rand() % nbJob;
			rnd2 = rand() % nbJob;
			if (abs(rnd1 - rnd2) <= radius) {
				--rest;
				if (1 == neighboursPerturb) { //exchange
					tmp = res->getJ(rnd1);
					res->setJ(rnd1, res->getJ(rnd2));
					res->setJ(rnd2, tmp);	
				} else /*if (2 == neighboursPerturb)*/ { //insert
					for(j = 0; j<=rnd2; ++j) {
						tmp = res->getJ( (rnd1+j)%nbJob );
						res->setJ((rnd1+j)%nbJob, res->getJ( (rnd1+j+1)%nbJob));
						res->setJ((rnd1+j+1)%nbJob, tmp);
					}
				}
			}
		}
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