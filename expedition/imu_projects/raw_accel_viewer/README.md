**IMU Raw Data Viewer**

![](https://lh7-us.googleusercontent.com/VE2ytwLFz0I69gcEvYXbgNsK0DoYbCqCIzzlx-KTBdqd08zij-B5oAZ-A_UR2ZvyP8J0sNvM_2_9cc9Ncns4w_7qlseGwNlpT_qGot8fJ_VUnx2Bw9UlP-6r48kp7mGxd31M6Jkxhe4KVlUb3DGGHw0)

**Hardware Details**

1.  [Pololu MiniIMU-9 v5](https://www.pololu.com/product/2738) - The Pololu MinIMU-9 v5 is an inertial measurement unit (IMU) that packs an LSM6DS33 3-axis gyro and 3-axis accelerometer and an LIS3MDL 3-axis magnetometer onto a tiny 0.8" × 0.5" board. An I²C interface accesses nine independent rotation, acceleration, and magnetic measurements that can be used to calculate the sensor's absolute orientation. 

1.  [LSM6DS33](https://www.pololu.com/file/0J1087/LSM6DS33.pdf)

2.  [LIS3MDL](https://www.pololu.com/file/0J1089/LIS3MDL.pdf)

3.  [Arduino Micro](https://store-usa.arduino.cc/products/arduino-micro?selectedStore=us)  ATmega32U4 [board](https://www.amazon.com/gp/product/B01MTU9GOB/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)

1.  [Pin diagram](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/hardware-overview-pro-micro)

5.  Laptop or computer with USB ports (VS2022, MicroUSB, and Qt required)

6.  3D printed enclosure to store it in

1.  ![](https://lh7-us.googleusercontent.com/OORIYneItNuhYiXt-bnhNqCiOupW358WDkaBLmw3khtBDzl19YBrjw7ReTe0eNoHQDH9zksktirXFVUBbx67qZ5AO4Mc4U-NDaImcvESPfCp3Msl57-6yLjLP2rO7ZwLySVXz6SCB5es0GxvC21z8ZU)![](https://lh7-us.googleusercontent.com/2tD8LRsMqavOjRPm2mt8B__zDEfcz9xVoQZN4YD4yIdWp2rV9G6PYbpEBDWa4VeryB1QZNedwKWxj2olhoLgrxZZ-PkZ5v0rThOLJHpJiNl1UsUw00AwmIupRvsl94tjMwnx0M8qgwzIGB_6icLLcLk)

The hardware in my project is an IMU (listed above) and an Arduino Micro (also listed above). The Arduino is connected to the IMU, and the two communicate over I2C. For PC - Arduino communication, serial is used. In order to initiate communication, the ASCII character 's' is sent over serial as a "heartbeat" character, and for the next 250ms, the Arduino sends data. To get a continuous stream of data, the ASCII "heartbeat" character is sent every 250ms. The details sent by the Arduino include accelerometer data, gyroscope data, magnetometer data, time, pressure, and etc.![](https://lh7-us.googleusercontent.com/AbHI_VeDT2FO0-LAwEZcFkP2HN6XLgMbzGA5gu8ve3fpkjpm75kSeJIRZb_UKRC10KPGVjNl0-z-8WXzjFRpO4GMws9ej7K8uwMQzIIERRbIgsj9wxZ64axqsRef683WwIHVhzLNuHaZgozAzKJBuI0)

**Software**

In order to display the data collected from the Arduino, I use Qt (as well as OpenGL) to display graphs of the data. Upon opening my program, the user will see one 3D Magnetometer Data visualizer, as well as two 2D data visualizers, for accelerometer and gyro data (respectively). I use OpenGL to draw the 2D graphs, as well as the 3D point data of the accelerometer data. A duration and serial port selector are available. Pressing "Start" starts data collection/display, and pressing "Stop" ends data collection and display. A checkbox entitled "Shift center" is available to shift the point data so it is centered around the origin (a helpful feature as it is likely to be off-center).

The code is cross-platform and can run on both Linux and Windows.
