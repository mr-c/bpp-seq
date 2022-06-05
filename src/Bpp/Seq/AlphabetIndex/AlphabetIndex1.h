//
// File: AlphabetIndex1.h
// Authors:
//   Julien Dutheil
// Created: 2005-02-21 17:42:00
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  Julien.Dutheil@univ-montp2.fr
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_SEQ_ALPHABETINDEX_ALPHABETINDEX1_H
#define BPP_SEQ_ALPHABETINDEX_ALPHABETINDEX1_H

#include <Bpp/Clonable.h>

#include "../Alphabet/Alphabet.h"
#include "../Alphabet/AlphabetTools.h"
#include "../Alphabet/ProteicAlphabet.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief One dimensionnal alphabet index interface.
 *
 * Derivatives of this interface implement properties for a single state.
 */
class AlphabetIndex1 :
  public virtual Clonable
{
public:
  AlphabetIndex1() {}
  virtual ~AlphabetIndex1() {}

public:
  virtual AlphabetIndex1* clone() const override = 0;

  /**
   * @brief Get the index associated to a state.
   *
   * @param state The state to consider, as a int value.
   * @return The index associated to the state
   */
  virtual double getIndex(int state) const = 0;

  /**
   * @brief Get the index associated to a state.
   *
   * @param state The state to consider, as a string value.
   * @return The index associated to the state
   */
  virtual double getIndex(const std::string& state) const = 0;

  /**
   * @brief Get the alphabet associated to this index.
   *
   * @return Alphabet The alphabet associated to this index.
   */
  virtual std::shared_ptr<const Alphabet> getAlphabet() const = 0;

  /**
   * @return A vector object with all indices.
   */
  virtual std::vector<double>* getIndexVector() const = 0;
};

/*
 * @brief Virtual AlphabetIndex1 for proteic alphabet
 *
 */

class ProteicAlphabetIndex1 :
  virtual public AlphabetIndex1
{
private:
  std::shared_ptr<const ProteicAlphabet> alpha_;

public:
  ProteicAlphabetIndex1() : alpha_(std::dynamic_pointer_cast<const ProteicAlphabet>(AlphabetTools::PROTEIN_ALPHABET.shared_from_this())) {}
  virtual ~ProteicAlphabetIndex1() {}

  virtual ProteicAlphabetIndex1* clone() const override = 0;

  ProteicAlphabetIndex1(const ProteicAlphabetIndex1& pt) :
    alpha_(pt.alpha_)
  {}

  ProteicAlphabetIndex1& operator=(const ProteicAlphabetIndex1& pt)
  {
    alpha_ = pt.alpha_;
    return *this;
  }

  std::shared_ptr<const Alphabet> getAlphabet() const override { return alpha_; }
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_ALPHABETINDEX1_H
