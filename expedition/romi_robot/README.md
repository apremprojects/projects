![](robot_side_1.jpg)  
![](robot_side_3.jpg)  
![](robot_top.jpg)
(Above, my particular robot)  
**What is the Romi Robot?**  
    The Polulu Romi Robot, which I will refer to as the Romi Robot, is a simple, 2-wheeled robot. It can be powered from a set of 6
AA batteries (which is the manufacturer-recommended method). However, with the Raspberry Pi connected, the battery drains quickly. I have modified the robot to accept a 2S (7.4 volt) rechargable LiPo (Lithium-Polymer) battery. This increased the battery life significantly. The battery is attached with a 3D-printed mount. This mount keeps the battery securely attached.
    The robot features two brushed DC motors and is controlled by a Pololu A-Star 32U4 microcontroller board. This microcontroller can 
control the motors through PWM (Pulse Width Modulation). However, it lacks many critical features, like significant processing power, Bluetooth and Wifi connectivity, and ease of programming.
    To supplement the microcontroller, and perform more computationally complex and advanced tasks, a Raspberry Pi is connected to the 
microcontroller. The Raspberry Pi is a SBC(single board computer) that is the size of a credit-card. While it may be small, it still packs a significant amount of features, even being able to be connected to a monitor, keyboard, and mouse and used like a normal desktop with the included Raspbian OS (a version of Linux). As a result, most of the code that will run on a standard Windows laptop will run on the Pi, which was helpful as I prefer to program in Windows.  
![](https://a.pololu-files.com/picture/0J9406.600x480.jpg?5fdb7348a15ae23dc870aee4e1fe0041)  
(the green PCB is the Raspberry Pi and the other PCB is the 32U4)  
    In all of my projects, the Pi first recieves a set of commands (a .txt file, user input, or a Bluetooth joystick), does some 
calculations, and breaks the commands into basic instructions (do X for Y time units), and sends these simplified movement commands to the microcontoller, removing the heavy lifting from the microcontroller. A good analogy would be an architect converting a complex architectural diagram to basic instructions (eg. Put this screw in this place).  

![](https://upload.wikimedia.org/wikipedia/commons/thumb/d/d3/Airbus-319-cockpit.jpg/330px-Airbus-319-cockpit.jpg)  
(an example of a critical real-time system)  
**Why use the microcontroller instead of directly connecting the Pi to the motors?**  
    First of all, it is impossible to connect the Pi directly to the motors, as they are physically connected to the microcontroller
and connecting them to the Pi would be impossible. Also, one major flaw with directly connecting the Raspberry Pi to the motors is its lack of realtime support. Most computers, such as the Pi, use something called a scheduler. A good analogy that describes the scheduler is a juggler switching between many tasks. The computer is never idle, it might handle some display tasks for 2 milliseconds, quickly switch to an OS routine for file access, then switch to a random program, then your program, and then on and on. While this scheduler allows for dramatically simplified coding, a major flaw with it is that this switching can lead to odd delays when trying to perform time-sensitive tasks. For example, if you need to set a motors "throttle" signal (equivalent to the accelerator in a car) every 25 ms, the scheduler could be switching between different tasks (or even getting briefly stuck on one), and as a result, the computer could "skip" a task. The consequences of this could range from mundane (nothing major happens) to catastrophic (a plane's fly-by-wire system suddenly gets stuck on a background task, causing the critical flight control software to malfunction and cause the plane to flip tail-first at supersonic speeds and break up). While this "scheduler" has near-negligible effects on the actual computation/coding (it doesn't decrease the program's net speed by any significant amount), for tasks that are time-sensitive with very short time periods (you HAVE to perform action X within 25ms of the order), the computer would not be perfect. Microcontrollers, which either run no OS (they run the code directly on their tiny CPU without ANY background tasks to worry about) or have a special OS called a Real-Time Operating System (RTOS) (they set deadlines to accomplish actions and switch between tasks in a way that guarantees they will complete all of their actions by the deadlines). By having the Pi do computationally-intensive, less time-critical tasks (maybe updating every 50ms instead of 20ms) and sending its output to the microcontroller, I create a more reliable robot.  
![](https://www.electricaltechnology.org/wp-content/uploads/2020/05/Serial-Communication.png)  
**Detailed description of the Pi-Microcontroller communication**  
    The microcontroller and the Pi communicate over a USB serial link. A simple communication protocol where information is 
sent one character at a time. Imagine it as a tunnel where only one car can enter/leave at a time (it is not slow as the "car" travels at light-speed). A description of the information sent is below:  

Pi->Microcontroller -> The Pi sends the PID gains, velocity setpoints, and duration whenever it wants to, telling the microcontroller to set each wheel to a certain velocity for a certain amount of time.
Microcontroller->Pi -> The microcontroller tells the Pi about the motor "throttle" strength, the instantaneous motor velocity in terms of wheel encoder ticks, its busy status, etc.  

**What does the Microcontroller do?**  
    The microcontroller runs a firmware developed in C/C++. The microcontroller's task is to wait for 
commands, and report important details (eg. left/right wheel velocity). Also, as a safety precaution, it will stop if disconnected. 

**Things I learned this project**    
1. Ring buffer data structures
2. Circular queue
3. Inter-thread communication using queues and mutexes
4. Efficient PID loops on microcontrollers

**Things I applied**  
1. OpenGL and Qt
2. Serial communication
3. Microcontroller (Arduino) programming
4. Robot control using an Xbox controller
5. PID tuning

**Romi Robot projects**  
Here is a comprehensive description of my Romi Robot projects.
![](pid_tuner.png)  
1. **PID tuner** (romi_robot/pid_tuner)
    a. Uses OpenGL and Qt to help the user tune my robot's PIDs, and calculate the ideal values for optimal performance.  
[![](https://markdown-videos-api.jorgenkh.no/youtube/hZ06y4L7jlU)](https://youtu.be/hZ06y4L7jlU)  
2. **Motion Commands** (romi_robot/velocity_pid_test)
    a. Uses a command line interface to make the robot perform a sequence of actions (eg. set the left wheel to speed X and right wheel to Y for Z seconds, etc.)  
[![](https://markdown-videos-api.jorgenkh.no/youtube/thmzI69DqIw)](https://youtu.be/thmzI69DqIw)  
3. **Romi Robot Controller Demo** (romi_robot/romi_robot_with_controller)
    a. Control the two-wheeled robot with conventional joystick mechanics - like a video game or toy car - over Bluetooth.
        a. Uses a standard Bluetooth controller, in my case, an XBox controller.

**What is graph_image?**  
    Graph_image is a simple library I have included to help me with my code. Graph_image generates a 2D graph and its' axes and numbers
(used to display 2D graphs). This simplifies things for me as I do not have to reimplement the code from scratch each time I want to draw a 2D graph.

**Is it cross-platform?**  
    The code is cross-platform (tested on Raspbian and Windows). It should be able to run on any platform, however, the 
intended platform is a Raspberry Pi or similiar SBC.