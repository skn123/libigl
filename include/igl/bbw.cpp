// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2016 Alec Jacobson <alecjacobson@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "bbw.h"
#include "min_quad_with_fixed.h"
#include "harmonic.h"
#include "parallel_for.h"
#include <Eigen/Sparse>
#include <iostream>
#include <mutex>
#include <cstdio>

igl::BBWData::BBWData():
  partition_unity(false),
  W0(),
  active_set_params(),
  verbosity(0)
{
  // We know that the Bilaplacian is positive semi-definite
  active_set_params.Auu_pd = true;
}

void igl::BBWData::print()
{
  std::cout<<"partition_unity: "<<partition_unity<<std::endl;
  std::cout<<"W0=["<<std::endl<<W0<<std::endl<<"];"<<std::endl;
}


template <
  typename DerivedV,
  typename DerivedEle,
  typename Derivedb,
  typename Derivedbc,
  typename DerivedW>
IGL_INLINE bool igl::bbw(
  const Eigen::MatrixBase<DerivedV> & V,
  const Eigen::MatrixBase<DerivedEle> & Ele,
  const Eigen::MatrixBase<Derivedb> & b,
  const Eigen::MatrixBase<Derivedbc> & bc,
  igl::BBWData & data,
  Eigen::PlainObjectBase<DerivedW> & W
  )
{
  assert(!data.partition_unity && "partition_unity not implemented yet");
  // number of domain vertices
  int n = V.rows();
  // number of handles
  int m = bc.cols();
  // Build biharmonic operator
  Eigen::SparseMatrix<typename DerivedV::Scalar> Q;
  harmonic(V,Ele,2,Q);
  W.derived().resize(n,m);
  // No linear terms
  Eigen::VectorXd c = Eigen::VectorXd::Zero(n);
  // No linear constraints
  Eigen::SparseMatrix<typename DerivedW::Scalar> A(0,n),Aeq(0,n),Aieq(0,n);
  Eigen::VectorXd Beq(0,1),Bieq(0,1);
  // Upper and lower box constraints (Constant bounds)
  Eigen::VectorXd ux = Eigen::VectorXd::Ones(n);
  Eigen::VectorXd lx = Eigen::VectorXd::Zero(n);
  active_set_params eff_params = data.active_set_params;
  if(data.verbosity >= 1)
  {
    std::cout<<"BBW: max_iter: "<<data.active_set_params.max_iter<<std::endl;
    std::cout<<"BBW: eff_max_iter: "<<eff_params.max_iter<<std::endl;
  }
  if(data.verbosity >= 1)
  {
    std::cout<<"BBW: Computing initial weights for "<<m<<" handle"<<
      (m!=1?"s":"")<<"."<<std::endl;
  }
  min_quad_with_fixed_data<typename DerivedW::Scalar > mqwf;
  min_quad_with_fixed_precompute(Q,b,Aeq,true,mqwf);
  min_quad_with_fixed_solve(mqwf,c,bc,Beq,W);
  // decrement
  eff_params.max_iter--;
  bool error = false;
  // Loop over handles
  std::mutex critical;
  const auto & optimize_weight = [&](const int i)
  {
    // Quicker exit for paralle_for
    if(error)
    {
      return;
    }
    if(data.verbosity >= 1)
    {
      std::lock_guard<std::mutex> lock(critical);
      std::cout<<"BBW: Computing weight for handle "<<i+1<<" out of "<<m<<
        "."<<std::endl;
    }
    Eigen::VectorXd bci = bc.col(i);
    Eigen::VectorXd Wi;
    // use initial guess
    Wi = W.col(i);
    SolverStatus ret = active_set(
        Q,c,b,bci,Aeq,Beq,Aieq,Bieq,lx,ux,eff_params,Wi);
    switch(ret)
    {
      case SOLVER_STATUS_CONVERGED:
        break;
      case SOLVER_STATUS_MAX_ITER:
#ifdef IGL_BBW_DEBUG
        cerr<<"active_set: max iter without convergence."<<endl;
#endif
        break;
      case SOLVER_STATUS_ERROR:
      default:
#ifdef IGL_BBW_DEBUG
        cerr<<"active_set error."<<endl;
#endif
        error = true;
    }
    W.col(i) = Wi;
  };

  parallel_for(m,optimize_weight,2);
  if(error)
  {
    return false;
  }

#ifndef NDEBUG
  const double min_rowsum = W.rowwise().sum().array().abs().minCoeff();
  if(min_rowsum < 0.1)
  {
    std::cerr<<"bbw.cpp: Warning, minimum row sum is very low. Consider more "
      "active set iterations or enforcing partition of unity."<<std::endl;
  }
#endif

  return true;
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template instantiation
template bool igl::bbw<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 1, 0, -1, 1>, Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1>>(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1>> const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1>> const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 1, 0, -1, 1>> const&, Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1>> const&, igl::BBWData&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1>>&);
#endif

