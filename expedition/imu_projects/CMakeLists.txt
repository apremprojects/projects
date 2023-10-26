cmake_minimum_required(VERSION 3.8.2)

project(imu_projects)

#adding helper and graph_image as libraries
add_subdirectory(./../../helper helper)
add_subdirectory(./graph_image graph_image)

add_subdirectory(raw_accel_viewer raw_accel_viewer)
add_subdirectory(raw_magnetometer_viewer raw_magnetometer_viewer)
add_subdirectory(raw_imu_viewer raw_imu_viewer)
