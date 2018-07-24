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


class HE_MESH : public OpenMesh::PolyMesh_ArrayKernelT<Traits> {
public:
    HE_MESH() {
        this->add_property(this->vp_fourth);
        this->add_property(this->limitpoint);
        this->add_property(this->limitnormal);
    }
    OpenMesh::VPropHandleT<float> vp_fourth;
    OpenMesh::VPropHandleT<Point> limitpoint;
    OpenMesh::VPropHandleT<Normal> limitnormal;
};

#endif //CG_ANW_MYMESH_H
