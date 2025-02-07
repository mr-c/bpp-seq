// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_SEQUENCEWITHQUALITY_H
#define BPP_SEQ_SEQUENCEWITHQUALITY_H

#include <Bpp/Numeric/VectorExceptions.h>
#include <Bpp/Numeric/VectorTools.h>

#include "SequenceWithAnnotation.h"

// From the STL

#include <string>
#include <vector>
#include <cstddef>

namespace bpp
{
/**
 * @brief The SequenceQuality class
 *
 * This is a sequence with quality score associated to each element.
 * The score is a signed int value that can represent the phred or the
 * Solexa quality score for nucleic sequence.
 *
 * @author Sylvain Gaillard, Vincent Cahais, Julien Dutheil
 */
class SequenceQuality :
  public virtual SequenceAnnotation
{
private:
  bool removable_;
  std::vector<int> qualScores_;

public:
  static const std::string QUALITY_SCORE;
  static const int DEFAULT_QUALITY_VALUE;

public:
  /**
   * @name Constructors
   * @{
   */

  /**
   * @brief Build a new SequenceQuality object
   *
   * Build a new SequenceQuality object and set the quality scores to
   * the default value DEFAULT_QUALITY_VALUE.
   *
   * @param size The size of the sequence.
   * @param removable Tell if this listener can be removed by the user.
   */
  SequenceQuality(size_t size = 0, bool removable = true) :
    removable_(removable),
    qualScores_(size, DEFAULT_QUALITY_VALUE) {}


  /**
   * @brief Build a new SequenceQuality object
   *
   * Build a new SequenceQuality and assign quality scores from
   * a vector of int.
   *
   * @param quality The quality scores
   * @param removable Tell if this listener can be removed by the user.
   */
  SequenceQuality(const std::vector<int>& quality, bool removable = true) :
    removable_(removable),
    qualScores_(quality)
  {}

  /** @} */

  /**
   * @name Destructor
   * @{
   */
  virtual ~SequenceQuality() {}
  /** @} */

  /**
   * @name The Clonable interface
   * @{
   */
  SequenceQuality* clone() const override { return new SequenceQuality(*this); }
  /** @} */

public:
  void init(const Sequence& seq) override
  {
    qualScores_.resize(seq.size());
    std::fill(qualScores_.begin(), qualScores_.end(), DEFAULT_QUALITY_VALUE);
  }

  const std::string& getType() const override { return QUALITY_SCORE; }

  bool isValidWith(const SequenceWithAnnotation& sequence, bool throwException = true) const override
  {
    if (throwException && qualScores_.size() != sequence.size()) throw Exception("SequenceQuality. Quality scores must match the sequence size.");
    return qualScores_.size() == sequence.size();
  }

  bool isRemovable() const override { return removable_; }
  bool isShared() const override { return false; }
  void beforeSequenceChanged(const IntSymbolListEditionEvent& event) override {}
  void afterSequenceChanged(const IntSymbolListEditionEvent& event) override;
  void beforeSequenceInserted(const IntSymbolListInsertionEvent& event) override {}
  void afterSequenceInserted(const IntSymbolListInsertionEvent& event) override;
  void beforeSequenceDeleted(const IntSymbolListDeletionEvent& event) override {}
  void afterSequenceDeleted(const IntSymbolListDeletionEvent& event) override;
  void beforeSequenceSubstituted(const IntSymbolListSubstitutionEvent& event) override {}
  void afterSequenceSubstituted(const IntSymbolListSubstitutionEvent& event) override {}

  size_t getSize() const { return qualScores_.size(); }

  const int& operator[](size_t i) const { return qualScores_[i]; }
  int& operator[](size_t i) { return qualScores_[i]; }

  void setScores(const std::vector<int>& scores)
  {
    if (scores.size() != qualScores_.size())
      throw DimensionException("SequenceQuality::setScores. Trying to replace score by a vector with different length.", scores.size(), qualScores_.size());
    qualScores_ = scores;
  }

  /**
   * @return All scores as a vector.
   */
  const std::vector<int>& getScores() const { return qualScores_; }

  void setScore(size_t pos, int score)
  {
    if (pos >= qualScores_.size())
      throw Exception("SequenceQuality::setScore. Vector overflow. Scores number: " + TextTools::toString(qualScores_.size()) + ", but trying to insert score at position " + TextTools::toString(pos) + ".");
    qualScores_[pos] = score;
  }

  void setScores(size_t pos, const std::vector<int>& scores)
  {
    if (pos + scores.size() > qualScores_.size())
      throw Exception("SequenceQuality::setScores. Vector overflow. Scores number: " + TextTools::toString(qualScores_.size()) + ", but trying to insert " + TextTools::toString(scores.size()) + " scores at position " + TextTools::toString(pos) + ".");
    std::copy(scores.begin(), scores.end(), qualScores_.begin() + static_cast<ptrdiff_t>(pos));
  }

  bool merge(const SequenceAnnotation& anno) override
  {
    try
    {
      const SequenceQuality* qual = &dynamic_cast<const SequenceQuality&>(anno);
      VectorTools::append(qualScores_, qual->getScores());
      return true;
    }
    catch (std::exception& e)
    {
      return false;
    }
  }

  std::unique_ptr<SequenceAnnotation> getPartAnnotation(size_t pos, size_t len) const override
  {
    return std::unique_ptr<SequenceAnnotation>(new SequenceQuality(
          std::vector<int>(
          qualScores_.begin() + static_cast<ptrdiff_t>(pos),
          qualScores_.begin() + static_cast<ptrdiff_t>(pos + len)),
          removable_));
  }
};


/**
 * @brief A SequenceWithAnnotation class with quality scores attached.
 *
 * This classes adds some usefull functions to handle quality scores.
 *
 * @see SequenceQuality
 * @author Sylvain Gaillard, Vincent Cahais, Julien Dutheil
 */
class SequenceWithQuality :
  public SequenceWithAnnotation
{
private:
  std::shared_ptr<SequenceQuality> qualScores_;

public:
  /**
   * @name Constructors
   * @{
   */

  /**
   * @brief Build a new empty SequenceWithQuality
   *
   * @param alpha    A pointer to an Alphabet
   *
   * @throw BadCharException if a state is not alowed by the Alphabet
   */
  SequenceWithQuality(
      std::shared_ptr<const Alphabet>& alpha) :
    AbstractTemplateSymbolList<int>(alpha),
    SequenceWithAnnotation(alpha),
    qualScores_(new SequenceQuality(0, false))
  {
    addAnnotation(qualScores_);
  }

  /**
   * @brief Build a new SequenceWithQuality from a std::string
   *
   * Build a new SequenceWithQuality and set the quality scores to
   * the default value DEFAULT_QUALITY_VALUE.
   *
   * @param name     The name of the sequence
   * @param sequence The string representing the sequence
   * @param alpha    A pointer to an Alphabet
   *
   * @throw BadCharException if a state is not alowed by the Alphabet
   */
  SequenceWithQuality(
      const std::string& name,
      const std::string& sequence,
      std::shared_ptr<const Alphabet>& alpha) :
    AbstractTemplateSymbolList<int>(alpha),
    SequenceWithAnnotation(name, sequence, alpha),
    qualScores_(new SequenceQuality(sequence.size(), false))
  {
    addAnnotation(qualScores_);
  }

  /**
   * @brief Build a new SequenceWithQuality from a std::string
   *
   * Build a new SequenceWithQuality and set the quality scores to
   * the default value DEFAULT_QUALITY_VALUE.
   *
   * @param name     The name of the sequence
   * @param sequence The string representing the sequence
   * @param comments Comments to add to the sequence
   * @param alpha    A pointer to an Alphabet
   *
   * @throw BadCharException if a state is not alowed by the Alphabet
   *
   * @author Vincent Cahais
   */
  SequenceWithQuality(
      const std::string& name,
      const std::string& sequence,
      const Comments& comments,
      std::shared_ptr<const Alphabet>& alpha) :
    AbstractTemplateSymbolList<int>(alpha),
    SequenceWithAnnotation(name, sequence, comments, alpha),
    qualScores_(new SequenceQuality(sequence.size(), false))
  {
    addAnnotation(qualScores_);
  }

  /**
   * @brief Build a new SequenceWithQuality from a std::string
   *
   * Build a new SequenceWithQuality and assign quality scores from
   * a vector of int.
   *
   * @param name The name of the sequence
   * @param sequence The string representing the sequence
   * @param quality The quality scores
   * @param alpha A pointer to an alphabet
   *
   * @throw BadCharException if a state is not alowed by the Alphabet
   * @throw DimensionException if the number of quality values is not equal
   * to the number of sequence states
   */
  SequenceWithQuality(
      const std::string& name,
      const std::string& sequence,
      const std::vector<int>& quality,
      std::shared_ptr<const Alphabet>& alpha) :
    AbstractTemplateSymbolList<int>(alpha),
    SequenceWithAnnotation(name, sequence, alpha),
    qualScores_(new SequenceQuality(quality, false))
  {
    addAnnotation(qualScores_);
  }

  /**
   * @brief Build a new SequenceWithQuality from a std::string
   *
   * Build a new SequenceWithQuality and assign quality scores from
   * a vector of int.
   *
   * @param name The name of the sequence
   * @param sequence The string representing the sequence
   * @param quality The quality scores
   * @param comments Comments to add to the sequence
   * @param alpha A pointer to an alphabet
   *
   * @throw BadCharException if a state is not alowed by the Alphabet
   * @throw DimensionException if the number of quality values is not equal
   * to the number of sequence states
   *
   * @author Vincent Cahais
   */
  SequenceWithQuality(
      const std::string& name,
      const std::string& sequence,
      const std::vector<int>& quality,
      const Comments& comments,
      std::shared_ptr<const Alphabet>& alpha) :
    AbstractTemplateSymbolList<int>(alpha),
    SequenceWithAnnotation(name, sequence, comments, alpha),
    qualScores_(new SequenceQuality(quality, false))
  {
    addAnnotation(qualScores_);
  }

  /**
   * @brief Build a new SequenceWithQuality from a std::vector<int>
   *
   * Build a new SequenceWithQuality and set the quality scores to
   * the default value DEFAULT_QUALITY_VALUE.
   *
   * @param name The name of the sequence
   * @param sequence The sequence in int
   * @param alpha A pointer to an Alphabet
   *
   * @throw BadIntException if a state is not alowed by the Alphabet
   */
  SequenceWithQuality(
      const std::string& name,
      const std::vector<int>& sequence,
      std::shared_ptr<const Alphabet>& alpha) :
    AbstractTemplateSymbolList<int>(alpha),
    SequenceWithAnnotation(name, sequence, alpha),
    qualScores_(new SequenceQuality(sequence.size(), false))
  {
    addAnnotation(qualScores_);
  }

  /**
   * @brief Build a new SequenceWithQuality from a std::vector<int>
   *
   * Build a new SequenceWithQuality and set the quality scores to
   * the default value DEFAULT_QUALITY_VALUE.
   *
   * @param name The name of the sequence
   * @param sequence The sequence in int
   * @param comments Comments to add to the sequence
   * @param alpha A pointer to an Alphabet
   *
   * @throw BadIntException if a state is not alowed by the Alphabet
   *
   * @author Vincent Cahais
   */
  SequenceWithQuality(
      const std::string& name,
      const std::vector<int>& sequence,
      const Comments& comments,
      std::shared_ptr<const Alphabet>& alpha) :
    AbstractTemplateSymbolList<int>(alpha),
    SequenceWithAnnotation(name, sequence, comments, alpha),
    qualScores_(new SequenceQuality(sequence.size(), false))
  {
    addAnnotation(qualScores_);
  }

  /**
   * @brief Build a new SequenceWithQuality from a std::vector<int>
   *
   * Build a new SequenceWithQuality and assign quality scores from
   * a vector of int.
   *
   * @param name The name of the sequence
   * @param sequence The sequence in int
   * @param quality The quality scores
   * @param alpha A pointer to an Alphabet
   *
   * @throw BadIntException if a state is not alowed by the Alphabet
   * @throw DimensionException if the number of quality values is not equal
   * to the number of sequence states
   */
  SequenceWithQuality(
      const std::string& name,
      const std::vector<int>& sequence,
      const std::vector<int>& quality,
      std::shared_ptr<const Alphabet>& alpha) :
    AbstractTemplateSymbolList<int>(alpha),
    SequenceWithAnnotation(name, sequence, alpha),
    qualScores_(new SequenceQuality(quality, false))
  {
    addAnnotation(qualScores_);
  }

  /**
   * @brief Build a new SequenceWithQuality from a std::vector<int>
   *
   * Build a new SequenceWithQuality and assign quality scores from
   * a vector of int.
   *
   * @param name The name of the sequence
   * @param sequence The sequence in int
   * @param quality The quality scores
   * @param comments Comments to add to the sequence
   * @param alpha A pointer to an Alphabet
   *
   * @throw BadIntException if a state is not alowed by the Alphabet
   * @throw DimensionException if the number of quality values is not equal
   * to the number of sequence states
   *
   * @author Vincent Cahais
   */
  SequenceWithQuality(
      const std::string& name,
      const std::vector<int>& sequence,
      const std::vector<int>& quality,
      const Comments& comments,
      std::shared_ptr<const Alphabet>& alpha) :
    AbstractTemplateSymbolList<int>(alpha),
    SequenceWithAnnotation(name, sequence, comments, alpha),
    qualScores_(new SequenceQuality(quality, false))
  {
    addAnnotation(qualScores_);
  }

  /**
   * @brief Build a new SequenceWithQuality
   *
   * Build a new SequenceWithQuality from a Sequence object and set the
   * quality scores to the default value DEFAULT_QUALITY_VALUE.
   *
   * @param s The Sequence object
   */
  SequenceWithQuality(const Sequence& s) :
    AbstractTemplateSymbolList<int>(s.getAlphabet()),
    SequenceWithAnnotation(s), qualScores_(new SequenceQuality(s.size(), false))
  {
    addAnnotation(qualScores_);
  }

  /**
   * @brief Build a new SequenceWithQuality
   *
   * Build a new SequenceWithQuality from a Sequence object and set the
   * quality scores from a vector of int.
   *
   * @param s The Sequence object
   * @param sc The quality scores
   *
   * @throw DimensionException if the number of quality values is not equal
   * to the number of sequence states
   */
  SequenceWithQuality(
      const Sequence& s,
      const std::vector<int>& sc) :
    AbstractTemplateSymbolList<int>(sc, s.getAlphabet()),
    SequenceWithAnnotation(s),
    qualScores_(new SequenceQuality(sc, false))
  {
    addAnnotation(qualScores_);
  }

  /** @} */

  /**
   * @name Destructor
   * @{
   */
  virtual ~SequenceWithQuality() {}
  /** @} */

  SequenceWithQuality(const SequenceWithQuality& sequence) :
    AbstractTemplateSymbolList<int>(sequence.getAlphabet()),
    SequenceWithAnnotation(sequence), qualScores_(nullptr)
  {
    qualScores_.reset(dynamic_cast<SequenceQuality*>(&annotation(SequenceQuality::QUALITY_SCORE)));
  }

  SequenceWithQuality& operator=(const SequenceWithQuality& sequence)
  {
    SequenceWithAnnotation::operator=(sequence);
    qualScores_.reset(dynamic_cast<SequenceQuality*>(&annotation(SequenceQuality::QUALITY_SCORE)));
    return *this;
  }

  /**
   * @name The Clonable interface
   * @{
   */
  SequenceWithQuality* clone() const override { return new SequenceWithQuality(*this); }
  /** @} */

  /**
   * @name Dealing with quality
   * @{
   */

  /**
   * @brief Set the quality score
   *
   * @param pos The position where the quality must be set
   * @param quality The quality value
   *
   * @throw IndexOutOfBoundsException if pos is greater than the
   * sequence size
   */
  void setQuality(size_t pos, int quality)
  {
    qualScores_->setScore(pos, quality);
  }

  /**
   * @brief Get the quality score
   *
   * @param pos The position where the quality is read
   *
   * @return The quality score
   *
   * @throw IndexOutOfBoundsException if pos is greater than the
   * sequence size
   */
  int getQuality(size_t pos) const
  {
    if (pos >= qualScores_->getSize())
      throw IndexOutOfBoundsException("SequenceWithQuality::getQuality: pos out of bounds", pos, 0, qualScores_->getSize() - 1);
    return (*qualScores_)[pos];
  }

  /**
   * @brief Set the whole quality scores
   *
   * @param quality The vector of quality scores
   *
   * @throw DimensionException if the quality vector does not feet the
   * sequence size
   */
  void setQualities(const std::vector<int>& quality)
  {
    if (quality.size() != qualScores_->getSize())
      throw DimensionException("SequenceWithQuality::setQualities: quality must fit sequence size", quality.size(), qualScores_->getSize());
    qualScores_->setScores(quality);
  }

  /**
   * @brief Get the whole quality scores
   *
   * @return A reference to the quality vector
   */
  const std::vector<int>& getQualities() const
  {
    return qualScores_->getScores();
  }

  using SequenceWithAnnotation::append;

  /**
   * @brief Append content with quality
   *
   * @param content A vector of int to append to the sequence
   * @param qualities A vector of int to append to the qualities
   *
   * @throw BadIntException if one of the content int is not in the
   * Alphabet
   * @throw DimensionException if qualities does not have the same size as
   * content
   */
  virtual void append(
      const std::vector<int>& content,
      const std::vector<int>& qualities)
  {
    if (content.size() != qualities.size())
      throw DimensionException("SequenceWithQuality::append: qualities must fit content size", qualities.size(), content.size());
    size_t pos = qualScores_->getSize();
    append(content); // This automatically extend scores array with default values through the listener
    // Update scores:
    qualScores_->setScores(pos, qualities);
  }

  /**
   * @brief Append content with quality
   *
   * @param content A vector of string to append to the sequence
   * @param qualities A vector of int to append to the qualities
   *
   * @throw BadCharException if one of the content string is not in the
   * Alphabet
   * @throw DimensionException if qualities does not have the same size as
   * content
   */
  virtual void append(
      const std::vector<std::string>& content,
      const std::vector<int>& qualities)
  {
    if (content.size() != qualities.size())
      throw DimensionException("SequenceWithQuality::append: qualities must fit content size", qualities.size(), content.size());
    size_t pos = qualScores_->getSize();
    SequenceWithAnnotation::append(content); // This automatically extend scores array with default values through the listener
    // Update scores:
    qualScores_->setScores(pos, qualities);
  }

  /**
   * @brief Append content with quality
   *
   * @param content A string to append to the sequence
   * @param qualities A vector of int to append to the qualities
   *
   * @throw BadCharException if one of the character of the string is not in
   * the Alphabet
   * @throw DimensionException if qualities does not have the same size as
   * content
   */
  virtual void append(
      const std::string& content,
      const std::vector<int>& qualities)
  {
    if (content.size() / this->getAlphabet()->getStateCodingSize()
        != qualities.size())
      throw DimensionException("SequenceWithQuality::append: qualities must fit content size", qualities.size(), content.size() / this->getAlphabet()->getStateCodingSize());
    size_t pos = qualScores_->getSize();
    SequenceWithAnnotation::append(content); // This automatically extend scores array with default values through the listener
    // Update scores:
    qualScores_->setScores(pos, qualities);
  }

  using SequenceWithAnnotation::addElement;

  /**
   * @brief Add a character to the end of the list with quality
   *
   * @param c The element to add to the sequence
   * @param q The quality of this element
   *
   * @throw BadCharException if one of the character of the string is not in
   * the Alphabet
   */
  using IntCoreSymbolListInterface::addElement;
  void addElement(const std::string& c, int q)
  {
    SequenceWithAnnotation::addElement(c);
    qualScores_->setScore(size() - 1, q);
  }

  /**
   * @brief Add a character to a certain position in the list with quality
   *
   * @param pos The position where the element will be inserted
   * @param c The element to add to the sequence
   * @param q The quality of this element
   *
   * @throw BadCharException if one of the character of the string is not in
   * the Alphabet
   * @throw IndexOutOfBoundsException if pos is greater than the sequence
   * size
   */
  void addElement(
      size_t pos, const std::string& c, int q)
  {
    SequenceWithAnnotation::addElement(pos, c);
    qualScores_->setScore(pos, q);
  }

  using EventDrivenIntSymbolList::addElement;

  /**
   * @brief Add a character to the end of the list with quality
   *
   * @param v The element to add to the sequence
   * @param q The quality of this element
   *
   * @throw BadIntException if the value does not match the current Alphabet
   */
  void addElement(int v, int q)
  {
    EventDrivenIntSymbolList::addElement(v);
    qualScores_->setScore(size() - 1, q);
  }

  /**
   * @brief Add a character to a certain position in the list with quality
   *
   * @param pos The position where the element will be inserted
   * @param v The element to add to the sequence
   * @param q The quality of this element
   *
   * @throw BadIntException if the value does not match the current Alphabet
   * @throw IndexOutOfBoundsException if pos is greater than the sequence
   * size
   */
  void addElement(size_t pos, int v, int q)
  {
    IntSymbolList::addElement(pos, v);
    qualScores_->setScore(pos, q);
  }

  /** @} */

  // const Comments& getComments() const { return Commentable::getComments(); }

  // void setComments(const Comments& comments) { Commentable::setComments(comments); }

  // void clearComments() { Commentable::clearComments(); }
};
} // end of namespace bpp.
#endif // BPP_SEQ_SEQUENCEWITHQUALITY_H
