//
// Created by Johannes on 12.05.2018.
//

#include "NormalCalculation.h"

namespace cg {

    void calcFaceNormal(FacePointer face) {
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
    }

    void calcVertexNormal(VertPointer vert) {
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

        std::cout << counter << std::endl;

        vert->vertNormal = glm::normalize(vertexNormal);
    }


    void calcNormalsOnMove(VertPointer vert) {
        EdgePointer start = vert->edge;
        EdgePointer curr = start;
        if (curr != nullptr) {
            do {
                if (!curr->face->isBoundary) {
                    calcFaceNormal(curr->face);

                    EdgePointer start = curr->face->edge;
                    EdgePointer curr2 = start;
                    do {
                        calcVertexNormal(curr2->vert);
                        curr2 = curr2->next;
                    } while (curr2 != start);

                }

                curr = curr->pair->next;
            } while (curr != start);

        }

    }


}
