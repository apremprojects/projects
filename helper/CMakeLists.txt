cmake_minimum_required(VERSION 3.8.2)

project(helper_opengl)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS OpenGL)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(glog REQUIRED)
find_package(gflags REQUIRED)

add_library(helper_opengl
    buffer.h 
    program.h 
    program.cpp 
    texture.h 
    texture.cpp)
target_link_libraries(helper_opengl glog::glog)
target_link_libraries(helper_opengl Eigen3::Eigen)
target_link_libraries(helper_opengl Qt5::OpenGL)

