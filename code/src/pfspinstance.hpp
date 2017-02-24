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

/**
 * \file pfspinstance.hpp
 * \brief define object wich containt instance's informations
 */

#ifndef _PFSPINSTANCEWT_H_
#define _PFSPINSTANCEWT_H_

#include <string>
#include <vector>

using namespace std;

/**
* \class PfspInstance : containt all instance's informations
*/
class PfspInstance{
  private:
    int nbJob; /*!<number of jobs to do */
    int nbMac; /*!<number of machines to use */
    std::vector< long int > dueDates; /*!< not used */
    std::vector< long int > priority; /*!< the weight of the job in the objective function*/

    std::vector< std::vector <long int> > processingTimesMatrix; /*!< time for a job to be processed by a machine*/

  public:
    /**
    * \brief initialise an empty instance
    */
    PfspInstance();
    
    /**
    * \brief free the memory taken by an instance
    */
    ~PfspInstance();

    /**
    * \brief get the number of job to do
    *
    * \return the number of job
    */
    int getNbJob();

    /**
    * \brief get the number of machine
    *
    * \return the number of machine
    */
    int getNbMac();

    /**
    * \brief to get the processing time of a job on a machine
    *
    * \param[in] job the index of the job to consider
    * \param[in] machine the index of the machine to consider
    *
    * \return the processing of the job "job" on the machine "machine"
    */
    long int getTime(int job, int machine);

    /* ????????? */
    long int getDueDate(int job);
    void setDueDate(int job, int value);

    /**
    * \brief to get the value of the weight of this job
    *
    * \param[in] jab the index of the job to consider
    *
    * \return the weight of this job
    */
    long int getPriority(int job);
    
    /* Read Data from a file : */
    bool readDataFromFile(char * fileName);

    /**
    * \brief compute the weighted sum of completion time
    *
    * \param[in] the solution to consider
    *
    * \return the value of the objective function on this permutation
    */
    long int computeWCT (vector< int > & sol);

  private:
    /**
    * \brief Allow the memory for the processing times matrix
    */
    void allowMatrixMemory(int nbJ, int nbM);

    /**
    * \brief to set the processing time of a job on a machine
    *
    * \param[in] job the index of the job to consider
    * \param[in] machine the index of the machine to consider
    * \param[in] processTime the time do do this task
    */
    void setTime(int job, int machine, long int processTime);

    /**
    * \brief to set the value of the weight of this job
    *
    * \param[in] jab the index of the job to consider
    * \param[in] value the weight of this job
    */
    void setPriority(int job, int value);

};

#endif
