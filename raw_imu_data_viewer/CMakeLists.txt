cmake_minimum_required(VERSION 3.8.2)

project(imu_raw_data_viewer)

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

add_executable(imu_raw_data_viewer 
	main.cpp
	main_dlg.h
	main_dlg.cpp
	work_thread.h
  work_thread.cpp)
target_link_libraries(imu_raw_data_viewer Eigen3::Eigen Qt5::Widgets Qt5::SerialPort)
