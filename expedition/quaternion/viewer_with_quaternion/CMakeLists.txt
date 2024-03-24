cmake_minimum_required(VERSION 3.8.2)

project(viewer_with_quaternion)

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
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(viewer_with_quaternion
	       			 widget_gl.h 
	       			 widget_gl.cpp
	       			 main_dlg.ui
	       			 main_dlg.h
	       			 main_dlg.cpp
               main.cpp)
target_include_directories(viewer_with_quaternion PRIVATE .)
target_link_libraries(viewer_with_quaternion 
		      						obj_data_gl 
		      						obj_reader 
		      						helper_opengl 
		      						Eigen3::Eigen
		      						Qt5::Widgets)
