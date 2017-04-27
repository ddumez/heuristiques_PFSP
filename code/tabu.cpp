#include <iostream>
#include <climits>

#include "tabu.hpp"
#include "solution.hpp"
#include "pfspinstance.hpp"

using namespace std;

Tabu::Tabu(const int tabuListLenght, PfspInstance * instance) {
	this->tabuListLenght = tabuListLenght;
	this->instance = instance;
	this->nbJob = instance->getNbJob();
	this->date = 0;
	
	//initialization of the tabu list
	TL.resize(nbJob);
	for (int i = 0; i<nbJob; ++i) {
		TL.at(i) = -1; //all job are othorize when we start
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
	long int bestparc;
	long int tmpval;
	int besti = -1; int bestj;
  	int j,i;
	clock_t tstart = clock();
	bool flag; //true iff the best solution so far had been improved
	
	//start
	while (clock() - tstart < tmax*CLOCKS_PER_SEC) {
		bestparc = LONG_MAX; flag = false;
cout<<date<<": ";
//for(i = 0; i<nbJob; ++i) {cout<<TL.at(i)<<" ";}
//cout<<endl;
		for(i = nbJob -1; i>0; --i) {
			for(j = i-1; j>=0; --j) {
				//transpose the ith job and the (i+1)th one
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
				} else if( (tmpval < bestparc) && (TL.at(i) < date) && (TL.at(j) < date)) { //basic case, both job aren't tabou
					bestparc = tmpval;
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

		//apply the best modification
		tmp = current.getJ(besti);
		current.setJ(besti, current.getJ(bestj));
		current.setJ(bestj, tmp);
cout<<besti<<" , "<<bestj<<" : "<<TL.at(besti)<<" , "<<TL.at(bestj); 
		if(flag) {
			delete(best);
			best = new Solution(current);
cout<<" !!!!!!! flag";
		}
cout<<endl;
		// move these two job become tabu
		TL.at(besti) = date + tabuListLenght;
		TL.at(bestj) = date + tabuListLenght;

		//next iteration
		++date;
		instance->computeWCT(current);
	}

	//end
	return best;
}