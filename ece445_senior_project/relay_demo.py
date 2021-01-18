import argparse
import time
from SCPSS import SCPSS


relayBoard = SCPSS("192.168.43.102",8888)


try: 
    while(1):
        relayBoard.turnOn(2)
        time.sleep(0.5)
        relayBoard.turnOff(2)
        time.sleep(1)
        relayBoard.turnOn(2)
        time.sleep(0.5)
        relayBoard.turnOff(2)
        time.sleep(1)
        relayBoard.turnOn(2)
        time.sleep(0.5)
        relayBoard.turnOff(2)
        relayBoard.turnOn(3)
        time.sleep(0.5)
        relayBoard.turnOff(3)
        time.sleep(3)

except KeyboardInterrupt: 
    relayBoard.turnOff(0)
    relayBoard.turnOff(1)
    relayBoard.turnOff(2)
    relayBoard.turnOff(3)
    relayBoard.closePort()