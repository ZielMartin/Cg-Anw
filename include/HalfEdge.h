/*
 * HalfEdge.h
 *
 *  Created on: 13.06.2017
 *      Author: martin
 */

#ifndef INCLUDE_HALFEDGE_H_
#define INCLUDE_HALFEDGE_H_

#include <ostream>
#include <list>
#include <glm/glm.hpp>
#include <memory>
#include <map>

namespace cg {
    struct HE_vert {
        float x = -1.0;
        float y = -1.0;
        float z = -1.0;

        glm::vec3 toVec() {
            return glm::vec3(x, y, z);
        }

        /**
         * one of the half-edges emanting from the vertex
         */
        struct HE_edge *edge = nullptr;

        friend std::ostream &operator<<(std::ostream &os, const HE_vert &vert) {
            os << "vert{ " << "x: " << vert.x << " y: " << vert.y << " z: " << vert.z << " }";
            return os;
        }
    };

    struct HE_edge {
        /**
         * start-vertex of the half-edge
         */
        struct HE_vert *vert = nullptr;

        /**
         * oppositely oriented adjacent half-edge
         */
        struct HE_edge *pair = nullptr;

        /**
         * face the half-edge borders
         */
        struct HE_face *face = nullptr;

        /**
         * next half-edge around the face
         */
        struct HE_edge *next = nullptr;

        friend std::ostream &operator<<(std::ostream &os, const HE_edge &edge) {
            if (edge.vert != nullptr)
                os << "edge{ " << "vert: " << *edge.vert << " pair: " << edge.pair << " face: " << edge.face
                   << " next: " << edge.next << " }";
            else
                os << "edge{ " << "vert: " << edge.vert << " pair: " << edge.pair << " face: " << edge.face << " next: "
                   << edge.next << " }";
            return os;
        }
    };


    struct HE_face {
        /**
         * one of the half-edges bordering the face
         */
        struct HE_edge *edge = nullptr;

        friend std::ostream &operator<<(std::ostream &os, const HE_face &face) {
            if (face.edge != nullptr)
                os << "face{ " << "edge: " << *face.edge << " }";
            else
                os << "face{ " << "edge: " << face.edge << " }";
            return os;
        }


    };

    struct HalfEdgeStruct {
        std::vector<std::shared_ptr<HE_vert>> verts;
        std::vector<std::shared_ptr<HE_edge>> edges;
        std::vector<std::shared_ptr<HE_face>> faces;

        std::map<std::shared_ptr<HE_vert>, std::vector<std::shared_ptr<HE_edge>>> beschleunigunsStruktur;

        void clear(void) {
            verts.clear();
            edges.clear();
            faces.clear();
            beschleunigunsStruktur.clear();
        }
    };
}
#endif /* INCLUDE_HALFEDGE_H_ */
