#include <iostream>
#include <cmath>
#include <math.h>
#include <QVector3D>

#include "CatmullClark1.h"
#include "MeshWrapper.h"
#include <OpenMesh/Tools/Utils/MeshCheckerT.hh>
#include <OpenMesh/Core/Utils/vector_traits.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>

float getFourthComponent(HE_MESH &mesh, HE_MESH::VertexHandle v) {
    float fourthComp = mesh.property(mesh.vp_fourth, v);
    if (fourthComp == 0.0f)
        fourthComp = 1.0f;
    return fourthComp;
}

CatmullClark1::CatmullClark1() {

}

HE_MESH CatmullClark1::operator()(HE_MESH &_m, size_t _n, const bool _update_points) {
    this->mesh = _m;
    HE_MESH newMesh = _m;

    newMesh.add_property(vp_pos_);
    newMesh.add_property(ep_pos_);
    newMesh.add_property(fp_pos_);
    newMesh.add_property(creaseWeights_);
    newMesh.add_property(sharpneighbours);

    for (HE_MESH::VertexIter v_it = _m.vertices_begin(); v_it != _m.vertices_end(); ++v_it) {
        HE_MESH::HalfedgeHandle start = _m.halfedge_handle(*v_it);
        HE_MESH::HalfedgeHandle he = start;
        int sharpNeighbours = 0;
        do {
            if (_m.property(_m.sharpedge, he) == true) {
                sharpNeighbours++;
            }

            he = _m.opposite_halfedge_handle(he);
            he = _m.next_halfedge_handle(he);
        } while (he != start);
        newMesh.property(newMesh.sharpneighbours, *v_it) = sharpNeighbours;

    }

    // initialize all weights to 0 (= smooth edge)
    for (HE_MESH::EdgeIter e_it = newMesh.edges_begin(); e_it != newMesh.edges_end(); ++e_it)
        newMesh.property(creaseWeights_, *e_it) = 0.0;

    for (size_t i = 0; i < _n; ++i) {

        HE_MESH::FaceIter f_itr = newMesh.faces_begin();
        HE_MESH::FaceIter f_end = newMesh.faces_end();
        for (; f_itr != f_end; ++f_itr) {
            newMesh.property(fp_pos_, *f_itr) = this->calc_face_centroid_weighted(*f_itr);
        }

        HE_MESH::EdgeIter e_itr = newMesh.edges_begin();
        HE_MESH::EdgeIter e_end = newMesh.edges_end();
        for (; e_itr != e_end; ++e_itr)
            compute_midpoint(newMesh, *e_itr, _update_points);

        if (_update_points) {
            HE_MESH::VertexIter v_itr = newMesh.vertices_begin();
            HE_MESH::VertexIter v_end = newMesh.vertices_end();
            for (; v_itr != v_end; ++v_itr)
                update_vertex(newMesh, *v_itr);

            v_itr = newMesh.vertices_begin();
            for (; v_itr != v_end; ++v_itr)
                newMesh.set_point(*v_itr, newMesh.property(vp_pos_, *v_itr));
        }

        e_itr = newMesh.edges_begin();
        for (; e_itr != e_end; ++e_itr)
            split_edge(newMesh, *e_itr);

        f_itr = newMesh.faces_begin();
        for (; f_itr != f_end; ++f_itr)
            split_face(newMesh, *f_itr);
    }
    newMesh.update_normals();
    return newMesh;
}

HE_MESH::Point CatmullClark1::calc_face_centroid_weighted(const HE_MESH::FaceHandle &_fh) {
    HE_MESH::Point pt;
    pt.vectorize(0);
    float valence = 0.0, fourthComp = 0.0f;
    for (HE_MESH::ConstFaceVertexIter cfv_it = this->mesh.cfv_iter(_fh); cfv_it.is_valid(); ++cfv_it) {
        fourthComp = getFourthComponent(this->mesh, cfv_it);


        pt += this->mesh.point(*cfv_it) * fourthComp;
        valence += fourthComp;
    }
    pt /= valence;
    return pt; // / fourthComp;
}

void CatmullClark1::compute_midpoint(HE_MESH &_m, const HE_MESH::EdgeHandle &_eh, const bool _update_points) {
    const HE_MESH::HalfedgeHandle &heh = _m.halfedge_handle(_eh, 0);
    HE_MESH::HalfedgeHandle opp_heh;

    //heh = _m.halfedge_handle(_eh, 0);
    opp_heh = _m.halfedge_handle(_eh, 1);

    float homo = 0.0f, fourthComponent;
    HE_MESH::Point pos(_m.point(_m.to_vertex_handle(heh)));
    fourthComponent = getFourthComponent(this->mesh, _m.to_vertex_handle(heh));
    homo += fourthComponent;
    pos *= fourthComponent;
    //pos *= this->mesh.property(this->mesh.vp_fourth, _m.to_vertex_handle(heh));
    fourthComponent = getFourthComponent(this->mesh, _m.to_vertex_handle(opp_heh));
    homo += fourthComponent;
    pos += (_m.point(_m.to_vertex_handle(opp_heh)) *
            fourthComponent);//* this->mesh.property(this->mesh.vp_fourth, _m.to_vertex_handle(opp_heh)) );

    std::vector<HE_MESH::HalfedgeHandle> edgesSharp;

    HE_MESH::HalfedgeHandle start = heh; //_eh;
    HE_MESH::HalfedgeHandle he = start;


    if (_m.property(_m.sharpedge, he) == true
        && _m.property(_m.sharpneighbours, _m.to_vertex_handle(he)) == 2
        && _m.property(_m.sharpneighbours, _m.from_vertex_handle(he)) == 2) {
        edgesSharp.push_back(he);
    }

    if (_m.is_boundary(_eh) || !_update_points
        || (
                _m.property(_m.sharpedge, heh) == true
                && _m.property(_m.sharpneighbours, _m.to_vertex_handle(heh)) == 2
                && _m.property(_m.sharpneighbours, _m.from_vertex_handle(heh)) == 2)
            ) {
        //|| edgesSharp.size() == 2) { //|| _m.property(_m.sharpedge, heh) == true) {
        pos /= homo;
    } else {
        pos += _m.property(fp_pos_, _m.face_handle(heh));
        pos += _m.property(fp_pos_, _m.face_handle(opp_heh));
        pos /= (homo + 2);
    }
    _m.property(ep_pos_, _eh) = pos;
}

void CatmullClark1::update_vertex(HE_MESH &_m, const HE_MESH::VertexHandle &_vh) {
    HE_MESH::Point pos(0.0, 0.0, 0.0), point = _m.point(_vh);
    HE_MESH::HalfedgeHandle heh = _m.halfedge_handle(_vh);

    std::vector<HE_MESH::HalfedgeHandle> edgesSharp;

    HE_MESH::HalfedgeHandle start = heh;
    HE_MESH::HalfedgeHandle he = start;

    do {
        if (_m.property(_m.sharpedge, he) == true
            && _m.property(_m.sharpneighbours, _m.to_vertex_handle(he)) == 2
            && _m.property(_m.sharpneighbours, _m.from_vertex_handle(he)) == 2)
            edgesSharp.push_back(he);

        he = _m.opposite_halfedge_handle(he);
        he = _m.next_halfedge_handle(he);
    } while (he != start);

    if (edgesSharp.size() == 2) {
        pos += point * 3.0f / 4.0f;
        float weight = 1.0f / 8.0f;

        HE_MESH::Point point0A = _m.point(_m.to_vertex_handle(edgesSharp[0]));
        pos += point0A * weight;

        HE_MESH::Point point1A = _m.point(_m.to_vertex_handle(edgesSharp[1]));
        pos += point1A * weight;

    } else if (_m.is_boundary(_vh)) {

        HE_MESH::Normal Vec;
        pos = _m.point(_vh);
        HE_MESH::VertexEdgeIter ve_itr;
        for (ve_itr = _m.ve_iter(_vh); ve_itr.is_valid(); ++ve_itr)
            if (_m.is_boundary(*ve_itr))
                pos += _m.property(ep_pos_, *ve_itr);
        pos /= static_cast<typename OpenMesh::vector_traits<typename HE_MESH::Point>::value_type>(3.0);
    } else // inner vertex
    {
        double valence(0.0);
        HE_MESH::VOHIter voh_it = _m.voh_iter(_vh);
        for (; voh_it.is_valid(); ++voh_it) {
            pos += _m.property(ep_pos_, _m.edge_handle(*voh_it));

            valence += 1.0;
        }
        pos /= valence;

        HE_MESH::VertexFaceIter vf_itr;
        HE_MESH::Point Q(0, 0, 0);

        for (vf_itr = _m.vf_iter(_vh); vf_itr.is_valid(); ++vf_itr) //, neigboring_faces += 1.0 )
        {
            Q += _m.property(fp_pos_, *vf_itr);
        }

        Q /= valence;//neigboring_faces;

        HE_MESH::Point old_point = _m.point(_vh);
        pos = old_point * (valence - 3) / valence + Q / valence + pos * 2 / valence;
        //      pos = vector_cast<Vec>(_m.point(_vh));
    }

    _m.property(vp_pos_, _vh) = pos;
}

void CatmullClark1::split_face(HE_MESH &_m, const HE_MESH::FaceHandle &_fh) {
/*
   Split an n-gon into n quads by connecting
   each vertex of fh to vh.

   - _fh will remain valid (it will become one of the quads)
   - the halfedge handles of the new quads will
   point to the old halfedges
 */

// Since edges already refined (valence*2)
    size_t valence = _m.valence(_fh) / 2;

// new mesh vertex from face centroid
    HE_MESH::VertexHandle vh = _m.add_vertex(_m.property(fp_pos_, _fh));

    HE_MESH::HalfedgeHandle hend = _m.halfedge_handle(_fh);
    HE_MESH::HalfedgeHandle hh = _m.next_halfedge_handle(hend);

    HE_MESH::HalfedgeHandle hold = _m.new_edge(_m.to_vertex_handle(hend), vh);

    _m.set_next_halfedge_handle(hend, hold);
    _m.set_face_handle(hold, _fh);

    hold = _m.opposite_halfedge_handle(hold);

    for (size_t i = 1; i < valence; i++) {
        HE_MESH::HalfedgeHandle hnext = _m.next_halfedge_handle(hh);

        HE_MESH::FaceHandle fnew = _m.new_face();

        _m.set_halfedge_handle(fnew, hh);

        HE_MESH::HalfedgeHandle hnew = _m.new_edge(_m.to_vertex_handle(hnext), vh);

        _m.set_face_handle(hnew, fnew);
        _m.set_face_handle(hold, fnew);
        _m.set_face_handle(hh, fnew);
        _m.set_face_handle(hnext, fnew);

        _m.set_next_halfedge_handle(hnew, hold);
        _m.set_next_halfedge_handle(hold, hh);
        _m.set_next_halfedge_handle(hh, hnext);
        hh = _m.next_halfedge_handle(hnext);
        _m.set_next_halfedge_handle(hnext, hnew);

        hold = _m.opposite_halfedge_handle(hnew);
    }

    _m.set_next_halfedge_handle(hold, hh);
    _m.set_next_halfedge_handle(hh, hend);
    hh = _m.next_halfedge_handle(hend);
    _m.set_next_halfedge_handle(hend, hh);
    _m.set_next_halfedge_handle(hh, hold);

    _m.set_face_handle(hold, _fh);

    _m.set_halfedge_handle(vh, hold);
}

void CatmullClark1::split_edge(HE_MESH &_m, const HE_MESH::EdgeHandle &_eh) {
    HE_MESH::HalfedgeHandle heh = _m.halfedge_handle(_eh, 0);
    HE_MESH::HalfedgeHandle opp_heh = _m.halfedge_handle(_eh, 1);

    HE_MESH::HalfedgeHandle new_heh, opp_new_heh, t_heh;
    HE_MESH::VertexHandle vh;
    HE_MESH::VertexHandle vh1(_m.to_vertex_handle(heh));
    HE_MESH::Point zero(0, 0, 0);

// new vertex
    vh = _m.new_vertex(zero);
    _m.set_point(vh, _m.property(ep_pos_, _eh));

// Re-link mesh entities
    if (_m.is_boundary(_eh)) {
        for (t_heh = heh;
             _m.next_halfedge_handle(t_heh) != opp_heh;
             t_heh = _m.opposite_halfedge_handle(_m.next_halfedge_handle(t_heh))) {}
    } else {
        for (t_heh = _m.next_halfedge_handle(opp_heh);
             _m.next_halfedge_handle(t_heh) != opp_heh;
             t_heh = _m.next_halfedge_handle(t_heh)) {}
    }

    new_heh = _m.new_edge(vh, vh1);
    opp_new_heh = _m.opposite_halfedge_handle(new_heh);
    _m.set_vertex_handle(heh, vh);

    _m.set_next_halfedge_handle(t_heh, opp_new_heh);
    _m.set_next_halfedge_handle(new_heh, _m.next_halfedge_handle(heh));
    _m.set_next_halfedge_handle(heh, new_heh);
    _m.set_next_halfedge_handle(opp_new_heh, opp_heh);

    if (_m.face_handle(opp_heh).is_valid()) {
        _m.set_face_handle(opp_new_heh, _m.face_handle(opp_heh));
        _m.set_halfedge_handle(_m.face_handle(opp_new_heh), opp_new_heh);
    }

    if (_m.face_handle(heh).is_valid()) {
        _m.set_face_handle(new_heh, _m.face_handle(heh));
        _m.set_halfedge_handle(_m.face_handle(heh), heh);
    }

    _m.property(_m.sharpedge, new_heh) = _m.property(_m.sharpedge, heh);
    _m.property(_m.sharpedge, opp_heh) = _m.property(_m.sharpedge, heh);

    _m.set_halfedge_handle(vh, new_heh);
    _m.set_halfedge_handle(vh1, opp_new_heh);

// Never forget this, when playing with the topology
    _m.adjust_outgoing_halfedge(vh);
    _m.adjust_outgoing_halfedge(vh1);
}

void CatmullClark1::calcLimitNormal(HE_MESH &newMesh, const OpenMesh::VertexHandle &vertex) {
    HE_MESH::Point VP(0.0, 0.0, 0.0);
    VP = newMesh.point(vertex);
    std::vector<HE_MESH::VertexHandle> vertices;

    HE_MESH::HalfedgeHandle start = newMesh.halfedge_handle(vertex);
    HE_MESH::HalfedgeHandle he = start;

    do {
        he = newMesh.opposite_halfedge_handle(he);
        vertices.push_back(newMesh.from_vertex_handle(he));
        he = newMesh.next_halfedge_handle(he);
    } while (he != start);

    std::vector<HE_MESH::HalfedgeHandle> halfEdges;

    start = newMesh.halfedge_handle(vertex);
    he = start;

    do {
        halfEdges.push_back(he);

        he = newMesh.opposite_halfedge_handle(he);
        he = newMesh.next_halfedge_handle(he);
    } while (he != start);

    QVector3D tangent1(0, 0, 0);
    QVector3D tangent2(0, 0, 0);

    int k = (int) vertices.size();

    float a = 1.0f + std::cos(2.0f * M_PI / k) +
              std::cos(M_PI / k) * std::sqrt(2.0f * (9.0f + std::cos(2.0f * M_PI / k)));

    int i = 0;

    for (HE_MESH::HalfedgeHandle halfEdge : halfEdges) {
        float beta1 = a * std::cos(2.0f * M_PI * (i + 1) / k);
        float gamma1 = std::cos(2.0f * M_PI * (i + 1) / k) + std::cos(2.0f * M_PI * (i + 2) / k);

        float beta2 = a * std::cos(2.0f * M_PI * i / k);
        float gamma2 = std::cos(2.0f * M_PI * i / k) + std::cos(2.0f * M_PI * (i + 1) / k);

        HE_MESH::HalfedgeHandle nextHE = newMesh.next_halfedge_handle(halfEdge);
        HE_MESH::Point nextPoint = newMesh.point(newMesh.from_vertex_handle(nextHE));

        HE_MESH::HalfedgeHandle nextNextHE = newMesh.next_halfedge_handle(nextHE);
        HE_MESH::Point nextNextPoint = newMesh.point(newMesh.from_vertex_handle(nextNextHE));

        QVector3D p1 = {nextPoint[0], nextPoint[1], nextPoint[2]};
        QVector3D p2 = {nextNextPoint[0], nextNextPoint[1], nextNextPoint[2]};

        tangent1 += p1 * beta1 + p2 * gamma1;
        tangent2 += p1 * beta2 + p2 * gamma2;
        i++;
    }

    QVector3D normal = QVector3D::normal(tangent2, tangent1);
    newMesh.property(newMesh.limitnormal, vertex) = HE_MESH::Normal(normal.x(), normal.y(), normal.z());

}
