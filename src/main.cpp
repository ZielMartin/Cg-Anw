#include <iostream>
#include <vector>
#include <GL/glew.h> // Before any gl headers
#include <GL/gl.h>
#include <GL/glu.h> // Always after gl.h
#include <GLFW/glfw3.h> // When all gl-headers have been included

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "GLSLProgram.h"

// Standard window width
#define WINDOW_WIDTH 640

// Standard window height
#define WINDOW_HEIGHT 480

cg::GLSLProgram program;

glm::mat4x4 view;
glm::mat4x4 projection;

float zNear = 0.1f;
float zFar = 100.0f;
int globalN = 3;

/*
 Simple struct to hold stuff for the rendering together.
*/
struct Object
{
    /* IDs for several buffers. */
    GLuint vao;

    GLuint positionBuffer;
    GLuint colorBuffer;

    GLuint indexBuffer;

    /* Model matrix */
    glm::mat4x4 model;
};

Object triangle;
Object quad;
Object circle;
Object ngon;
Object cube;

void renderCube() {

    // Create mvp.
    glm::mat4x4 mvp = projection * view * cube.model;

    // Bind the shader program and set uniform(s).
    program.use();
    program.setUniform("mvp", mvp);

    // Bind vertex array object so we can render the 1 triangle.
    glBindVertexArray(cube.vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

}

void initNGon(int n) {
    // Construct triangle. These vectors can go out of scope after we have send all data to the graphics card.
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;// = { { 1.0f, 0.0f, 0.0f } };
    std::vector<GLushort> indices;// = { 0, 1, 2, 3,4,5,6,7,8 };

    GLuint programId = program.getHandle();
    GLuint pos;


    for (int i = 0; i < n; i++) {
        float num = i * 3.14 * 2 / n;
        vertices.push_back({ cos(num), sin(num), 0.0f });
        colors.push_back({ 1.0f, 0.0f, 0.0f });
        indices.push_back(i);
    }


    // Step 0: Create vertex array object.
    glGenVertexArrays(1, &ngon.vao);
    glBindVertexArray(ngon.vao);

    // Step 1: Create vertex buffer object for position attribute and bind it to the associated "shader attribute".
    glGenBuffers(1, &ngon.positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ngon.positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Bind it to position.
    pos = glGetAttribLocation(programId, "position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 2: Create vertex buffer object for color attribute and bind it to...
    glGenBuffers(1, &ngon.colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ngon.colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

    // Bind it to color.
    pos = glGetAttribLocation(programId, "color");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 3: Create vertex buffer object for indices. No binding needed here.
    glGenBuffers(1, &ngon.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ngon.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

    // Unbind vertex array object (back to default).
    glBindVertexArray(0);

    // Modify model matrix.
    ngon.model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.25f, -1.25f, 0.0f));
}

void initCube() {
    // Construct triangle. These vectors can go out of scope after we have send all data to the graphics card.
    std::vector<glm::vec3> vertices = {
            { 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0 },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 0.0f },	//front
            { 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0 },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f }, //back
            { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0 },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 1.0f }, //left
            { 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 1.0 },{ 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f }, //right
            { 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 1.0 },{ 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f }, //up
            { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0 },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 1.0f } //down
    };
    std::vector<glm::vec3> colors = {
            { 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },
            { 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },
            { 1.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 1.0f }
    };
    std::vector<GLushort> indices = {
            0,2,3, 0,3,1,	//front
            5,7,6, 5,6,4,	//back
            9,11,10, 9,10,8,	//left
            12,14,15, 12,15,13,	//right
            16,17,19, 16,19,18,	//up
            21,20,22, 21,22,23	//down
    };

    GLuint programId = program.getHandle();
    GLuint pos;


    // Step 0: Create vertex array object.
    glGenVertexArrays(1, &cube.vao);
    glBindVertexArray(cube.vao);

    // Step 1: Create vertex buffer object for position attribute and bind it to the associated "shader attribute".
    glGenBuffers(1, &cube.positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cube.positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Bind it to position.
    pos = glGetAttribLocation(programId, "position");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 2: Create vertex buffer object for color attribute and bind it to...
    glGenBuffers(1, &cube.colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cube.colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

    // Bind it to color.
    pos = glGetAttribLocation(programId, "color");
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Step 3: Create vertex buffer object for indices. No binding needed here.
    glGenBuffers(1, &cube.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

    // Unbind vertex array object (back to default).
    glBindVertexArray(0);

    // Modify model matrix.
    cube.model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.25f, 0.25f, 0.0f));
}

/*
 Initialization. Should return true if everything is ok and false if something went wrong.
 */
bool init()
{
    // OpenGL stuff. Set "background" color and enable depth testing.
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Construct view matrix.
    glm::vec3 eye(0.0f, 0.0f, 4.0f);
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    view = glm::lookAt(eye, center, up);

    // Create a shader program and set light direction.
    if (!program.compileShaderFromFile("shader/simple.vert", cg::GLSLShader::VERTEX))
    {
        std::cerr << program.log();
        return false;
    }

    if (!program.compileShaderFromFile("shader/simple.frag", cg::GLSLShader::FRAGMENT))
    {
        std::cerr << program.log();
        return false;
    }

    if (!program.link())
    {
        std::cerr << program.log();
        return false;
    }

    // Create objects.
    initCube();
    glEnable(GL_CULL_FACE);

    return true;
}

/*
 Release object resources.
*/
void releaseObject(Object& obj)
{
    glDeleteVertexArrays(1, &obj.vao);
    glDeleteBuffers(1, &obj.indexBuffer);
    glDeleteBuffers(1, &obj.colorBuffer);
    glDeleteBuffers(1, &obj.positionBuffer);
}

/*
 Release resources on termination.
 */
void release()
{
    // Shader program will be released upon program termination.
    releaseObject(triangle);
    releaseObject(quad);
    releaseObject(circle);
    releaseObject(ngon);
    releaseObject(cube);
}

/*
 Rendering.
 */
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    renderCube();
}

/*
 Resize callback.
 */
void resize(GLFWwindow* window, int width, int height)
{
    // Division by zero is bad... mkay!
    height = height < 1 ? 1 : height;
    glViewport(0, 0, width, height);

    // Construct projection matrix.
    projection = glm::perspective(45.0f, (float) width / height, zNear, zFar);
}

/*
 Callback for char input.
 */
void keyboard(GLFWwindow* window, unsigned int codepoint)
{
    switch (codepoint)
    {
        case '+':
            // do something
            if (globalN < 30)
                globalN++;
            initNGon(globalN);
            break;
        case '-':
            // do something
            if (globalN > 3)
                globalN--;
            initNGon(globalN);
            break;
        case 'x':
            // do something
            cube.model = glm::rotate(cube.model, .05f, glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case 'y':
            // do something
            cube.model = glm::rotate(cube.model, .05f, glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case 'z':
            // do something
            cube.model = glm::rotate(cube.model, .05f, glm::vec3(0.0f, 0.0f, 1.0f));
            break;
    }
}

int main(int argc, char** argv)
{
    GLFWwindow* window;

    // Initialize glfw library (window toolkit).
    if (!glfwInit())
        return -1;

    // Create a window and opengl context (version 3.3 core profile).
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(	WINDOW_WIDTH, WINDOW_HEIGHT, "Cg_Anw", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return -2;
    }

    // Make the window's opengl context current.
    glfwMakeContextCurrent(window);

    // Load opengl extensions
    glewExperimental = GL_TRUE;
    GLenum result = glewInit();

    if (result != GLEW_OK)
    {
        glfwTerminate();
        return -3;
    }

    // Set callbacks for resize and keyboard events.
    glfwSetWindowSizeCallback(window, resize);
    glfwSetCharCallback(window, keyboard);

    if (!init())
    {
        release();
        glfwTerminate();

        return -4;
    }

    // We have to call resize once for a proper setup.
    resize(window, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Loop until the user closes the window.
    while (!glfwWindowShouldClose(window))
    {
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up everything on termination.
    release();
    glfwTerminate();

    return 0;
}
