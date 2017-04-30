/**
 * \file sa.hpp
 * \author dorian dumez
 * \brief define object to perform a simulated annealing
 */

#ifndef SA
#define SA

#include "solution.hpp"
#include "pfspinstance.hpp"

/**
* \class SA : contain all procedure for the simulated anealing
*/
class SA{
	private:
		double t; /*< temperature */
		long l; /*< lenght of a constant temperature time */
		double alpha; /*< cooling factor */
		long date; /*< number of cycle before the end of the plateau */
		Solution * best; /*< best solution found so far */
  		Solution current; /*< current state of our search */
  		PfspInstance * instance; /*< instance on which we work */
  		int nbJob; /*< number of job in this instance, copy to quick access it*/
  		long bestval; /*< value of the best solution found so far */

	public:
		/**
		* \brief initialise the object for a simulated anealing search
		*
		* \param[in] t0 initial temperature
		* \param[in] l lenght of temperature plateau
		* \param[in] alpha cooling factor
		* \param[in] instance instance on which we work
		*/
		SA(const double t0, const long l, const double alpha, PfspInstance * instance);

		/**
		* \brief free the memory
		*/
		~SA();

		/**
	  	* \brief explore the seraching space during a certain amount of time
	  	*
	  	* \param[in] tmax time budget in seconds
	  	* \return the best solution found
	  	*/
	  	Solution * search(const clock_t tmax);

  private:
		/**
		* \brief generate a random number in an interval
		*
		* \param[in] min the lower bound
		* \param[in] max the upper bound
		*
		* \return a random int between max and min
		*/
		int generateRndPosition(const int min, const int max) const;

};

#endif