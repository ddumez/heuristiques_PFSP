#include <iostream>
#include <climits>

#include "tabu.hpp"
#include "solution.hpp"
#include "pfspinstance.hpp"

using namespace std;

Tabu::Tabu(const int tabuListLenght, const double longTimeMemoryImpact, const double restartThreshold, PfspInstance * instance) {
	this->tabuListLenght = tabuListLenght;
	this->longTimeMemoryImpact = longTimeMemoryImpact;
	this->restartThreshold = restartThreshold;
	this->instance = instance;
	this->nbJob = instance->getNbJob();
	this->date = 1;
	
	//initialization of the tabu list
	TL.resize(nbJob);
	for (int i = 0; i<nbJob; ++i) {
		TL.at(i).resize(nbJob);
		for(int j = 0; j<nbJob; ++j) {
			//all job are othorize when we start and none of them have been used
			TL.at(i).at(j) = 0;
		}
	}

	//initialization of the solution
	current.initialize(*instance);
	current.constructRZ(*instance);
	bestval = instance->computeWCT(current);
	best = new Solution(current);
}

Tabu::~Tabu() {}

void Tabu::changetTLL(const int tll) {
	tabuListLenght = tll;
}

Solution * Tabu::search(const clock_t tmax) {
	//variable
	int tmp;
	double bestparc;
	long int tmpval;
	int besti = -1; int bestj;
  	int j,i;
	clock_t tstart = clock();
	bool flag; //true iff the best solution so far had been improved
	
	//start
	while (clock() - tstart < tmax*CLOCKS_PER_SEC) {
		bestparc = LONG_MAX; flag = false;
//cout<<date<<": ";

		//loop for the exchange movement
		for(i = nbJob -1; i>0; --i) {
			for(j = i-1; j>=0; --j) {
				//exchange the ith job and the jth one
				tmp = current.getJ(i);
				current.setJ(i, current.getJ(j));
				current.setJ(j, tmp);

				//compute if it's the new best
				if ( (tmpval = instance->recomputeWCT(current,j)) < bestval ) { //Aspiration criteria
					bestparc = tmpval;
					bestval = tmpval;
					besti = i;
					bestj = j;
					flag = true;
				} else if( (tmpval * (1 + TL.at(max(i,j)).at(min(i,j))*longTimeMemoryImpact) < bestparc) && (TL.at(min(i,j)).at(max(i,j)) < date)) {
					bestparc = tmpval * (1 + TL.at(max(i,j)).at(min(i,j))*longTimeMemoryImpact);
					besti = i;
					bestj = j;
				}

				//after cancel it
				tmp = current.getJ(i);
				current.setJ(i, current.getJ(j));
				current.setJ(j, tmp);
			}
			instance->computeWCT(current);
		}
/*
		//loop for the insert moove
		for(int i = 0; i<nbJob; ++i) {
			for(j = 0; j<nbJob-1; ++j) {
				//bring forward the job
				tmp = current.getJ( (i+j)%nbJob  );
				current.setJ((i+j)%nbJob, current.getJ( (i+j+1)%nbJob));
				current.setJ((i+j+1)%nbJob, tmp);
				
				//compute if it's the new best
				if ( (tmpval = instance->recomputeWCT(current,j)) < bestval ) { //Aspiration criteria
					bestparc = tmpval;
					bestval = tmpval;
					besti = i;
					bestj = j;
					flag = true;
				} else if( (tmpval * (1 + TL.at(max(i,j)).at(min(i,j))*longTimeMemoryImpact) < bestparc) && (TL.at(min(i,j)).at(max(i,j)) < date)) {
					bestparc = tmpval * (1 + TL.at(max(i,j)).at(min(i,j))*longTimeMemoryImpact);
					besti = i;
					bestj = j;
				}
			}

			//replace the solution in the initial state
			for (j = nbJob-2; j>=0; --j) {
				tmp = current.getJ( (i+j)%nbJob  );
				current.setJ((i+j)%nbJob, current.getJ( (i+j+1)%nbJob));
				current.setJ((i+j+1)%nbJob, tmp);
			}
			instance->computeWCT(current);
		}
*/
		//apply the best modification
			//with exchange
			tmp = current.getJ(besti);
			current.setJ(besti, current.getJ(bestj));
			current.setJ(bestj, tmp);
/*			//with insert
			for(j = 0; j<=bestj; ++j) {
				//bring forward the job
				tmp = current.getJ( (besti+j)%nbJob  );
				current.setJ((besti+j)%nbJob, current.getJ( (besti+j+1)%nbJob));
				current.setJ((besti+j+1)%nbJob, tmp);
			}
*/
//cout<<besti<<" , "<<bestj; 
		
		//update the end date table, before the copy
		instance->computeWCT(current);

		if(flag) {
			delete(best);
			best = new Solution(current);
//cout<<" !!!!!!! flag";
		}

		// move these two job become tabu
		TL.at(min(besti,bestj)).at(max(besti,bestj)) = date + tabuListLenght;
		//update of the long time memory
		TL.at(max(besti,bestj)).at(min(besti,bestj)) = TL.at(max(besti,bestj)).at(min(besti,bestj))+1;

		//test if a restart is needed
		if (tmpval > (1+restartThreshold)*bestval) {
//cout<<" !!!!!!! restart";
			current = Solution(*best);
		}

		//next iteration
		++date;

//cout<<endl;
	}

	//end
	return best;
}