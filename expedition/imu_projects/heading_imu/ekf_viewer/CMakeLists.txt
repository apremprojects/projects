cmake_minimum_required(VERSION 3.8.2)

project(ekf_viewer)

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

add_library(quaternion_ekf quaternion_ekf.h quaternion_ekf.cpp)
target_link_libraries(quaternion_ekf Eigen3::Eigen)

add_executable(ekf_quaternion_test ekf_quaternion_test.cpp)
target_link_libraries(ekf_quaternion_test Eigen3::Eigen quaternion_ekf)

add_executable(ekf_viewer
	       widget_gl.h 
	       widget_gl.cpp
	       main_dlg.ui
	       main_dlg.h
	       main_dlg.cpp
	       work_thread.h
	       work_thread.cpp
         main.cpp)
target_include_directories(ekf_viewer PRIVATE .)
target_link_libraries(ekf_viewer 
											obj_data_gl 
											obj_reader 
											helper_opengl 
											quaternion_ekf 
											Eigen3::Eigen 
											Qt5::Widgets 
											Qt5::SerialPort )
