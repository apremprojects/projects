cmake_minimum_required(VERSION 3.8.2)

project(obj_file)

# Find includes in the build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Find the QtWidgets library
find_package(gflags REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_library(obj_reader obj_reader.h obj_reader.cpp)
target_include_directories(obj_reader PRIVATE .)
target_link_libraries(obj_reader Eigen3::Eigen)

add_library(obj_data_gl obj_data_gl.h obj_data_gl.cpp)
target_include_directories(obj_data_gl PRIVATE .)
target_link_libraries(obj_data_gl obj_reader helper_opengl)

add_executable(obj_reader_test obj_reader_test.cpp)
target_include_directories(obj_reader_test PRIVATE .)
target_link_libraries(obj_reader_test obj_reader Eigen3::Eigen)
