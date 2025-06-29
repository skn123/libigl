// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2016 Alec Jacobson
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "unproject_onto_mesh.h"
#include "unproject.h"
#include "unproject_ray.h"
#include "ray_mesh_intersect.h"
#include <vector>

template < typename DerivedV, typename DerivedF, typename Derivedbc>
IGL_INLINE bool igl::unproject_onto_mesh(
  const Eigen::Vector2f& pos,
  const Eigen::Matrix4f& model,
  const Eigen::Matrix4f& proj,
  const Eigen::Vector4f& viewport,
  const Eigen::MatrixBase<DerivedV> & V,
  const Eigen::MatrixBase<DerivedF> & F,
  int & fid,
  Eigen::PlainObjectBase<Derivedbc> & bc)
{
  const auto Vf = V.template cast<float>().eval();
  const auto & shoot_ray = [&Vf,&F](
    const Eigen::Vector3f& s,
    const Eigen::Vector3f& dir,
    igl::Hit<float> & hit)->bool
  {
    std::vector<igl::Hit<float>> hits;
    if(!ray_mesh_intersect(s,dir,Vf,F,hits))
    {
      return false;
    }
    hit = hits[0];
    return true;
  };
  return unproject_onto_mesh(pos,model,proj,viewport,shoot_ray,fid,bc);
}

template <typename Derivedbc>
IGL_INLINE bool igl::unproject_onto_mesh(
  const Eigen::Vector2f& pos,
  const Eigen::Matrix4f& model,
  const Eigen::Matrix4f& proj,
  const Eigen::Vector4f& viewport,
  const std::function<
    bool(
      const Eigen::Vector3f&,
      const Eigen::Vector3f&,
      igl::Hit<float> &)
      > & shoot_ray,
  int & fid,
  Eigen::PlainObjectBase<Derivedbc> & bc)
{
  Eigen::Vector3f s,dir;
  unproject_ray(pos,model,proj,viewport,s,dir);
  Hit<float> hit;
  if(!shoot_ray(s,dir,hit))
  {
    return false;
  }
  bc.resize(3, 1);
  bc << 1.0-hit.u-hit.v, hit.u, hit.v;
  fid = hit.id;
  return true;
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template instantiation
// generated by autoexplicit.sh
template bool igl::unproject_onto_mesh<Eigen::Matrix<float, 3, 1, 0, 3, 1> >(Eigen::Matrix<float, 2, 1, 0, 2, 1> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 1, 0, 4, 1> const&, std::function<bool (Eigen::Matrix<float, 3, 1, 0, 3, 1> const&, Eigen::Matrix<float, 3, 1, 0, 3, 1> const&, igl::Hit<float>&)> const&, int&, Eigen::PlainObjectBase<Eigen::Matrix<float, 3, 1, 0, 3, 1> >&);
// generated by autoexplicit.sh
template bool igl::unproject_onto_mesh<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, 1, 3, 1, 1, 3> >(Eigen::Matrix<float, 2, 1, 0, 2, 1> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 1, 0, 4, 1> const&, Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, int&, Eigen::PlainObjectBase<Eigen::Matrix<double, 1, 3, 1, 1, 3> >&);
// generated by autoexplicit.sh
template bool igl::unproject_onto_mesh<Eigen::Matrix<float, -1, 3, 1, -1, 3>, Eigen::Matrix<int, -1, 3, 1, -1, 3>, Eigen::Matrix<float, 3, 1, 0, 3, 1> >(Eigen::Matrix<float, 2, 1, 0, 2, 1> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 1, 0, 4, 1> const&, Eigen::MatrixBase<Eigen::Matrix<float, -1, 3, 1, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 1, -1, 3> > const&, int&, Eigen::PlainObjectBase<Eigen::Matrix<float, 3, 1, 0, 3, 1> >&);
template bool igl::unproject_onto_mesh<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<float, 3, 1, 0, 3, 1> >(Eigen::Matrix<float, 2, 1, 0, 2, 1> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 1, 0, 4, 1> const&, Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, int&, Eigen::PlainObjectBase<Eigen::Matrix<float, 3, 1, 0, 3, 1> >&);
template bool igl::unproject_onto_mesh<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 1, 0, -1, 1> >(Eigen::Matrix<float, 2, 1, 0, 2, 1> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 1, 0, 4, 1> const&, Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, int&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&);
#endif
