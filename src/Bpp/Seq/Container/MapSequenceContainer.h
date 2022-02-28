//
// File: MapSequenceContainer.h
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
//   Sylvain Gaillard
// Last modified: 2004-06-25 00:00:00
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

#ifndef BPP_SEQ_CONTAINER_MAPSEQUENCECONTAINER_H
#define BPP_SEQ_CONTAINER_MAPSEQUENCECONTAINER_H

#include <map>
#include <string>

#include "../Alphabet/Alphabet.h"
#include "../Sequence.h"
#include "AbstractSequenceContainer.h"

namespace bpp
{
/**
 * @brief MapSequenceContainer class
 *
 * Sequences are stored using a key std::string, in a map object.
 * Sequences are ordered according to the key order (defined by the < operator).
 *
 */

class MapSequenceContainer :
  public AbstractSequenceContainer
{
private:
  std::map<std::string, Sequence*> sequences_;

public:
  MapSequenceContainer(const std::map<std::string, Sequence*>& ms, const Alphabet* alpha);
  MapSequenceContainer(const Alphabet* alpha) :
    AbstractSequenceContainer(alpha), sequences_() {}

  MapSequenceContainer(const MapSequenceContainer& msc);
  MapSequenceContainer& operator=(const MapSequenceContainer& msc);

  virtual ~MapSequenceContainer();

public:
  /**
   * @brief Get a sequence.
   *
   * @param key The key of the sequence to retrieve.
   * @return The sequence associated to the given key.
   */
  const Sequence& getSequenceByKey(const std::string& key)  const;

  /**
   * @brief Set a sequence.
   *
   * @param key The key of the sequence.
   * @param sequence The new sequence that will be associated to the key.
   * @param checkNames Tell is the sequence name must be checked.
   */
  void setSequenceByKey(const std::string& key, const Sequence& sequence, bool checkNames = true);

  /**
   * @brief Remove a sequence.
   *
   * @param key The key of the sequence.
   * @return The sequence previously associated to the given key.
   */
  Sequence* removeSequenceByKey(const std::string& key);

  /**
   * @brief Delete a sequence.
   *
   * @param key The key of the sequence.
   */
  void deleteSequenceByKey(const std::string& key);

  /**
   * @brief Add a sequence and key.
   *
   * @param key The key of the new sequence.
   * @param sequence The new sequence that will be associated to the key.
   * @param checkNames Tell is the sequence name must be checked.
   */
  void addSequence(const std::string& key, const Sequence& sequence, bool checkNames = true);

  /**
   * @return All sequences keys.
   */
  std::vector<std::string> getKeys() const;

  /**
   * @return The key of a given sequence specified by its position in the container.
   * @param pos The index of the sequence.
   */
  std::string getKey(size_t pos) const;

  /**
   * @return The key of a given sequence specified by its name.
   * @param name The name of the sequence.
   */
  std::string getKey(const std::string& name) const;

  /**
   * @name The clonable interface
   *
   * @{
   */
  MapSequenceContainer* clone() const { return new MapSequenceContainer(*this); }
  /**
   * @}
   */

  /**
   * @name The SequenceContainer interface implementation:
   *
   * @{
   */
  const Sequence& getSequence(const std::string& name) const;

  bool hasSequence(const std::string& name) const;

  /**
   * @brief The SequenceContainer method. Calls the addSeqeucne(key, Sequence) method while using the resut of sequence.getName() as a key.
   */
  void addSequence(const Sequence& sequence, bool checkNames = true)
  {
    addSequence(sequence.getName(), sequence, checkNames);
  }
  void setSequence(const std::string& name, const Sequence& sequence, bool checkName = true);
  std::shared_ptr<Sequence> removeSequence(const std::string& name);
  void deleteSequence(const std::string& name);
  size_t getNumberOfSequences() const { return sequences_.size(); }
  void clear();
  MapSequenceContainer* createEmptyContainer() const;

  int& valueAt(const std::string& sequenceName, size_t elementIndex)
  {
    return getSequence_(sequenceName)[elementIndex];
  }
  const int& valueAt(const std::string& sequenceName, size_t elementIndex) const
  {
    return getSequence(sequenceName)[elementIndex];
  }
  int& operator()(const std::string& sequenceName, size_t elementIndex)
  {
    return getSequence_(sequenceName)[elementIndex];
  }
  const int& operator()(const std::string& sequenceName, size_t elementIndex) const
  {
    return getSequence(sequenceName)[elementIndex];
  }

  int& valueAt(size_t sequenceIndex, size_t elementIndex)
  {
    return getSequence_(sequenceIndex)[elementIndex];
  }
  const int& valueAt(size_t sequenceIndex, size_t elementIndex) const
  {
    return getSequence(sequenceIndex)[elementIndex];
  }
  int& operator()(size_t sequenceIndex, size_t elementIndex)
  {
    return getSequence_(sequenceIndex)[elementIndex];
  }
  const int& operator()(size_t sequenceIndex, size_t elementIndex) const
  {
    return getSequence(sequenceIndex)[elementIndex];
  }
  /** @} */

  /**
   * @name The OrderedSequenceContainer interface implementation:
   *
   * @{
   */
  const Sequence& getSequence(size_t sequenceIndex) const;
  size_t getSequencePosition(const std::string& name) const;
  void            setSequence(size_t sequenceIndex, const Sequence& sequence, bool checkName = true);
  std::shared_ptr<Sequence> removeSequence(size_t sequenceIndex);
  void         deleteSequence(size_t sequenceIndex);
  void setComments(size_t sequenceIndex, const Comments& comments);
  std::vector<std::string> getSequencesNames() const;
  void setSequencesNames(const std::vector<std::string>& names, bool checkNames);
  /** @} */

  /**
   * @name AbstractSequenceContainer methods.
   *
   * @{
   */
  Sequence& getSequence_(size_t i);
  Sequence& getSequence_(const std::string& name);
  /** @} */

  /**
   * @name SequencedValuesContainer methods.
   *
   * @{
   */
  double getStateValueAt(size_t siteIndex, const std::string& sequenceName, int state) const
  {
    return getSequence(sequenceName).getStateValueAt(siteIndex, state);
  }

  double operator()(size_t siteIndex, const std::string& sequenceName, int state) const
  {
    return getSequence(sequenceName)(siteIndex, state);
  }

  /*
   *
   * @}
   *
   */

  /**
   * @name OrderedValuesContainer methods.
   *
   * @{
   */
  double getStateValueAt(size_t siteIndex, size_t sequenceIndex, int state) const
  {
    if (sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSequenceContainer::getStateValueAt.", sequenceIndex, 0, getNumberOfSequences() - 1);
    const Sequence& seq = getSequence(sequenceIndex);

    if (siteIndex >= seq.size())
      throw IndexOutOfBoundsException("VectorSequenceContainer::getStateValueAt.", siteIndex, 0, seq.size() - 1);

    return getAlphabet()->isResolvedIn(seq[siteIndex], state) ? 1. : 0.;
  }

  double operator()(size_t siteIndex, size_t sequenceIndex, int state) const
  {
    return getAlphabet()->isResolvedIn(getSequence(sequenceIndex)[siteIndex], state) ? 1. : 0.;
  }

  /*
   *
   * @}
   *
   */
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_MAPSEQUENCECONTAINER_H
