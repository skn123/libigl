// This file is part of libigl, a simple c++ geometry processing library.
// 
// Copyright (C) 2014 Alec Jacobson <alecjacobson@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#include "cdt.h"
#include "../../bounding_box.h"
#include "../../PlainMatrix.h"
#include "tetrahedralize.h"

template <
  typename DerivedV, 
  typename DerivedF, 
  typename DerivedTV, 
  typename DerivedTT, 
  typename DerivedTF>
IGL_INLINE bool igl::copyleft::tetgen::cdt(
  const Eigen::MatrixBase<DerivedV>& V,
  const Eigen::MatrixBase<DerivedF>& F,
  const CDTParam & param,
  Eigen::PlainObjectBase<DerivedTV>& TV,
  Eigen::PlainObjectBase<DerivedTT>& TT,
  Eigen::PlainObjectBase<DerivedTF>& TF)
{
  // Effective input mesh
  PlainMatrix<DerivedV,Eigen::Dynamic> U;
  PlainMatrix<DerivedF,Eigen::Dynamic> G;
  if(param.use_bounding_box)
  {
    // Construct bounding box mesh
    PlainMatrix<DerivedV,Eigen::Dynamic> BV;
    PlainMatrix<DerivedF,Eigen::Dynamic> BF;
    bounding_box(V,BV,BF);
    // scale bounding box
    const Eigen::RowVector3d mid =
     (BV.colwise().minCoeff() + BV.colwise().maxCoeff()).eval()*0.5;
    BV.rowwise() -= mid;
    assert(param.bounding_box_scale >= 1.);
    BV.array() *= param.bounding_box_scale;
    BV.rowwise() += mid;
    // Append bounding box to mesh
    U.resize(V.rows()+BV.rows(),V.cols());
    U<<V,BV;
    BF.array() += V.rows();
    G.resize(F.rows()+BF.rows(),F.cols());
    G<<F,BF;
  }else
  {
    // needless copies
    U = V;
    G = F;
  }
  // effective flags;
  std::string flags = param.flags + (param.use_bounding_box ? "" : "c");
  return tetrahedralize(U,G,flags,TV,TT,TF);
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template instantiation
template bool igl::copyleft::tetgen::cdt<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, igl::copyleft::tetgen::CDTParam const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&);
#endif
