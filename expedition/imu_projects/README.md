**Inertial Measurement Unit (IMU) Raw Data Viewer**

**What is an IMU ([Wikipedia Definition](https://en.wikipedia.org/wiki/Inertial_measurement_unit))?**

An IMU measures various physical parameters in all 3-axes (3D), like angular rate (how fast it is turning in one direction), acceleration, and magnetic field strength. Some IMUs also measure pressure and temperature. For example, shaking the Polulu IMU (below) in the Y-axis would be observed as Y-axis acceleration.

![](https://lh7-us.googleusercontent.com/t9l3ruqOE-mQLxreXB9-4FUAn1uZIgL5IYlfNtF3v17W4TDYxnbm66Wyj8TaUy_1VsXTKCiwk95W2OTypAg0-XO4yuOToHUI-9viTO1b57s006xhkCcX6JkT9aGSbaCX2NlAKp1XswVJch4Hu7zcxGs)

**How do IMUs work?**

Specialty IMUs used in full-scale planes and other critical applications typically operate using gyroscopic or optical mechanics. Gyroscopic specialty IMUs, which use gyroscopic forces to keep a ring level (shown below), spin a mass (the rotor) at high speed. The gyroscopic forces keep the ring level, providing a stable basis to measure against. While the gyroscope may not be level, the rotor will be level as long as it spins. However, spinning the rotor is prone to failure, as the motor can break. 

Highly sophisticated IMUs use ring laser gyros, which use the properties of lasers to measure angular rate (not the exact angle itself). Their operating principles are more complex (and will not be explained here), but they have near zero moving parts and are less prone to failure. Using a bit of calculus, the angular rate can be integrated to calculate the exact angle. Nevertheless, one major flaw with both of the earlier gyros is that they consume significant amounts of energy and take up a large space.

Consumer-grade IMUs used in small-scale drones, phones, etc. use solid-state principles and are extremely small. An example of this would be the Polulu MiniIMU which  I use. They are made out of extremely tiny silicon circuits and are made like other semiconductors. They measure acceleration, angular rate, and magnetic field along XYZ planes

Also, all of the earlier IMUs can be integrated with other sensors, like temperature, pressure, and acceleration. As long as all the other sensors are on the same circuit board, it is considered an IMU.
![](imu_image.png)  

(left to right, gyroscope, ring laser gyroscope, and MEMS gyroscope)

**Why did I choose this project?**

![](https://lh7-us.googleusercontent.com/06fwX4k7rLzynak49m7B4jXngJx2pBWcomsCx-5y5PGIMVM1cIlsFRGnTk6IEmSsfl-8SwxE-GkcR_vbfcr5Qz63aq6mnObX1PgvON2VpNYPIBDItq1wJxUtXiHwA3ri8AmKjqnIDahdUqTvw9zWJDE)

I chose this project because I was interested in how robots, drones, and planes know their orientation and altitude. By choosing this project, I familiarized myself with microcontroller programming, Serial and I2C communication protocols, Qt, and OpenGL. With this project, I set myself up for my second project, where I will learn about Quaternions and orientation estimation.

![](https://lh7-us.googleusercontent.com/R3cvjmnCD5UM2Vy0E8O8W3bkh9hw5-Mcv3YV9Vf4CxXLLBfxUHiYKF0gWBaCDN8EpcQvIzF9IRyRQZKfJa7oGQU80Q5Jeo_ZDFD6fIx65a2aFmk9tXUQ9eGNjZhlh4vTQNdmepNFC8Up81YcmZ7jRec)

**Hardware Details**

1.  [Pololu MiniIMU-9 v5](https://www.pololu.com/product/2738) - The Pololu MinIMU-9 v5 is an inertial measurement unit (IMU) that packs an LSM6DS33 3-axis gyro and 3-axis accelerometer and an LIS3MDL 3-axis magnetometer onto a tiny 0.8" × 0.5" board. An I²C interface accesses nine independent rotation, acceleration, and magnetic measurements that can be used to calculate the sensor's absolute orientation. 

1.  [LSM6DS33](https://www.pololu.com/file/0J1087/LSM6DS33.pdf) (Accelerometer and Gyroscope)

2.  [LIS3MDL](https://www.pololu.com/file/0J1089/LIS3MDL.pdf) (Magnetometer)

3.  [Arduino Micro](https://store-usa.arduino.cc/products/arduino-micro?selectedStore=us)  ATmega32U4 [board](https://www.amazon.com/gp/product/B01MTU9GOB/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)

1.  [Pin diagram](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/hardware-overview-pro-micro)

5.  Laptop or Raspberry Pi (USB port and Qt required)

6.  3D printed enclosure to store it in

1.  ![](https://lh7-us.googleusercontent.com/Y8pKEk2GZcIQVUWldxKPZv_OBQ5cylhBpwDLfDXqREApPNnYuHmQjEkFd3W8eex6uhm9WIzk5cSU12GA-5_GwjrbI-hgDNFMe5tJ-ztffpugGQqO_6juFnRJgBZGklwm2LjrHHD7yI7Ne95LF2g3gHE)![](https://lh7-us.googleusercontent.com/voUzxrnnbVQYTzBj7i6avi0FWqwpbUlxFok2NLVb6f_pH38Iz6AanBlj7lmeedrbub4BNqEHlGpmui2EaaTbK3KGY4Ax4b8zaZMT0IFZDpJy8HIFonFN7xg9E2aybdY2Qb-HQ-fRS27-MMBSCX6BIs0)

The hardware in my project is an IMU (listed above) and an Arduino Micro (also listed above). The Arduino is connected to the IMU, and the two communicate over I2C. The computer communicates with the Arduino over a USB serial link. To initiate communication, the ASCII character 's' is sent over serial as a "heartbeat" character, and for the next 250ms, the Arduino sends data. To get a continuous stream of data, the ASCII "heartbeat" character is sent every 250ms. The details sent by the Arduino include accelerometer data, gyroscope data, magnetometer data, and a timestamp.![](https://lh7-us.googleusercontent.com/n5Ul4_AJLVCjdGgMKKfDdl2YFodPbUDfVCC98f9jWMttZue17wNMl2Ui2iMrGWustvb1zznv4zr1l1JVoQ-uJAyhwzPbng2F0LXM5apLfGBGGEbUzKzvhTCxYfs1yMYRQpqGv1KM-o_ybfHCUjJRD4c)

**Software**

To display the data collected from the Arduino, I use Qt and OpenGL. Upon opening my program, the user will see one 3D Magnetometer widget, as well as two 2D widgets (one for acceleration and one for angular rate). I use OpenGL to draw the 2D graphs and the 3D point data of the magnetometer data. A duration and serial port selector are available. Pressing "Start" starts data collection/display, and pressing "Stop" ends data collection and display. A checkbox entitled "Shift center" is available to shift the magnetometer data so it is centered around the origin (a helpful feature as it is likely to be off-center).

The code is cross-platform and can run on both Linux and Windows.

**Demo**

My demo is  available here:  
[![IMU DEMO VIDEO](http://img.youtube.com/vi/JowkhfD-yT4/0.jpg)](http://www.youtube.com/watch?v=JowkhfD-yT4 "IMU Demo")
