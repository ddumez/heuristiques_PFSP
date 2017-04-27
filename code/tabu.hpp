/**
 * \file solution.hpp
 * \author dorian dumez
 * \brief define object to perform a tabu search
 */

#ifndef TABU
#define TABU

#include "solution.hpp"
#include "pfspinstance.hpp"

/**
* \class LocalSearch : contain all procedure for the tabu search
*/
class Tabu{
  private:
  	int tabuListLenght; /*< size of the tabu list */
  	std::vector< int > TL; /*< tabu list */
  	long date; /*< number of performed loops */
  	Solution * best; /*< best solution found so far */
  	Solution current; /*< current state of our search */
  	PfspInstance * instance; /*< instance on which we work */
  	int nbJob; /*< number of job in this instance, copy to quick access it*/
  	long bestval; /*< value of the best solution found so far */

  public:
  	/**
  	* \brief initialize the search
  	*
  	* \param[in] tabuListLenght size of the tabu list
  	* \param[in] instance instance on which we sould work
  	*/
  	Tabu(const int tabuListLenght, PfspInstance * instance);

  	/**
  	* \brief free memory
  	*/
  	~Tabu();

  	/**
  	* \brief change the tabu list lenght
  	*
  	* \param[in] tll the new tabu list lenght
  	*/
  	void changetTLL(const int tll);

  	/**
  	* \brief explore the seraching space during a certain amount of time
  	*
  	* \param[in] tmax time budget in seconds
  	* \return the best solution found
  	*/
  	Solution * search(const clock_t tmax);

};
#endif