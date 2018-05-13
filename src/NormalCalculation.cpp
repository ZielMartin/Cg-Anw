//
// Created by Johannes on 12.05.2018.
//

#include "NormalCalculation.h"


namespace cg {


    void NormalCalculation::calcFaceNormal(FacePointer face) {
        VertPointer a, b, c;
        a = face->edge->vert;
        b = face->edge->next->vert;
        c = face->edge->next->next->vert;

        glm::vec3 vecA, vecB;
        vecA.x = b->pos.x - a->pos.y;
        vecA.y = b->pos.y - a->pos.y;
        vecA.z = b->pos.z - a->pos.z;

        vecB.x = c->pos.x - a->pos.y;
        vecB.y = c->pos.y - a->pos.y;
        vecB.z = c->pos.z - a->pos.z;

        face->faceNormal = glm::normalize(glm::cross(vecA, vecB));

        //std::cout << "VertA: x:" << a->pos.x << " y:" << a->pos.y << " z:" << a->pos.z << std::endl;
        //std::cout << "VertB: x:" << b->pos.x << " y:" << b->pos.y << " z:" << b->pos.z << std::endl;
        //std::cout << "VertC: x:" << c->pos.x << " y:" << c->pos.y << " z:" << c->pos.z << std::endl;

    }

    void NormalCalculation::calcVertexNormal(VertPointer vert) {
        glm::vec3 vertexNormal;
        int counter = 0;

        EdgePointer start = vert->edge;
        EdgePointer curr = start;
        do {
            if (!curr->face->isBoundary) {
                //calcFaceNormal(edge->face);
                vertexNormal += curr->face->faceNormal;
                counter++;
            }
            curr = curr->pair->next;
        } while (curr != start);


        vertexNormal.x = vertexNormal.x / counter;
        vertexNormal.y = vertexNormal.y / counter;
        vertexNormal.z = vertexNormal.z / counter;

        //std::cout << counter << std::endl;

        vert->vertNormal = glm::normalize(vertexNormal);
    }


    void NormalCalculation::recalcNormalsAroundVertex(std::map<FacePointer,EdgeList> neighborhood) {
        for (std::pair<FacePointer,EdgeList> face : neighborhood) {
            calcFaceNormal(face.first);
        }

        for (std::pair<FacePointer,EdgeList> face : neighborhood) {
            for(EdgePointer edge : face.second){
                calcVertexNormal(edge->vert);
            }
        }

    }

}



