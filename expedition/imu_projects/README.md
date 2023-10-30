<!-----

You have some errors, warnings, or alerts. If you are using reckless mode, turn it off to see inline alerts.
* ERRORs: 0
* WARNINGs: 1
* ALERTS: 10

Conversion time: 2.725 seconds.


Using this Markdown file:

1. Paste this output into your source file.
2. See the notes and action items below regarding this conversion run.
3. Check the rendered output (headings, lists, code blocks, tables) for proper
   formatting and use a linkchecker before you publish this page.

Conversion notes:

* Docs to Markdown version 1.0β34
* Sun Oct 29 2023 18:04:01 GMT-0700 (PDT)
* Source doc: IMU Readme

WARNING:
Inline drawings not supported: look for ">>>>>  gd2md-html alert:  inline drawings..." in output.

* This document has images: check for >>>>>  gd2md-html alert:  inline image link in generated source and store images to your server. NOTE: Images in exported zip file from Google Docs may not appear in  the same order as they do in your doc. Please check the images!

----->


<p style="color: red; font-weight: bold">>>>>>  gd2md-html alert:  ERRORs: 0; WARNINGs: 1; ALERTS: 10.</p>
<ul style="color: red; font-weight: bold"><li>See top comment block for details on ERRORs and WARNINGs. <li>In the converted Markdown or HTML, search for inline alerts that start with >>>>>  gd2md-html alert:  for specific instances that need correction.</ul>

<p style="color: red; font-weight: bold">Links to alert messages:</p><a href="#gdcalert1">alert1</a>
<a href="#gdcalert2">alert2</a>
<a href="#gdcalert3">alert3</a>
<a href="#gdcalert4">alert4</a>
<a href="#gdcalert5">alert5</a>
<a href="#gdcalert6">alert6</a>
<a href="#gdcalert7">alert7</a>
<a href="#gdcalert8">alert8</a>
<a href="#gdcalert9">alert9</a>
<a href="#gdcalert10">alert10</a>

<p style="color: red; font-weight: bold">>>>>> PLEASE check and correct alert issues and delete this message and the inline alerts.<hr></p>


**Inertial Measurement Unit (IMU) Raw Data Viewer**

**What is an IMU ([Wikipedia Definition](https://en.wikipedia.org/wiki/Inertial_measurement_unit))?**

	An IMU measures various physical parameters in all 3-axes (3D), like angular rate (how fast it is turning in one direction), acceleration, and magnetic field strength. Some IMUs also measure pressure and temperature. For example, shaking the Polulu IMU (below) in the Y-axis would be observed as Y-axis acceleration.



<p id="gdcalert1" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image1.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert2">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image1.png "image_tooltip")


**How do IMUs work?**

	Specialty IMUs used in full-scale planes and other critical applications typically operate using gyroscopic or optical mechanics. Gyroscopic specialty IMUs, which use gyroscopic forces to keep a ring level (shown below), spin a mass (the rotor) at high speed. The gyroscopic forces keep the ring level, providing a stable basis to measure against. While the gyroscope may not be level, the rotor will be level as long as it spins. However, spinning the rotor is prone to failure, as the motor can break. 

Highly sophisticated IMUs use ring laser gyros, which use the properties of lasers to measure angular rate (not the exact angle itself). Their operating principles are more complex (and will not be explained here), but they have near zero moving parts and are less prone to failure. Using a bit of calculus, the angular rate can be integrated to calculate the exact angle. Nevertheless, one major flaw with both of the earlier gyros is that they consume significant amounts of energy and take up a large space.

	Consumer-grade IMUs used in small-scale drones, phones, etc. use solid-state principles and are extremely small. An example of this would be the Polulu MiniIMU which  I use. They are made out of extremely tiny silicon circuits and are made like other semiconductors. They measure acceleration, angular rate, and magnetic field along XYZ planes

	Also, all of the earlier IMUs can be integrated with other sensors, like temperature, pressure, and acceleration. As long as all the other sensors are on the same circuit board, it is considered an IMU.



<p id="gdcalert2" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image2.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert3">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image2.png "image_tooltip")


<p id="gdcalert3" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image3.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert4">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image3.png "image_tooltip")


<p id="gdcalert4" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image4.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert5">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image4.png "image_tooltip")


(left to right, gyroscope, ring laser gyroscope, and MEMS gyroscope)

**Why did I choose this project?**



<p id="gdcalert5" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert6">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/drawings/d/12345/export/png)

<p id="gdcalert6" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image5.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert7">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image5.png "image_tooltip")


I chose this project because I was interested in how robots, drones, and planes know their orientation and altitude. By choosing this project, I familiarized myself with microcontroller programming, Serial and I2C communication protocols, Qt, and OpenGL. With this project, I set myself up for my second project, where I will learn about Quaternions and orientation estimation.



<p id="gdcalert7" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image6.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert8">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image6.png "image_tooltip")


**Hardware Details**



1. [Pololu MiniIMU-9 v5](https://www.pololu.com/product/2738) - The Pololu MinIMU-9 v5 is an inertial measurement unit (IMU) that packs an LSM6DS33 3-axis gyro and 3-axis accelerometer and an LIS3MDL 3-axis magnetometer onto a tiny 0.8″ × 0.5″ board. An I²C interface accesses nine independent rotation, acceleration, and magnetic measurements that can be used to calculate the sensor’s absolute orientation. 
    1. [LSM6DS33](https://www.pololu.com/file/0J1087/LSM6DS33.pdf) (Accelerometer and Gyroscope)
    2. [LIS3MDL](https://www.pololu.com/file/0J1089/LIS3MDL.pdf) (Magnetometer)
2. [Arduino Micro](https://store-usa.arduino.cc/products/arduino-micro?selectedStore=us) ATmega32U4 [board](https://www.amazon.com/gp/product/B01MTU9GOB/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
    3. [Pin diagram](https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/hardware-overview-pro-micro)
3. Laptop or Raspberry Pi (USB port and Qt required)
4. 3D printed enclosure to store it in
    4. 

<p id="gdcalert8" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image7.jpg). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert9">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image7.jpg "image_tooltip")


<p id="gdcalert9" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image8.jpg). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert10">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image8.jpg "image_tooltip")


The hardware in my project is an IMU (listed above) and an Arduino Micro (also listed above). The Arduino is connected to the IMU, and the two communicate over I2C. The computer communicates with the Arduino over a USB serial link. To initiate communication, the ASCII character ‘s’ is sent over serial as a “heartbeat” character, and for the next 250ms, the Arduino sends data. To get a continuous stream of data, the ASCII “heartbeat” character is sent every 250ms. The details sent by the Arduino include accelerometer data, gyroscope data, magnetometer data, and a timestamp.

<p id="gdcalert10" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/image9.png). Store image on your image server and adjust path/filename/extension if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert11">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/image9.png "image_tooltip")


**Software**

	To display the data collected from the Arduino, I use Qt and OpenGL. Upon opening my program, the user will see one 3D Magnetometer widget, as well as two 2D widgets (one for acceleration and one for angular rate). I use OpenGL to draw the 2D graphs and the 3D point data of the magnetometer data. A duration and serial port selector are available. Pressing “Start” starts data collection/display, and pressing “Stop” ends data collection and display. A checkbox entitled “Shift center” is available to shift the magnetometer data so it is centered around the origin (a helpful feature as it is likely to be off-center).

The code is cross-platform and can run on both Linux and Windows.
