cmake_minimum_required(VERSION 3.6)
project(Cg_Anw)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_VERBOSE_MAKEFILE TRUE)

if (UNIX AND NOT CYGWIN)
    set(CMAKE_CXX_FLAGS -fPIC)

    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(GLUT REQUIRED)

    set(GTEST_PATH ${PROJECT_SOURCE_DIR}/libs/googletest-release-1.8.0-mingw)
    set(GTEST_INCLUDE_DIRS ${GTEST_PATH}/include)

    set(OPENMESH_LIBRARIES "OpenMeshCore")
    link_directories("/usr/local/lib")


    set(GTEST_LIBRARIES gtest)
elseif (WIN32)
    find_package(OPENGL REQUIRED)

    set(GLEW_PATH ${PROJECT_SOURCE_DIR}/libs/glew-2.1.0)
    set(GLFW_PATH ${PROJECT_SOURCE_DIR}/libs/glfw-3.1.2.bin.WIN64/)
    set(GLM_PATH ${PROJECT_SOURCE_DIR}/libs/glm-0.9.7.2)
    set(GTEST_PATH ${PROJECT_SOURCE_DIR}/libs/googletest-release-1.8.0-mingw)
    set(OPENMESH_PATH ${PROJECT_SOURCE_DIR}/libs/OpenMesh)
    set(QT_PATH ${PROJECT_SOURCE_DIR}/libs/Qt)

    set(GLEW_INCLUDE_DIRS ${GLEW_PATH}/include)
    set(GLFW_INCLUDE_DIRS ${GLFW_PATH}/include)
    set(GLM_INCLUDE_DIRS ${GLM_PATH})
    set(GTEST_INCLUDE_DIRS ${GTEST_PATH}/include)
    set(OPENMESH_INCLUDE_DIRS ${OPENMESH_PATH}/include)

    #cmake link_libraries removes .dll and .dll.a ending, but not .dll.dll wtf
    set(GLEW_LIBRARIES "glew32.dll.dll")
    set(GLFW_STATIC_LIBRARIES "glfw3.dll.dll")
    set(GLUT_LIBRARIES "freeglut.dll.dll" winmm gdi32)
    set(GTEST_LIBRARIES "gmock.dll.dll")
    set(OPENMESH_LIBRARIES "OpenMeshCored.dll.dll")

    link_directories(${GLFW_PATH}/lib-mingw-w64 ${GLEW_PATH}/lib ${GTEST_PATH}/lib ${OPENMESH_PATH}/lib)

    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:\\Qt\\5.11.0\\mingw53_32\\")


else ()
    message("UNSUPPORTED OS")
endif ()

# Find the Qt-libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

get_target_property(QtCore_location Qt5::Core LOCATION)
include_directories(include ui ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS} ${OPENMESH_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB SOURCE_FILES
        src/*.cpp)

file(GLOB INCLUDE_FILES
        include/*.h
        include/*.hpp)

file(GLOB UI_FILES
        ui/*.ui
        ui/*.h)

file(GLOB RES_FILES
        res/*.qrc)

file(GLOB TEST_FILES
        test/*.cpp)

#SET(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static -static-libgcc -static-libstdc++")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${UI_FILES} ${INCLUDE_FILES})

add_executable(${PROJECT_NAME}_TEST ${TEST_FILES})

target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES} Qt5::Core Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} ${OPENMESH_LIBRARIES})

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
            ${GTEST_PATH}/lib/libgmock.dll
            ${QT_PATH}/Qt5Cored.dll
            ${QT_PATH}/Qt5Guid.dll
            ${QT_PATH}/Qt5OpenGLd.dll
            ${QT_PATH}/Qt5Widgetsd.dll
            ${OPENMESH_PATH}/lib/libOpenMeshCored.dll
            ${CMAKE_CURRENT_BINARY_DIR}/
    )

    add_custom_command(
            TARGET ${PROJECT_NAME}_TEST POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${GTEST_PATH}/lib/libgmock.dll
            ${CMAKE_CURRENT_BINARY_DIR}/
    )
endif ()
