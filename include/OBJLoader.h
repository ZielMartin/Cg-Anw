//
// Created by martin on 05.07.17.
//

#ifndef BLATT06_OBJLOADER_H
#define BLATT06_OBJLOADER_H

#include <string>
#include <vector>
#include <ostream>
#include "HalfEdge.h"

namespace cg {

    struct FacePos {
        int first;
        int second;
        int third;

        friend std::ostream &operator<<(std::ostream &os, const FacePos &pos) {
            os << "first: " << pos.first << " second: " << pos.second << " third: " << pos.third;
            return os;
        }
    };

    class OBJLoader {
    public:
        OBJLoader();

        virtual ~OBJLoader();

        void readFile(std::string filename);

        const HalfEdgeStruct &getHalfEdgeStruct() const;

        const std::vector<std::shared_ptr<glm::vec3>> getVertices() const;

        const std::vector<std::shared_ptr<glm::vec3>> getNormals() const;

        const std::vector<std::shared_ptr<glm::vec2>> getTextures() const;

        const std::vector<std::vector<std::shared_ptr<FacePos>>> &getFListe() const;

        const std::vector<GLuint> getTriangles() const;

        const std::string &getType() const;

        unsigned int getDeg() const;

    private:

        void createHE_STRUCT();

        void addVert(std::vector<std::string> &tokenList);

        void addVertNormal(std::vector<std::string> &tokenList);

        void addVertText(std::vector<std::string> &tokenList);

        void addFace(const std::vector<std::string> &tokenList);

        void setType(std::string type);

        void setDeg(int deg);

    public:
        friend std::ostream &operator<<(std::ostream &os, const OBJLoader &loader);

    private:
        std::vector<std::shared_ptr<glm::vec3>> vListe;
        std::vector<std::shared_ptr<glm::vec3>> vnListe;
        std::vector<std::shared_ptr<glm::vec2>> vtListe;
        std::vector<std::vector<std::shared_ptr<FacePos>>> fListe;
        HalfEdgeStruct halfEdgeStruct;
        std::string type;
        unsigned int deg = 0;

        void initFields();

        void readLine(const std::string &line);
    };

    std::vector<GLuint> lineIndices(GLuint vecSize);

    std::vector<glm::vec3> convCopy(std::vector<std::shared_ptr<glm::vec3>> in);
}

#endif //BLATT06_OBJLOADER_H
