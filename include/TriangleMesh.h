#ifndef _triangle_H
#define _triangle_H

#include <vector>
#include <cmath>
#include <fstream>
#include <cstring>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "utils.h"

class Triangle;
class TriangleMesh;

class Triangle {
    friend class TriangleMesh;
	int _vertex[3];

    public:
        Triangle(int v1, int v2, int v3) { _vertex[0] = v1;  _vertex[1] = v2;  _vertex[2] = v3; }
};

class TriangleMesh {
    std::vector <glm::vec3> _vertices;
	std::vector <glm::vec2> _uvs;
	std::vector <Triangle> _triangles;
	glm::vec3 _min, _max;

    public:
        TriangleMesh(char * filename) { LoadFile(filename) ;};
        TriangleMesh() {};
        void LoadFile(char * filename);
        int TriangleCount() { return _triangles.size() ;};
        int VertexCount() { return _vertices.size();};
        std::vector<glm::vec3> &Vertices() { return _vertices; }
        std::vector<glm::vec2> &UVs() { return _uvs; }
};

#endif
