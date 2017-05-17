#include <iostream>
#include <climits>
#include <ctime>
#include <time.h>
#include <vector>

#include "tabu.hpp"
#include "solution.hpp"
#include "pfspinstance.hpp"

using namespace std;

Tabu::Tabu(const int tabuListLenght, const double longTimeMemoryImpact, const double restartThreshold, PfspInstance * instance, const int neighbours) {
	//initialization and save of parameter
	this->tabuListLenght = tabuListLenght;
	this->longTimeMemoryImpact = longTimeMemoryImpact;
	this->restartThreshold = restartThreshold;
	this->instance = instance;
	this->nbJob = instance->getNbJob();
	this->date = 1;
	this->neighbours = neighbours;
	
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

Solution * Tabu::search(const clock_t tmax) {
	//variable
	int tmp; //variable for exchange value
	double bestparc; //stock the value of the best reachable solution in the neighbourhood at each loop
	long int tmpval; //to temporary stock the value of the tested solution
	int besti = -1; int bestj; //index of the best move
  	int j,i; //loop variable
	clock_t tstart = clock(); //for the stop criterion 
	bool flag; //true iff the best solution so far had been improved
	
	//start
	while (clock() - tstart < tmax*CLOCKS_PER_SEC) {
		bestparc = LONG_MAX; flag = false;

		if (1 == neighbours) {//exchange movement
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
					//} else if( (tmpval * (1 + TL.at(current.getJ(max(i,j))-1).at(current.getJ(min(i,j))-1)*longTimeMemoryImpact) < bestparc) && (TL.at(current.getJ(min(i,j))-1).at(current.getJ(max(i,j))-1) < date)) {
						//this movement is legit acording to the tabu list and it's the best found in the neighbourhood of the current solution
						//so we save this movement
						//bestparc = tmpval * (1 + TL.at(current.getJ(max(i,j))-1).at(current.getJ(min(i,j))-1)*longTimeMemoryImpact);
						bestparc = tmpval * (1 + TL.at(max(i,j)).at(min(i,j))*longTimeMemoryImpact);
						besti = i;
						bestj = j;
					}

					//after cancel it
					tmp = current.getJ(i);
					current.setJ(i, current.getJ(j));
					current.setJ(j, tmp);
				}
				instance->computeWCT(current); //need to refresh the end date table for quick computation of the value
			}
		} else /*if (2 == neighbours)*/ {//insert moove
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
						//this movement is legit acording to the tabu list and it's the best found in the neighbourhood of the current solution
						//so we save this movement
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
				instance->computeWCT(current); //need to refresh the end date table for quick computation of the value
			}
		}


		// move become tabu
		if (1 == neighbours) { //exchange
			//TL.at(current.getJ(min(besti,bestj))-1).at(current.getJ(max(besti,bestj))-1) = date + tabuListLenght;
			TL.at(min(besti,bestj)).at(max(besti,bestj)) = date + tabuListLenght;
		} else /*if (2 == neighbours)*/ { //insert
			TL.at(min(besti,bestj)).at(max(besti,bestj)) = date + tabuListLenght;
		}
		//update of the long time memory
		if (1 == neighbours) { //exchange
			//TL.at(current.getJ(max(besti,bestj))-1).at(current.getJ(min(besti,bestj))-1) = TL.at(current.getJ(max(besti,bestj))-1).at(current.getJ(min(besti,bestj))-1)+1;
			TL.at(max(besti,bestj)).at(min(besti,bestj)) = TL.at(max(besti,bestj)).at(min(besti,bestj))+1;
		} else /*if (2 == neighbours)*/ { //insert
			TL.at(max(besti,bestj)).at(min(besti,bestj)) = TL.at(max(besti,bestj)).at(min(besti,bestj))+1;
		}

		//apply the best modification
		if (1 == neighbours) { //with exchange
			tmp = current.getJ(besti);
			current.setJ(besti, current.getJ(bestj));
			current.setJ(bestj, tmp);
		} else if (2 == neighbours) { //with insert
			for(j = 0; j<=bestj; ++j) {
				//bring forward the job
				tmp = current.getJ( (besti+j)%nbJob  );
				current.setJ((besti+j)%nbJob, current.getJ( (besti+j+1)%nbJob));
				current.setJ((besti+j+1)%nbJob, tmp);
			}
		}

		//update the end date table, before the copy
		instance->computeWCT(current);

		//update the best found so far
		if(flag) {
			delete(best);
			best = new Solution(current);
		}

		//test if a restart is needed
		if (tmpval > (1+restartThreshold)*bestval) {
			current = Solution(*best);
		}

		//next iteration
		++date;
	}

	//end
	return best;
}