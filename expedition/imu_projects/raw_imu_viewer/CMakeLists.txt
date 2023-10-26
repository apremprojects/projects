cmake_minimum_required(VERSION 3.8.2)

project(raw_imu_viewer)

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

add_executable(raw_imu_viewer magnetometer_widget.h magnetometer_widget.cpp main_dlg.ui main_dlg.h main_dlg.cpp work_thread.h work_thread.cpp graph_widget.h graph_widget.cpp main.cpp)
target_link_libraries(raw_imu_viewer Qt5::Widgets Qt5::SerialPort helper_opengl graph_image)
