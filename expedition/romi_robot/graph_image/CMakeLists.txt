cmake_minimum_required(VERSION 3.8.2)

project(graph_image)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_library(graph_image graph_image.h graph_image.cpp)
target_link_libraries(graph_image Qt5::Widgets)

