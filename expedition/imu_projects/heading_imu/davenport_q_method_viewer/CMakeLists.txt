cmake_minimum_required(VERSION 3.8.2)

project(davenport_q_method_viewer)

# Find includes in the build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(gflags REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Qt5 COMPONENTS Widgets SerialPort REQUIRED)

add_executable(davenport_q_method_viewer
	       widget_gl.h 
	       widget_gl.cpp
	       main_dlg.ui
	       main_dlg.h
	       main_dlg.cpp
	       work_thread.h
	       work_thread.cpp
         main.cpp)
target_include_directories(davenport_q_method_viewer PRIVATE .)
target_link_libraries(davenport_q_method_viewer 
			obj_data_gl 
			obj_reader 
			helper_opengl  
			Eigen3::Eigen 
			Qt5::Widgets 
			Qt5::SerialPort)
