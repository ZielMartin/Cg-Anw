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

        //bool: true if selected
        std::vector<std::pair<glm::vec4*,bool>> vertices;


    public:
        /* Static access method. */
        static VerticesWrapper* getInstance();

    std::vector<std::pair<glm::vec4*,bool>> *addVertex(glm::vec4 *vertex, bool selected);
    std::vector<std::pair<glm::vec4*,bool>> *deleteVertex(glm::vec4 *vertex);

        bool selectVertex(glm::vec4 *vertex);
        void moveSelected(glm::vec3 relativeMovement);


        //Getters & Setters
        std::vector<std::pair<glm::vec4*,bool>> *getVertices();
        void setVertices(const std::vector<std::pair<glm::vec4*,bool>> &vertices);





};


#endif //CG_ANW_VERTICESWRAPPER_H
