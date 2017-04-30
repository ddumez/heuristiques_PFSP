#include <iostream>
#include <climits>
#include <cstdlib>
#include <math.h>

#include "SA.hpp"
#include "solution.hpp"
#include "pfspinstance.hpp"

using namespace std;

SA::SA(const double t0, const long l, const double alpha, PfspInstance * instance) {
	this->t = t0;
	this->l = l;
	this->alpha = alpha;
	this->date = l;
	this->instance = instance;
	this->nbJob = instance->getNbJob();

	//initialization of the solution
	current.initialize(*instance);
	current.constructRZ(*instance);
	bestval = instance->computeWCT(current);
	best = new Solution(current);
}

SA::~SA(){}

Solution * SA::search(const clock_t tmax) {
	//variable
	clock_t tstart = clock();
	int rndi, rndj, j;
	int tmp;
	long valcur;
	long valprev = bestval;

	//start
	while (clock() - tstart < tmax*CLOCKS_PER_SEC) {
		//select a random move
		rndi = generateRndPosition(0, nbJob);
		rndj = generateRndPosition(0, nbJob);

		//for exchange move
			tmp = current.getJ(rndi);
			current.setJ(rndi, current.getJ(rndj));
			current.setJ(rndj, tmp);

/*		//for insert move
			for(j = 0; j<=rndj; ++j) {
				tmp = current.getJ( (rndi+j)%nbJob  );
				current.setJ((rndi+j)%nbJob, current.getJ( (rndi+j+1)%nbJob));
				current.setJ((rndi+j+1)%nbJob, tmp);
			}
*/
		//acceptation rule
		valcur = instance->computeWCT(current);
		if (valcur < valprev) {
			//we have an improvement
			valprev = valcur;

			if (valcur < bestval) {
				//improve the best one
				delete(best);
				best = new Solution(current);
				bestval = valcur;
			}
		} else if (rand() / RAND_MAX < exp( -1.0 * (double)(valcur - valprev)/t ) ) {
			//not an improvement but we keep
			valprev = valcur;
		} else {
			//discard this move
			//for exchange move
				tmp = current.getJ(rndi);
				current.setJ(rndi, current.getJ(rndj));
				current.setJ(rndj, tmp);

/*			//for insert move
				for(j = rndj; j>=0; --j) {
					tmp = current.getJ( (rndi+j)%nbJob  );
					current.setJ((rndi+j)%nbJob, current.getJ( (rndi+j+1)%nbJob));
					current.setJ((rndi+j+1)%nbJob, tmp);
				}
*/
		}

		//update for the next cycle
		--date;
		if (0 == date) {
			t *= alpha;
			date = l;
		}
	}

	//end
	return best;
}

int SA::generateRndPosition(const int min, const int max) const {
    return ( rand() % max + min );
}