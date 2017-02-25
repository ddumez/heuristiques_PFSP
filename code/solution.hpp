/**
 * \file solution.hpp
 * \brief define object wich containt solution's information and function
 */

#ifndef SOLUTION
#define SOLUTION

#include <vector>

#include "pfspinstance.hpp"

class PfspInstance;

/**
* \class Solution : containt all solution's informations
*/
class Solution{
  private:
  	std::vector< int > solution; /*!<contains the job's index in the order they should be done */
  	int nbJobs; /*!< number of jobs to do */

  public:

  	/**
  	* \brief create an empty solution for an instance with nbJObs jobs
  	*
  	* \param[in] nbJobs number of jobs to do in the related instance
  	*/
  	Solution(int nbJobs);

  	/**
  	* \brief free the memmory allocated to this solution
  	*/
  	~Solution();

  	/**
  	* \brief to get the jth job to do (start from 0)
  	*
  	* \param[in] the index of the jth job to do
  	*
  	* \return the index of the jth job to do
  	*/
  	int getJ(int j);

  	/**
	* \brief Fill the solution with numbers between 1 and nbJobs, shuffled
	*/
	void randomPermutation();

	/**
	* \brief construct a solution with the RZ heuristic
	*/
	void constructRZ(PfspInstance & instance);

  private:
  	/**
	* \brief generate a random number in an interval
	*
	* \param[in] min the lower bound
	* \param[in] max the upper bound
	*
	* \return a random int between max and min
	*/
	int generateRndPosition(int min, int max);

};

#endif