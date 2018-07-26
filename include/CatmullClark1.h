//
// Created by ZielM on 13.06.2018.
//

#ifndef CG_ANW_CATMULLCLARK1_H
#define CG_ANW_CATMULLCLARK1_H

#include "MyMesh.h"
#include <OpenMesh/Core/Utils/Property.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/SubdividerT.hh>
#include <map>


/** \class CatmullClark1 CatmullClark1.h
  Based on code from Leon Kos, CAD lab, Mech.Eng., University of Ljubljana, Slovenia
  (http://www.lecad.fs.uni-lj.si/~leon)

  \note Needs a PolyMesh to work on!
*/

class CatmullClark1 {
public:
    CatmullClark1();

    HE_MESH operator()(HE_MESH &_m, size_t _n, bool _update_points = true);

    HE_MESH::Point calc_face_centroid_weighted(const HE_MESH::FaceHandle &);
    void calcLimitNormal(HE_MESH &newMesh, const OpenMesh::VertexHandle &vertex);
private:
    HE_MESH mesh, limitmesh;

    void compute_midpoint(HE_MESH &, const OpenMesh::EdgeHandle &, bool);
    void update_vertex(HE_MESH &, const OpenMesh::VertexHandle &);
    void split_edge(HE_MESH &, const OpenMesh::EdgeHandle &);
    void split_face(HE_MESH &, const OpenMesh::FaceHandle &);


    OpenMesh::VPropHandleT<HE_MESH::Point> vp_pos_; // next vertex pos
    OpenMesh::EPropHandleT<HE_MESH::Point> ep_pos_; // new edge pts
    OpenMesh::FPropHandleT<HE_MESH::Point> fp_pos_; // new face pts
    OpenMesh::EPropHandleT<double> creaseWeights_;// crease weights
    OpenMesh::VPropHandleT<int> sharpneighbours;
};

#endif //CG_ANW_CATMULLCLARK1_H
