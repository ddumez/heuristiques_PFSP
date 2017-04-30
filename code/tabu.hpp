/**
 * \file tabu.hpp
 * \author dorian dumez
 * \brief define object to perform a tabu search
 */

#ifndef TABU
#define TABU

#include "solution.hpp"
#include "pfspinstance.hpp"

/**
* \class Tabu : contain all procedure for the tabu search
*/
class Tabu{
  private:
  	int tabuListLenght; /*< size of the tabu list */
  	std::vector< std::vector< int > > TL; /*< tabu list + long time memory */
  	long date; /*< number of performed loops */
  	Solution * best; /*< best solution found so far */
  	Solution current; /*< current state of our search */
  	PfspInstance * instance; /*< instance on which we work */
  	int nbJob; /*< number of job in this instance, copy to quick access it*/
  	long bestval; /*< value of the best solution found so far */
    double longTimeMemoryImpact; /*< impact of the long time memory to the choice of the movement */
    double restartThreshold; /*< thresold ratio for restart */

  public:
  	/**
  	* \brief initialize the search
  	*
  	* \param[in] tabuListLenght size of the tabu list
    * \param[in] longTimeMemoryImpact factor of penalisation of the long time memory
    * \param[in] restartThreshold ratio between the curent and the best solution to initiate a restart from the best solution
  	* \param[in] instance instance on which we sould work
  	*/
  	Tabu(const int tabuListLenght, const double longTimeMemoryImpact, const double restartThreshold, PfspInstance * instance);

  	/**
  	* \brief free memory
  	*/
  	~Tabu();

  	/**
  	* \brief explore the seraching space during a certain amount of time
  	*
  	* \param[in] tmax time budget in seconds
  	* \return the best solution found
  	*/
  	Solution * search(const clock_t tmax);

};
#endif