// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2014 Daniele Panozzo <daniele.panozzo@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "frame_to_cross_field.h"
#include "local_basis.h"

IGL_INLINE void igl::frame_to_cross_field(
  const Eigen::MatrixXd& V,
  const Eigen::MatrixXi& F,
  const Eigen::MatrixXd& FF1,
  const Eigen::MatrixXd& FF2,
  Eigen::MatrixXd& X)
{
  // Generate local basis
  Eigen::MatrixXd B1, B2, B3;

  igl::local_basis(V,F,B1,B2,B3);

  // Project the frame fields in the local basis
  Eigen::MatrixXd d1, d2;
  d1.resize(F.rows(),2);
  d2.resize(F.rows(),2);

  //d1 << igl::dot_row(B1,FF1), igl::dot_row(B2,FF1);
  //d2 << igl::dot_row(B1,FF2), igl::dot_row(B2,FF2);
  // Use Eigen directly
  d1 << (B1.array()*FF1.array()).rowwise().sum(), (B2.array()*FF1.array()).rowwise().sum();
  d2 << (B1.array()*FF2.array()).rowwise().sum(), (B2.array()*FF2.array()).rowwise().sum();

  X.resize(F.rows(), 3);

	for (int i=0;i<F.rows();i++)
	{
		Eigen::Vector2d v1 = d1.row(i);
		Eigen::Vector2d v2 = d2.row(i);

    // define inverse map that maps the canonical axis to the given frame directions
		Eigen::Matrix2d A;
		A <<    v1[0], v2[0],
            v1[1], v2[1];

		// find the closest rotation
		Eigen::JacobiSVD<Eigen::Matrix<double,2,2> > svd(A, Eigen::ComputeFullU | Eigen::ComputeFullV );
    Eigen::Matrix2d C = svd.matrixU() * svd.matrixV().transpose();

    Eigen::Vector2d v = C.col(0);
    X.row(i) = v(0) * B1.row(i) + v(1) * B2.row(i);
  }
}


#ifdef IGL_STATIC_LIBRARY
// Explicit template instantiation
#endif
