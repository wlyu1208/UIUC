# SCPSS
Software Controlled Physical Sound Sources

This Git repo contains all the work pertaining to the sponsored Software Controlled Physical Sound Sources senior design project.

Some files included were used for testing and I would encourage to make use of these test files if there are any issues you are debugging.

The only files neccesary for using this project are the SCPSS.py module and the relay_firmware, servo_firmware, and ring_firmware arduino scripts.

## Set Up

### Setting up Arduino IDE with ESP32-S2

1) Download Arduino IDE: [Website Link](https://www.arduino.cc/en/software)

2) Open Arduino IDE and Install ESP32 board package by adding stable release link in the "Website Link" on the Additional Boards Manager URLS in File -> Preferences -> Settings: [Website Link](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md)

3) Download Arduino Core for the ESP32-S2: [Website Link](https://github.com/espressif/arduino-esp32/tree/esp32s2) 

4) Extract zip file of Arduino Core and run get.exe in tools folder.

5) Open preferences of Arduino IDE and go to the directory of ESP32 board package.

6) Inside packages folder, backup original esp32 folder and go inside the esp32 folder until there are folders of cores. (esp32 -> hardware -> esp32 -> 1.0.4)

7) Replace all folders and files to same ones from Arduino Core of the ESP32-S2.

8) There will be a new board of ESP32S2 Dev Module on the ESP32 Arduino Board Manager of Arduino IDE. 

[Guide Video for Setting Up](https://tutorial.cytron.io/2020/06/25/program-esp32-s2-using-arduino-ide-unofficial/)

## Usage

### Controlling the SCPSS devices using the Python Module

#### Initializing a SCPSS device

1) Open the firmware code for the device you want to use. Look for the section of the code near the beginning of the file that defines the 
   WiFi credentials. Enter the credentials you want to use and upload the code to the esp32-s2.
   
2) Allow the board time to connect to WiFi. When the connection is established, the LCD will display the IP address and port for the TCP connection.
   You will need this information to connect to the device from your PC.
   
3) Initiate an instance of the SCPSS Class

```
#import the SCPSS object
import SCPSS from SCPSS

#initialize the SCPSS object
relay_device = SCPSS(<string IP>, <int port>)  
```
#### Sending Commands to the device

Once the the SCPSS object is initialized, the esp32-s2 is connected and waiting for commands. You should be able to verify this connection by the status displayed on the LCD display.

To send commands to device use the following functions:
```
relay_device.turnOn() # on device to continously make sound

relay_device.turnOff() # deactivate sound

# for the relay board, you can specify which relay to activate or deactivate by adding the a number from 0 - 3

relay_device.turnOn(1)   # activate the relay 2
relay_device.turnOff(2)  # deactivate relay 3

# for the relay board using no argument is the same as using index 0
relay_board.turnOn()
relay_board.turnOn(0)   # both activate relay 1
```

It is recommended to properly close the port using:
```
relay_board.closePort()
```
After this the board will lose connection to the host PC and can be paired with a different host.
