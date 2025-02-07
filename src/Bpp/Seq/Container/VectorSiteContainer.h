// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_CONTAINER_VECTORSITECONTAINER_H
#define BPP_SEQ_CONTAINER_VECTORSITECONTAINER_H

#include <Bpp/Numeric/VectorTools.h>

#include "SequenceContainer.h"
#include "AbstractSequenceContainer.h"
#include "SiteContainer.h"
#include "VectorPositionedContainer.h"
#include "VectorMappedContainer.h"

// From the STL library:
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <memory>

namespace bpp
{
/**
 * @brief The VectorSiteContainer class.
 *
 * Sites are stored in a std::vector of pointers.
 * Site access is hence in \f$O(1)\f$, and sequence access in \f$O(l)\f$, where
 * \f$l\f$ is the number of sites in the container.
 *
 * Sequences are built & stored on the fly, with a cache for time
 * efficiency.
 *
 * See VectorSequenceContainer for an alternative implementation.
 *
 * @see Sequence, Site, AlignedSequenceContainer
 *
 */
template<class SiteType, class SequenceType>
class TemplateVectorSiteContainer :
  public AbstractTemplateSequenceContainer<SequenceType, std::string>,
  public virtual TemplateSiteContainerInterface<SiteType, SequenceType, std::string>
{
protected:
  VectorPositionedContainer<SiteType> siteContainer_;
  VectorMappedContainer<SequenceType> sequenceContainer_;
  std::vector<std::string> sequenceNames_;
  std::vector<Comments> sequenceComments_;

public:
  /**
   * @brief Build a new container from a set of sites.
   *
   * @param vs A std::vector of sites.
   * @param alphabet The common alphabet for all sites.
   * @param checkPositions Check for the redundancy of site position tag. This may turn to be very time consuming!
   * @throw Exception If sites differ in size or in alphabet.
   */
  TemplateVectorSiteContainer(
      std::vector<std::unique_ptr<SiteType>>& vs,
      std::shared_ptr<const Alphabet> alphabet,
      bool checkPositions = true) :
    AbstractTemplateSequenceContainer<SequenceType>(alphabet),
    siteContainer_(),
    sequenceContainer_(),
    sequenceNames_(),
    sequenceComments_()
  {
    if (vs.size() == 0)
      throw Exception("VectorSiteContainer::VectorSiteContainer. Empty site set.");

    size_t nbSeq = vs[0]->size();
    sequenceComments_.resize(nbSeq);
    for (size_t i = 0; i < nbSeq; ++i)
    {
      sequenceNames_.push_back("Seq_" + TextTools::toString(i));
      sequenceContainer_.appendObject(nullptr, "Seq_" + TextTools::toString(i));
    }

    for (auto& vi : vs)
    {
      addSite(vi, checkPositions); // This may throw an exception if position argument already exists or is size is not valid.
    }
  }


  /**
   * @brief Build a new empty container with specified size.
   *
   * @param size     Number of sequences in the container.
   * @param alphabet The alphabet for this container.
   */
  TemplateVectorSiteContainer(
      size_t size,
      std::shared_ptr<const Alphabet> alphabet) :
    AbstractTemplateSequenceContainer<SequenceType>(alphabet),
    siteContainer_(),
    sequenceContainer_(),
    sequenceNames_(),
    sequenceComments_(size)
  {
    for (size_t i = 0; i < size; ++i)
    {
      sequenceNames_.push_back("Seq_" + TextTools::toString(i));
      sequenceContainer_.appendObject(nullptr, "Seq_" + TextTools::toString(i));
    }
  }


  /**
   * @brief Build a new empty container with specified sequence keys.
   *
   * @param sequenceKeys Sequence keys. This will set the number of sequences in the container.
   * @param alphabet The alphabet for this container.
   * @param useKeysAsNames If yes, the sequence keys will also be used as sequence names (default). Otherwise, sequence names will be set to Seq_1, Seq_2, etc.
   */
  TemplateVectorSiteContainer(
      const std::vector<std::string>& sequenceKeys,
      std::shared_ptr<const Alphabet> alphabet,
      bool useKeysAsNames = true) :
    AbstractTemplateSequenceContainer<SequenceType>(alphabet),
    siteContainer_(),
    sequenceContainer_(),
    sequenceNames_(),
    sequenceComments_(sequenceKeys.size())
  {
    unsigned int i = 0;
    if (useKeysAsNames)
    {
      for (auto key : sequenceKeys)
      {
        ++i;
        sequenceNames_.push_back(key);
        sequenceContainer_.appendObject(nullptr, key);
      }
    }
    else
    {
      for (auto key : sequenceKeys)
      {
        ++i;
        sequenceNames_.push_back("Seq_" + TextTools::toString(i));
        sequenceContainer_.appendObject(nullptr, key);
      }
    }
  }


  /**
   * @brief Build a new empty container.
   *
   * @param alphabet The alphabet for this container.
   */
  TemplateVectorSiteContainer(std::shared_ptr<const Alphabet> alphabet) :
    AbstractTemplateSequenceContainer<SequenceType>(alphabet),
    siteContainer_(),
    sequenceContainer_(),
    sequenceNames_(),
    sequenceComments_()
  {}


  TemplateVectorSiteContainer(const TemplateVectorSiteContainer<SiteType, SequenceType>& vsc) :
    AbstractTemplateSequenceContainer<SequenceType>(vsc),
    siteContainer_(),
    sequenceContainer_(),
    sequenceNames_(vsc.sequenceNames_),
    sequenceComments_(vsc.sequenceComments_)
  {
    for (auto sequenceKey : vsc.getSequenceKeys())
    {
      sequenceContainer_.appendObject(nullptr, sequenceKey);
    }

    for (size_t i = 0; i < vsc.getNumberOfSites(); ++i)
    {
      auto sitePtr = std::unique_ptr<SiteType>(vsc.site(i).clone());
      addSite(sitePtr, false); // We assume that positions are already correct.
    }
  }


  TemplateVectorSiteContainer(const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>&  sc) :
    AbstractTemplateSequenceContainer<SequenceType>(sc),
    siteContainer_(),
    sequenceContainer_(),
    sequenceNames_(sc.getSequenceNames()),
    sequenceComments_(sc.getSequenceComments())
  {
    for (auto& sequenceKey : sc.getSequenceKeys())
    {
      sequenceContainer_.appendObject(nullptr, sequenceKey);
    }

    for (size_t i = 0; i < sc.getNumberOfSites(); ++i)
    {
      std::unique_ptr<SiteType> sitePtr(sc.site(i).clone());
      addSite(sitePtr, false); // We assume that positions are already correct.
    }
  }

  TemplateVectorSiteContainer(const TemplateSequenceContainerInterface<SequenceType, std::string>& sc) :
    AbstractTemplateSequenceContainer<SequenceType>(sc),
    siteContainer_(),
    sequenceContainer_(),
    sequenceNames_(),
    sequenceComments_()
  {
    for (auto& sequenceKey: sc.getSequenceKeys())
    {
      auto seqPtr = std::make_unique<SequenceType>(sc.sequence(sequenceKey));
      addSequence(sequenceKey, seqPtr);
    }

    reindexSites();
  }

  TemplateVectorSiteContainer<SiteType, SequenceType>& operator=(const TemplateVectorSiteContainer<SiteType, SequenceType>& vsc)
  {
    clear();
    AbstractTemplateSequenceContainer<SequenceType>::operator=(vsc);
    sequenceNames_ = vsc.sequenceNames_;
    sequenceComments_ = vsc.sequenceComments_;

    for (size_t i = 0; i < vsc.getNumberOfSites(); ++i)
    {
      auto sitePtr = std::make_unique<SiteType>(vsc.site(i));
      addSite(sitePtr, false); // We assume that positions are already correct.
    }

    return *this;
  }


  TemplateVectorSiteContainer<SiteType, SequenceType>& operator=(const TemplateSiteContainerInterface<SiteType, SequenceType, std::string>&  sc)
  {
    clear();
    AbstractTemplateSequenceContainer<SequenceType>::operator=(sc);
    sequenceNames_ = sc.sequenceNames_;
    sequenceComments_ = sc.sequenceComments_;

    for (auto sequenceKey : sc.getSequenceKeys())
    {
      sequenceContainer_.appendObject(nullptr, sequenceKey);
    }

    for (size_t i = 0; i < sc.getNumberOfSites(); ++i)
    {
      auto sitePtr = std::make_unique<SiteType>(sc.site(i));
      addSite(sitePtr, false); // We assume that positions are already correct.
    }

    return *this;
  }


  TemplateVectorSiteContainer<SiteType, SequenceType>& operator=(const TemplateSequenceContainerInterface<SequenceType, std::string>&  sc)
  {
    clear();
    AbstractTemplateSequenceContainer<SequenceType>::operator=(sc);

    for (auto sequenceKey: sc.getSequenceKeys())
    {
      addSequence(sequenceKey, sc.getSequence(sequenceKey));
    }

    reindexSites();

    return *this;
  }


  virtual ~TemplateVectorSiteContainer()
  {
    clear();
  }

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  TemplateVectorSiteContainer<SiteType, SequenceType>* clone() const override
  {
    return new TemplateVectorSiteContainer<SiteType, SequenceType>(*this);
  }
  /** @} */

  /**
   * @name The SiteContainer interface implementation:
   *
   * @{
   */
  const SiteType& site(size_t sitePosition) const override
  {
    return *siteContainer_.getObject(sitePosition);
  }

  void setSite(size_t sitePosition, std::unique_ptr<SiteType>& site, bool checkCoordinate = true) override
  {
    if (sitePosition >= getNumberOfSites())
      throw IndexOutOfBoundsException("TemplateVectorSiteContainer::setSite.", sitePosition, 0, getNumberOfSites() - 1);

    // Check size:
    if (site->size() != getNumberOfSequences())
    {
      throw SiteException("TemplateVectorSiteContainer::setSite. Site does not have the appropriate length", site.get());
    }

    // New site's alphabet and site container's alphabet matching verification
    if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("TemplateVectorSiteContainer::setSite", getAlphabet(), site->getAlphabet());

    // Check coordinate:
    if (checkCoordinate)
    {
      int coordinate = site->getCoordinate();
      // For all coordinates in vector: throw exception if position already exists
      for (size_t i = 0; i < getNumberOfSites(); ++i)
      {
        if (i != sitePosition && this->site(i).getCoordinate() == coordinate)
        {
          throw SiteException("TemplateVectorSiteContainer::setSite: Site position already exists in container", site.get());
        }
      }
    }

    std::shared_ptr<SiteType> sitePtr(site.release(), SwitchDeleter<SiteType>());
    siteContainer_.addObject(sitePtr, sitePosition, false);

    // Clean Sequence Container cache
    sequenceContainer_.nullify();
  }

  std::unique_ptr<SiteType> removeSite(size_t sitePosition) override
  {
    // Clean Sequence Container cache
    sequenceContainer_.nullify();

    auto sitePtr = siteContainer_.removeObject(sitePosition);
    std::get_deleter< SwitchDeleter<SiteType>>(sitePtr)->off();
    return std::unique_ptr<SiteType>(sitePtr.get());
  }

  void deleteSite(size_t sitePosition) override
  {
    siteContainer_.deleteObject(sitePosition);
    // Clean Sequence Container cache
    sequenceContainer_.nullify();
  }


  void addSite(std::unique_ptr<SiteType>& site, bool checkCoordinate = true) override
  {
    // Check size:
    if (getNumberOfSequences() != 0 && (site->size() != getNumberOfSequences()))
      throw SiteException("TemplateVectorSiteContainer::addSite. Site does not have the appropriate length", site.get());

    // New site's alphabet and site container's alphabet matching verification
    if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("TemplateVectorSiteContainer::addSite", getAlphabet(), site->getAlphabet());

    // Check coordinate:
    if (checkCoordinate)
    {
      int coordinate = site->getCoordinate();
      // For all positions in vector : throw exception if position already exists
      for (size_t i = 0; i < getNumberOfSites(); ++i)
      {
        if (this->site(i).getCoordinate() == coordinate)
          throw SiteException("TemplateVectorSiteContainer::addSite(site, bool): Site position already exists in container", site.get());
      }
    }

    std::shared_ptr<SiteType> sitePtr(site.release(), SwitchDeleter<SiteType>());
    siteContainer_.appendObject(sitePtr);

    // Clean Sequence Container cache
    if (getNumberOfSequences() == 0)
    {
      sequenceNames_.resize(sitePtr->size());
      sequenceComments_.resize(sitePtr->size());
      for (size_t i = 0; i < sitePtr->size(); ++i)
      {
        sequenceNames_[i] = "Seq_" + TextTools::toString(i);
        sequenceContainer_.appendObject(nullptr, sequenceNames_[i]);
      }
    }
    else
    {
      sequenceContainer_.nullify();
    }
  }


  void addSite(std::unique_ptr<SiteType>& site, size_t sitePosition, bool checkCoordinate = true) override
  {
    if (sitePosition >= getNumberOfSites())
      throw IndexOutOfBoundsException("TemplateVectorSiteContainer::addSite", sitePosition, 0, getNumberOfSites() - 1);

    // Check size:
    if (site->size() != getNumberOfSequences())
      throw SiteException("TemplateVectorSiteContainer::addSite. Site does not have the appropriate length", site.get());

    // New site's alphabet and site container's alphabet matching verification
    if (site->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("TemplateVectorSiteContainer::addSite", getAlphabet(), site->getAlphabet());

    // Check coordinate:
    if (checkCoordinate)
    {
      int coordinate = site->getCoordinate();
      // For all positions in vector : throw exception if position already exists
      for (size_t i = 0; i < getNumberOfSites(); i++)
      {
        if (i != sitePosition && this->site(i).getCoordinate() == coordinate)
          throw SiteException("TemplateVectorSiteContainer::addSite. Site coordinate already exists in container", site.get());
      }
    }

    std::shared_ptr<SiteType> sitePtr(site.release(), SwitchDeleter<SiteType>());
    siteContainer_.insertObject(sitePtr, sitePosition);

    // Clean Sequence Container cache
    if (getNumberOfSequences() == 0)
    {
      sequenceNames_.resize(sitePtr->size());
      sequenceComments_.resize(sitePtr->size());
      for (size_t i = 0; i < sitePtr->size(); ++i)
      {
        sequenceNames_[i] = "Seq_" + TextTools::toString(i);
        sequenceContainer_.appendObject(nullptr, sequenceNames_[i]);
      }
    }
    else
    {
      sequenceContainer_.nullify();
    }
  }


  void deleteSites(size_t sitePosition, size_t length) override
  {
    siteContainer_.deleteObjects(sitePosition, length);
  }

  size_t getNumberOfSites() const override
  {
    return siteContainer_.getSize();
  }

  void reindexSites() override
  {
    for (size_t i = 0; i < getNumberOfSites(); ++i)
    {
      site_(i).setCoordinate(static_cast<int>(i) + 1);
    }
  }

  Vint getSiteCoordinates() const override
  {
    Vint coordinates(getNumberOfSites());
    for (size_t i = 0; i < getNumberOfSites(); ++i)
    {
      coordinates[i] = site(i).getCoordinate();
    }
    return coordinates;
  }

  void setSiteCoordinates(const Vint& vCoordinates) override
  {
    if (vCoordinates.size() != getNumberOfSites())
      throw BadSizeException("TemplateVectorSiteContainer::setSiteCoordinates bad size of coordinates vector", vCoordinates.size(), getNumberOfSites());

    for (size_t i = 0; i < getNumberOfSites(); ++i)
    {
      site_(i).setCoordinate(vCoordinates[i]);
    }
  }

  /** @} */


  /**
   * @name The SequenceContainer interface.
   *
   * @{
   */
  bool hasSequence(const std::string& sequenceKey) const override
  {
    // Look for sequence key:
    return sequenceContainer_.hasObject(sequenceKey);
  }

  size_t getSequencePosition(const std::string& sequenceKey) const override
  {
    // Look for sequence key:
    return sequenceContainer_.getObjectPosition(sequenceKey);
  }

  const SequenceType& sequence(const std::string& sequenceKey) const override
  {
    size_t pos = getSequencePosition(sequenceKey);
    return sequence(pos);
  }

  const SequenceType& sequence(size_t sequencePosition) const override
  {
    if (sequencePosition >= getNumberOfSequences())
      throw IndexOutOfBoundsException("TemplateVectorSiteContainer::getSequence.", sequencePosition, 0, getNumberOfSequences() - 1);

    // If Sequence already exsits
    auto name = sequenceContainer_.getObjectName(sequencePosition);
    if (!sequenceContainer_.isAvailableName(name))
    {
      return *sequenceContainer_.getObject(sequencePosition);
    }

    // Main loop : for all sites
    size_t n = getNumberOfSites();
    std::vector<typename SequenceType::SymbolType> sequence(n);
    for (size_t j = 0; j < n; ++j)
    {
      sequence[j] = site(j)[sequencePosition];
    }

    auto alphaPtr = getAlphabet();
    auto ns = std::shared_ptr<SequenceType>(
          new SequenceType(
          sequenceNames_[sequencePosition],
          sequence,
          sequenceComments_[sequencePosition],
          alphaPtr),
          SwitchDeleter<SequenceType>());

    sequenceContainer_.addObject_(ns, sequencePosition, sequenceKey(sequencePosition), false);

    return *ns;
  }


  std::unique_ptr<SequenceType> removeSequence(size_t sequencePosition) override
  {
    sequence(sequencePosition); // this creates the sequence if it does not exist.

    for (size_t i = 0; i < getNumberOfSites(); ++i)
    {
      site_(i).deleteElement(sequencePosition);
    }

    auto d = static_cast<std::vector<std::string>::difference_type>(sequencePosition);
    sequenceNames_.erase(std::next(sequenceNames_.begin(), d));
    sequenceComments_.erase(std::next(sequenceComments_.begin(), d));

    auto seq = sequenceContainer_.removeObject(sequencePosition);
    std::get_deleter<SwitchDeleter<SequenceType>>(seq)->off();
    std::unique_ptr<SequenceType> seq2(seq.get());
    return seq2;
  }

  std::unique_ptr<SequenceType> removeSequence(const std::string& sequenceKey) override
  {
    // Look for sequence key:
    size_t sequencePosition = getSequencePosition(sequenceKey);
    return removeSequence(sequencePosition);
  }

  void deleteSequence(size_t sequencePosition) override
  {
    for (size_t i = 0; i < getNumberOfSites(); ++i)
    {
      site_(i).deleteElement(sequencePosition);
    }

    auto posN = static_cast<std::vector<std::string>::difference_type>(sequencePosition);
    sequenceNames_.erase(sequenceNames_.begin() + posN);
    auto posC = static_cast<std::vector<Comments>::difference_type>(sequencePosition);
    sequenceComments_.erase(sequenceComments_.begin() + posC);

    sequenceContainer_.deleteObject(sequencePosition);
  }

  void deleteSequence(const std::string& sequenceKey) override
  {
    // Look for sequence key:
    size_t sequencePosition = getSequencePosition(sequenceKey);
    deleteSequence(sequencePosition);
  }

  size_t getNumberOfSequences() const override
  {
    return sequenceContainer_.getNumberOfObjects();
  }

  std::vector<std::string> getSequenceKeys() const override
  {
    return sequenceContainer_.getObjectNames();
  }

  void setSequenceKeys(const std::vector<std::string>& sequenceKeys) override
  {
    sequenceContainer_.setObjectNames(sequenceKeys);
  }

  const std::string& sequenceKey(size_t sequencePosition) const override
  {
    return sequenceContainer_.getObjectName(sequencePosition);
  }

  std::vector<std::string> getSequenceNames() const override
  {
    return sequenceNames_;
  }

  void setSequenceNames(const std::vector<std::string>& names, bool updateKeys) override
  {
    if (names.size() != getNumberOfSequences())
      throw DimensionException("TemplateVectorSiteContainer::setSequenceNames : bad number of names", names.size(), getNumberOfSequences());
    sequenceContainer_.nullify();
    sequenceNames_ = names;
    if (updateKeys)
    {
      setSequenceKeys(names);
    }
  }

  std::vector<Comments> getSequenceComments() const override
  {
    return sequenceComments_;
  }

  void clear() override
  {
    siteContainer_.clear();
    sequenceContainer_.clear();
    sequenceNames_.clear();
    sequenceComments_.clear();
  }

  TemplateVectorSiteContainer<SiteType, SequenceType>* createEmptyContainer() const override
  {
    auto alphaP = getAlphabet();
    auto vsc = new TemplateVectorSiteContainer<SiteType, SequenceType>(alphaP);
    vsc->setComments(getComments());
    return vsc;
  }

  const typename SequenceType::ElementType& valueAt(const std::string& sequenceKey, size_t sitePosition) const override
  {
    return site(sitePosition)[getSequencePosition(sequenceKey)];
  }

  typename SequenceType::ElementType& valueAt(const std::string& sequenceKey, size_t sitePosition) override
  {
    size_t sequencePosition = getSequencePosition(sequenceKey);
    // Reset sequence buffer for this position:
    sequenceContainer_.addObject(nullptr, sequencePosition, sequenceContainer_.getObjectName(sequencePosition));
    return this->site_(sitePosition)[sequencePosition];
  }

  const typename SequenceType::ElementType& valueAt(size_t sequencePosition, size_t sitePosition) const override
  {
    return site(sitePosition)[sequencePosition];
  }

  typename SequenceType::ElementType& valueAt(size_t sequencePosition, size_t sitePosition) override
  {
    // Reset sequence buffer for this position:
    sequenceContainer_.addObject(nullptr, sequencePosition, sequenceContainer_.getObjectName(sequencePosition));
    return site_(sitePosition)[sequencePosition];
  }

  double getStateValueAt(size_t sitePosition, const std::string& sequenceKey, int state) const override
  {
    return site(sitePosition).getStateValueAt(getSequencePosition(sequenceKey), state);
  }

  double operator()(size_t sitePosition, const std::string& sequenceKey, int state) const override
  {
    return site(sitePosition).getStateValueAt(getSequencePosition(sequenceKey), state);
  }

  double getStateValueAt(size_t sitePosition, size_t sequencePosition, int state) const override
  {
    return site(sitePosition).getStateValueAt(sequencePosition, state);
  }

  double operator()(size_t sitePosition, size_t sequencePosition, int state) const override
  {
    return site(sitePosition).getStateValueAt(sequencePosition, state);
  }


  void setSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequence) override
  {
    size_t pos = getSequencePosition(sequenceKey);
    setSequence(pos, sequence);
  }


  void setSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequence) override
  {
    setSequence(sequencePosition, sequence, sequenceContainer_.getObjectName(sequencePosition));
  }


  void setSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequence, const std::string& sequenceKey) override
  {
    if (sequencePosition >= getNumberOfSequences())
      throw IndexOutOfBoundsException("VectorSiteContainer::setSequence.", sequencePosition, 0, getNumberOfSequences() - 1);

    if (sequence->size() != getNumberOfSites())
      throw SequenceNotAlignedException("VectorSiteContainer::setSequence", sequence.get());

    // New sequence's alphabet and site container's alphabet matching verification
    if (sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSiteContainer::setSequence", getAlphabet(), sequence->getAlphabet());

    // Update elements at each site:
    for (size_t i = 0; i < getNumberOfSites(); i++)
    {
      site_(i).addElement(sequencePosition, sequence->getValue(i));
    }

    sequenceContainer_.addObject(std::move(sequence), sequencePosition, sequenceKey);
  }


  void addSequence(const std::string& sequenceKey, std::unique_ptr<SequenceType>& sequence) override
  {
    // If the container has no sequence, we set the size to the size of this sequence:
    if (getNumberOfSequences() == 0)
      realloc_(sequence->size());

    // New sequence's alphabet and site container's alphabet matching verification
    if (sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSiteContainer::addSequence", getAlphabet(), sequence->getAlphabet());

    if (sequence->size() != getNumberOfSites())
      throw SequenceException("VectorSiteContainer::addSequence. Sequence has not the appropriate length: " + TextTools::toString(sequence->size()) + ", should be " + TextTools::toString(getNumberOfSites()) + ".", sequence.get());

    if (sequenceContainer_.hasObject(sequenceKey))
      throw SequenceException("VectorSiteContainer::addSequence. Name already exists in container.", sequence.get());

    // Update elements at each site:
    for (size_t i = 0; i < getNumberOfSites(); ++i)
    {
      site_(i).addElement(sequence->getValue(i));
    }

    // Add name and comments:
    sequenceNames_.push_back(sequence->getName());
    sequenceComments_.push_back(sequence->getComments());

    // Since the sequence is built already, we save it in the cache:
    sequenceContainer_.appendObject(std::move(sequence), sequenceKey);
  }


  void insertSequence(size_t sequencePosition, std::unique_ptr<SequenceType>& sequence, const std::string& sequenceKey) override
  {
    if (sequencePosition >= getNumberOfSequences())
      throw IndexOutOfBoundsException("VectorSiteContainer::insertSequence.", sequencePosition, 0, getNumberOfSequences() - 1);

    if (sequence->size() != getNumberOfSites())
      throw SequenceNotAlignedException("VectorSiteContainer::insertSequence", sequence.get());

    // New sequence's alphabet and site container's alphabet matching verification
    if (sequence->getAlphabet()->getAlphabetType() != getAlphabet()->getAlphabetType())
      throw AlphabetMismatchException("VectorSiteContainer::insertSequence", getAlphabet(), sequence->getAlphabet());

    // Update elements at each site:
    for (size_t i = 0; i < getNumberOfSites(); ++i)
    {
      site_(i).addElement(sequencePosition, sequence->getValue(i));
    }

    // Update name and comments:
    sequenceNames_[sequencePosition] = sequence->getName();
    sequenceComments_[sequencePosition] = sequence->getComments();

    // Since the sequence is built already, we save it in the cache:
    sequenceContainer_.insertObject(std::move(sequence), sequencePosition, sequenceKey);
  }

  // Needed because of the template class
  using AbstractTemplateSequenceContainer<SequenceType>::getAlphabet;
  using AbstractTemplateSequenceContainer<SequenceType>::getComments;
  /** @} */

protected:
  /**
   * Get a non-const reference to a site in the container.
   * It is a convenient short-cut for use within the class only, as this can potentially mess up the data.
   *
   * @param sitePosition the index of the site to retrieve.
   * @return A reference to the selected site.
   */
  SiteType& site_(size_t sitePosition)
  {
    return *siteContainer_.getObject(sitePosition);
  }

  // Create n void sites:
  void realloc_(size_t n)
  {
    clear();
    std::shared_ptr<const Alphabet> alphaPtr = getAlphabet();
    SiteType s(alphaPtr);

    for (size_t i = 0; i < n; ++i)
    {
      std::unique_ptr<SiteType> sitePtr(s.clone());
      addSite(sitePtr, false);
    }

    reindexSites();
  }
};

// Aliases:
using VectorSiteContainer = TemplateVectorSiteContainer<Site, Sequence>;
using ProbabilisticVectorSiteContainer = TemplateVectorSiteContainer<ProbabilisticSite, ProbabilisticSequence>;
} // end of namespace bpp.
#endif // BPP_SEQ_CONTAINER_VECTORSITECONTAINER_H
