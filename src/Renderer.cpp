//
// Created by Johannes on 28.05.2018.
//


#define GLEW_STATIC
#include "glew.h"

#include "Renderer.h"

#include <libs/glm-0.9.7.2/glm/glm.hpp>
#include <include/utils.h>
#include <c++/map>
#include <include/Shader.h>


using namespace glm;


Renderer::Renderer() {
    gridPaneColor = vec3(0, 0, 0);
    selectedPointsColor = vec3(1, 0, 0);
    pointsColor = vec3(0.1, 0.2, 0.15);
    gridColor = vec3(1, 1, 1);
    faceColor = vec3(0.25, 0.4, 0.3);

    drawGrid = true;
}

void Renderer::initRenderer(Shader &shader, char* model_path){
    trig.LoadFile(model_path);
    this->shader = shader;
    initMesh();
    initGrid();
    initGridPane();
}



void Renderer::render() {
    renderObject(meshPoints, GL_POINTS);
    renderObject(mesh, GL_TRIANGLES);

    if (drawGrid) {
        renderObject(grid, GL_LINES);
        renderObject(gridPane, GL_TRIANGLES);
    }

}


void Renderer::renderObject(Object &object, int gl_draw_type) {


    //compute model-matrix of object
    GLint modelMatrix_location = glGetUniformLocation(shader.ID(), "modelMatrix");
    glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, &object.model[0][0]);


    glBindVertexArray(object.vaoID);
    glDrawArrays(gl_draw_type, 0, object.vertices.size());



}

void Renderer::initGrid() {
    glm::vec3 color = gridColor;
    float grid_lenght = 200;//dimensions.getGridSize();
    glm::vec3 gridPosition = glm::vec3(0.0f, 0.0f, 0.0f); //dimensions.getGridPosition();

    float stepsize = grid_lenght / 50;
    float i;
    for (i = -grid_lenght; i <= grid_lenght; i += stepsize) {
        if (i <= 0) {
            grid.vertices.push_back(glm::vec3(gridPosition.x - grid_lenght / 2, gridPosition.y,
                                              gridPosition.z + (i + grid_lenght / 2)));
            grid.vertices.push_back(glm::vec3(gridPosition.x + grid_lenght / 2, gridPosition.y,
                                              gridPosition.z + (i + grid_lenght / 2)));

            grid.colors.push_back(color);
            grid.colors.push_back(color);

            grid.radius.push_back(0);
            grid.radius.push_back(0);


        }

        if (i >= 0) {

            //glRotatef(-90, 0, 1, 0);

            glm::vec3 vert1 = glm::vec3((gridPosition.x - grid_lenght / 2), gridPosition.y, gridPosition.z);
            glm::vec3 vert2 = glm::vec3((gridPosition.x + grid_lenght / 2), gridPosition.y, gridPosition.z);

            //1,5708
            GLfloat tempX = vert1.x;
            vert1.x = (cos(1.5708) * vert1.x + sin(1.5708) * vert1.z) + (i - grid_lenght / 2);
            vert1.z = (-sin(1.5708) * tempX + cos(1.5708) * vert1.z);

            tempX = vert2.x;
            vert2.x = (cos(1.5708) * vert2.x + sin(1.5708) * vert2.z) + (i - grid_lenght / 2);
            vert2.z = -sin(1.5708) * tempX + cos(1.5708) * vert2.z;

            grid.vertices.push_back(vert1);
            grid.vertices.push_back(vert2);

            grid.colors.push_back(color);
            grid.colors.push_back(color);

            grid.radius.push_back(0);
            grid.radius.push_back(0);


        }

    }

    setup_vertex_position_buffer_object(grid);
    setup_vertex_color_buffer_object(grid);
    //setup_vertex_normal_buffer_object_tri(grid, true);
    setup_vao(grid);



}

void Renderer::initGridPane() {
    float grid_lenght = 200;//dimensions.getGridSize();
    glm::vec3 gridPosition = glm::vec3(0, 0, 0); //dimensions.getGridPosition();



    gridPane.vertices.push_back(
            vec3(gridPosition.x - grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z - grid_lenght / 2));
    gridPane.vertices.push_back(
            vec3(gridPosition.x - grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z + grid_lenght / 2));
    gridPane.vertices.push_back(
            vec3(gridPosition.x + grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z + grid_lenght / 2));

    gridPane.vertices.push_back(
            vec3(gridPosition.x + grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z + grid_lenght / 2));
    gridPane.vertices.push_back(
            vec3(gridPosition.x + grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z - grid_lenght / 2));
    gridPane.vertices.push_back(
            vec3(gridPosition.x - grid_lenght / 2, gridPosition.y - 0.1, gridPosition.z - grid_lenght / 2));

    gridPane.colors.push_back(gridPaneColor);
    gridPane.colors.push_back(gridPaneColor);
    gridPane.colors.push_back(gridPaneColor);
    gridPane.colors.push_back(gridPaneColor);
    gridPane.colors.push_back(gridPaneColor);
    gridPane.colors.push_back(gridPaneColor);

    gridPane.radius.push_back(0);
    gridPane.radius.push_back(0);
    gridPane.radius.push_back(0);
    gridPane.radius.push_back(0);
    gridPane.radius.push_back(0);
    gridPane.radius.push_back(0);






    setup_vertex_position_buffer_object(gridPane);
    setup_vertex_color_buffer_object(gridPane);
    setup_vertex_normal_buffer_object_tri(gridPane, true);
    setup_vao(gridPane);
}


void Renderer::initMesh() {


    mesh.vertices = trig.Vertices();
    for (glm::vec3 vert : mesh.vertices) {
        mesh.colors.push_back(faceColor);
        mesh.radius.push_back(0);


    }

    setup_vertex_position_buffer_object(mesh);
    setup_vertex_color_buffer_object(mesh);
    setup_vertex_normal_buffer_object_tri(mesh, true);
    setup_vao(mesh);

    meshPoints.vertices = trig.Vertices();
    for (glm::vec3 vert : meshPoints.vertices) {
        meshPoints.colors.push_back(pointsColor);
        meshPoints.radius.push_back(50.0);
    }

    setup_vertex_position_buffer_object(meshPoints);
    setup_vertex_color_buffer_object(meshPoints);
    setup_vertex_normal_buffer_object_tri(meshPoints, true);
    setup_vertex_radius_buffer_object(meshPoints);
    setup_vao(meshPoints);

}

void Renderer::setup_vao(Object &object) {



    glGenVertexArrays(1, &object.vaoID);
    glBindVertexArray(object.vaoID);


    // bind vertex positions to shader
    GLint position_location = glGetAttribLocation(shader.ID(), "vertex_position");
    if (position_location != -1) {
        glEnableVertexAttribArray(position_location);
        glBindBuffer(GL_ARRAY_BUFFER, object.vertex_position_buffer);
        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    // bind vertex normals to shader
    GLint normal_location = glGetAttribLocation(shader.ID(), "vertex_normal");
    if (normal_location != -1) {
        glEnableVertexAttribArray(normal_location);
        glBindBuffer(GL_ARRAY_BUFFER, object.vertex_normal_buffer);
        glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    // bind color to shader
    GLint color_location = glGetAttribLocation(shader.ID(), "v_color");
    if (color_location != -1) {
        glEnableVertexAttribArray(color_location);
        glBindBuffer(GL_ARRAY_BUFFER, object.vertex_color_buffer);
        glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // bind radius to shader
    GLint radius_location = glGetAttribLocation(shader.ID(), "radius_attr");
    if (radius_location != -1) {
        glEnableVertexAttribArray(radius_location);
        glBindBuffer(GL_ARRAY_BUFFER, object.vertex_radius_buffer);
        glVertexAttribPointer(radius_location, 1, GL_FLOAT, GL_FALSE, 0, 0);
    }

    glBindVertexArray(0);
}


void Renderer::setup_vertex_position_buffer_object(Object &object) {
    glGenBuffers(1, &object.vertex_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, object.vertex_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.vertices.size(),
                 &object.vertices.at(0), GL_STATIC_DRAW);
}

void Renderer::setup_vertex_color_buffer_object(Object &object) {
    glGenBuffers(1, &object.vertex_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, object.vertex_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.colors.size(),
                 &object.colors.at(0), GL_STATIC_DRAW);
}

void Renderer::setup_vertex_radius_buffer_object(Object &object) {
    glGenBuffers(1, &object.vertex_radius_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, object.vertex_radius_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * object.radius.size(),
                 &object.radius.at(0), GL_STATIC_DRAW);
}

void Renderer::setup_vertex_normal_buffer_object_tri(Object &object, bool smoothed) {

    std::vector<glm::vec3> normals;
    if (smoothed) {
        // initialize map of normals to zero
        // note that we store readily hashable vector<double> types instead of
        // vec3s and convert between the two as required
        std::map<std::vector<double>, std::vector<double> > normal_map;
        for (int i = 0; i < object.vertices.size(); i++) {
            std::vector<double> zeros;
            zeros.push_back(0.0);
            zeros.push_back(0.0);
            zeros.push_back(0.0);
            normal_map[to_vector(object.vertices[i])] = zeros;
        }
        for (int i = 0; i < object.vertices.size(); i += 3) {
            // get vertices of the current triangle
            glm::vec3 v1 = object.vertices[i];
            glm::vec3 v2 = object.vertices[i + 1];
            glm::vec3 v3 = object.vertices[i + 2];
            std::vector<double> v1_key = to_vector(v1);
            std::vector<double> v2_key = to_vector(v2);
            std::vector<double> v3_key = to_vector(v3);
            // compute face normal
            glm::vec3 face_normal = glm::cross(v3 - v2, v1 - v2);
            // get the old vertex normal
            glm::vec3 v1_old = to_vec3(normal_map[v1_key]);
            glm::vec3 v2_old = to_vec3(normal_map[v2_key]);
            glm::vec3 v3_old = to_vec3(normal_map[v3_key]);
            // replace the old value with the new value
            normal_map.erase(v1_key);
            normal_map.erase(v2_key);
            normal_map.erase(v3_key);
            normal_map[v1_key] = to_vector(glm::normalize(v1_old + face_normal));
            normal_map[v2_key] = to_vector(glm::normalize(v2_old + face_normal));
            normal_map[v3_key] = to_vector(glm::normalize(v3_old + face_normal));
        }
        // convert the map of normals to a vector of normals
        for (int i = 0; i < object.vertices.size(); i++) {
            normals.push_back(to_vec3(normal_map[to_vector(object.vertices[i])]));
        }
    } else {
        for (int i = 0; i < object.vertices.size(); i += 3) {
            // get vertices of this triangle
            glm::vec3 v1 = object.vertices[i];
            glm::vec3 v2 = object.vertices[i + 1];
            glm::vec3 v3 = object.vertices[i + 2];
            // compute face normal
            glm::vec3 face_normal = glm::cross(v3 - v2, v1 - v2);
            normals.push_back(glm::normalize(face_normal));
            normals.push_back(glm::normalize(face_normal));
            normals.push_back(glm::normalize(face_normal));
        }
    }
    glGenBuffers(1, &object.vertex_normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, object.vertex_normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
                 &normals[0], GL_STATIC_DRAW);
}


void Renderer::select(glm::vec3 pos){
    bool found = false;
    int i = 0;
    for(glm::vec3 vert : meshPoints.vertices){{
        if(glm::distance(vert, pos) < 1){
            meshPoints.colors.at(i) = selectedPointsColor;
            found = true;
        }
        i++;
    }
    if(found == true){
        glBindBuffer(GL_ARRAY_BUFFER, meshPoints.vertex_color_buffer);
        glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(glm::vec3) * meshPoints.colors.size(), &meshPoints.colors.at(0));


    }


    }
}

bool Renderer::isDrawGrid() const {
    return drawGrid;
}

void Renderer::setDrawGrid(bool drawGrid) {
    Renderer::drawGrid = drawGrid;
}

