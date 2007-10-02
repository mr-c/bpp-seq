//
// File: SequenceTools.h
// Created by: Guillaume Deuchst
//             Julien Dutheil
// Created on: Tue Aug 21 2003
//

/*
Copyright or � or Copr. CNRS, (November 17, 2004)

This software is a computer program whose purpose is to provide classes
for sequences analysis.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef _SEQUENCETOOLS_H_
#define _SEQUENCETOOLS_H_

#include "Alphabet.h"
#include "DNA.h"
#include "RNA.h"
#include "Sequence.h"
#include "SymbolListTools.h"
#include "NucleicAcidsReplication.h"

// From NumCalc:
#include <NumCalc/RandomTools.h>

// From utils: 
#include <Utils/Exceptions.h>

// From the STL:
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief SequenceTools static class
 *
 * Implements methods to manipulate sequences
 */

class SequenceTools: public SymbolListTools
{
	private:
		static DNA _DNA;
		static RNA _RNA;
		static NucleicAcidsReplication _DNARep;
		static NucleicAcidsReplication _RNARep;
		static NucleicAcidsReplication _transc;
	
	public:
		
    SequenceTools() {}
		virtual ~SequenceTools() {}
	
	public:

		/**
		 * @brief Get a sub-sequence.
		 * 
		 * @param sequence The sequence to trunc.
		 * @param begin The first position of the subsequence.
		 * @param end   The last position of the subsequence.
		 * @return A new sequence object with the given subsequence.
		 * @throw IndexOutOfBoundsException, Exception In case of bad indices.
		 */
		static Sequence * subseq(const Sequence & sequence, unsigned int begin, unsigned int end) throw (IndexOutOfBoundsException, Exception) ;

		/**
		 * @brief Concatenate two sequences.
		 *
		 * Sequences must have the same name and alphabets.
		 * Only first sequence's commentaries are kept.
		 *
		 * @param seq1 The first sequence.
		 * @param seq2 The second sequence.
		 * @return A new sequence object with the concatenation of the two sequences.
		 * @throw AlphabetMismatchException If the two alphabets do not match.
		 * @throw Exception If the sequence names do not match.
		 */
		static Sequence * concatenate(const Sequence & seq1, const Sequence & seq2)
		     throw (AlphabetMismatchException, Exception) ;

		/**
		 * @brief change the sense of a sequence.
		 *
		 * ABCDEF becomes FEDCBA, and the sense attribute is changed.
		 *
		 * @return A new sequence object containing the reversed sequence.
		 * @param sequence The sequence to reverse.
		 */
		static Sequence * reverse(const Sequence & sequence);

		/**
		 * @brief Get the complementary sequence of a nucleotide sequence.
		 *
		 * @see DNAReplication
		 * @return sequence A new sequence object with the complementary sequence.
		 * @param sequence The sequence to complement.
		 * @throw AlphabetException If the sequence is not a nucleotide sequence.
		 */
		static Sequence * complement(const Sequence & sequence) throw (AlphabetException);
		
		/**
		 * @brief Get the transcription sequence of a DNA sequence.
		 *
		 * @see DNAReplication
		 * @return sequence A new sequence object with the transcription sequence.
		 * @param sequence The sequence to transcript.
		 * @throw AlphabetException If the sequence is not a DNA sequence.
		 */
		static Sequence * transcript(const Sequence & sequence) throw (AlphabetException);
		
		/**
		 * @brief Get the reverse-transcription sequence of a RNA sequence.
		 *
		 * @see DNAReplication
		 * @return sequence A new sequence object with the reverse-transcription sequence.
		 * @param sequence The sequence to reverse-transcript.
		 * @throw AlphabetException If the sequence is not a RNA sequence.
		 */
		static Sequence * reverseTranscript(const Sequence & sequence) throw (AlphabetException);
		
		/**
		 * @return The identity percent of 2 sequence.
		 * One match is counted if the two sequences have identical states.
		 * @param seq1 The first sequence.
		 * @param seq2 The second sequence.
		 * @throw AlphabetMismatchException If the two sequences do not have the same alphabet.
		 * @throw SequenceNotAlignedException If the two sequences do not have the same length.
		 */
		static double getPercentIdentity(const Sequence & seq1, const Sequence & seq2) throw (AlphabetMismatchException, SequenceNotAlignedException);

		/**
		 * @return The number of sites in the sequences, <i>i.e.</i> all positions without gaps.
		 *
		 * @param seq The sequence to analyse.
		 */
		static unsigned int getNumberOfSites(const Sequence & seq);

		/**
		 * @return The number of complete sites in the sequences, <i>i.e.</i> all positions without gaps and unresolved states (generic characters).
		 *
		 * @param seq The sequence to analyse.
		 */
		static unsigned int getNumberOfCompleteSites(const Sequence & seq);

    /**
     * @brief Remove gaps from a sequence.
     *
		 * @param seq The sequence to analyse.
     * @return A new sequence object without gaps.
     */
    static Sequence * removeGaps(const Sequence & seq);
    
};

#endif // _SEQUENCETOOLS_H_

