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

using namespace std;

int main(int argc, char *argv[]) {
  //variable
    int i;
    long int totalWeightedTardiness;
    PfspInstance instance; // Create instance object

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

    Solution solution ( instance.getNbJob() ); // Create a Solution to represent the solution

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
  //end
  return 0;
}
