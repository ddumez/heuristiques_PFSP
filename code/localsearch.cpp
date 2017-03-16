#include <iostream>

#include "localsearch.hpp"
#include "solution.hpp"
#include "pfspinstance.hpp"

using namespace std;

LocalSearch::LocalSearch() {
	this->choix = 1;
	this->PPD = false;
	this->dofor = true;
}

LocalSearch::LocalSearch(int choix, bool PPD, bool dofor){
	this->choix = choix;
	this->PPD = PPD;
	this->dofor = dofor;
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
	if(dofor && (!PPD) ) {
		switch(choix) {
			case 1:
				while ( transposedofor(instance, sol) ) { }
			  break;
			case 2:
				while ( exchangedofor(instance, sol) ) { }
			  break;
			case 3:
				while ( insertdofor(instance, sol) ) { }
			  break;
			default:
				cout<<"bad neighborhoud code"<<endl;
			  break;
		}
	} else {
		while ( search(instance, sol) ) { }
	}
}

bool LocalSearch::transpose(PfspInstance & instance, Solution & sol){
  //variable
	bool res = false;
	int i = instance.getNbJob() -2;
	int tmp;
	long int base = instance.computeWCT(sol);
  //start
	while ((!res) && (i>=0)) { //reverse order to use recomputeWCT
		//transpose the ith job and the (i+1)th one
		tmp = sol.getJ(i);
		sol.setJ(i, sol.getJ(i+1));
		sol.setJ(i+1, tmp);
		res = (instance.recomputeWCT(sol, i) < base); //compute if it's better
		if (!res) { //if it's not cancel it
			tmp = sol.getJ(i);
			sol.setJ(i, sol.getJ(i+1));
			sol.setJ(i+1, tmp);
		}
		--i; //next job (previous one in fact)
	}
  //end
return res;
}

bool LocalSearch::transposedofor(PfspInstance & instance, Solution & sol){  
  //variable
	bool res = false;
	int tmp;
	long int base = instance.computeWCT(sol);
	long int tmpscore;
  //start	
	for(int i = instance.getNbJob() -2; i>=0; --i) {
		//transpose the ith job and the (i+1)th one
		tmp = sol.getJ(i);
		sol.setJ(i, sol.getJ(i+1));
		sol.setJ(i+1, tmp);
		if ( (tmpscore = instance.recomputeWCT(sol, i)) < base) { //compute if it's better
			res = true;
			base = tmpscore;
		} else { //if it's not cancel it
			tmp = sol.getJ(i);
			sol.setJ(i, sol.getJ(i+1));
			sol.setJ(i+1, tmp);
		}
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
	for(int i = instance.getNbJob()-2; i>=0; --i) {
		//transpose the ith job and the (i+1)th one
		tmp = sol.getJ(i);
		sol.setJ(i, sol.getJ(i+1));
		sol.setJ(i+1, tmp);

		//compute if it's the new best
		if ( (tmpval = instance.recomputeWCT(sol, i)) < best ) {
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
	int i = instance.getNbJob() -1; int j;
	int tmp;
	long int base = instance.computeWCT(sol);
  //start
	while ((!res) && (i>0)) { //reverse order to use recompute
		j = i-1;
		while ((!res) && (j>=0)) {
			//transpose the ith job and the jth one
			tmp = sol.getJ(i);
			sol.setJ(i, sol.getJ(j));
			sol.setJ(j, tmp);
			res = (instance.recomputeWCT(sol,j) < base); //compute if it's better
			if (!res) { //if it's not cancel it
				tmp = sol.getJ(i);
				sol.setJ(i, sol.getJ(j));
				sol.setJ(j, tmp);
			}
			--j; //next
		}
		--i; //next job
		if (!res) {instance.computeWCT(sol);} //if we don't stop the enddate table must be compute from the start
	}
  //end
return res;
}

bool LocalSearch::exchangedofor(PfspInstance & instance, Solution & sol){
  //variable
	bool res = false;
	int j;
	int tmp;
	long int base = instance.computeWCT(sol);
	long int tmpscore;
  //start
	for(int i = instance.getNbJob() -1; i>0; --i) {
		for(j = i-1; j>=0; --j) {
			//transpose the ith job and the jth one
			tmp = sol.getJ(i);
			sol.setJ(i, sol.getJ(j));
			sol.setJ(j, tmp);
			if ( (tmpscore = instance.recomputeWCT(sol,j)) < base) { //compute if it's better
				res = true;
				base = tmpscore;
			} else { //if it's not cancel it
				tmp = sol.getJ(i);
				sol.setJ(i, sol.getJ(j));
				sol.setJ(j, tmp);
			}
		}
		instance.computeWCT(sol);
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
	for(int i = instance.getNbJob() -1; i>0; --i) {
		for(j = i-1; j>=0; --j) {
			//transpose the ith job and the (i+1)th one
			tmp = sol.getJ(i);
			sol.setJ(i, sol.getJ(j));
			sol.setJ(j, tmp);

			//compute if it's the new best
			if ( (tmpval = instance.recomputeWCT(sol,j)) < best ) {
				best = tmpval;
				besti = i;
				bestj = j;
			}

			//after cancel it
			tmp = sol.getJ(i);
			sol.setJ(i, sol.getJ(j));
			sol.setJ(j, tmp);
		}
		instance.computeWCT(sol);
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

bool LocalSearch::insertdofor(PfspInstance & instance, Solution & sol){
  //variable
	int j;
	bool res = false; bool improving;
	int tmp;
	long int base = instance.computeWCT(sol);
	long int tmpscore;
  //start
	for(int i = 0; i < instance.getNbJob(); ++i) {
		j = 0; improving = false;
		while ((!improving) && (j < instance.getNbJob()-1)) {
			//bring forward the job
			tmp = sol.getJ( (i+j)%instance.getNbJob()  );
			sol.setJ((i+j)%instance.getNbJob(), sol.getJ( (i+j+1)%instance.getNbJob()));
			sol.setJ((i+j+1)%instance.getNbJob(), tmp);

			improving = ( (tmpscore = instance.computeWCT(sol)) < base); //compute if it's better
			
			++j; //next place
		}
		if (!improving) { //move this job it isn't good so we return to the initial solution
			for (j = instance.getNbJob()-2; j>=0; --j) {
				tmp = sol.getJ( (i+j)%instance.getNbJob()  );
				sol.setJ((i+j)%instance.getNbJob(), sol.getJ( (i+j+1)%instance.getNbJob()));
				sol.setJ((i+j+1)%instance.getNbJob(), tmp);				
			}
			res = true;
			base = tmpscore;
			--i; //the i-th job is now the (i+1)th one
		}
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

void LocalSearch::changedofor(bool dofor) {
	this->dofor = dofor;
}