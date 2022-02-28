//
// File: CompressedVectorSiteContainer.h
// Authors:
//   Julien Dutheil
// Created: 2009-12-16 12:08:00
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

#ifndef BPP_SEQ_CONTAINER_COMPRESSEDVECTORSITECONTAINER_H
#define BPP_SEQ_CONTAINER_COMPRESSEDVECTORSITECONTAINER_H

#include <Bpp/Numeric/VectorTools.h>

#include "../Site.h"
#include "AbstractSequenceContainer.h"
#include "AlignedSequenceContainer.h"
#include "OrderedSequenceContainer.h"
#include "SiteContainer.h"

// From the STL library:
#include <string>
#include <vector>
#include <iostream>

namespace bpp
{
/**
 * @brief A low memory, yet restricted, version of the VectorSiteContainer class.
 *
 * This implementation is very similar to VectorSiteContainer, but identical sites
 * are stored only once, which significantly reduce memory usage in the case of
 * containers where the number of sites is large compared to the number of sequences.
 * site access is as fast as in the standard VectorSiteContainer class, but site
 * addition takes more time, as the new site must be first compared to the existing set.
 * A major restriction of this container is that you can't add or remove sequences.
 * The number of sequences is fixed after the first site has been added.
 *
 * @warning Since the data is compressed, the sites given as input are modified. The
 * major pratical consequence is that the 'position' attribute of sites will be lost.
 * Instead, the position will correspond to the position in the compressed container.
 * In addition, this container may lead to unexpected behavior if used with derived
 * classes of Site. Use with care then...
 *
 * @see Sequence, Site, VectorSiteContainer
 */
class CompressedVectorSiteContainer :
  public AbstractSequenceContainer,
// This container implements the SequenceContainer interface
// and use the AbstractSequenceContainer adapter.
public virtual SiteContainer,
// This container is a
// SiteContainer.
public virtual VectorPositionedContainer<Site>,
public virtual VectorMappedContainer<Sequence>
{
protected:
  std::vector<size_t> index_; // For all sites, give the actual position in the set.

public:
  /**
   * @brief Build a new container from a set of sites.
   *
   * @param vs A std::vector of sites.
   * @param alpha The common alphabet for all sites.
   * @throw Exception If sites differ in size or in alphabet.
   */
  CompressedVectorSiteContainer(const std::vector<const Site*>& vs, const Alphabet* alpha);
  /**
   * @brief Build a new empty container with specified size.
   *
   * @param size Number of sequences in the container.
   * @param alpha The alphabet for this container.
   */
  CompressedVectorSiteContainer(size_t size, const Alphabet* alpha);
  /**
   * @brief Build a new empty container with specified sequence names.
   *
   * @param names Sequence names. This will set the number of sequences in the container.
   * @param alpha The alphabet for this container.
   */
  CompressedVectorSiteContainer(const std::vector<std::string>& names, const Alphabet* alpha);

  /**
   * @brief Build a new empty container.
   *
   * @param alpha The alphabet for this container.
   */
  CompressedVectorSiteContainer(const Alphabet* alpha);

  CompressedVectorSiteContainer(const CompressedVectorSiteContainer& vsc);
  CompressedVectorSiteContainer(const SiteContainer& sc);

  CompressedVectorSiteContainer& operator=(const CompressedVectorSiteContainer& vsc);
  CompressedVectorSiteContainer& operator=(const SiteContainer& sc);

  virtual ~CompressedVectorSiteContainer() { clear(); }

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  CompressedVectorSiteContainer* clone() const { return new CompressedVectorSiteContainer(*this); }
  /** @} */

  /**
   * @name The SiteContainer interface implementation:
   *
   * @{
   */
  const Site& getSite(size_t siteIndex) const
  {
    return *VectorPositionedContainer<Site>::getObject(index_[siteIndex]);
  }

  Site& getSite(size_t siteIndex)
  {
    return *VectorPositionedContainer<Site>::getObject(index_[siteIndex]);
  }

  void setSite(size_t siteIndex, const Site& site, bool checkPosition = true);

  std::shared_ptr<Site> removeSite(size_t siteIndex);
  
  void deleteSite(size_t siteIndex);


  void addSite(const Site& site, bool checkPosition = false);
  void addSite(const Site& site, int position, bool checkPosition = false)
  {
    addSite(site, checkPosition);
  }
  void addSite(const Site& site, size_t siteIndex, bool checkPosition = false);
  void addSite(const Site& site, size_t siteIndex, int position, bool checkPosition = false)
  {
    addSite(site, siteIndex, checkPosition);
  }

  /*
   * @name From AlignedValuesContainer interface
   *
   * @{
   */

  void deleteSites(size_t siteIndex, size_t length);

  size_t getNumberOfSites() const { return index_.size(); }

  void reindexSites();

  Vint getSitePositions() const;

  void setSitePositions(Vint vPositions);


  /** @} */

  /**
   * @return The number of site instances
   *
   **/
  size_t getNumberOfUniqueSites() const
  {
    return VectorPositionedContainer<Site>::getSize();
  }


  // These methods are implemented for this class:

  /**
   * @name The SequenceContainer interface.
   *
   * @{
   */
  using OrderedSequenceContainer::setComments;
  void setComments(size_t sequenceIndex, const Comments& comments);

  /*
   * @brief If needed, those methods will create Sequences from the
   * Sites Container, BUT those Sequences are independent from the set
   * of Sites. Which means that if those are modified, the sites
   * are not, and information is not consistent any more.
   *
   */
  
  const Sequence& getSequence(size_t sequenceIndex) const;
  const Sequence& getSequence(const std::string& name) const;
  bool hasSequence(const std::string& name) const
  {
    // Look for sequence name:
    return VectorMappedContainer<Sequence>::hasObject(name);
  }

  /**
   *
   * @brief Method to get position of a sequence in sequence
   * container from his name. This method is used by delete and
   * remove methods
   *
   */
  size_t getSequencePosition(const std::string& name) const
  {
    try
    {
      // Look for sequence name:
      return VectorMappedContainer<Sequence>::getObjectPosition(name);
    }
    catch (Exception& e)
    {
      throw SequenceNotFoundException("VectorMappedContainer::getSequencePosition", name);
    }
  }

  std::shared_ptr<Sequence> removeSequence(size_t sequenceIndex)
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::removeSequence.");
  }

  std::shared_ptr<Sequence> removeSequence(const std::string& name)
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::removeSequence.");
  }

  size_t getNumberOfSequences() const { return VectorMappedContainer<Sequence>::getNumberOfObjects(); }

  std::vector<std::string> getSequencesNames() const
  {
    return VectorMappedContainer<Sequence>::getObjectsNames();
  }

  void setSequencesNames(const std::vector<std::string>& names, bool checkNames = true)
  {
    VectorMappedContainer<Sequence>::setObjectsNames(names);
  }

  void clear();

  CompressedVectorSiteContainer* createEmptyContainer() const;

  int& valueAt(const std::string& sequenceName, size_t elementIndex)
  {
    if (elementIndex >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::operator(std::string, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<Site>::getObject(index_[elementIndex]))[getSequencePosition(sequenceName)];
  }

  const int& valueAt(const std::string& sequenceName, size_t elementIndex) const
  {
    if (elementIndex >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::operator(std::string, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<Site>::getObject(index_[elementIndex]))[getSequencePosition(sequenceName)];
  }

  int& operator()(const std::string& sequenceName, size_t elementIndex)
  {
    return (*VectorPositionedContainer<Site>::getObject(index_[elementIndex]))[getSequencePosition(sequenceName)];
  }

  const int& operator()(const std::string& sequenceName, size_t elementIndex) const
  {
    return (*VectorPositionedContainer<Site>::getObject(index_[elementIndex]))[getSequencePosition(sequenceName)];
  }

  int& valueAt(size_t sequenceIndex, size_t elementIndex)
  {
    if (sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::operator(size_t, size_t).", sequenceIndex, 0, getNumberOfSequences() - 1);
    if (elementIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::operator(size_t, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<Site>::getObject(index_[elementIndex]))[sequenceIndex];
  }

  const int& valueAt(size_t sequenceIndex, size_t elementIndex) const
  {
    if (sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("VectorSiteContainer::operator(size_t, size_t).", sequenceIndex, 0, getNumberOfSequences() - 1);
    if (elementIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::operator(size_t, size_t).", elementIndex, 0, getNumberOfSites() - 1);
    return (*VectorPositionedContainer<Site>::getObject(index_[elementIndex]))[sequenceIndex];
  }

  int& operator()(size_t sequenceIndex, size_t elementIndex)
  {
    return (*VectorPositionedContainer<Site>::getObject(index_[elementIndex]))[sequenceIndex];
  }

  const int& operator()(size_t sequenceIndex, size_t elementIndex) const
  {
    return (*VectorPositionedContainer<Site>::getObject(index_[elementIndex]))[sequenceIndex];
  }

  /** @} */

  void addSequence(const Sequence& sequence, bool checkName = true)
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::addSequence.");
  }

  void addSequence(const Sequence& sequence, size_t sequenceIndex, bool checkName = true)
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::addSequence.");
  }

  void setSequence(const std::string& name, const Sequence& sequence, bool checkName)
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::setSequence.");
  }

  void setSequence(size_t sequenceIndex, const Sequence& sequence, bool checkName)
  {
    // Implementing this function would involve (partially) decompressing the data...
    throw NotImplementedException("CompressedVectorSiteContainer::setSequence.");
  }

  /**
   * @name SequencedValuesContainer methods.
   *
   * @{
   */
  double getStateValueAt(size_t siteIndex, const std::string& sequenceName, int state) const
  {
    if (siteIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::getStateValueAt.", siteIndex, 0, getNumberOfSites() - 1);

    return getAlphabet()->isResolvedIn(valueAt(sequenceName, siteIndex), state) ? 1. : 0.;
  }

  double operator()(size_t siteIndex, const std::string& sequenceName, int state) const
  {
    return getAlphabet()->isResolvedIn(valueAt(sequenceName, siteIndex), state) ? 1. : 0.;
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
    if (sequenceIndex >= getNumberOfSequences()) throw IndexOutOfBoundsException("CompressedVectorSequenceContainer::getStateValueAt.", sequenceIndex, 0, getNumberOfSequences() - 1);

    if (siteIndex  >= getNumberOfSites()) throw IndexOutOfBoundsException("VectorSiteContainer::getStateValueAt.", siteIndex, 0, getNumberOfSites() - 1);

    return getAlphabet()->isResolvedIn(valueAt(sequenceIndex, siteIndex), state) ? 1. : 0.;
  }

  double operator()(size_t siteIndex, size_t sequenceIndex, int state) const
  {
    return getAlphabet()->isResolvedIn(valueAt(sequenceIndex, siteIndex), state) ? 1. : 0.;
  }

  /*
   *
   * @}
   *
   */

protected:
  /**
   * @return The position of the site in the compressed set. If the site is not found,
   * this will return the number of sites in the compressed set.
   */
  size_t getSiteIndex_(const Site& site);
};
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_COMPRESSEDVECTORSITECONTAINER_H
