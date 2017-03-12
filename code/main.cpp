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
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "solution.hpp"
#include "pfspinstance.hpp"
#include "localsearch.hpp"

using namespace std;

/**
* \brief improve the solution with the vnd indea with the 3 local search
*
* \param[in, out] sol the solution to improve
* \param[in] instance the context of hte solution
* \param[in] first the first local search to use
* \param[in] second the second neighborhoud to use
* \param[in] third the last neighborhourd to use
*/
void vnd(Solution & sol, PfspInstance & instance, LocalSearch & first, LocalSearch & second, LocalSearch & third);

int main(int argc, char *argv[]) {
  //variable
    int i;
    long int totalWeightedTardiness;
    PfspInstance instance; // Create instance object
    LocalSearch test (3, false);

  //start
    // initialize random seed
    srand ( time(NULL) );

    if (argc == 1) {
        cout << "Usage: ./flowshopWCT <instance_file>" << endl;
        return 0;
    }

    // Read data from file
    if (! instance.readDataFromFile(argv[1]) ) {
        return 1;
    }

    Solution solution (instance); // Create a Solution to represent the permutation

    // Fill the vector with a random permutation
    solution.randomPermutation();

    cout << "Random solution: " ;

    for (i = 0; i < instance.getNbJob(); ++i) {
        cout << solution.getJ(i) << " " ;
    }
    cout << endl;

    // Compute the TWT of this solution
    totalWeightedTardiness = instance.computeWCT(solution);
    cout << "Total weighted completion time: " << totalWeightedTardiness << endl;
  
    // compute a solution with the RZ heuristic
    solution.constructRZ(instance);

    cout << "RZ solution: " ;

    for (i = 0; i < instance.getNbJob(); ++i) {
        cout << solution.getJ(i) << " " ;
    }
    cout << endl;

    // Compute the TWT of this solution
    totalWeightedTardiness = instance.computeWCT(solution);
    cout << "Total weighted completion time: " << totalWeightedTardiness << endl;

    //improve by descent
    test.descent(instance, solution);

    cout << "RZ solution improved: " ;

    for (i = 0; i < instance.getNbJob(); ++i) {
        cout << solution.getJ(i) << " " ;
    }
    cout << endl;

    // Compute the TWT of this solution
    totalWeightedTardiness = instance.computeWCT(solution);
    cout << "Total weighted completion time: " << totalWeightedTardiness << endl;

  //end
  return 0;
}

void vnd(Solution & sol, PfspInstance & instance, LocalSearch neighborhoud [], int nbneighborhoud) {
    int i = 0;
    while (i < nbneighborhoud) {
        if (neighborhoud[i].search(instance,sol)) {
            i = 0;
        }  else {
            ++i;
        }
    }
}