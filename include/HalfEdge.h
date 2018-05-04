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
        HE_vert(glm::vec3 pos) {
            this->pos = glm::vec4(pos, 1.0);
        }

        HE_vert(glm::vec4 pos) {
            this->pos = pos;
        }

        glm::vec4 pos;

        /**
         * one of the half-edges emanting from the vertex
         */
        std::shared_ptr<struct HE_edge> edge = nullptr;
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
    };


    struct HE_face {
        /**
         * one of the half-edges bordering the face
         */
        std::shared_ptr<struct HE_edge> edge = nullptr;
    };

    class HE_Wrapper {
    public:
        void addVert(VertPointer);
        void addEdge(EdgePointer);
        void addFace(FacePointer);

        VertPointer createVert(glm::vec4);
        FacePointer createFace(VertList &);

        void clear(void);

        void deleteVert(VertPointer);
        void deleteEdge(EdgePointer);
        void deleteFace(FacePointer);

        const VertList &getVerts() const;

        const EdgeList &getEdges() const;

        const FaceList &getFaces() const;

    private:
        VertList verts;
        EdgeList edges;
        FaceList faces;
        std::map<VertPointer, EdgeList> accelerationStruct;

        EdgeList createEdgesFromVerts(const VertList &verts, FacePointer &newFace);
        void checkOrientation(VertList &verts);

        bool shouldReverse(const VertPointer &vert) const;

        EdgePointer findPairAndSetIt(EdgePointer &currentEdge, EdgeList &outgoingEdgesFromDestination);
    };
}
#endif /* INCLUDE_HALFEDGE_H_ */
