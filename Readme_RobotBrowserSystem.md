This project realizes a complete Robot System based om an ESP32 Camera front end SERVER 
which implements a Browser and a separate port for an XBOX Controller. The browser pages 
include a video feed which can be used for human operator robot control(teleop) and  
OpenCV.js for autonomous control. Teleop control includes a python GUI for 
the human-operated XBOX Controller which exchanges data(control and sensor) with the 
ESP32 via the separate port.

The ESP32 (in turn) transmits control data to an Arduino Mega and receives sensor
from the Mega, via I2C. The Mega, in turn, can run the robot actuators and processes sensor inputs,
although these tasks are not part of the project and are left to an interested reader. The Mega also
drives an OLED display using an SPI interface and U8G2. The display shows the IP address of the ESP32.

If the Mega requires the SPI interface for other purposes, a dedicated Arduino Pro Mini is shown 
connected the the Mega I2C bus and drives a duplicate display, for demonstration purposes..


This is a multi program project involving C++, HTML,Javascript, AJAX, and Python. Because of the
breadth of the project, some part of almost every program is based on code found in the literature 
or in Github and referenced appropriately. All code original to this project can be used freely and 
for any purpose. 

An extensive tutorial describing the system is in process of being written.

BRIEF OPERATING INSTRUCTIONS

0.INSTALL ESP32 LIBRARIES. USE ESP 32 WROVER MODULE, HUGE APP, FLASH FREQ 80Mhz, 115,200
  upload
  
00.INSTALL PYTHON 3.6.5 IDLE is very convenient for 2 and 3 below. 

1.Run ROBOT_BROWSER_SERVER_P after inserting your SSID and PASSWORD

2.Put the IP address shown in the Serial Monitor into CHROME browser and run. When 
  OpenCV.js is ready, click Color Track button. Check out Teleop and Auton mode.

3.Put the ip address into ROBOT_BROWSER_XBOX_P.py and run. INSTANCE GUI MUST BE ACTIVE
  TO ENTER COMMANDS INTO CONTROLLER. 