'''
Author : Ben Sisserman
Date : 10/18/2020
Description  :  This python module is meant to be used by a PC to intialize and send commands to SCPSS devices. 

    Currently each instance of SCPSS establishes communication with exactly one device. May revisit this design for managing
    mulitple devices later on.

NOTE : all IPs are strings and all port are ints, this matters for using both the socket library and decoding on devices
'''

import socket 
import time

getMilliSecs = lambda : int(round(time.time() * 1000))

class SCPSS:
    # defines the user's socket for recieving data from devices
    def __init__(self, deviceIP, devicePort):
        self.deviceIP = deviceIP
        self.devicePort = devicePort

        # create a TCP/IP socket, and connect to device. 
        self.mySocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.mySocket.settimeout(5)
        self.mySocket.connect((deviceIP,devicePort))
        
        # The allows to create time stamps for when messages are sent, but without using values as large as system time
        self.startTime = getMilliSecs()

        # initialze the time difference between device and host
        # self.setTimeDifference(500)


    # send a string with C for command code, T for time, E for end
    def sendStringWithTime(self, msg):
        self.mySocket.send(bytes('C' + str(msg).lower() + 'E' + 'T' + str(self.getTime()) + 'E','utf-8'))

    def sendTime(self):
        self.mySocket.send(bytes('T' + str(self.getTime()) + 'E','utf-8'))

    # function sends a custom msg
    # msg_code - Capitalized letter that tells esp how to process msg
    # msg - any value to send to ESP
    # note that msgs without valid msg codes are ignored by the esp
    def sendCustomMsg(self, msg_code, msg):
        self.mySocket.send(bytes(msg_code + str(msg).lower() + 'E','utf-8'))


    def closePort(self):
        self.mySocket.close()

    # on and off functions, turn on for a 1, next number sets which relay from 0 to 3
    # other devices ignore the second number
    def turnOn(self, relayNum=0):
        self.sendStringWithTime("1" + str(relayNum))
    
    # turn off by sending 0 and relay number 
    def turnOff(self, relayNum=0):
        self.sendStringWithTime("0" + str(relayNum))

    def syncClocks(self, latency = 200):
        self.sendCustomMsg('J', '')
        self.startTime = getMilliSecs()
    
    def setWiFi(self,SSID,PASS):
        pass

    def getTime(self):
        return getMilliSecs() - self.startTime



    ##### DEPRACATED
    # Function called during initialization for esp32s2 to calculate time difference between machines
    '''
    def getTimeFromEsp(self):
        # 4 is the size of the timestamp being sent
        msg = self.mySocket.recv(4)
        #print(msg)
        time = int.from_bytes(msg,byteorder='little', signed=False)
        return time
    

    
    def setTimeDifference(self, num_msgs):
        # pass
        # send msg to tell esp that we are setting the time difference
        self.sendCustomMsg('J',num_msgs)
        
        # send small msg with timestamp 10 times to the esp 32
        for i in range(num_msgs):
            self.sendTime()

        # esp32 will store minimum of those 10 - may need to increase this value
    
        # now recieve timestamp from esp32 10 times
        times = []
        for i in range(num_msgs):
            timeESP = self.getTimeFromEsp()
            #print(timeESP)
            #print(self.getTime())
            times.append(self.getTime() - timeESP)
        
        # send the minimum value back
        #print(min(times))
        self.sendCustomMsg('J', min(times))
        '''