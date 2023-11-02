cmake_minimum_required(VERSION 3.8.2)

project(velocity_pid_test)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Qt5 REQUIRED COMPONENTS Core SerialPort)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(velocity_pid_test main.cpp)
target_link_libraries(velocity_pid_test Qt5::Core Qt5::SerialPort)