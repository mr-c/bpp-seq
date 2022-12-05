//
// File: test_probseq.cpp
// Created by: Murray Patterson
// Created on: Fri Oct 9 2015
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

This software is a computer program whose purpose is to provide classes
for numerical calculus. This file is part of the Bio++ project.

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

// from the STL
#include <iostream>
#include <string>
#include <vector>

// alphabets

#include <Bpp/Seq/Alphabet/AllelicAlphabet.h>
#include <Bpp/Seq/Alphabet/AlphabetTools.h>

#include <Bpp/Numeric/Table.h>

// sequences
#include <Bpp/Seq/Sequence.h>
#include <Bpp/Seq/ProbabilisticSequence.h>

// sites
#include <Bpp/Seq/Site.h>
#include <Bpp/Seq/ProbabilisticSite.h>

// containers
#include <Bpp/Seq/Container/VectorSiteContainer.h>
#include "../src/Bpp/Seq/Container/VectorProbabilisticSequenceContainer.h"
#include <Bpp/Seq/Container/VectorProbabilisticSiteContainer.h>

// file formats
#include <Bpp/Seq/Io/Fasta.h>
#include <Bpp/Seq/Io/Pasta.h>

using namespace std;
using namespace bpp;

typedef Table<double> DataTable;

// convert a vector of string to a string
const string str(const vector<string> & v) {

  string s = "[";
  for(vector<string>::const_iterator i = v.begin(); i != v.end(); ++i)
    s += " " + *i;
  s += " ]";

  return s;
}

/*
 * MAIN
 */
int main() {


  string nameSeq = "counts.pa";
  Pasta pasta;
  auto alphaall = std::make_shared<AllelicAlphabet>(AlphabetTools::DNA_ALPHABET, 4);
  
  auto sites = std::shared_ptr<VectorProbabilisticSiteContainer>(pasta.readAlignment(nameSeq , &alphaall->getStateAlphabet()));

  VectorProbabilisticSiteContainer sites2(alphaall.get());
  for (size_t ns=0;ns < sites->getNumberOfSequences(); ns++)
  {
    auto seq=alphaall->convertFromStateAlphabet(sites->getSequence(ns));
    sites2.addSequence(*seq);
  }


  pasta.writeSequence(cerr, sites2.getSequence("D"));

  

  return 0;
}