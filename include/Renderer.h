//
// Created by Johannes on 28.05.2018.
//

#ifndef CG_ANW_VBOHANDLER_H
#define CG_ANW_VBOHANDLER_H



#include "Shader.h"
#include "MeshWrapper.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>





struct Object {
    /* IDs for several buffers. */
    uint32_t vaoID;

    uint32_t vertex_position_buffer;
    uint32_t vertex_index_buffer;
    uint32_t vertex_normal_buffer;
    uint32_t vertex_color_buffer;
    uint32_t vertex_radius_buffer;


    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    //indices can be empty, using DRAW_ARRAY then
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> colors;
    std::vector<float> radius;

    /* Model matrix */
    glm::mat4x4 model;
};


class Renderer {
public:
    Renderer();

    void renderObject(Object &object, int gl_draw_type);

    void render();

    bool isRenderGrid() const;

    void setRenderGrid(bool drawGrid);

    bool isRenderPoints() const;

    void setRenderPoints(bool renderPoints);

    void initRenderer(Shader &shader, char* model_path);

    void select(glm::vec3 pos);

    void moveSelected(glm::vec3 relativeMovement);

    void deleteSelectedVertices();

    void addVertex(glm::vec3 worldPos);

    void addFace();

    void subdivision();

    void undo();

    void rotateMesh(glm::vec3 rotationVec, float angle);


private:

    void initGrid();

    void initGridPane();

    void initMesh();

    void setup_vao(Object &object);


    template<typename T>
    void updateBufferData(uint32_t &bufferID, std::vector<T> &data);

    template<typename T>
    void setupBufferData(uint32_t &bufferID, std::vector<T> &data);

    void clearObject(Object &object);

    void updateMesh(bool updatePointColor);

    void fillMeshData(bool updatePointColor);

    void bindBufferToShader(uint32_t &bufferID, uint32_t &location, int size);



private:

    Object meshObject, gridObject, meshPointsObject, meshLinesObject, gridPaneObject;

    MeshWrapper meshWrapper;

    Shader shader;

    glm::vec3 gridPaneColor;
    glm::vec3 selectedColor;
    glm::vec3 pointsColor;
    glm::vec3 gridColor;
    glm::vec3 faceColor;
    glm::vec3 meshLineColor;
    float grid_lenght;
    int pointSize;

    bool renderGrid, renderPoints;


    void recreateMesh();

};


#endif //CG_ANW_VBOHANDLER_H
