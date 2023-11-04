cmake_minimum_required(VERSION 3.8.2)

project(romi_robot_app)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Find the Qtimages library
find_package(gflags REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Gamepad SerialPort)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(romi_robot_app 
	       work_thread.h 
	       work_thread.cpp
		   romi_robot.cpp
		   romi_robot.h
	       main.cpp)
target_link_libraries(romi_robot_app Qt5::Core Qt5::Gamepad Qt::SerialPort)
