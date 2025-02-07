// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_SEQ_ALPHABETINDEX_GRANTHAMAACHEMICALDISTANCE_H
#define BPP_SEQ_ALPHABETINDEX_GRANTHAMAACHEMICALDISTANCE_H


// from the STL:
#include <string>

#include "AlphabetIndex2.h"
#include "../Alphabet/AlphabetExceptions.h"
#include <Bpp/Exceptions.h>
#include <Bpp/Numeric/Matrix/Matrix.h>

namespace bpp
{
/**
 * @brief Grantham (1974) Amino-Acid chemical distance.
 *
 * Two kinds of matrix can be built:
 * - a symmetric one, with \f$I_{i,j} = I_{i,j}\f$,
 * - or a non-symmetric one, with \f$I_{i,j} = -I_{i,j}\f$.
 * In the second case, which one of the two entries between \f$I_{i,j}\f$ and \f$I_{i,j}\f$ is positive is arbitrary by default.
 * It is also possible to use the coordinate on a principal component analysis between the elementary propoerties of the distance instead (setPC1Sign(true)).
 * The following R code was use in order to get those signs:
 * @code
 * library(seqinr)
 * data(aaindex)
 * data<-data.frame(composition=aaindex[["GRAR740101"]]$I,
 *                     polarity=aaindex[["GRAR740102"]]$I,
 *                       volume=aaindex[["GRAR740103"]]$I)
 * library(ade4)
 * pca<-dudi.pca(data)
 *
 * plot(pca$li[, 1:2], type="n")
 * text(pca$li[, 1:2], rownames(data))
 *
 * s.corcircle(pca$co)
 * layout(matrix(1:3,nrow=1))
 * a1<-pca$li[,1]; names(a1)<-rownames(data); dotchart(sort(a1))
 * a2<-pca$li[,2]; names(a2)<-rownames(data); dotchart(sort(a2))
 * a3<-pca$li[,3]; names(a3)<-rownames(data); dotchart(sort(a3))
 *
 * x<-pca$li[,1] #Contains the coordinates on the first axis.
 * m<-matrix(nrow=20, ncol=20)
 * for(i in 1:length(x))
 *   for(j in 1:length(x))
 *     m[i,j]<-sign(x[j] - x[i])
 *
 * @endcode
 *
 * Reference:
 * Grantham, R.
 * Amino acid difference formula to help explain protein evolution
 * Science 185, 862-864 (1974)
 *
 * Data from AAIndex2 database, Accession Number GRAR740104.
 */
class GranthamAAChemicalDistance :
  public ProteicAlphabetIndex2
{
private:
  LinearMatrix<double> distanceMatrix_;
  LinearMatrix<double> signMatrix_;
  LinearMatrix<double> indexMatrix_;
  short int sign_;

public:
  GranthamAAChemicalDistance();

  GranthamAAChemicalDistance(const GranthamAAChemicalDistance& gd) :
    ProteicAlphabetIndex2(gd),
    distanceMatrix_(gd.distanceMatrix_),
    signMatrix_(gd.signMatrix_),
    indexMatrix_(gd.indexMatrix_),
    sign_(gd.sign_)
  {}

  GranthamAAChemicalDistance& operator=(const GranthamAAChemicalDistance& gd)
  {
    ProteicAlphabetIndex2::operator=(*this);

    distanceMatrix_ = gd.distanceMatrix_;
    signMatrix_ = gd.signMatrix_;
    indexMatrix_ = gd.indexMatrix_;
    sign_ = gd.sign_;
    return *this;
  }

  GranthamAAChemicalDistance* clone() const override { return new GranthamAAChemicalDistance(); }

  virtual ~GranthamAAChemicalDistance() {}

public:
  /**
   * @name Methods from the AlphabetIndex2 interface.
   *
   * @{
   */
  double getIndex(int state1, int state2) const override;
  double getIndex(const std::string& state1, const std::string& state2) const override;
  const Matrix<double>& getIndexMatrix() const override { return indexMatrix_; }
  /** @} */

protected:
  void computeIndexMatrix_();

public:
  void setSymmetric(bool yn)
  {
    sign_ = (yn ? SIGN_NONE : SIGN_ARBITRARY);
    computeIndexMatrix_();
  }
  bool isSymmetric() const override { return sign_ == SIGN_NONE; }
  /**
   * @brief The sign of the distance is computed using the coordinate on the first axis
   * of a principal component analysis with the 3 elementary properties (Volume, Polarity, Composition).
   * Otherwise, use the default arbitrary sign. Using this option will lead isSymmetric to return false.
   *
   * @param yn Tell is the PC1-based sign should be used instead of the arbitrary one.
   */
  void setPC1Sign(bool yn)
  {
    sign_ = (yn ? SIGN_PC1 : SIGN_ARBITRARY);
    computeIndexMatrix_();
  }

  static short int SIGN_ARBITRARY;
  static short int SIGN_PC1;
  static short int SIGN_NONE;
};
} // end of namespace bpp.
#endif // BPP_SEQ_ALPHABETINDEX_GRANTHAMAACHEMICALDISTANCE_H
