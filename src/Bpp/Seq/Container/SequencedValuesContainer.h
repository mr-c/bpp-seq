// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_SEQUENCEDVALUESCONTAINER_H
#define BPP_SEQ_CONTAINER_SEQUENCEDVALUESCONTAINER_H

#include <Bpp/Clonable.h>

#include "../Alphabet/Alphabet.h"
#include "../Commentable.h"
#include "SequenceContainerExceptions.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief The Sequenced Values Container interface.
 *
 * This interface is the most general one in the container hierarchy.
 * No assumption is made on the data in the container (no ordering, no
 * alignment, no type).
 *
 */

class Sequence;

class SequencedValuesContainer :
  public virtual Clonable
{
// public:
//   SequencedValuesContainer() {}
//   virtual ~SequencedValuesContainer() {}

public:
  /**
   * @brief Get container's alphabet.
   *
   * @return The alphabet associated to this container.
   */
  virtual const Alphabet* getAlphabet() const = 0;

  /**
   * @brief Convert a particular sequence to a string.
   *
   * @param name The name of the sequence.
   * @return A string describing the content of the sequence.
   * @throw SequenceNotFoundException If the name does not match any sequence in the container.
   */
  virtual std::string toString(const std::string& name) const = 0;

  /**
   * @brief Check if a sequence with a given name is present in the container.
   *
   * @param name The name of the sequence.
   * @return True if a sequence with the given name is present in the container.
   */
  virtual bool hasSequence(const std::string& name) const = 0;

  /**
   * @brief Get the number of sequences in the container.
   *
   * @return The number of sequences in the container.
   */
  virtual size_t getNumberOfSequences() const = 0;

  /**
   * @brief Get all the names of the sequences in the container.
   *
   * @return A vector of strings with all sequence names.
   */
  virtual std::vector<std::string> getSequenceNames() const = 0;

  /**
   * @brief Set all sequence names.
   *
   * @param names A vector of strings with all sequence names.
   * Its size must be strictly equal to the the size of the container (the number of
   * sequences).
   * @param checkNames Tell if the container must check if the name of the sequence
   * is already used in the container before adding it.
   * @throw Exception If there are redundant names in the input vector.
   */
  virtual void setSequenceNames(const std::vector<std::string>& names, bool checkNames) = 0;

  /**
   * @brief get value of a state in a position
   * @param siteIndex  index of the site
   * @param sequenceName name of the sequence in the container
   * @param state  state in the alphabet
   */

  virtual double getStateValueAt(size_t siteIndex, const std::string& sequenceName, int state) const = 0;

  virtual double operator()(size_t siteIndex, const std::string& sequenceName, int state) const = 0;

  /**
   * @brief Get comments of a particular sequence.
   *
   * @param name The name of the sequence.
   * @return The comments associated to sequence with name 'name'.
   * @throw SequenceNotFoundException If the name does not match any sequence in
   * the container.
   */
  virtual const Comments& getComments(const std::string& name) const = 0;

  /**
   * @brief Set the comments of a particular sequence.
   *
   * @param name     The name of the sequence.
   * @param comments The comments to set to sequence with name 'name'.
   * @throw SequenceNotFoundException If the name does not match any sequence in
   * the container.
   */
  virtual void setComments(const std::string& name, const Comments& comments) = 0;

  /**
   * @brief Get the comments of this container.
   *
   * @return The comments associated to this container.
   */

  virtual const Comments& getGeneralComments() const = 0;

  /**
   * @brief Set the comments of this container.
   *
   * @param comments The comments to be associated to this container.
   */

  virtual void setGeneralComments(const Comments& comments) = 0;

  /**
   * @brief Delete the comments associated to this container.
   */
  virtual void deleteGeneralComments() = 0;

  /**
   * @brief Delete all data in the container.
   */
  virtual void clear() = 0;

  /**
   * @brief Return a copy of this container, but with no data inside.
   *
   * This method creates a new SequencedValuesContainer objet.
   * The class of this container depends on the derivative class.
   *
   * @return A new empty container, with the same alphabet as this one.
   */

  virtual SequencedValuesContainer* createEmptyContainer() const = 0;
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_SEQUENCEDVALUESCONTAINER_H
