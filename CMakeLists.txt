cmake_minimum_required(VERSION 3.6)
project(Cg_Anw)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

if (UNIX AND NOT CYGWIN)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(GLUT REQUIRED)
elseif (WIN32)
    add_definitions(-D GLEW_STATIC)
    add_definitions(-D FREEGLUT_STATIC)
    find_package(OPENGL REQUIRED)

    set(GLFW_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/libs/glfw-3.1.2.bin.WIN64/include)
    set(GLEW_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/libs/glew-2.1.0/include)
    set(GLM_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/libs/glm-0.9.7.2)
    set(GLUT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/libs/freeglut-3.0.0-mingw/include)

    set(GLFW_STATIC_LIBRARIES ${PROJECT_SOURCE_DIR}/libs/glfw-3.1.2.bin.WIN64/lib-mingw-w64/libglfw3.a)
    set(GLEW_LIBRARIES ${PROJECT_SOURCE_DIR}/libs/glew-2.1.0/lib/libglew32.a)
    set(GLUT_LIBRARIES ${PROJECT_SOURCE_DIR}/libs/freeglut-3.0.0-mingw/lib/libfreeglut_static.a winmm gdi32)

else ()
    message("UNSUPPORTED OS")
endif ()

include_directories(include ${GLEW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp src/VerticesWrapper.cpp)
set(INCLUDE_FILES include/GLSLProgram.h include/VerticesWrapper.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})

target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/shader
        ${CMAKE_CURRENT_BINARY_DIR}/shader
)


