//
// File: SimpleScore.cpp
// Created by: Julien Dutheil
// Created on: Fri May 04 09:35 2007
//

/*
   Copyright or © or Copr. CNRS, (November 17, 2004)

   This software is a computer program whose purpose is to provide classes
   for sequences analysis.

   This software is governed by the CeCILL  license under French law and
   abiding by the rules of distribution of free software.  You can  use,
   modify and/ or redistribute the software under the terms of the CeCILL
   license as circulated by CEA, CNRS and INRIA at the following URL
   "http://www.cecill.info".

   As a counterpart to the access to the source code and  rights to copy,
   modify and redistribute granted by the license, users are provided only
   with a limited warranty  and the software's author,  the holder of the
   economic rights,  and the successive licensors  have only  limited
   liability.

   In this respect, the user's attention is drawn to the risks associated
   with loading,  using,  modifying and/or developing or reproducing the
   software by the user in light of its specific status of free software,
   that may mean  that it is complicated to manipulate,  and  that  also
   therefore means  that it is reserved for developers  and  experienced
   professionals having in-depth computer knowledge. Users are therefore
   encouraged to load and test the software's suitability as regards their
   requirements in conditions enabling the security of their systems and/or
   data to be ensured and,  more generally, to use and operate it in the
   same conditions as regards security.

   The fact that you are presently reading this means that you have had
   knowledge of the CeCILL license and that you accept its terms.
 */

// from the STL:
#include <string>

using namespace std;

#include "SimpleScore.h"

using namespace bpp;

SimpleScore::SimpleScore(const Alphabet* alphabet, double match, double mismatch) :
  distanceMatrix_(alphabet->getSize(), alphabet->getSize()),
  alphabet_(alphabet)
{
  // Load the matrix:
  unsigned int n = alphabet_->getSize();
  for (unsigned int i = 0; i < n; i++)
  {
    for (unsigned int j = 0; j < n; j++)
    {
      distanceMatrix_(i, j) = (i == j ? match : mismatch);
    }
  }
}

double SimpleScore::getIndex(int state1, int state2) const
throw (BadIntException)
{
  if (state1 < 0 || state1 > (int)alphabet_->getSize())
    throw BadIntException(state1, "SimpleScore::getIndex(). Invalid state1.", alphabet_);
  if (state2 < 0 || state2 > (int)alphabet_->getSize())
    throw BadIntException(state2, "SimpleScore::getIndex(). Invalid state2.", alphabet_);
  return distanceMatrix_(state1, state2);
}

double SimpleScore::getIndex(const std::string& state1, const std::string& state2) const
throw (BadCharException)
{
  return distanceMatrix_(alphabet_->charToInt(state1), alphabet_->charToInt(state2));
}

LinearMatrix<double>* SimpleScore::getIndexMatrix() const
{
  return new LinearMatrix<double>(distanceMatrix_);
}
