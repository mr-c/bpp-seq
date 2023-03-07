//
// File: CoreSymbolList.h
// Authors:
//   Laurent Guéguen
// Created: dimanche 2 avril 2017, Ã  22h 23
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

#ifndef BPP_SEQ_CORESYMBOLLIST_H
#define BPP_SEQ_CORESYMBOLLIST_H

#include <Bpp/Clonable.h>

#include "Alphabet/Alphabet.h"

// From the STL:
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

namespace bpp
{
/**
 * @brief The CruxSymbolList interface.
 *
 * This contains all methods that do not depend on the content type of the SymbolList
 */

class CruxSymbolListInterface :
  public virtual Clonable
{
public:
  /**
   * @name The Clonable interface
   *
   * @{
   */
  CruxSymbolListInterface* clone() const override = 0;
  /** @} */

  // Class destructor
  virtual ~CruxSymbolListInterface() {}

public:
  /**
   * @brief Get the alphabet associated to the list.
   *
   * @return A const pointer to the alphabet.
   * @see Alphabet class.
   */
  virtual std::shared_ptr<const Alphabet> getAlphabet() const = 0;

  /**
   * @brief Get the alphabet associated to the list.
   *
   * @return A const reference to the alphabet.
   * @see Alphabet class.
   */
  virtual const Alphabet& alphabet() const = 0;

  /**
   * @brief Get the number of elements in the list.
   *
   * @return The number of sites in the list.
   */
  virtual size_t size() const = 0;

  /**
   * @brief Convert the list as a string.
   *
   * This method is useful for dumping a list to a file or to the screen for display.
   *
   * @return The whole list as a string.
   */

  virtual std::string toString() const = 0;

  /**
   * @name Edition methods.
   *
   * @{
   */

  /**
   * @brief Remove the element at position 'pos'.
   *
   * @param pos The position of the element to remove.
   */
  virtual void deleteElement(size_t pos) = 0;

  /**
   * @brief Remove the elements at position 'pos'.
   *
   * @param pos The position of the first element to remove.
   * @param len The length of the region to remove.
   */
  virtual void deleteElements(size_t pos, size_t len) = 0;

  /** @} */

  /**
   * @name Provide direct access to the list content.
   *
   * @warning These operators allow you to modifiy the list content.
   * No alphabet checking is performed for your modifications, so use with care, or
   * consider using the setContent() method.
   *
   * @{
   */

  /**
   * @brief Randomly shuffle the content of the list, with linear complexity.
   */
  virtual void shuffle() = 0;
  /** @} */

  /**
   * @brief get value of a state at a position
   *
   * @param position  position in the list
   * @param state     state in the alphabet
   * @return          The state value at the given position.
   */
  virtual double getStateValueAt(size_t position, int state) const = 0;

  /**
   * @brief get value of a state at a position
   *
   * Short-cut for getStateValueAt.
   *
   * @param position  position in the list
   * @param state     state in the alphabet
   * @return          The state value at the given position.
   */
 virtual double operator()(size_t position, int state) const = 0;
};


/**
 * @brief The CoreSymbolList interface.
 *
 */
template<class T>
class TemplateCoreSymbolListInterface :
  public virtual CruxSymbolListInterface
{
public:
  /**
   * @name The Clonable interface
   *
   * @{
   */
  TemplateCoreSymbolListInterface* clone() const override = 0;
  /** @} */

  // Class destructor
  virtual ~TemplateCoreSymbolListInterface() {}

public:
  /**
   * @name Acting on the content of the list.
   *
   * @{
   */

  /**
   * @brief Set the whole content of the list.
   *
   * @param list The new content of the list.
   * @see The list constructor for information about the way lists are internaly stored.
   */
  virtual void setContent(const std::vector<T>& list) = 0;

  virtual const std::vector<T>& getContent() const = 0;

  /** @} */

  /**
   * @name Edition methods.
   *
   * @{
   */

  /**
   * @brief Add a character to the end of the list.
   *
   * @param c The character to add.
   */
  virtual void addElement(const T& c) = 0;

  /**
   * @brief Add a character at a certain position in the list.
   *
   * @param pos The postion where to insert the element.
   * @param c   The character to add.
   */
  virtual void addElement(size_t pos, const T& c) = 0;

  /**
   * @brief Set the element at position 'pos' to character 'c'.
   *
   * @param pos The position of the character to set.
   * @param c   The value of the element.
   */
  virtual void setElement(size_t pos, const T& c) = 0;

  /**
   * @brief Get the element at position 'pos' as a character.
   *
   * @param pos The position of the character to retrieve.
   */
  virtual const T& getElement(size_t pos) const = 0;

  /** @} */

  /**
   * @name Provide direct access to the list content.
   *
   * @warning These operators allow you to modifiy the list content.
   * No alphabet checking is performed for your modifications, so use with care, or
   * consider using the setContent() method.
   *
   * @{
   */

  /**
   * @brief checked access to a character in list.
   *
   * @param pos The position to retrieve.
   * @return The T value of character at position pos.
   */
  virtual const T& getValue(size_t pos) const = 0;

  /**
   * @brief Operator [] overloaded for quick access to a character in list.
   *
   * @param pos The position to retrieve.
   * @return The T value of character at position pos.
   */
  virtual const T& operator[](size_t pos) const = 0;

  /**
   * @brief Operator [] overloaded for quick access to a character in list.
   *
   * @param pos The position to retrieve.
   * @return The T value of character at position pos.
   */
  virtual T& operator[](size_t pos) = 0;

  /** @} */
};

using IntCoreSymbolListInterface = TemplateCoreSymbolListInterface<int>;
using ProbabilisticCoreSymbolListInterface = TemplateCoreSymbolListInterface< std::vector<double> >;


template<class T>
class CoreSymbolListEditionEvent
{
private:
  TemplateCoreSymbolListInterface<T>* list_;

public:
  CoreSymbolListEditionEvent(TemplateCoreSymbolListInterface<T>* list) :
    list_(list) {}

  CoreSymbolListEditionEvent(const CoreSymbolListEditionEvent<T>& slee) : list_(slee.list_) {}

  CoreSymbolListEditionEvent<T>& operator=(const CoreSymbolListEditionEvent<T>& slee)
  {
    list_ = slee.list_;
    return *this;
  }

  virtual ~CoreSymbolListEditionEvent() {}

public:
  virtual TemplateCoreSymbolListInterface<T>* getCoreSymbolList() { return list_; }
  virtual const TemplateCoreSymbolListInterface<T>* getCoreSymbolList() const { return list_; }
};


template<class T>
class CoreSymbolListInsertionEvent :
  public CoreSymbolListEditionEvent<T>
{
private:
  size_t pos_;
  size_t len_;

public:
  CoreSymbolListInsertionEvent(TemplateCoreSymbolListInterface<T>* list, size_t pos, size_t len) :
    CoreSymbolListEditionEvent<T>(list), pos_(pos), len_(len) {}

public:
  virtual size_t getPosition() const { return pos_; }
  virtual size_t getLength() const { return len_; }
};


template<class T>
class CoreSymbolListDeletionEvent :
  public CoreSymbolListEditionEvent<T>
{
private:
  size_t pos_;
  size_t len_;

public:
  CoreSymbolListDeletionEvent(TemplateCoreSymbolListInterface<T>* list, size_t pos, size_t len) :
    CoreSymbolListEditionEvent<T>(list), pos_(pos), len_(len) {}

public:
  virtual size_t getPosition() const { return pos_; }
  virtual size_t getLength() const { return len_; }
};


template<class T>
class CoreSymbolListSubstitutionEvent :
  public CoreSymbolListEditionEvent<T>
{
private:
  size_t begin_;
  size_t end_;

public:
  CoreSymbolListSubstitutionEvent(TemplateCoreSymbolListInterface<T>* list, size_t begin, size_t end) :
    CoreSymbolListEditionEvent<T>(list), begin_(begin), end_(end) {}

public:
  virtual size_t getBegin() const { return begin_; }
  virtual size_t getEnd() const { return end_; }
};

template<class T>
class CoreSymbolListListener :
  public virtual Clonable
{
public:
  virtual ~CoreSymbolListListener() {}

  virtual CoreSymbolListListener* clone() const = 0;

public:
  virtual bool isRemovable() const = 0;
  virtual bool isShared() const = 0;
  virtual void beforeSequenceChanged(const CoreSymbolListEditionEvent<T>& event) = 0;
  virtual void afterSequenceChanged(const CoreSymbolListEditionEvent<T>& event) = 0;
  virtual void beforeSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) = 0;
  virtual void afterSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) = 0;
  virtual void beforeSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) = 0;
  virtual void afterSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) = 0;
  virtual void beforeSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) = 0;
  virtual void afterSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) = 0;
};


/**
 * @brief Interface for event-driven CoreSymbolList objects.
 *
 * This is a general purpose container, containing an ordered list of states(= letters).
 * The states that allowed to be present in the list are defined by an alphabet object,
 * which is passed to the list constructor by a pointer.
 *
 * For programming convenience, the states are stored as Ts, but the translation toward
 * and from a char description is easily performed with the Alphabet classes.
 *
 * @see Alphabet
 */
template<class T>
class TemplateEventDrivenCoreSymbolListInterface :
  public virtual TemplateCoreSymbolListInterface<T>
{
public:
  /**
   * @name The Clonable interface
   *
   * @{
   */
  virtual TemplateEventDrivenCoreSymbolListInterface* clone() const = 0;
  /** @} */

  // Class destructor
  virtual ~TemplateEventDrivenCoreSymbolListInterface(){}

public:

  /**
   * @name Events handling
   *
   * @{
   */

  virtual size_t getNumberOfListeners() const = 0;

  virtual const CoreSymbolListListener<T>& getListener(size_t i) const = 0;

  virtual CoreSymbolListListener<T>& getListener(size_t i) = 0;

  virtual void addCoreSymbolListListener(CoreSymbolListListener<T>* listener)  = 0;

  virtual void removeCoreSymbolListListener(CoreSymbolListListener<T>* listener) = 0;

protected:
  virtual void beforeSequenceChanged(const CoreSymbolListEditionEvent<T>& event) = 0;

  virtual void afterSequenceChanged(const CoreSymbolListEditionEvent<T>& event) = 0;

  virtual void beforeSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) = 0;
  virtual void afterSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) = 0;

  virtual void beforeSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) = 0;
  virtual void afterSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) = 0;

  virtual void beforeSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) = 0;
  virtual void afterSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) = 0;

  virtual void fireBeforeSequenceChanged(const CoreSymbolListEditionEvent<T>& event) = 0;

  virtual void fireAfterSequenceChanged(const CoreSymbolListEditionEvent<T>& event) = 0;

  virtual void fireBeforeSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) = 0;

  virtual void fireAfterSequenceInserted(const CoreSymbolListInsertionEvent<T>& event) = 0;

  virtual void fireBeforeSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) = 0;

  virtual void fireAfterSequenceDeleted(const CoreSymbolListDeletionEvent<T>& event) = 0;

  virtual void fireBeforeSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) = 0;

  virtual void fireAfterSequenceSubstituted(const CoreSymbolListSubstitutionEvent<T>& event) = 0;

/** @} */

protected:
  virtual void propagateEvents(bool yn) = 0;
  virtual bool propagateEvents() const = 0;
};

using IntEventDrivenCoreSymbolListInterface = TemplateEventDrivenCoreSymbolListInterface<int>;
using ProbabilisticEventDrivenCoreSymbolListInterface = TemplateEventDrivenCoreSymbolListInterface< std::vector<double> >;


} // end of namespace bpp.
#endif // BPP_SEQ_CORESYMBOLLIST_H
