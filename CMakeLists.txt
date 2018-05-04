cmake_minimum_required(VERSION 3.6)
project(Cg_Anw)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

if (UNIX AND NOT CYGWIN)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(GLUT REQUIRED)

    set(GTEST_PATH ${PROJECT_SOURCE_DIR}/libs/googletest-release-1.8.0-mingw)
    set(GTEST_INCLUDE_DIRS ${GTEST_PATH}/include)

    set(GTEST_LIBRARIES gtest)
elseif (WIN32)
    find_package(OPENGL REQUIRED)

    set(GLFW_PATH ${PROJECT_SOURCE_DIR}/libs/glfw-3.1.2.bin.WIN64/)
    set(GLEW_PATH ${PROJECT_SOURCE_DIR}/libs/glew-2.1.0)
    set(GLM_PATH ${PROJECT_SOURCE_DIR}/libs/glm-0.9.7.2)
    set(GLUT_PATH ${PROJECT_SOURCE_DIR}/libs/freeglut-3.0.0)
    set(GTEST_PATH ${PROJECT_SOURCE_DIR}/libs/googletest-release-1.8.0-mingw)

    set(GLFW_INCLUDE_DIRS ${GLFW_PATH}/include)
    set(GLEW_INCLUDE_DIRS ${GLEW_PATH}/include)
    set(GLM_INCLUDE_DIRS ${GLM_PATH})
    set(GLUT_INCLUDE_DIRS ${GLUT_PATH}/include)
    set(GTEST_INCLUDE_DIRS ${GTEST_PATH}/include)

    set(GLFW_STATIC_LIBRARIES ${GLFW_PATH}/lib-mingw-w64/glfw3.dll)
    set(GLEW_LIBRARIES ${GLEW_PATH}/lib/glew32.dll)
    set(GLUT_LIBRARIES ${GLUT_PATH}/lib/libfreeglut.dll winmm gdi32)
    set(GTEST_LIBRARIES ${GTEST_PATH}/lib/libgmock.dll)

else ()
    message("UNSUPPORTED OS")
endif ()

include_directories(include ${GLEW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})

file(GLOB SOURCE_FILES
        src/*.cpp)

file(GLOB INCLUDE_FILES
        include/*.h
        include/*.hpp)

file(GLOB TEST_FILES
        test/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})

SET( CMAKE_EXE_LINKER_FLAGS  ${CMAKE_EXE_LINKER_FLAGS} "-static -static-libgcc -static-libstdc++" )

add_executable(${PROJECT_NAME}_TEST ${TEST_FILES} src/HalfEdge.cpp src/SimpleObjParser.cpp ${GTEST_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GTEST_LIBRARIES})

target_link_libraries(${PROJECT_NAME}_TEST ${GTEST_LIBRARIES})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/shader
        ${CMAKE_CURRENT_BINARY_DIR}/shader
)
if (WIN32)
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${GLEW_PATH}/lib/glew32.dll
            ${GLUT_PATH}/lib/libfreeglut.dll
            ${GTEST_PATH}/lib/libgmock.dll
            ${CMAKE_CURRENT_BINARY_DIR}/
    )

    add_custom_command(
            TARGET ${PROJECT_NAME}_TEST POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${GTEST_PATH}/lib/libgmock.dll
            ${CMAKE_CURRENT_BINARY_DIR}/
    )
endif ()