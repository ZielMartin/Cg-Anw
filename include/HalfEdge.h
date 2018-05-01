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
#include <vector>

#define VertPointer std::shared_ptr<HE_vert>
#define EdgePointer std::shared_ptr<HE_edge>
#define FacePointer std::shared_ptr<HE_face>

#define VertList std::vector<VertPointer>
#define EdgeList std::vector<EdgePointer>
#define FaceList std::vector<FacePointer>

namespace cg {
    struct HE_vert {

        HE_vert(double x, double y, double z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        double x = -1.0;
        double y = -1.0;
        double z = -1.0;

        glm::vec3 toVec() {
            return glm::vec3(x, y, z);
        }

        /**
         * one of the half-edges emanting from the vertex
         */
        std::shared_ptr<struct HE_edge> edge = nullptr;

        friend std::ostream &operator<<(std::ostream &os, const HE_vert &vert) {
            os << "vert{ " << "x: " << vert.x << " y: " << vert.y << " z: " << vert.z << " }";
            return os;
        }
    };

    struct HE_edge {
        /**
         * start-vertex of the half-edge
         */
        std::shared_ptr<struct HE_vert> vert = nullptr;

        /**
         * oppositely oriented adjacent half-edge
         */
        std::shared_ptr<struct HE_edge> pair = nullptr;

        /**
         * face the half-edge borders
         */
        std::shared_ptr<struct HE_face> face = nullptr;

        /**
         * next half-edge around the face
         */
        std::shared_ptr<struct HE_edge> next = nullptr;

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
        std::shared_ptr<struct HE_edge> edge = nullptr;

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

    class HE_Wrapper {
    public:
        void clear(void);
        void deleteVert(std::shared_ptr<HE_vert>);
        void deleteEdge(std::shared_ptr<HE_edge>);
        void deleteFace(std::shared_ptr<HE_face>);
    private:
        HalfEdgeStruct halfEdgeStruct;
    };
}
#endif /* INCLUDE_HALFEDGE_H_ */
