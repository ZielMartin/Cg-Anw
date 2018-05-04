//
// Created by martin on 04.07.17.
//

#include "SimpleObject.h"

#include <iostream>
#include <algorithm>

namespace cg {

    SimpleObject::SimpleObject(const vec3List &vertices, const gluintList &indices, GLenum mode,
                               const vec3List &normals, Shader shader) {
        std::cout << "Constructor SimpleObject" << std::endl;
        initBoundingBox(vertices);
        vec3List colors;

        glm::vec3 baseColor; //glm::vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
//                                        static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
//                                        static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

        baseColor = glm::vec3(0.0, 0.0, 1.0);

        for (int i = 0; i < indices.size(); i++) {
#ifdef changeColor
            baseColor = glm::vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                                  static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                                  static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
#endif
            colors.push_back(baseColor);


        }

        initMaterial();
        initShader(programSimple, shader);

        createVao(vertices, indices, colors, mode, normals);
    }

    SimpleObject::SimpleObject(const vec3List &vertices, const gluintList &indices, const vec3List &colors, GLenum mode,
                               const vec3List &normals, Shader shader) {
        std::cout << "Constructor SimpleObject" << std::endl;
        initBoundingBox(vertices);
        initMaterial();
        initShader(programSimple, shader);

        createVao(vertices, indices, colors, mode, normals);

    }

    SimpleObject::~SimpleObject() {
        children.clear();
        std::cout << "Destructor SimpleObject" << std::endl;
    }

    void SimpleObject::render(void) {
        throw -1;
//        // Create mvp.
//        glm::mat4 modelview = cg::ViewProjection::view * model * rotate * center;
//        glm::mat4 mvp = cg::ViewProjection::projection * modelview;
//        glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelview));
//
//        // Bind the shader program and set uniform(s).
//        programSimple.use();
//        programSimple.setUniform("mvp", mvp);
//
//        programSimple.setUniform("modelview", modelview);
//
//        programSimple.setUniform("normalMatrix", normalMatrix);
//
//        programSimple.setUniform("LightSource[0].position", lights[0].position);
//        programSimple.setUniform("LightSource[0].color", lights[0].color);
//        programSimple.setUniform("LightSource[0].intensity", lights[0].intensity);
//        programSimple.setUniform("LightSource[0].directional", lights[0].directional);
//
//        programSimple.setUniform("LightSource[1].position", lights[1].position);
//        programSimple.setUniform("LightSource[1].color", lights[1].color);
//        programSimple.setUniform("LightSource[1].intensity", lights[1].intensity);
//        programSimple.setUniform("LightSource[1].directional", lights[1].directional);
//
//
//        programSimple.setUniform("material.ka", material.ka);
//        programSimple.setUniform("material.kd", material.kd);
//        programSimple.setUniform("material.ks", material.ks);
//        programSimple.setUniform("material.shininess", material.shininess);
//
//
//        glBindVertexArray(object->vao);
//        glDrawElements(object->mode, object->indexCount, GL_UNSIGNED_INT, 0);
//
//        glBindVertexArray(0);
    }


    void SimpleObject::addChild(std::shared_ptr<cg::SimpleObject> child) {
        if (child == nullptr) return;

        children.push_back(child);
    }

    void SimpleObject::removeChild(std::shared_ptr<cg::SimpleObject> child) {
        if (child == nullptr) return;

        children.erase(std::remove(children.begin(), children.end(), child), children.end());
    }


    void SimpleObject::createVao(const vec3List &vertices, const gluintList &indices, const vec3List &colors, GLenum mode,
                            const vec3List &normals) {
        GLuint programId = programSimple.getHandle();
        GLuint pos;

        // Vertex array object.
        glGenVertexArrays(1, &object->vao);
        glBindVertexArray(object->vao);

        // Position buffer.
        glGenBuffers(1, &object->positionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, object->positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                     vertices.data(), GL_STATIC_DRAW);

        pos = glGetAttribLocation(programId, "position");
        glEnableVertexAttribArray(pos);
        glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Normal buffer.
        glGenBuffers(1, &object->normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, object->normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
                     normals.data(), GL_STATIC_DRAW);

        pos = glGetAttribLocation(programId, "normal");
        glEnableVertexAttribArray(pos);
        glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Color buffer.
        glGenBuffers(1, &object->colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, object->colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3),
                     colors.data(), GL_STATIC_DRAW);

        pos = glGetAttribLocation(programId, "color");
        glEnableVertexAttribArray(pos);
        glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Index buffer.
        object->indexCount = (GLuint) indices.size();

        glGenBuffers(1, &object->indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->indexCount * sizeof(GLuint),
                     indices.data(), GL_STATIC_DRAW);

        object->mode = mode;
    }

    void SimpleObject::initShader(GLSLProgram &program, Shader shaderType) {
        std::string vert, frag;
        std::cout << "initShader" << std::endl;
        switch (shaderType) {
            case simple:
                vert = "../shader/simple.vert";
                frag = "../shader/simple.frag";
                break;
            case gouraud:
                vert = "../shader/gouraud.vert";
                frag = "../shader/gouraud.frag";
                break;
            case phong:
                vert = "../shader/phong.vert";
                frag = "../shader/phong.frag";
                break;
        }
        if (!program.compileShaderFromFile(vert.c_str(), cg::GLSLShader::VERTEX)) {
            throw std::runtime_error(program.log());
        }

        if (!program.compileShaderFromFile(frag.c_str(), cg::GLSLShader::FRAGMENT)) {
            throw std::runtime_error(program.log());
        }

        if (!program.link()) {
            throw std::runtime_error(program.log());
        }
    }

    const std::vector<std::shared_ptr<SimpleObject>> &SimpleObject::getChildren() const {
        return children;
    }

    void SimpleObject::initBoundingBox(const vec3List &vertices) {
        for (glm::vec3 vec : vertices) {
            if (vec.x > boundingBox.maxX) {
                boundingBox.maxX = vec.x;
            }
            if (vec.x < boundingBox.minX) {
                boundingBox.minX = vec.x;
            }
            if (vec.y > boundingBox.maxY) {
                boundingBox.maxY = vec.y;
            }
            if (vec.y < boundingBox.minY) {
                boundingBox.minY = vec.y;
            }
            if (vec.z > boundingBox.maxZ) {
                boundingBox.maxZ = vec.z;
            }
            if (vec.z < boundingBox.minZ) {
                boundingBox.minZ = vec.z;
            }
        }
    }

    void SimpleObject::centerThis(void) {

        glm::vec3 toCenter = {
                -1.0 / 2 * (boundingBox.maxX + boundingBox.minX),
                -1.0 / 2 * (boundingBox.maxY + boundingBox.minY),
                -1.0 / 2 * (boundingBox.maxZ + boundingBox.minZ)
        };

        glm::vec3 toScale = getScalingFactor();


        center = glm::scale(center, toScale);
        center = glm::translate(center, toCenter);

        for (std::shared_ptr<SimpleObject> child : children) {
            child->center = glm::scale(child->center, toScale);
            child->center = glm::translate(child->center, toCenter);
        }
    }

    glm::vec3 SimpleObject::getScalingFactor() const {
        glm::vec3 boundingBoxDiff = {
                (boundingBox.maxX - boundingBox.minX),
                (boundingBox.maxY - boundingBox.minY),
                (boundingBox.maxZ - boundingBox.minZ)

        };

        glm::vec3 toScale;
        if (boundingBoxDiff.x > boundingBoxDiff.z) {
            if (boundingBoxDiff.x > boundingBoxDiff.y) {
                toScale = glm::vec3(1.0 / boundingBoxDiff.x);
            } else {
                toScale = glm::vec3(1.0 / boundingBoxDiff.y);
            }
        } else if (boundingBoxDiff.y > boundingBoxDiff.z) {
            toScale = glm::vec3(1.0 / boundingBoxDiff.y);
        } else {
            toScale = glm::vec3(1.0 / boundingBoxDiff.z);
        }
        return toScale;
    }

    void SimpleObject::initMaterial() {
        material.shininess = 200;
        material.ka = .1;
        material.kd = .4;
        material.ks = .5;
    }

    Light SimpleObject::lights[2];
}