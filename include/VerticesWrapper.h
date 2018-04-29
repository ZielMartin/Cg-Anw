//
// Created by Johannes on 29.04.2018.
//

#ifndef CG_ANW_VERTICESWRAPPER_H
#define CG_ANW_VERTICESWRAPPER_H


#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <c++/4.8.3/vector>


class VerticesWrapper {

    private:
        /* Here will be the instance stored. */
        static VerticesWrapper* instance;

    /* Private constructor to prevent instancing. */
        VerticesWrapper();

        std::vector<glm::vec4*> vertices;
        std::vector<glm::vec4*> selectedVertices;


    public:
        /* Static access method. */
        static VerticesWrapper* getInstance();

        std::vector<glm::vec4 *> *addVertex(glm::vec4 *vertex);
        std::vector<glm::vec4 *> *deleteVertex(glm::vec4 *vertex);

        bool selectVertex(glm::vec4 *vertex);
        void moveSelected(glm::vec3 relativeMovement);


        //Getters & Setters
        const std::vector<glm::vec4 *> *getVertices() const;
        void setVertices(const std::vector<glm::vec4 *> &vertices);
        const std::vector<glm::vec4 *> *getSelectedVertices() const;
        void setSelectedVertices(const std::vector<glm::vec4 *> *selectedVertices);





};


#endif //CG_ANW_VERTICESWRAPPER_H
