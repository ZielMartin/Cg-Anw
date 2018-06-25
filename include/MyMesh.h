//
// Created by ZielM on 14.06.2018.
//

#ifndef CG_ANW_MYMESH_H
#define CG_ANW_MYMESH_H

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

// Define my personal traits
struct Traits : OpenMesh::DefaultTraits
{
    // Let Point and Normal be a float vec3
    typedef OpenMesh::Vec3f Point;
    typedef OpenMesh::Vec3f Normal;

};

// Define my mesh with the new traits!
typedef OpenMesh::PolyMesh_ArrayKernelT<Traits>  HE_MESH;
#endif //CG_ANW_MYMESH_H
