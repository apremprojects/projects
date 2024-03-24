**Round 1**  
![](imu_projects/imu_image.png)  
1. **IMU Data visualizer** (imu_projects/raw_imu_viewer)
    1. Uses OpenGL and Qt to display 2D graphs of accelerometer and gyro data and 3D point data of magnetometer data.  
    2. Many subprojects were used for practice. The final project is a combination of them.  
        1. **Raw Accelerometer Data** (imu_projects/raw_accel_viewer)  
            1. Uses Qt and OpenGL to display a graph of accelerometer data in the X, Y, and Z axes.  
        1. **Raw Magnetometer Viewer** (imu_projects/raw_magnetometer_data)  
            1. Uses Qt and OpenGL to display a 3D graph of magnetometer data in the X, Y, and Z axes.  
        1. **Raw IMU Data Viewer** (imu_projects/raw_imu_data_viewer)  
            1. A combination of the Raw Accelerometer Data Viewer, Raw Magnetometer Data Viewer, and a new Raw Gyro Data viewer.  
[![](https://markdown-videos-api.jorgenkh.no/youtube/JowkhfD-yT4)](https://youtu.be/JowkhfD-yT4)  
3. **PID tuner** (romi_robot/pid_tuner)  
    1. Uses OpenGL and Qt to help the user tune my robot's PIDs, and calculate the ideal values for optimal performance.  
[![](https://markdown-videos-api.jorgenkh.no/youtube/wvVs3TY9yZE)](https://youtu.be/wvVs3TY9yZE)  
4. **Motion Commands** (romi_robot/velocity_pid_test)  
    1. Uses a command line interface to make the robot perform a sequence of actions (eg. set the left wheel to speed X and right wheel to Y for Z seconds, etc.)  
[![](https://markdown-videos-api.jorgenkh.no/youtube/hZ06y4L7jlU)](https://youtu.be/hZ06y4L7jlU)  
5. **Romi Robot Controller Demo** (romi_robot/romi_robot_with_controller)  
    1. Control the two-wheeled robot with conventional joystick mechanics - like a video game or toy car - over Bluetooth.  
        a. Uses a standard Bluetooth controller, in my case, an Xbox controller.  
[![](https://markdown-videos-api.jorgenkh.no/youtube/thmzI69DqIw)](https://youtu.be/thmzI69DqIw)  
  
**Round 2**  
1. **Magnetometer Calibration** (imu_projects/magnetometer_calibration)  
    1. Uses Eigen and magnetometer data to generate calibration data to process raw magnetometer data.

[![](https://markdown-videos-api.jorgenkh.no/youtube/NJRtEHCLCtw)](https://youtu.be/NJRtEHCLCtw)  
3. **Magnetometer Calibration Viewer** (imu_projects/view_magnetometer_calibration)  
    1. Uses Eigen, earlier calibration data, and magnetometer data to display calibrated magnetometer data, useful to calculate heading.

[![](https://markdown-videos-api.jorgenkh.no/youtube/q61ZD1pd00A)](https://youtu.be/q61ZD1pd00A)  
  
**Round 3**
1. **Romi Robot Waypoint App**  (romi_robot/romi_waypoint_app)  
    1. Waypoint system for the Polulu Romi Robot involving 3 separate programs (client-side, server-side, and Arduino) that allow the robot to localize itself, navigate to waypoints, and
       communicate trajectory and current position to a PC using TCP.

[![](https://markdown-videos-api.jorgenkh.no/youtube/VHrM9P7A7Nk)](https://youtu.be/VHrM9P7A7Nk)  
2. **Object Viewer with Euler**  (quaternion/viewer_with_euler)  
    1. This is a .obj viewer using Euler angles as a baseline. It has 3 spinboxes, each storing the x, y, and z (roll, pitch, and yaw). It is not possible to use the mouse to drag the camera around. Diffuse lighting is implemented.  
        1. It uses quaternion/obj_file to parse the provided .obj file.  

[![](https://markdown-videos-api.jorgenkh.no/youtube/APsEHQz6kWw)](https://youtu.be/APsEHQz6kWw)  
3. **Object Viewer with Quaternion**  (quaternion/viewer_with_quaternion)  
    1. This is a .obj viewer using Quaternions, built off viewer_with_euler. It is possible to use the mouse to drag the camera around. Diffuse lighting is implemented.  
        1. It uses obj_file to parse the provided .obj file.  
        1. Eigen’s Quaternion class is used instead of quaternion_demo’s quaternion library as it is more advanced and has more features.  

[![](https://markdown-videos-api.jorgenkh.no/youtube/RonZaKVwmh0)](https://youtu.be/RonZaKVwmh0)  
