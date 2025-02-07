// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_GENETICCODE_GENETICCODE_H
#define BPP_SEQ_GENETICCODE_GENETICCODE_H

#include <Bpp/Exceptions.h>

#include "../Alphabet/AlphabetTools.h"
#include "../Alphabet/CodonAlphabet.h"
#include "../Alphabet/ProteicAlphabet.h"
#include "../Transliterator.h"

namespace bpp
{
/**
 * @brief Exception thrown when a stop codon is found.
 */
class StopCodonException :
  public virtual Exception
{
private:
  std::string codon_;

public:
  // Class constructor
  StopCodonException(const std::string& text, const std::string& codon);

  // Class destructor
  virtual ~StopCodonException() {}

public:
  virtual const std::string& getCodon() const { return codon_; }
};

/**
 * @brief Partial implementation of the Transliterator interface for genetic code object.
 *
 * A genetic code object if a translator from a codon alphabet to a
 * proteic alphabet. Depending on the codon alphabet used, several
 * genetic code can be implemented.
 *
 * @see CodonAlphabet, ProteicAlphabet
 */

class GeneticCode :
  public AbstractTransliterator,
  public virtual Clonable
{
protected:
  std::shared_ptr<const CodonAlphabet> codonAlphabet_;
  std::shared_ptr<const ProteicAlphabet> proteicAlphabet_;
  std::map<int, int> tlnTable_;

public:
  GeneticCode(std::shared_ptr<const NucleicAlphabet> alphabet) :
    AbstractTransliterator(),
    codonAlphabet_(new CodonAlphabet(alphabet)),
    proteicAlphabet_(AlphabetTools::PROTEIN_ALPHABET),
    tlnTable_()
  {}

  virtual ~GeneticCode() {}

  virtual GeneticCode* clone() const override = 0;

public:
  /**
   * @name Methods form the Transliterator interface.
   *
   * @{
   */
  std::shared_ptr<const Alphabet> getSourceAlphabet() const override { return codonAlphabet_; }

  const Alphabet& sourceAlphabet() const override { return *codonAlphabet_; }

  std::shared_ptr<const Alphabet> getTargetAlphabet() const override { return proteicAlphabet_; }

  const Alphabet& targetAlphabet() const override { return *proteicAlphabet_; }

  int translate(int state) const override;

  std::string translate(const std::string& state) const override;

  std::unique_ptr<Sequence> translate(const SequenceInterface& sequence) const override
  {
    return AbstractTransliterator::translate(sequence);
  }
  /** @} */

public:
  /**
   * @name Specific methods.
   *
   * @{
   */

  /**
   * @brief Alias for getSourceAlphabet return a pointer toward a CodonAlphabet.
   *
   * @return A pointer toward the codon alphabet.
   */
  virtual std::shared_ptr<const CodonAlphabet> getCodonAlphabet() const { return codonAlphabet_; }

  /**
   * @brief Alias for getSourceAlphabet return a pointer toward a CodonAlphabet.
   *
   * @return A reference toward the codon alphabet.
   */
  virtual const CodonAlphabet& codonAlphabet() const { return *codonAlphabet_; }

  /**
   * @brief Alias for getTargetAlphabet return a pointer toward a ProteicAlphabet.
   *
   * @return A pointer toward the protein alphabet.
   */
  virtual std::shared_ptr<const ProteicAlphabet> getProteicAlphabet() const { return proteicAlphabet_; }

  /**
   * @brief Alias for getTargetAlphabet return a pointer toward a ProteicAlphabet.
   *
   * @return A reference toward the protein alphabet.
   */
  virtual const ProteicAlphabet& proteicAlphabet() const { return *proteicAlphabet_; }

  /**
   * @return The number of stop codons.
   */
  virtual size_t getNumberOfStopCodons() const = 0;

  /**
   * @return A vector will all int codes for stop codons.
   */
  virtual std::vector<int> getStopCodonsAsInt() const = 0;

  /**
   * @return A vector will all char codes for stop codons.
   */
  virtual std::vector<std::string> getStopCodonsAsChar() const = 0;

  /**
   * @brief Tells is a particular codon is a stop codon.
   *
   * @param state The numeric code for the state to test.
   * @return True if the state corresponds to a stop codon.
   */
  virtual bool isStop(int state) const = 0;

  /**
   * @brief Tells is a particular codon is a stop codon.
   *
   * @param state The character code for the state to test.
   * @return True if the state corresponds to a stop codon.
   */
  virtual bool isStop(const std::string& state) const = 0;

  /**
   * @brief Tells is a particular codon is a start codon.
   *
   * @param state The numeric code for the state to test.
   * @return True if the state corresponds to a start codon.
   */
  virtual bool isStart(int state) const
  {
    // Test:
    codonAlphabet_->intToChar(state); // throw exception if invalid state!
    return state == 14;
  }

  /**
   * @brief Tells is a particular codon is a start codon.
   *
   * @param state The character code for the state to test.
   * @return True if the state corresponds to a start codon.
   */
  virtual bool isStart(const std::string& state) const
  {
    return isStart(codonAlphabet_->charToInt(state));
  }

  /**
   * @brief Tells is a particular codon is an alternative start codon.
   *
   * @param state The numeric code for the state to test.
   * @return True if the state corresponds to an alternative start codon.
   */
  virtual bool isAltStart(int state) const = 0;

  /**
   * @brief Tells is a particular codon is an alternative start codon.
   *
   * @param state The character code for the state to test.
   * @return True if the state corresponds to an alternative start codon.
   */
  virtual bool isAltStart(const std::string& state) const = 0;

  /**
   * @brief Tell if two codons are synonymous, that is, if they encode the same amino-acid.
   *
   * @param i The numeric code for the first codon.
   * @param j The numeric code for the second codon.
   * @return True if the two codons are synonymous.
   */
  bool areSynonymous(int i, int j) const
  {
    return translate(i) == translate(j);
  }

  /**
   * @brief Tell if two codons are synonymous, that is, if they encode the same amino-acid.
   *
   * @param i The character code for the first codon.
   * @param j The character code for the second codon.
   * @return True if the two codons are synonymous.
   */
  bool areSynonymous(const std::string& i, const std::string& j) const
  {
    return translate(i) == translate(j);
  }

  std::vector<int> getSynonymous(int aminoacid) const;

  std::vector<std::string> getSynonymous(const std::string& aminoacid) const;

  /**
   * @return True if the specified codon is fourfold degenerated
   * (that is, if a mutation in the fourth position does not change the aminoacid).
   * @author Benoit Nabholz, Annabelle Haudry
   * @param codon The codon to test.
   */
  bool isFourFoldDegenerated(int codon) const;

  /**
   * @brief Get the subsequence corresponding to the coding part of a given sequence.
   *
   * If lookForInitCodon if set to 'true', the subsequence will start at the first AUG motif,
   * otherwise the subsequence will start at the begining of the sequence.
   * The subsequence ends at the first stop codon (excluded) found, or the end of the sequence.
   *
   * The sequence may have a nucleotide or codon alphabet.
   * The subsequence has the same alphabet, name and comments of the input sequence.
   * In case of nucleotide sequence and if the lookForInitCodon option is checked, the phase
   * will be determined from the sequence.
   *
   * @param sequence The sequence to parse.
   * @param lookForInitCodon Tell if the AUG codon must be found.
   * @param includeInitCodon (if lookForInitCodon is true) tell if the init codon must be included in the subsequence.
   * @return A nucleotide/codon subsequence.
   */
  std::unique_ptr<Sequence> getCodingSequence(const SequenceInterface& sequence, bool lookForInitCodon = false, bool includeInitCodon = false) const;
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_SEQ_GENETICCODE_GENETICCODE_H
