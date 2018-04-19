cmake_minimum_required(VERSION 3.7)
project(Cg_Anw)

set(CMAKE_CXX_STANDARD 14)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)


find_package(Qt5Core)
# Find the QtWidgets library
find_package(Qt5Widgets)


get_target_property(QtCore_location Qt5::Core LOCATION)

include_directories(include ${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})




set(SOURCE_FILES src/logo.cpp src/glwidget.cpp src/window.cpp src/mainwindow.cpp)
set(INCLUDE_FILES include/logo.h include/glwidget.h include/mainwindow.h include/window.h)
add_executable(Cg_Anw main.cpp ${SOURCE_FILES} ${INCLUDE_FILES})

# Use the Widgets module from Qt 5.
target_link_libraries(Cg_Anw Qt5::Widgets)