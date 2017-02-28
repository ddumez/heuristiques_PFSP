/***************************************************************************
 *   Copyright (C) 2012 by Jérémie Dubois-Lacoste   *
 *   jeremie.dl@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>

#include "pfspinstance.hpp"
#include "solution.hpp"

using namespace std;

PfspInstance::PfspInstance(){}

PfspInstance::~PfspInstance(){}

int PfspInstance::getNbJob() {
 	return nbJob;
}

int PfspInstance::getNbMac() {
	return nbMac;
}

void PfspInstance::allowMatrixMemory(int nbJ, int nbM) {
	nbJob = nbJ; nbMac = nbM;

	processingTimesMatrix.resize(nbJ+1);          // 1st dimension

	for (int cpt = 0; cpt < nbJ+1; ++cpt) {
    	processingTimesMatrix[cpt].resize(nbM+1); // 2nd dimension
	}
	
	priority.resize(nbJ+1);
}


long int PfspInstance::getTime(int job, int machine) {
	if (job == 0) {
		return 0;
	} else {
		if ((job < 1) || (job > nbJob) || (machine < 1) || (machine > nbMac)) {
			cout<< "ERROR. file:pfspInstance.cpp, method:getTime. Out of bound. job=" << job << ", machine=" << machine << std::endl;
		}
    	return processingTimesMatrix[job][machine];
	}
}

long int PfspInstance::getPriority(int job) {
	return priority[job];
}

bool PfspInstance::readDataFromFile(char * fileName) {
	bool everythingOK = true;
	int j, m; // iterators
	long int readValue;
	string str;
	ifstream fileIn;
	char * aux2;
	char fileNameOK[100] = "";

	aux2 = (strrchr(fileName, '/'));

	if (aux2 == NULL)
		aux2 = fileName;
	else
		aux2 += 1;

	strcat(fileNameOK, aux2);

	cout << "name : " << fileNameOK << endl;
	cout << "file : " << fileName << endl;

	fileIn.open(fileName);

	if ( fileIn.is_open() ) {
        cout << "File " << fileName << " is now open, start to read..." << std::endl;

		fileIn >> nbJob;
        cout << "Number of jobs : " << nbJob << std::endl;
		fileIn >> nbMac;
        cout << "Number of machines : " << nbMac << std::endl;
        cout << "Allow memory for the matrix..." << std::endl;
		allowMatrixMemory(nbJob, nbMac);
        cout << "Memory allowed." << std::endl;
        cout << "Start to read matrix..." << std::endl;

		for (j = 1; j <= nbJob; ++j) {
			for (m = 1; m <= nbMac; ++m) {
				fileIn >> readValue; // The number of each machine, not important !
				fileIn >> readValue; // Process Time

				processingTimesMatrix[j][m] = readValue;
			}
		}
        fileIn >> str; // this is not read

		for (j = 1; j <= nbJob; ++j) {
			fileIn >> readValue; // -1
			fileIn >> readValue;
			// dueDates[j] = readValue;
			fileIn >> readValue; // -1
			fileIn >> readValue;
            priority[j] = readValue;
		}

        cout << "All is read from file." << std::endl;
		fileIn.close();
	} else {
		cout    << "ERROR. file:pfspInstance.cpp, method:readDataFromFile, "
				<< "error while opening file " << fileName << std::endl;

		everythingOK = false;
	}

	return everythingOK;
}

long int PfspInstance::computeWCT (Solution & sol) {
  //variable
	int j, m;
	long int wct = 0;

  //start
	// others machines : 
	for ( m = 1; m <= nbMac; ++m ) {
		for ( j = 1; j <= nbJob; ++j ) {
			if (sol.enddate[j][m-1] > sol.enddate[j-1][m]) {
				//the previous job is already finished
				sol.enddate[j][m] = sol.enddate[j][m-1] + processingTimesMatrix[sol.getJ(j-1)][m];
			} else {
				//we need to wait until the previous job is finished
				sol.enddate[j][m] = sol.enddate[j-1][m] + processingTimesMatrix[sol.getJ(j-1)][m];
			}
		}
	}

	for ( j = 1; j<= nbJob; ++j ) {
	    wct += sol.enddate[j][nbMac] * priority[sol.getJ(j-1)];
	}

  //end
return wct;
}

long int PfspInstance::computeWCTpartial (Solution & sol, int end) {
  //variable
	int j, m;
	long int wct = 0;

  //start
	// others machines : 
	for ( m = 1; m <= nbMac; ++m ) {
		for ( j = 1; j <= end; ++j ) {
			if (sol.enddate[j][m-1] > sol.enddate[j-1][m]) {
				//the previous job is already finished
				sol.enddate[j][m] = sol.enddate[j][m-1] + processingTimesMatrix[sol.getJ(j-1)][m];
			} else {
				//we need to wait until the previous job is finished
				sol.enddate[j][m] = sol.enddate[j-1][m] + processingTimesMatrix[sol.getJ(j-1)][m];
			}
		}
	}

	for ( j = 1; j<= end; ++j ) {
	    wct += sol.enddate[j][nbMac] * priority[sol.getJ(j-1)];
	}

  //end
return wct;
}

long int PfspInstance::recomputeWCT (Solution & sol, int start) {
  //variable
	int j, m;
	long int wct = 0;

  //start
	// others machines : 
	for ( m = 1; m <= nbMac; ++m ) {
		for ( j = start; j <= nbJob; ++j ) {
			if (sol.enddate[j][m-1] > sol.enddate[j-1][m]) {
				//the previous job is already finished
				sol.enddate[j][m] = sol.enddate[j][m-1] + processingTimesMatrix[sol.getJ(j-1)][m];
			} else {
				//we need to wait until the previous job is finished
				sol.enddate[j][m] = sol.enddate[j-1][m] + processingTimesMatrix[sol.getJ(j-1)][m];
			}
		}
	}

	for ( j = 1; j<= nbJob; ++j ) {
	    wct += sol.enddate[j][nbMac] * priority[sol.getJ(j-1)];
	}

  //end
return wct;
}