![](https://a.pololu-files.com/picture/0J9406.600x480.jpg?5fdb7348a15ae23dc870aee4e1fe0041)
(Above is an unmodified Pololu Romi Robot)
**What is the Romi Robot?**
    The Polulu Romi Robot, which I will refer to as the Romi Robot, is a simple, 2-wheeled robot. It can be powered from a set of 4-6
AA batteries (which is the recommended manufacturer method), however, there is support to modify the robot to accept a 2S (7.4 volt) rechargable LiPo(Lithium-Polymer) battery (the kind used in hobbyist RC cars and drones). I chose to modify the robot to accept this new LiPo battery as the AA batteries were cumbersome to install and needed frequent charging. I ended up having to install a 3D-printed battery mount, which I made using my Creality K1 3D printer. This mount dramatically increased the ease of use of the robot, as it kept the battery securely attached AND was easily removable with a standard screwdriver.
    The robot features two brushed DC motors (which are commonplace in various applications due to their simplicity) and it is 
controlled by a Pololu A-Star 32U4 (based on a popular ATMega 32U4 microcontroller). This microcontroller is connected to to the motors, and can issue commands to them. However, due to being a microcontroller, it lacks many critical features, like significant processing power, Bluetooth and Wifi connectivity, and the advantages of a familiar Linux programming environment (such as easy programming (microcontroller programming lacks many features)).
![](https://a.pololu-files.com/picture/0J11109.1200x600.jpg?941cfd35ed2641cf22f5120ca7176958)
(the black PCB is the Polulu A-Star 32U4)
    To supplement the microcontroller, and perform more computationally complex and advanced tasks, a Raspberry Pi is connected to the 
microcontroller. The Raspberry Pi is a SBC(single board computer) that is the size of a credit-card. While it may be small, it still packs a significant amount of features, even being able to be connected to a monitor, keyboard, and mouse and used like a normal desktop with the included Raspbian OS (being a tiny device, it cannot run Windows, but it runs a version of Linux with basic productivity features (like a file manager, full browser, and Python/C/C++ programming suite)).

**Romi Robot projects**
Here is a comprehensive description of my Romi Robot projects.
1. **PID tuner** (romi_robot/pid_tuner)
    a. Uses OpenGL and Qt to help the user tune my robot's PIDs, and calculate the ideal values for optimal performance.
2. **Motion Commands** (romi_robot/velocity_pid_test)
    a. Uses a command line interface to make the robot perform a sequence of actions (eg. set the left wheel to speed X and right wheel to Y for Z seconds, etc.)
3. **Romi Robot Controller Demo** (romi_robot/romi_robot_with_controller)
    a. Control the two-wheeled robot with conventional joystick mechanics - like a video game or toy car - over bluetooth.
        a. Uses a standard Bluetooth controller, in my case, an XBox controller.