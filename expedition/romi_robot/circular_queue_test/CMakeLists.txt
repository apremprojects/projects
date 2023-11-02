cmake_minimum_required(VERSION 3.8.2)

project(circular_queue_test)

set(CMAKE_CXX_STANDARD 14)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(glog REQUIRED)
find_package(gflags REQUIRED)

add_executable(circular_queue_test circular_queue.h circular_queue_test.cpp)
target_link_libraries(circular_queue_test glog::glog)
