//
// Created by Johannes on 28.05.2018.
//


#define GLEW_STATIC

#include "GL/glew.h"

#include "Renderer.h"

#include <glm/glm.hpp>
#include <include/utils.h>
#include <map>
#include <Shader.h>
#include <glm/gtc/matrix_transform.hpp>


using namespace glm;


Renderer::Renderer() {
    gridPaneColor = vec3(0, 0, 0);
    selectedColor = vec3(1, 0, 0);
    pointsColor = vec3(0.1, 0.2, 0.15);
    gridColor = vec3(1, 1, 1);
    faceColor = vec3(0.25, 0.4, 0.3);
    meshLineColor = vec3(0.2, 0.1, 0.1);
    grid_lenght = 10;
    pointSize = 30;

    renderGrid = true;
    renderPoints = true;
}

void Renderer::initRenderer(Shader &shader, char *model_path) {
    meshWrapper.loadMesh(model_path);

    this->shader = shader;
    initMesh();
    initGrid();
    initGridPane();
}


void Renderer::render() {
    renderObject(meshObject, GL_TRIANGLES);
    renderObject(meshLinesObject, GL_LINES);

    if (renderPoints) {
        renderObject(meshPointsObject, GL_POINTS);
    }

    if (renderGrid) {
        renderObject(gridObject, GL_LINES);
        renderObject(gridPaneObject, GL_TRIANGLES);
    }

}


void Renderer::renderObject(Object &object, int gl_draw_type) {

    //compute model-matrix of object
    GLint modelMatrix_location = glGetUniformLocation(shader.ID(), "modelMatrix");
    glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, &object.model[0][0]);


    glBindVertexArray(object.vaoID);

    if (object.indices.empty()) {
        glDrawArrays(gl_draw_type, 0, object.vertices.size());
    } else {
        glDrawElements(gl_draw_type, object.indices.size(), GL_UNSIGNED_INT, NULL);
    }


}

void Renderer::initGrid() {
    glm::vec3 color = gridColor;
    glm::vec3 gridPosition = glm::vec3(0.0f, 0.0f, 0.0f); //dimensions.getGridPosition();

    float stepsize = grid_lenght / 50;
    float i;
    for (i = -grid_lenght; i <= grid_lenght; i += stepsize) {
        if (i <= 0) {
            gridObject.vertices.push_back(glm::vec3(gridPosition.x - grid_lenght / 2, gridPosition.y,
                                                    gridPosition.z + (i + grid_lenght / 2)));
            gridObject.vertices.push_back(glm::vec3(gridPosition.x + grid_lenght / 2, gridPosition.y,
                                                    gridPosition.z + (i + grid_lenght / 2)));

            gridObject.colors.push_back(color);
            gridObject.colors.push_back(color);

            gridObject.radius.push_back(0);
            gridObject.radius.push_back(0);
        }

        if (i >= 0) {

            glm::vec3 vert1 = glm::vec3((gridPosition.x - grid_lenght / 2), gridPosition.y, gridPosition.z);
            glm::vec3 vert2 = glm::vec3((gridPosition.x + grid_lenght / 2), gridPosition.y, gridPosition.z);

            //rotation
            GLfloat tempX = vert1.x;
            vert1.x = (cos(1.5708) * vert1.x + sin(1.5708) * vert1.z) + (i - grid_lenght / 2);
            vert1.z = (-sin(1.5708) * tempX + cos(1.5708) * vert1.z);

            tempX = vert2.x;
            vert2.x = (cos(1.5708) * vert2.x + sin(1.5708) * vert2.z) + (i - grid_lenght / 2);
            vert2.z = -sin(1.5708) * tempX + cos(1.5708) * vert2.z;

            gridObject.vertices.push_back(vert1);
            gridObject.vertices.push_back(vert2);

            gridObject.colors.push_back(color);
            gridObject.colors.push_back(color);

            gridObject.radius.push_back(0);
            gridObject.radius.push_back(0);


        }

    }

    setup_vao(gridObject);


}

void Renderer::initGridPane() {
    glm::vec3 gridPosition = glm::vec3(0, -0.001, 0); //dimensions.getGridPosition();



    gridPaneObject.vertices.push_back(
            vec3(gridPosition.x - grid_lenght / 2, gridPosition.y, gridPosition.z - grid_lenght / 2));
    gridPaneObject.vertices.push_back(
            vec3(gridPosition.x - grid_lenght / 2, gridPosition.y, gridPosition.z + grid_lenght / 2));

    gridPaneObject.vertices.push_back(
            vec3(gridPosition.x + grid_lenght / 2, gridPosition.y, gridPosition.z + grid_lenght / 2));

    gridPaneObject.vertices.push_back(
            vec3(gridPosition.x + grid_lenght / 2, gridPosition.y, gridPosition.z - grid_lenght / 2));


    gridPaneObject.colors.push_back(gridPaneColor);
    gridPaneObject.colors.push_back(gridPaneColor);
    gridPaneObject.colors.push_back(gridPaneColor);
    gridPaneObject.colors.push_back(gridPaneColor);


    gridPaneObject.radius.push_back(0);
    gridPaneObject.radius.push_back(0);
    gridPaneObject.radius.push_back(0);
    gridPaneObject.radius.push_back(0);


    gridPaneObject.indices = {0, 1, 2, 2, 3, 0};


    setup_vao(gridPaneObject);
}


void Renderer::initMesh() {


    fillMeshData(true);

    setup_vao(meshObject);
    setup_vao(meshLinesObject);
    setup_vao(meshPointsObject);


}

void Renderer::setup_vao(Object &object) {


    glGenVertexArrays(1, &object.vaoID);
    glBindVertexArray(object.vaoID);

    if (!object.vertices.empty()) {
        setupBufferData(object.vertex_position_buffer, object.vertices);
        // bind vertex positions to shader
        uint32_t position_location = glGetAttribLocation(shader.ID(), "vertex_position");
        bindBufferToShader(object.vertex_position_buffer, position_location, 3);
    }
    if (!object.colors.empty()) {
        setupBufferData(object.vertex_color_buffer, object.colors);
        // bind color to shader
        uint32_t color_location = glGetAttribLocation(shader.ID(), "v_color");
        bindBufferToShader(object.vertex_color_buffer, color_location, 3);
    }
    if (!object.indices.empty()) {
        setupBufferData(object.vertex_index_buffer, object.indices);
        // bind indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.vertex_index_buffer);
    }
    if (!object.normals.empty()) {
        setupBufferData(object.vertex_normal_buffer, object.normals);
        // bind vertex normals to shader
        uint32_t normal_location = glGetAttribLocation(shader.ID(), "vertex_normal");
        bindBufferToShader(object.vertex_normal_buffer, normal_location, 3);
    }
    if (!object.radius.empty()) {
        setupBufferData(object.vertex_radius_buffer, object.radius);
        // bind radius to shader
        uint32_t radius_location = glGetAttribLocation(shader.ID(), "radius_attr");
        bindBufferToShader(object.vertex_radius_buffer, radius_location, 1);
    }

    glBindVertexArray(0);
}

void Renderer::bindBufferToShader(uint32_t &bufferID, uint32_t &location, int size) {
    if (location != -1) {
        glEnableVertexAttribArray(location);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, 0, 0);
    }
}


template<typename T>
void Renderer::setupBufferData(uint32 &bufferID, std::vector<T> &data) {
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(),
                 &data.at(0), GL_DYNAMIC_DRAW);
}

template<typename T>
void Renderer::updateBufferData(uint32 &bufferID, std::vector<T> &data) {
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(T) * data.size(),
                    &data.at(0));
}


void Renderer::select(glm::vec3 pos) {
    meshWrapper.selectVertex(pos, 0.02);

    std::vector<glm::vec3> selected = meshWrapper.getSelectedVertices();

    int i = 0;
    for (glm::vec3 vert : meshPointsObject.vertices) {
        bool found = false;
        for (glm::vec3 selectedVert : selected) {
            if (vert.x - selectedVert.x == 0 && vert.y - selectedVert.y == 0 && vert.z - selectedVert.z == 0) {
                found = true;
            }
        }
        if (found) {
            meshPointsObject.colors.at(i) = selectedColor;
        } else {
            meshPointsObject.colors.at(i) = pointsColor;
        }
        i++;

    }
    updateBufferData(meshPointsObject.vertex_color_buffer, meshPointsObject.colors);
    updateMesh(false);

}

void Renderer::moveSelected(glm::vec3 relativeMovement) {
    meshWrapper.moveSelectedVertices(relativeMovement);

    updateMesh(false);

}


void Renderer::deleteSelectedVertices() {
    meshWrapper.deleteSelectedVertices();
    updateMesh(true);
}


/**
 * does not work if vertex-count of mesh is changed
 */
void Renderer::updateMesh(bool updatePointColor) {
    clearObject(meshObject);
    if (updatePointColor) {
        clearObject(meshPointsObject);
    } else {
        meshPointsObject.vertices.clear();
    }
    clearObject(meshLinesObject);


    fillMeshData(updatePointColor);


    updateBufferData(meshObject.vertex_position_buffer, meshObject.vertices);
    updateBufferData(meshObject.vertex_normal_buffer, meshObject.normals);
    updateBufferData(meshObject.vertex_color_buffer, meshObject.colors);
    updateBufferData(meshObject.vertex_radius_buffer, meshObject.radius);

    updateBufferData(meshPointsObject.vertex_position_buffer, meshPointsObject.vertices);
    updateBufferData(meshPointsObject.vertex_color_buffer, meshPointsObject.colors);
    updateBufferData(meshPointsObject.vertex_radius_buffer, meshPointsObject.radius);

    updateBufferData(meshLinesObject.vertex_position_buffer, meshLinesObject.vertices);
    updateBufferData(meshLinesObject.vertex_color_buffer, meshLinesObject.colors);
    updateBufferData(meshLinesObject.vertex_radius_buffer, meshLinesObject.radius);
}

void Renderer::fillMeshData(bool updatePointColor) {
    meshWrapper.getVerticesAndNormalsTriangulated(meshObject.vertices, meshObject.normals);
    meshWrapper.getVertices(meshPointsObject.vertices);

    std::vector<bool> selected;
    meshWrapper.getLineVertices(meshLinesObject.vertices, selected);


    for (vec3 vert : meshObject.vertices) {
        meshObject.colors.push_back(faceColor);
        meshObject.radius.push_back(0);
    }
    for (vec3 vert : meshPointsObject.vertices) {
        if (updatePointColor) {
            meshPointsObject.colors.push_back(pointsColor);
        }
        meshPointsObject.radius.push_back(pointSize);

    }
    for (int i = 0; i < meshLinesObject.vertices.size(); i++) {
        meshLinesObject.radius.push_back(0);
        if (selected.at(i)) {
            meshLinesObject.colors.push_back(selectedColor);
        } else {
            meshLinesObject.colors.push_back(meshLineColor);
        }
    }


}

void Renderer::clearObject(Object &object) {
    object.vertices.clear();
    object.normals.clear();
    object.colors.clear();
    object.radius.clear();
    object.indices.clear();
}

void Renderer::addVertex(glm::vec3 worldPos) {
    meshWrapper.addVertex(worldPos);

    meshWrapper.deselectAll();

    recreateMesh();

    select(worldPos);


}

void Renderer::addFace() {
    meshWrapper.makeSelectedFace();

    meshWrapper.deselectAll();

    recreateMesh();

}

void Renderer::subdivision() {
    meshWrapper.subdivision();
    meshWrapper.deselectAll();
    recreateMesh();
}

void Renderer::undo() {
    meshWrapper.undo();

    recreateMesh();


}

void Renderer::recreateMesh() {
    clearObject(meshObject);
    clearObject(meshPointsObject);
    clearObject(meshLinesObject);


    initMesh();
}

void Renderer::rotateMesh(glm::vec3 rotationVec, float angle) {
    meshObject.model = glm::rotate(meshObject.model, angle, rotationVec);
    meshPointsObject.model = glm::rotate(meshPointsObject.model, angle, rotationVec);
}


bool Renderer::isRenderGrid() const {
    return renderGrid;
}

void Renderer::setRenderGrid(bool drawGrid) {
    Renderer::renderGrid = drawGrid;
}

bool Renderer::isRenderPoints() const {
    return renderPoints;
}

void Renderer::setRenderPoints(bool renderPoints) {
    Renderer::renderPoints = renderPoints;
}



