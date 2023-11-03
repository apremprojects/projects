cmake_minimum_required(VERSION 3.8.2)

project(romi_robot)

#adding helper and graph_image as libraries
add_subdirectory(./../../helper helper)

add_subdirectory(./graph_image graph_image)
add_subdirectory(./pid_tuner pid_tuner)
add_subdirectory(./velocity_pid_test velocity_pid_test)

add_subdirectory(./romi_robot_with_controller romi_robot_app)