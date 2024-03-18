cmake_minimum_required(VERSION 3.8.2)

project(quaternion_demo)

# Find includes in the build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Find the QtWidgets library
find_package(gflags REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(quaternion_test quaternion.h quaternion_test.cpp)
target_include_directories(quaternion_test PRIVATE .)
target_link_libraries(quaternion_test Eigen3::Eigen)

add_executable(quaternion_demo
	       			 widget_gl.h 
	       			 widget_gl.cpp
	       			 main_dlg.ui
	       			 main_dlg.h
	       			 main_dlg.cpp
         			 main.cpp)
target_include_directories(quaternion_demo PRIVATE .)
target_link_libraries(quaternion_demo 
		      						helper_opengl 
		      						Eigen3::Eigen
		      						Qt5::Widgets)
