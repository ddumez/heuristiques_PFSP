/**
 * \file solution.hpp
 * \author dorian dumez
 * \brief define object wich containt solution's information and function
 */

#ifndef LOCALSEARCH
#define LOCALSEARCH

#include "solution.hpp"
#include "pfspinstance.hpp"

/**
* \class LocalSearch : caontains all neibouhoud relation
*/
class LocalSearch{
  private:
  	int choix; /*!< 1->transpose, 2-> exchange, 3->insert */
    bool PPD; /*!< true iff the local search is in deapest descent */

  public:

  	/**
  	* \brief create an object to aplly a choosen neighborhoud relation in a local search
  	*
  	* \param[in] choix the index of the neighborhoud to use
    * \param[in] PPD the way to use the local search
  	*/
  	LocalSearch(int choix, bool PPD);

    /**
    * \brief free the memory of the object
    */
    ~LocalSearch();

    /**
    * \brief improve the solution with the prevously defined way
    *
    * \param[in] instance the instance of the problem
    * \param[in,out] sol the solution to improve
    *
    * \return true iff the solution was imrove
    */
    bool search(PfspInstance & instance, Solution & sol);

    /**
    * \brief use the neighborhoud relation in the defined way until no improvement are possible
    *
    * \param[in] instance the instance of the problem
    * \param[in,out] sol the solution to improve
    *
    * \return true iff the solution was imrove
    */
    void descent(PfspInstance & instance, Solution & sol);

  private:
    /**
    * \brief improve the solution with the transpose neighborhoud
    *
    * \param[in] instance the instance of the problem
    * \param[in,out] sol the solution to improve
    *
    * \return true iff the solution was imrove
    */
    bool transpose(PfspInstance & instance, Solution & sol);

    /**
    * \brief improve the solution with transpose neighborhoud in deapest descent
    *
    * \param[in] instance the instance of the problem
    * \param[in,out] sol the solution to improve
    *
    * \return true iff the solution was imrove
    */
    bool transposePPD(PfspInstance & instance, Solution & sol);

    /**
    * \brief improve the solution with exchange neighborhoud
    *
    * \param[in] instance the instance of the problem
    * \param[in,out] sol the solution to improve
    *
    * \return true iff the solution was imrove
    */
    bool exchange(PfspInstance & instance, Solution & sol);

    /**
    * \brief improve the solution with exchange neighborhoud in deapest descent
    *
    * \param[in] instance the instance of the problem
    * \param[in,out] sol the solution to improve
    *
    * \return true iff the solution was imrove
    */
    bool exchangePPD(PfspInstance & instance, Solution & sol);

    /**
    * \brief improve the solution with insert neighborhoud
    *
    * \param[in] instance the instance of the problem
    * \param[in,out] sol the solution to improve
    *
    * \return true iff the solution was imrove
    */
    bool insert(PfspInstance & instance, Solution & sol);

    /**
    * \brief improve the solution with insert neighborhoud in deapest descent
    *
    * \param[in] instance the instance of the problem
    * \param[in,out] sol the solution to improve
    *
    * \return true iff the solution was imrove
    */
    bool insertPPD(PfspInstance & instance, Solution & sol);

};

#endif