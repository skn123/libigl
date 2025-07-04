// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2017 Alec Jacobson <alecjacobson@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#ifndef IGL_UNIQUE_ROWS_H
#define IGL_UNIQUE_ROWS_H
#include "igl_inline.h"

#include <vector>
#include <Eigen/Core>
namespace igl
{
  /// Act like matlab's [C,IA,IC] = unique(X,'rows')
  ///
  /// @tparam  DerivedA derived scalar type, e.g. Eigen::MatrixXi or Eigen::MatrixXd
  /// @tparam  DerivedIA derived integer type, e.g. Eigen::MatrixXi
  /// @tparam  DerivedIC derived integer type, e.g. Eigen::MatrixXi
  /// @param[in] A  m by n matrix whose entries are to unique'd according to rows
  /// @param[out] C  #C vector of unique rows in A
  /// @param[out] IA  #C index vector so that C = A(IA,:);
  /// @param[out] IC  #A index vector so that A = C(IC,:);
  template <typename DerivedA, typename DerivedC, typename DerivedIA, typename DerivedIC>
  IGL_INLINE void unique_rows(
    const Eigen::DenseBase<DerivedA>& A,
    Eigen::PlainObjectBase<DerivedC>& C,
    Eigen::PlainObjectBase<DerivedIA>& IA,
    Eigen::PlainObjectBase<DerivedIC>& IC);

}

#ifndef IGL_STATIC_LIBRARY
#  include "unique_rows.cpp"
#endif

#endif
