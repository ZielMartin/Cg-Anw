#include "TriangleMesh.h"

// This function loads an obj format file This is a utility and should not have
// to be modified for teapot.obj (the assignment).  Note that this function does
// not load the normals for the mesh.  These are not provided in the teapot.obj
// file, you must compute these yourself
void TriangleMesh::LoadFile(char * filename) {
	FILE *file = fopen(filename, "r");
	if(file  == NULL){
        std::cerr << "Can't open file " << filename << std::endl;
		return;
	}
    std::vector<unsigned int> vertexIndices, uvIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	_max.x =-10000; _max.y =-10000; _max.z =-10000;
	_min.x =10000; _min.y =10000; _min.z =10000;
	glm::vec3 averageVertex(0.0f);
	//Read in .obj
	while(1){
		char lineHeader[128];
		//read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if(res == EOF){
			break;
		}
		if(strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
			averageVertex += vertex;
			if (vertex.x > _max.x) _max.x = vertex.x;
			if (vertex.y > _max.y) _max.y = vertex.y;
			if (vertex.z > _max.z) _max.z = vertex.z;
			if (vertex.x < _min.x) _min.x = vertex.x;
			if (vertex.y < _min.y) _min.y = vertex.y;
			if (vertex.z < _min.z) _min.z = vertex.z;
		}
		else if(strcmp(lineHeader, "vt" ) == 0){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tempUVs.push_back(uv);
		}
		else if(strcmp(lineHeader, "f" ) == 0){
            std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3];
			int matches = fscanf(file, "%d/%d %d/%d %d/%d\n",&vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2] );
			if(matches != 6){
                std::cerr << "Can't be read by simple parser!" << std::endl;
				return;
			}
			Triangle trig(vertexIndex[0],vertexIndex[1],vertexIndex[2]);
			_triangles.push_back(trig);
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
		}
	}
	//Arrange in triangle order to fit with OpenGL's glDrawArrays(...) function.
	//i.e. _vertices[0],_vertices[1], and _vertices[2] are the first triangle,
	//_vertices[3],_vertices[4], and _vertices[5] are the second etc.
	//Note this invalidates the Triangle structures in _triangles.  The Triangle class
	//is used to help with creating an array of indexes for glDrawElements(...); if you want to use
	//this instead of reordering the vertices
	_vertices.resize(vertexIndices.size());
	_uvs.resize(uvIndices.size());
	for(unsigned int i = 0; i < vertexIndices.size(); ++i){
		_vertices[i] = tempVertices[vertexIndices[i]-1];
		_uvs[i] = tempUVs[uvIndices[i]-1];
	}
	float range;
	if (_max.x-_min.x > _max.y-_min.y){
		range = _max.x-_min.x;
	}
	else{
		range = _max.y-_min.y;
	}
	averageVertex /= _vertices.size();
	for (int i = 0; i < _vertices.size(); i++)
	{
		_vertices[i] = (_vertices[i]-averageVertex)/range*400.0f;
	}
};
