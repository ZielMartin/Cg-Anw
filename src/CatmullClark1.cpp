//
// Created by ZielM on 13.06.2018.
//

#include "CatmullClark1.h"
#include "MeshWrapper.h"
#include <OpenMesh/Tools/Utils/MeshCheckerT.hh>
#include <OpenMesh/Core/Utils/vector_traits.hh>

CatmullClark1::CatmullClark1() {

}

HE_MESH CatmullClark1::operator()(HE_MESH &_m, size_t _n, const bool _update_points) {
    this->mesh = _m;
    HE_MESH newMesh = _m;

    newMesh.add_property(vp_pos_);
    newMesh.add_property(ep_pos_);
    newMesh.add_property(fp_pos_);
    newMesh.add_property(creaseWeights_);

    // initialize all weights to 0 (= smooth edge)
    for (HE_MESH::EdgeIter e_it = newMesh.edges_begin(); e_it != newMesh.edges_end(); ++e_it)
        newMesh.property(creaseWeights_, *e_it) = 0.0;

    // Do _n subdivisions
    for (size_t i = 0; i < _n; ++i) {

        // Compute face centroid
        HE_MESH::FaceIter f_itr = newMesh.faces_begin();
        HE_MESH::FaceIter f_end = newMesh.faces_end();
        for (; f_itr != f_end; ++f_itr) {
            newMesh.property(fp_pos_, *f_itr) = this->calc_face_centroid_weighted(*f_itr);
        }

        // Compute position for new (edge-) vertices
        HE_MESH::EdgeIter e_itr = newMesh.edges_begin();
        HE_MESH::EdgeIter e_end = newMesh.edges_end();
        for (; e_itr != e_end; ++e_itr)
            compute_midpoint(newMesh, *e_itr, _update_points);

        // position updates activated?
        if (_update_points) {
            // compute new positions for old vertices
            HE_MESH::VertexIter v_itr = newMesh.vertices_begin();
            HE_MESH::VertexIter v_end = newMesh.vertices_end();
            for (; v_itr != v_end; ++v_itr)
                update_vertex(newMesh, *v_itr);

            // Commit changes in geometry
            v_itr = newMesh.vertices_begin();
            for (; v_itr != v_end; ++v_itr)
                newMesh.set_point(*v_itr, newMesh.property(vp_pos_, *v_itr));
        }

        // Split each edge at midpoint stored in edge property ep_pos_;
        // Attention! Creating new edges, hence make sure the loop ends correctly.
        e_itr = newMesh.edges_begin();
        for (; e_itr != e_end; ++e_itr)
            split_edge(newMesh, *e_itr);

        // Commit changes in topology and reconsitute consistency
        // Attention! Creating new faces, hence make sure the loop ends correctly.
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
    float valence = 0.0;
    for (HE_MESH::ConstFaceVertexIter cfv_it = this->mesh.cfv_iter(_fh); cfv_it.is_valid(); ++cfv_it) {
        float fourthComp = this->mesh.property(this->mesh.vp_fourth, *cfv_it);
        if (fourthComp == 0.0f)
            fourthComp = 1.0f;

        pt += this->mesh.point(*cfv_it) * fourthComp;
        valence += fourthComp;
    }
    pt /= valence;
    return pt;
}

void CatmullClark1::compute_midpoint(HE_MESH &_m, const HE_MESH::EdgeHandle &_eh, const bool _update_points) {
    HE_MESH::HalfedgeHandle heh, opp_heh;

    heh = _m.halfedge_handle(_eh, 0);
    opp_heh = _m.halfedge_handle(_eh, 1);

    HE_MESH::Point pos(_m.point(_m.to_vertex_handle(heh)));

    pos += _m.point(_m.to_vertex_handle(opp_heh));

    // boundary edge: just average vertex positions
    // this yields the [1/2 1/2] mask
    if (_m.is_boundary(_eh) || !_update_points) {
        pos *= 0.5f;
    } else // inner edge: add neighbouring Vertices to sum
        // this yields the [1/16 1/16; 3/8 3/8; 1/16 1/16] mask
    {
        pos += _m.property(fp_pos_, _m.face_handle(heh));
        pos += _m.property(fp_pos_, _m.face_handle(opp_heh));
        pos *= 0.25;
    }
    _m.property(ep_pos_, _eh) = pos;
}

void CatmullClark1::update_vertex(HE_MESH &_m, const HE_MESH::VertexHandle &_vh) {
    HE_MESH::Point pos(0.0, 0.0, 0.0);

    if (_m.is_boundary(_vh)) {
        HE_MESH::Normal Vec;
        pos = _m.point(_vh);
        HE_MESH::VertexEdgeIter ve_itr;
        for (ve_itr = _m.ve_iter(_vh); ve_itr.is_valid(); ++ve_itr)
            if (_m.is_boundary(*ve_itr))
                pos += _m.property(ep_pos_, *ve_itr);
        pos /= static_cast<typename OpenMesh::vector_traits<typename HE_MESH::Point>::value_type>(3.0);
    } else // inner vertex
    {
        /* For each (non boundary) vertex V, introduce a new vertex whose
           position is F/n + 2E/n + (n-3)V/n where F is the average of
           the new face vertices of all faces adjacent to the old vertex
           V, E is the average of the midpoints of all edges incident
           on the old vertex V, and n is the number of edges incident on
           the vertex.
           */

        /*
        Normal           Vec;
        VertexEdgeIter   ve_itr;
        double           valence(0.0);

        // R = Calculate Valence and sum of edge midpoints
        for ( ve_itr = _m.ve_iter( _vh); ve_itr; ++ve_itr)
        {
          valence+=1.0;
          pos += _m.property(ep_pos_, *ve_itr);
        }
        pos /= valence*valence;
        */

        double valence(0.0);
        HE_MESH::VOHIter voh_it = _m.voh_iter(_vh);
        for (; voh_it.is_valid(); ++voh_it) {
            pos += _m.point(_m.to_vertex_handle(*voh_it));
            valence += 1.0;
        }
        pos /= valence * valence;

        HE_MESH::VertexFaceIter vf_itr;
        HE_MESH::Point Q(0, 0, 0);

        for (vf_itr = _m.vf_iter(_vh); vf_itr.is_valid(); ++vf_itr) //, neigboring_faces += 1.0 )
        {
            Q += _m.property(fp_pos_, *vf_itr);
        }

        Q /= valence * valence;//neigboring_faces;

        pos += _m.point(_vh) * (valence - 2.0) / valence + Q;
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

    _m.set_halfedge_handle(vh, new_heh);
    _m.set_halfedge_handle(vh1, opp_new_heh);

// Never forget this, when playing with the topology
    _m.adjust_outgoing_halfedge(vh);
    _m.adjust_outgoing_halfedge(vh1);
}