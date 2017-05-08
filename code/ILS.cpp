#include <iostream>
#include <climits>
#include <ctime>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "ILS.hpp"
#include "solution.hpp"
#include "pfspinstance.hpp"

using namespace std;

Ils::Ils(const int neighbours, const int neighboursPerturb, const bool DD, const int acceptanceCrit, const double longTimeMemoryImpact, PfspInstance * instance, const double T0, const double alpha, const long l, const double warmupThreshold, const double T1) {
	this->neighbours = new LocalSearch(neighbours+1, DD, true);
	this->neighboursPerturb = neighboursPerturb;
	this->DD = DD;
	this->acceptanceCrit = acceptanceCrit;
	this->longTimeMemoryImpact = longTimeMemoryImpact;
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
	search.descent(*instance, *current);
	bestval = instance->computeWCT(*current);
	best = new Solution(*current);

	//initialisation of the memory
	mem.resize(nbJob);
	for (int i = 0; i<nbJob; ++i) {
		mem.at(i).resize(nbJob);
		for(int j = 0; j<nbJob; ++j) {
			mem.at(i).at(j) = 0;
		}
	}
}

Ils::~Ils() {}

Solution * Ils::search(const clock_t tmax) {
	//variable
	clock_t tstart = clock();
	Solution * parc;
	long valprev = bestval;

	//start
	while (clock() - tstart < tmax*CLOCKS_PER_SEC) {
		parc = perturbe();
		search.descent(*instance, current);
		if keep(valprev, parc) {
			valprev = instance->computeWCT(*sol);
			delete(current);
			current = parc;
			if (valprev < bestval) {
				delete(best);
				best = new Solution(parc);
				bestval = valprev;
			}
		} else {
			delete(parc);
		}

		--date;
		if (0 == date) {
			T = alpha * T;
			date = l;
			if (T < warmupThreshold) {
				T = T1;
			}
		}
	}

	//end
	return best;
}

Solution * Ils::perturbe() {
	//variable
	Solution * res = new Solution(*current);

	//start
//TODO
	//end
	return res;
}

bool Ils::keep(const long valprev, Solution * sol) const {
	if (1 == acceptanceCrit) { //only improvement
		return instance->computeWCT(*sol) < valprev;
	} else if (2 == acceptanceCrit) { //always
		return true;
	} else /*if (3 == acceptanceCrit)*/ { //metropolis
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