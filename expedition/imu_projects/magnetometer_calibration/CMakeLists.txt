cmake_minimum_required(VERSION 3.8.2)

project(magnetometer_calibration)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS Widgets SerialPort)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(magnetometer_calibration
               magnetometer_widget.h 
							 magnetometer_widget.cpp
							 main_dlg.ui
	             main_dlg.h
	             main_dlg.cpp
	             work_thread.h
	             work_thread.cpp
               main.cpp)
target_include_directories(magnetometer_calibration PRIVATE .)
target_link_libraries(magnetometer_calibration Qt5::Widgets Qt5::SerialPort helper_opengl)
