/**
*Created by martin on 05.07.17.
*/
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <algorithm>
#include <iostream>
#include "OBJLoader.h"

class string;
namespace cg {

    OBJLoader::OBJLoader() {

    }

    OBJLoader::~OBJLoader() {

    }

    HE_Wrapper &OBJLoader::getHalfEdgeStruct() {
        return halfEdgeStruct;
    }

    void OBJLoader::readFile(std::string filename) {
        initFields();

        std::ifstream infile(filename);
        std::string line;

        if(infile.fail()){
            std::cout << "COULD NOT OPEN FILE" << std::endl;
        }else{
            std::cout << "OPEN FILE: " << infile.is_open() << std::endl;
        }

        while (std::getline(infile, line)) {
            readLine(line);
        }

        createHE_STRUCT();
    }

    void OBJLoader::readLine(const std::string &line) {
        std::string token;
        std::vector<std::string> tokenList;
        std::stringstream lineStream(line);

        while (getline(lineStream, token, ' ')) {
            tokenList.push_back(token);
        }

        if (tokenList.size() != 0) {

            if (tokenList.at(0).compare("v") == 0) {
                addVert(tokenList);
            } else if (tokenList.at(0).compare("vn") == 0) {
                addVertNormal(tokenList);
            } else if (tokenList.at(0).compare("vt") == 0) {
                addVertText(tokenList);
            } else if (tokenList.at(0).compare("f") == 0 || tokenList.at(0).compare("curv") == 0) {
                addFace(tokenList);
            } else if (tokenList.at(0).compare("cstype") == 0) {
                setType(tokenList.at(1));
            } else if (tokenList.at(0).compare("deg") == 0) {
                setDeg(stoi(tokenList.at(1)));
            }
        }
    }

    void OBJLoader::setDeg(int deg) {
        OBJLoader::deg = deg;
    }

    void OBJLoader::setType(std::string type) {
        OBJLoader::type = type;
        transform(OBJLoader::type.begin(), OBJLoader::type.end(), OBJLoader::type.begin(), tolower);
    }

    void OBJLoader::addFace(const std::vector<std::string> &tokenList) {
        std::vector<std::shared_ptr<FacePos>> face;
        for (const std::string &trio : tokenList) {
            if (trio.compare("f") != 0 && trio.compare("curv") != 0) {
                std::string num;
                FacePos *point = new FacePos;
                int pos = 0;
                std::stringstream trioStream(trio);
                while (getline(trioStream, num, '/')) {
                    //std::cout << num << std::endl;

                    if (pos == 0) {
                        point->vertIndex = stoi(num) - 1;
                    } else if (pos == 1) {
                        point->texturIndex = num.compare("")==0?0:stoi(num) - 1;
                    } else {
                        point->normalIndex = stoi(num) - 1;
                    }
                    pos++;
                }
                face.push_back(std::shared_ptr<FacePos>(point));
            }
        }
        fListe.push_back(face);
    }

    void OBJLoader::addVertText(std::vector<std::string> &tokenList) {
        if (tokenList.size() != 3) throw "tokenListe muss fuer vt 4 groß sein!";
        glm::vec2 *x = new glm::vec2(stof(tokenList.at(1)), stof(tokenList.at(2)));
        vtListe.push_back(std::shared_ptr<glm::vec2>(x));
    }

    void OBJLoader::addVertNormal(std::vector<std::string> &tokenList) {
        if (tokenList.size() != 4) throw "tokenListe muss fuer vb 4 groß sein!";
        glm::vec3 *x = new glm::vec3(stof(tokenList.at(1)), stof(tokenList.at(2)),
                                     stof(tokenList.at(3)));
        vnListe.push_back(std::shared_ptr<glm::vec3>(x));
    }

    void OBJLoader::addVert(std::vector<std::string> &tokenList) {
        if (tokenList.size() != 4) throw "tokenListe muss fuer v 4 groß sein!";
        glm::vec3 *x = new glm::vec3(stof(tokenList.at(1)), stof(tokenList.at(2)),
                                     stof(tokenList.at(3)));
        vListe.push_back(std::shared_ptr<glm::vec3>(x));
    }

    void OBJLoader::initFields() {
        vListe = std::vector<std::shared_ptr<glm::vec3>>();
        vnListe = std::vector<std::shared_ptr<glm::vec3>>();
        vtListe = std::vector<std::shared_ptr<glm::vec2>>();
        fListe = std::vector<std::vector<std::shared_ptr<FacePos>>>();
        halfEdgeStruct = HE_Wrapper();
    }

    void OBJLoader::createHE_STRUCT() {

        for (std::shared_ptr<glm::vec3> vert : vListe) {
            halfEdgeStruct.createVert(glm::vec4(*vert, 1.0));
        }


        /* int i = 0;
         for(std::shared_ptr<glm::vec3> vertexNormal : vnListe){
             halfEdgeStruct.getVerts()[i]->vertNormal = *vertexNormal;
             i++;
         }*/

        for (std::vector<std::shared_ptr<FacePos>> facePoses : fListe) {
            VertList vl;
            for (std::shared_ptr<FacePos> facePos : facePoses) {
                vl.push_back(halfEdgeStruct.getVerts().at(facePos->vertIndex));
            }
            halfEdgeStruct.createFace(vl);
        }


    }



    std::ostream &operator<<(std::ostream &os, const cg::OBJLoader &loader) {
        os << "vListe: " << std::endl;
        for (std::shared_ptr<glm::vec3> vec : loader.vListe) {
            os << "[x:" << vec->x << " y:" << vec->y << " z:" << vec->z << "]" << std::endl;
        }

        os << "vnListe: " << std::endl;
        for (std::shared_ptr<glm::vec3> vec : loader.vnListe) {
            os << "[x:" << vec->x << " y:" << vec->y << " z:" << vec->z << "]" << std::endl;
        }

        os << "vtListe: " << std::endl;
        for (std::shared_ptr<glm::vec2> vec : loader.vtListe) {
            os << "[x:" << vec->x << " y:" << vec->y << "]" << std::endl;
        }

        os << "fListe: " << std::endl;
        for (std::vector<std::shared_ptr<FacePos>> face : loader.fListe) {
            os << "[";

            for (std::shared_ptr<FacePos> faceP : face) {
                os << faceP->vertIndex << "/" << faceP->texturIndex << "/" << faceP->normalIndex << ";";
            }

            os << "]" << std::endl;
        }
        /*os << " halfEdgeStruct: " << loader.halfEdgeStruct;*/
        return os;
    }

    const std::vector<std::shared_ptr<glm::vec3>> OBJLoader::getVertices() const {
        std::vector<std::shared_ptr<glm::vec3>> vertices;
        for (std::vector<std::shared_ptr<FacePos>> polygon : fListe) {
            for (std::shared_ptr<FacePos> facePos : polygon) {
                vertices.push_back(vListe[facePos->vertIndex]);
            }
        }

        return vertices;
    }

    const std::vector<std::shared_ptr<glm::vec3>> OBJLoader::getNormals() const {
        std::vector<std::shared_ptr<glm::vec3>> vertices;
        for (std::vector<std::shared_ptr<FacePos>> polygon : fListe) {
            for (std::shared_ptr<FacePos> facePos : polygon) {
                vertices.push_back(vnListe[facePos->vertIndex]);
            }
        }

        return vertices;
    }


    const std::vector<std::shared_ptr<glm::vec2>> OBJLoader::getTextures() const {
        throw "not implemented!";
        return vtListe;
    }

    const std::vector<GLuint> OBJLoader::getTriangles() const {
        std::vector<GLuint> indices;
        unsigned int triangleNum = 0;
        for (std::vector<std::shared_ptr<FacePos>> polygon : fListe) {
            int j = 0;
            for (; j + 2 < polygon.size(); j++) {
                indices.push_back(triangleNum);
                indices.push_back(triangleNum + j + 2);
                indices.push_back(triangleNum + j + 1);
            }
            triangleNum += j + 2;
        }
        return indices;
    }

    const std::string &OBJLoader::getType() const {
        return type;
    }

    const std::vector<std::vector<std::shared_ptr<FacePos>>> &OBJLoader::getFListe() const {
        return fListe;
    }

    unsigned int OBJLoader::getDeg() const {
        return deg;
    }

    std::vector<GLuint> lineIndices(GLuint vecSize) {
        std::vector<GLuint> indices;
        if (vecSize < 2 || vecSize % 2 != 0) return indices;
        for (GLuint i = 0; i < vecSize; i++) {
            indices.push_back(i);
        }
        return indices;
    }

    std::vector<glm::vec3> convCopy(std::vector<std::shared_ptr<glm::vec3>> in) {
        std::vector<glm::vec3> out;
        for (std::shared_ptr<glm::vec3> vec : in) {
            out.push_back(*vec);
        }
        return out;
    }
}