#include <iostream>

#include "localsearch.hpp"
#include "solution.hpp"
#include "pfspinstance.hpp"

using namespace std;


LocalSearch::LocalSearch(int choix, bool PPD){
	this->choix = choix;
	this->PPD = PPD;
}

LocalSearch::~LocalSearch(){}

bool LocalSearch::search(PfspInstance & instance, Solution & sol){
	switch(choix) {
		case 1:
			if (PPD) {
				return transposePPD(instance, sol);
			} else {
				return transpose(instance, sol);
			}
		  break;
		case 2:
			if (PPD) {
				return exchangePPD(instance, sol);
			} else {
				return exchange(instance, sol);
			}
		  break;
		case 3:
			if (PPD) {
				return insertPPD(instance, sol);
			} else {
				return insert(instance, sol);
			}
		  break;
		default:
			cout<<"bad neighborhoud code"<<endl;
			return false;
		  break;
	}
}

void LocalSearch::descent(PfspInstance & instance, Solution & sol) {
	while ( search(instance, sol) ) { }
}

bool LocalSearch::transpose(PfspInstance & instance, Solution & sol){
  //variable
	bool res = false;
	int i = 0;
	int tmp;
	long int base = instance.computeWCT(sol);
  //start
	while ((!res) && (i<instance.getNbJob() -1)) {
		//transpose the ith job and the (i+1)th one
		tmp = sol.getJ(i);
		sol.setJ(i, sol.getJ(i+1));
		sol.setJ(i+1, tmp);
		res = (instance.computeWCT(sol) < base); //compute if it's better
		if (!res) { //if it's not cancel it
			tmp = sol.getJ(i);
			sol.setJ(i, sol.getJ(i+1));
			sol.setJ(i+1, tmp);
		}
		++i; //next job
	}
  //end
return res;
}

bool LocalSearch::transposePPD(PfspInstance & instance, Solution & sol){
  //variable
	int tmp;
	long int best = instance.computeWCT(sol);
	long int tmpval;
	int besti = -1;
  //start
	for(int i = 0; i<instance.getNbJob()-1; ++i) {
		//transpose the ith job and the (i+1)th one
		tmp = sol.getJ(i);
		sol.setJ(i, sol.getJ(i+1));
		sol.setJ(i+1, tmp);

		//compute if it's the new best
		if ( (tmpval = instance.computeWCT(sol)) < best ) {
			best = tmpval;
			besti = i;
		}

		//after cancel it
		tmp = sol.getJ(i);
		sol.setJ(i, sol.getJ(i+1));
		sol.setJ(i+1, tmp);
	}

	//apply the best modification
	if (-1 == besti) {
		return false;
	} else {
		tmp = sol.getJ(besti);
		sol.setJ(besti, sol.getJ(besti+1));
		sol.setJ(besti+1, tmp);
		return true;
	}
  //end
}

bool LocalSearch::exchange(PfspInstance & instance, Solution & sol){
  //variable
	bool res = false;
	int i = 0; int j;
	int tmp;
	long int base = instance.computeWCT(sol);
  //start
	while ((!res) && (i<instance.getNbJob() -1)) {
		j = i+1;
		while ((!res) && (j<instance.getNbJob())) {
			//transpose the ith job and the jth one
			tmp = sol.getJ(i);
			sol.setJ(i, sol.getJ(j));
			sol.setJ(j, tmp);
			res = (instance.computeWCT(sol) < base); //compute if it's better
			if (!res) { //if it's not cancel it
				tmp = sol.getJ(i);
				sol.setJ(i, sol.getJ(j));
				sol.setJ(j, tmp);
			}
			++j; //next
		}
		++i; //next job
	}
  //end
return res;
}

bool LocalSearch::exchangePPD(PfspInstance & instance, Solution & sol){
  //variable
	int tmp;
	long int best = instance.computeWCT(sol);
	long int tmpval;
	int besti = -1; int bestj;
  	int j;
  //start
	for(int i = 0; i<instance.getNbJob()-1; ++i) {
		for(j = i+1; j<instance.getNbJob(); ++j) {
			//transpose the ith job and the (i+1)th one
			tmp = sol.getJ(i);
			sol.setJ(i, sol.getJ(j));
			sol.setJ(j, tmp);

			//compute if it's the new best
			if ( (tmpval = instance.computeWCT(sol)) < best ) {
				best = tmpval;
				besti = i;
				bestj = j;
			}

			//after cancel it
			tmp = sol.getJ(i);
			sol.setJ(i, sol.getJ(j));
			sol.setJ(j, tmp);
		}
	}

	//apply the best modification
	if (-1 == besti) {
		return false;
	} else {
		tmp = sol.getJ(besti);
		sol.setJ(besti, sol.getJ(bestj));
		sol.setJ(bestj, tmp);
		return true;
	}
  //end
}

bool LocalSearch::insert(PfspInstance & instance, Solution & sol){
  //variable
	int i = 0; int j;
	bool res = false;
	int tmp;
	long int base = instance.computeWCT(sol);
  //start
	while ((!res) && (i < instance.getNbJob())) {
		j = 0;
		while ((!res) && (j < instance.getNbJob()-1)) {
			//bring forward the job
			tmp = sol.getJ( (i+j)%instance.getNbJob()  );
			sol.setJ((i+j)%instance.getNbJob(), sol.getJ( (i+j+1)%instance.getNbJob()));
			sol.setJ((i+j+1)%instance.getNbJob(), tmp);

			res = (instance.computeWCT(sol) < base); //compute if it's better
			
			++j; //next place
		}
		if (!res) { //move this job it isn't good so we return to the initial solution
			for (j = instance.getNbJob()-2; j>=0; --j) {
				tmp = sol.getJ( (i+j)%instance.getNbJob()  );
				sol.setJ((i+j)%instance.getNbJob(), sol.getJ( (i+j+1)%instance.getNbJob()));
				sol.setJ((i+j+1)%instance.getNbJob(), tmp);				
			}
		}
		++i; //next job
	}
  //end
return res;
}

bool LocalSearch::insertPPD(PfspInstance & instance, Solution & sol){
  //variable
	int tmp;
	long int best = instance.computeWCT(sol);
	long int tmpval;
	int besti = -1; int bestj;
  	int j;
  //start
	for(int i = 0; i<instance.getNbJob(); ++i) {
		for(j = 0; j<instance.getNbJob()-1; ++j) {
			//bring forward the job
			tmp = sol.getJ( (i+j)%instance.getNbJob()  );
			sol.setJ((i+j)%instance.getNbJob(), sol.getJ( (i+j+1)%instance.getNbJob()));
			sol.setJ((i+j+1)%instance.getNbJob(), tmp);
			
			//compute if it's the new best
			if ( (tmpval = instance.computeWCT(sol)) < best ) {
				best = tmpval;
				besti = i;
				bestj = j;
			}
		}
		//replace the solution in the initial state
		for (j = instance.getNbJob()-2; j>=0; --j) {
			tmp = sol.getJ( (i+j)%instance.getNbJob()  );
			sol.setJ((i+j)%instance.getNbJob(), sol.getJ( (i+j+1)%instance.getNbJob()));
			sol.setJ((i+j+1)%instance.getNbJob(), tmp);				
		}
	}

	//apply the best modification
	if (-1 == besti) {
		return false;
	} else {
		for(j = 0; j<=bestj; ++j) {
			//bring forward the job
			tmp = sol.getJ( (besti+j)%instance.getNbJob()  );
			sol.setJ((besti+j)%instance.getNbJob(), sol.getJ( (besti+j+1)%instance.getNbJob()));
			sol.setJ((besti+j+1)%instance.getNbJob(), tmp);
		}
		return true;
	}
  //end
}

void LocalSearch::changechoix(int choix) {
	this->choix = choix;
}

void LocalSearch::changePPD(bool PPD) {
	this->PPD = PPD;
}