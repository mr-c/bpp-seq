// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_STRINGSEQUENCETOOLS_H
#define BPP_SEQ_STRINGSEQUENCETOOLS_H

#include <Bpp/Exceptions.h>

#include "Alphabet/Alphabet.h"
#include "Alphabet/AlphabetExceptions.h"
#include "SequenceExceptions.h"

// From the STL:
#include <string>
#include <vector>

namespace bpp
{
/**
 * @brief Utilitary methods working on raw string objects.
 *
 * Sequences may be stored as strings, but this approach is not as
 * powerful as using true sequence objects.
 * Consider using the Sequence and SequenceTools classes for more methods.
 *
 * Some of the methods implemented here are internally used by the Sequence object.
 *
 * @see Sequence, Site, SequenceTools, SymbolListTools
 */
class StringSequenceTools
{
public:
  StringSequenceTools() {}
  ~StringSequenceTools() {}

public:
  /**
   * @brief Get a subsequence.
   *
   * @param sequence The input sequence.
   * @param begin    The begining position (included).
   * @param end      The ending position (included).
   * @return A string with the subsequence.
   * @throw Exception If position does not not match the interval [0, length].
   */
  static std::string subseq(const std::string& sequence, size_t begin, size_t end);

  /**
   * @brief Set up the size of a sequence from the right side.
   *
   * All new characters are filled with gaps.
   * If the specified size is < to the sequence size, the sequence will be truncated.
   *
   * @param sequence The input sequence.
   * @param size The new size of the sequence.
   */
  static std::string setToSizeR(const std::string& sequence, size_t size);

  /**
   * @brief Set up the size of a sequence from the left side.
   *
   * All new characters are filled with gaps.
   * If the specified size is < to the sequence size, the sequence will be truncated.
   *
   * @param sequence The input sequence.
   * @param size The new size of the sequence.
   */
  static std::string setToSizeL(const std::string& sequence, size_t size);

  /**
   * @brief Delete all occurence of a character in the sequence.
   *
   * @param sequence The sequence to parse.
   * @param chars    The character to remove.
   * @return         The sequence with all specified characters removed.
   */
  static std::string deleteChar(const std::string& sequence, char chars);

  /**
   * @brief Delete all occurence of several characters in the sequence.
   *
   * @param sequence The sequence to parse.
   * @param chars    The characters to remove.
   * @return         The sequence with all specified characters removed.
   */
  static std::string deleteChar(const std::string& sequence, std::string chars);

  /**
   * @brief Reverse the sequence.
   *
   * @param sequence The sequence to reverse.
   * @return The reversed sequence.
   */
  static std::string* reverse(const std::string& sequence);

  /**
   * @brief Get the complement of a sequence.
   * @deprecated Consider working with sequence objects and translators.
   *
   * For this method, sequence is supposed to be of type DNA.
   *
   * @param sequence The sequence to complement.
   * @return The complementary sequence.
   * @see Sequence
   * @see NucleicAcidsReplication
   */
  static std::string* complement(const std::string& sequence);

  /**
   * @brief Calculate the local GC content of a sequence.
   *
   * GC contents are calculated using a window of specified size around the given position.
   * Note : Calculus for last nucleotides (sequence's size - window size) will return
   * the last possible rate calculated.
   *
   * @param sequence The sequence to analyse.
   * @param pos      The position where to compute the GC content.
   * @param window   The size of the window to use.
   * @return The GC content as a ratio (# of GC / window).
   * @throw BadIntException If the sequence is not of type DNA or RNA.
   * @throw Exception       Error in calculus (if the sequence contain gaps for instance).
   */

  static double getGCcontent(const std::string& sequence, size_t pos, size_t window);

  /**
   * @brief Convert a string sequence to a vector of int.
   *
   * This method is used in the sequence constructor.
   * This deals with the most simple cases: the sequence (string) contains
   * a succession of all characters. This is indeed the case for simple
   * alphabets, but may be more complicated if the alphabet is coded with
   * variable code length.
   *
   * @param sequence The sequence to parse.
   * @param alphabet The alphabet to use to code the sequence.
   * @return A vector of int codes.
   * @throw BarCharException If some character does not match the specified alphabet.
   */
  static std::vector<int> codeSequence(const std::string& sequence, std::shared_ptr<const Alphabet>& alphabet);

  /**
   * @brief Convert a sequence to its string representation.
   *
   * @param sequence The sequence object to convert.
   * @param alphabet The alphabet to use to decode the sequence.
   * @return A string representation of the sequence.
   * @throw BarIntException If some value does not match the specified alphabet.
   */
  static std::string decodeSequence(const std::vector<int>& sequence, std::shared_ptr<const Alphabet>& alphabet);

  /**
   * @brief Parse a sequence and try to guess the correct alphabet to use.
   *
   * @param sequence The sequence to parse.
   * @return A pointer toward a new Alphabet object.
   * @throw EmptySequenceException if the sequence is empty.
   * @throw SequenceException if their is an ambiguity between several alphabet.
   * @throw AlphabetException if the sequence does not match any alphabet.
   */
  static std::shared_ptr<const Alphabet> getAlphabetFromSequence(const std::string& sequence);
};
} // end of namespace bpp.
#endif // BPP_SEQ_STRINGSEQUENCETOOLS_H
