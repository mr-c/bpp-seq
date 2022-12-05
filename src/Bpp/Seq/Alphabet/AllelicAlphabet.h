//
// File: AllelicAlphabet.h
// Authors:
//   Laurent Gueguen
// Created: lundi 20 décembre 2021, à 10h 28
//

/*
  Copyright or Â© or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for sequences analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided
  only with a limited warranty and the software's author, the holder of
  the economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards
  their requirements in conditions enabling the security of their
  systems and/or data to be ensured and, more generally, to use and
  operate it in the same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_SEQ_ALPHABET_ALLELICALPHABET_H
#define BPP_SEQ_ALPHABET_ALLELICALPHABET_H

#include "AbstractAlphabet.h"

// From the STL:
#include <string>
#include <vector>

#include "../Transliterator.h"
#include <Bpp/Seq/ProbabilisticSequence.h>

namespace bpp
{
/**
 * @brief The base class for allelic alphabets.
 *
 * These alphabets are made of unoriented pairs (element from an
 * alphabet, N)^2, such that both numbers sum a given constant @f$N@f$
 * number of individuals. The construction is made from a
 * shared_pointer to an AbstractAlphabet.
 *
 * The strings of the AllelicAlphabet are pairs of the strings of the
 * Alphabets, with the attributed numbers, such that the first element
 * is before the second element in the base Alphabet (such as A3C4,
 * and not C4A3). They are made from the resolved letters of the
 * Alphabets.
 *
 * The letters are numbered as:
 *
 *  -1  : gap
 *
 *  0 -> alphabet size:  alphabet states
 *
 *  (i * (alphabet size) + j) * (nbAlleles-1) + size + k -1 : @f$ a_i k  a_j (N-k)@f$ state
 *
 *  (alphabet size)^2 * (nbAlleles -1) : Unknown "?N?0"
 *
 * For nucleotides with three alleles, the underlying states are for instance:
 *
 * _3_0 (-1), A3_0 (0), C3_0 (1), G3_0 (2), T3_0 (3), A2C1 (6), A1C2 (7), A2G1 (8), A1G2 (9),
 * A2T1 (10), A1T2 (11), C2G1 (16), C1G2 (17), C2T1 (18), C1T2 (19), G2T1 (26), G1T2 (27), ?3_0 (32)
 *
 * 
 */
  
  class AllelicAlphabet :
    public AbstractAlphabet
  {
  protected:
    const Alphabet& alph_;
    
    /*
     * @brief the number of alleles.
     *
     */
    
    uint nbAlleles_;
    
    /*
     *@brief the unknown state number
     *
     */

    int nbUnknown_;
    
  public:
    // Constructor and destructor.
    /**
     * @brief Builds a new word alphabet from an  Alphabet
     *
     */
    AllelicAlphabet(const Alphabet& alph, uint nbAlleles);

    AllelicAlphabet(const AllelicAlphabet& bia) : AbstractAlphabet(bia), alph_(bia.alph_), nbAlleles_(bia.nbAlleles_), nbUnknown_(bia.nbUnknown_)
    {}

     // AllelicAlphabet& operator=(const AllelicAlphabet& bia)
     // {
     //   AbstractAlphabet::operator=(bia);
     //   alph_ = bia.pAlph_;
     //   nbAlleles_ = bia.nbAlleles_;
     //   nbUnknown_ = bia.nbUnknown_;
  
     //   return *this;
     // }

     AllelicAlphabet* clone() const
     {
       return new AllelicAlphabet(*this);
     }

    virtual ~AllelicAlphabet() {}

   public:
     /**
      * @name Methods redefined from Alphabet
      *
      * @{
      */

    int charToInt(const std::string& state) const
    {
       if (state.size() != getStateCodingSize())
         throw BadCharException(state, "AllelicAlphabet::charToInt", this);
       else
         return  AbstractAlphabet::charToInt(state);
    }

    unsigned int getSize() const
    {
      return getNumberOfChars() - 2;
    }
    
    unsigned int getNumberOfTypes() const
     {
       return getNumberOfChars() - 1;
     }

 /** @} */


     bool isResolvedIn(int state1, int state2) const;

     /**
      * @brief Returns the number of alleles
      *
      */
     unsigned int getNbAlleles() const
     {
       return nbAlleles_;
     }

    /**
     * @brief Returns Base Alphabet
     *
     */
    const Alphabet& getStateAlphabet() const
    {
      return alph_;
    }

     std::string getAlphabetType() const;

     int getUnknownCharacterCode() const
     {
       return nbUnknown_;
     }

     bool isUnresolved(int state) const { return state == getUnknownCharacterCode(); }

     bool isUnresolved(const std::string& state) const { return charToInt(state) == getUnknownCharacterCode(); }

     std::vector<int> getAlias(int state) const;

     std::vector<std::string> getAlias(const std::string& state) const;

     int getGeneric(const std::vector<int>& states) const
     {
       return states[0];
     }

     std::string getGeneric(const std::vector<std::string>& states) const
     {
       return states[0];
     }

    class AllelicTransliterator : public AbstractTransliterator
    {
    private:

      const AllelicAlphabet& alph_;

    public:
      AllelicTransliterator(const AllelicAlphabet& alph) : AbstractTransliterator(), alph_(alph) {}

      const Alphabet* getSourceAlphabet() const { return &alph_.getStateAlphabet(); }
      const Alphabet* getTargetAlphabet() const { return &alph_; }

      std::string translate(const std::string& state) const
      {
        return getTargetAlphabet()->intToChar(getSourceAlphabet()->charToInt(state));
      }

      /**
       * @brief States of the original alphabet are the first ones of
       * the allelic alphabet.
       *
       */
      
      int translate(int state) const
      {
        return state;
      }
      
      Sequence* translate(const Sequence& sequence) const
      {
        return AbstractTransliterator::translate(sequence);
      }
    };

    /**
     * @brief Convert a CoreSequence in StateAlphabet to a
     * ProbabilisticSequence of the likelihoods of the di-allelic
     * Alphabet (so if counts are not on only two states, the
     * likelihood is null).
     *
     * @param sequence the CoreSequence to be converted.
     * 
     *  Gaps are directly translated in vectors of 1.
     */
    
    BasicProbabilisticSequence* convertFromStateAlphabet(const CoreSequence& sequence) const;
    
    /**
     * @brief Fills the vector of the likelihoods of a vector of
     * counts in the states alphabet, given the di-alleless.
     *
     **/

    void computeLikelihoods(const Vdouble& counts, Vdouble& likelihoods) const;
    
   private:
     /**
      * @name Inner utilitary functions
      *
      * @{
      */
     // bool containsUnresolved(const std::string& state) const;
     // bool containsGap(const std::string& state) const;

     /** @} */

     /**
      * @name Overloaded AbstractAlphabet methods.
      * @{
      */
     unsigned int getStateCodingSize() const
    {
      auto x = 2*((uint)alph_.getStateCodingSize()+(uint)std::to_string(nbAlleles_).size());
      return x;
     }
    
    /** @} */
  };
} // end of namespace bpp.

#endif // BPP_SEQ_ALPHABET_ALLELICALPHABET_H
