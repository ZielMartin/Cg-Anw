//
// Created by Johannes on 28.05.2018.
//

#ifndef CG_ANW_VBOHANDLER_H
#define CG_ANW_VBOHANDLER_H



#include "Shader.h"
#include "TriangleMesh.h"

#include <libs/glm-0.9.7.2/glm/mat4x4.hpp>
#include <libs/glm-0.9.7.2/glm/vec3.hpp>
#include <vector>




struct Object {
    /* IDs for several buffers. */
    uint32_t vaoID;

    uint32_t vertex_position_buffer;
    uint32_t vertex_normal_buffer;
    uint32_t vertex_color_buffer;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;

    /* Model matrix */
    glm::mat4x4 model;
};


class Renderer {
public:
    Renderer();

    void renderObject(Object &object, int gl_draw_type, Shader &shader);

    void render(Shader &shader);

    bool isDrawGrid() const;

    void setDrawGrid(bool drawGrid);

    void initRenderer(Shader &shader, char* model_path);


private:

    void initGrid(Shader &shader);

    void initGridPane(Shader &shader);

    void initMesh(Shader &shader);

    void setup_vao(Object &object, Shader &shader);

    void setup_vertex_normal_buffer_object_tri(Object &object, bool smoothed);

    void setup_vertex_color_buffer_object(Object &object);

    void setup_vertex_position_buffer_object(Object &object);



    bool drawGrid;

    Object mesh, grid, meshPoints, gridPane;

    TriangleMesh trig;

    glm::vec3 gridPaneColor;
    glm::vec3 selectedPointsColor;
    glm::vec3 pointsColor;
    glm::vec3 gridColor;
    glm::vec3 faceColor;


};


#endif //CG_ANW_VBOHANDLER_H
