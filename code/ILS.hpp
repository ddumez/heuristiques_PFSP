/**
 * \file ILS.hpp
 * \author dorian dumez
 * \brief define object wich containt the ILS
 */

#ifndef ILS
#define ILS

#include <climits>

#include "localsearch.hpp"
#include "pfspinstance.hpp"
#include "solution.hpp"

/**
* \class Ils : containt the Ils
*/
class Ils{
	private:
		LocalSearch ** neighborhoud; /*<local search to use */
		int nbneighborhoud; /* number of neighbourhood relation */
		int neighboursPerturb; /*<code of the neighbours relation to use for perturbation : 1->exchange, 2->insert*/
		double perturbFrac; /*< fraction of the solution that must be perturbed */
		double perturbRadius; /*< caracterise the maximal distance between two perturb job */
		int acceptanceCrit; /*<code of the acceptance criterion to use : 1->only improvement, 2->always, 3->metropolis(need to defined other parameter), 4->simulated annealing type*/
		double alpha; /*<cooling factor for the metropolis criterion*/
		long l; /*<size of a cooling plateau for the metropolis criterion*/
		double warmupThreshold; /*<if this temperature is reach a warm up is use for the metropolis criterion*/
		double T1; /*<value of the temperature after a warm up for the metropolis criterion*/
  		long date; /*< number of loop to performed before the next cooling*/
  		double T; /*< current temperature, for the metropolis criterion*/
  		Solution * best; /*< best solution found so far */
  		long bestval; /*< value of the best solution found so far */
  		Solution * current; /*< current state of our search */
  		PfspInstance * instance; /*< instance on which we work */
  		int nbJob; /*< number of job in this instance, copy to quick access it*/
  	
	public:
		/**
		* \brief empty constructor, need to call init before use this object
		*/
		Ils();

		/**
		* \brief free the memory
		*/
		~Ils();

		/**
		* \brief initialize the Ils with all parameter
		*
		* \param[in] neighbours code of the neighbours relation to use : 1-> transpose, 2->exchange, 3->insert, 4->VND1, 5->VND2
		* \param[in] neighboursPerturb code of the neighbours relation to use for perturbation : 1->exchange, 2->insert
		* \param[in] DD true iff the neighbourhood should be crossed in deapest descent
		* \param[in] acceptanceCrit code of the acceptance criterion to use : 1->only improvement, 2->always, 3->metropolis, 4->simulated annealing type
		* \param[in] perturbFrac fraction of the solution that must be perturbed
		* \param[in] perturbRadius caracterise the maximal distance between two perturb job
		* \param[in] instance instance on which we work
		* \param[in] T0 initial temperature for the metropolis criterion or the simulated annealing type
		* \param[in] alpha cooling factor for the metropolis criterion
		* \param[in] l size of a cooling plateau for the metropolis criterion
		* \param[in] warmupThreshold if this temperature is reach a warm up is use for the metropolis criterion
		* \param[in] T1 value of the temperature after a warm up for the metropolis criterion
		*/
		void init(const int neighbours, const int neighboursPerturb, const bool DD, const int acceptanceCrit, const double perturbFrac, const double perturbRadius, PfspInstance * instance, const double T0 = 0.0, const double alpha = 1.0, const long l = LONG_MAX, const double warmupThreshold = -1.0, const double T1 = 0.0);

		/**
	  	* \brief explore the seraching space during a certain amount of time
	  	*
	  	* \param[in] tmax time budget in seconds
	  	* \return the best solution found
	  	*/
	  	Solution * search(const clock_t tmax);

	private:
		/**
		* \brief randomly modified the current solution
		*/
		Solution * perturbe();

		/**
		* \brief compute if we should keep this solution
		*
		* \param[in] valprev value of the previous keeped solution
		* \param[in] sol the solution to test
		* \return true iff we should keep this modification
		*/
		bool keep(const long valprev, Solution * sol) const;

		/**
		* \brief do the selected local search (a VND if multiple neighbourhood else a descent)
		*
		* \param[in,out] parc the starting solution for the local search
		*/
		void Localsearch(Solution * parc);

};

#endif