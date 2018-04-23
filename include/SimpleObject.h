//
// Created by martin on 04.07.17.
//

#ifndef BLATT06_SIMPLEOBJECT_H
#define BLATT06_SIMPLEOBJECT_H

#define vec3List std::vector<glm::vec3>
#define gluintList std::vector<GLuint>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <ostream>
#include <memory>
#include "GLSLProgram.h"
#include "Object.h"

#ifndef DEFAULTSHADER
#define DEFAULTSHADER simple
#endif

namespace cg {

    struct Light {
        glm::vec3 position;
        glm::vec3 color;
        float intensity;

        //true = directional, otherwise pointlight
        int directional;
    };

    struct Material {
        float kd, ks, ka;
        int shininess;
    };

    enum Shader {
        simple,
        gouraud,
        phong
    };

    struct BoundingBox {
        float maxX = 0, minX = 0, maxY = 0, minY = 0, maxZ = 0, minZ = 0;
    };

    class SimpleObject {
    public:

        SimpleObject(const vec3List &vertices, const gluintList &indices, GLenum mode = GL_TRIANGLES,
                     const vec3List &normals = vec3List(),
                     Shader shader = DEFAULTSHADER);

        SimpleObject(const vec3List &vertices, const gluintList &indices, const vec3List &colors,
                     GLenum mode = GL_TRIANGLES, const vec3List &normals = vec3List(), Shader shader = DEFAULTSHADER);

        virtual ~SimpleObject();

        void render();

        void addChild(std::shared_ptr<cg::SimpleObject> child);

        void removeChild(std::shared_ptr<cg::SimpleObject> child);

        const std::vector<std::shared_ptr<SimpleObject>> &getChildren() const;

        void centerThis(void);

        glm::vec3 getScalingFactor() const;


    private:
        void createVao(const vec3List &vertices, const gluintList &indices, const vec3List &colors,
                       GLenum mode = GL_TRIANGLES, const vec3List &normals = vec3List());

        static void initShader(GLSLProgram &program, Shader shaderType);

        void initMaterial();

        void initBoundingBox(const vec3List &vertices);

        friend std::ostream &operator<<(std::ostream &os, const SimpleObject &object) {
            os << object.name << " SimpleObject [ " << "programSimple: " << object.programSimple << " object: "
               << object.object << " ]";
            return os;
        }


    public:
        glm::mat4 model;
        glm::mat4 rotate;
        std::string name = "";

        Material material;
        static Light lights[];
    private:
        GLSLProgram programSimple;

        BoundingBox boundingBox;

        std::vector<std::shared_ptr<cg::SimpleObject>> children;

        Object *object = new Object;

        glm::mat4 center;
    };

}
#endif //BLATT06_SIMPLEOBJECT_H
