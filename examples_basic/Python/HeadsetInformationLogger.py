import sys
import os
import platform
import time
import ctypes

if sys.platform.startswith('win32'):
    import msvcrt
elif sys.platform.startswith('linux'):
    import atexit
    from select import select
    
from ctypes import *
from fileinput import close

try:
    if sys.platform.startswith('win32'):
        libEDK = cdll.LoadLibrary("../../bin/win32/edk.dll")
    elif sys.platform.startswith('linux'):
        srcDir = os.getcwd()
	if platform.machine().startswith('arm'):
            libPath = srcDir + "/../../bin/armhf/libedk.so"
	else:
            libPath = srcDir + "/../../bin/linux64/libedk.so"
        libEDK = CDLL(libPath)
    else:
        raise Exception('System not supported.')
except Exception as e:
    print 'Error: cannot load EDK lib:', e
    exit()

IEE_EmoEngineEventCreate = libEDK.IEE_EmoEngineEventCreate
IEE_EmoEngineEventCreate.restype = c_void_p
eEvent = IEE_EmoEngineEventCreate()

IS_GetTimeFromStart = libEDK.IS_GetTimeFromStart
IS_GetTimeFromStart.argtypes = [ctypes.c_void_p]
IS_GetTimeFromStart.restype = c_float

IS_GetWirelessSignalStatus = libEDK.IS_GetWirelessSignalStatus
IS_GetWirelessSignalStatus.restype = c_int
IS_GetWirelessSignalStatus.argtypes = [c_void_p]

IS_GetContactQuality = libEDK.IS_GetContactQuality
IS_GetContactQuality.restype = c_int
IS_GetContactQuality.argtypes = [c_void_p, c_int]

IEE_EmoEngineEventGetEmoState = libEDK.IEE_EmoEngineEventGetEmoState
IEE_EmoEngineEventGetEmoState.argtypes = [c_void_p, c_void_p]
IEE_EmoEngineEventGetEmoState.restype = c_int

IEE_EmoStateCreate = libEDK.IEE_EmoStateCreate
IEE_EmoStateCreate.restype = c_void_p
eState = IEE_EmoStateCreate()

# -------------------------------------------------------------------------

def kbhit():
    ''' Returns True if keyboard character was hit, False otherwise.
    '''
    if sys.platform.startswith('win32'):
        return msvcrt.kbhit()   
    else:
        dr,dw,de = select([sys.stdin], [], [], 0)
        return dr != []

# -------------------------------------------------------------------------

userID       = c_uint(0)
user         = pointer(userID)
ready        = 0
state        = c_int(0)
systemUpTime = c_float(0.0)

batteryLevel     = c_long(0)
batteryLevelP    = pointer(batteryLevel)
maxBatteryLevel  = c_int(0)
maxBatteryLevelP = pointer(maxBatteryLevel)

systemUpTime     = c_float(0.0)
wirelessStrength = c_int(0)

header = "Time, Wireless Strength, Battery Level, AF3, T7, Pz, T8, AF4"
# -------------------------------------------------------------------------

print "==================================================================="
print "This example allows getting headset infor: contactquality, wireless " \
"strength, battery level."
print "==================================================================="

# -------------------------------------------------------------------------

if libEDK.IEE_EngineConnect("Emotiv Systems-5") != 0:
    print "Emotiv Engine start up failed."

print "Press any key to stop logging...\n"

f = file('HeadsetInformationLoggerP.csv', 'w')
f = open('HeadsetInformationLoggerP.csv', 'w')

print >>f, header, "\n",

while True:    
    
    if kbhit():
        break
    
    state = libEDK.IEE_EngineGetNextEvent(eEvent)
    
    if state == 0:
        eventType = libEDK.IEE_EmoEngineEventGetType(eEvent)
        libEDK.IEE_EmoEngineEventGetUserId(eEvent, user)
        
        if eventType == 16:  # libEDK.IEE_Event_enum.IEE_UserAdded
            print "User added"
            ready = 1
            
        if eventType == 64:  # libEDK.IEE_Event_enum.IEE_EmoStateUpdated
                        
            libEDK.IEE_EmoEngineEventGetEmoState(eEvent, eState);
            
            systemUpTime = IS_GetTimeFromStart(eState);            
            wirelessStrength = libEDK.IS_GetWirelessSignalStatus(eState);
            
            if wirelessStrength > 0:
                print (systemUpTime)
                                
                libEDK.IS_GetBatteryChargeLevel(eState, batteryLevelP, maxBatteryLevelP);
                
                print >>f, systemUpTime, ",",
                print >>f, wirelessStrength, ",",
                print >>f, batteryLevel.value, ",",
                print >>f, IS_GetContactQuality(eState, 3), ",",
                print >>f, IS_GetContactQuality(eState, 7), ",",
                print >>f, IS_GetContactQuality(eState, 9), ",",
                print >>f, IS_GetContactQuality(eState, 12), ",",
                print >>f, IS_GetContactQuality(eState, 16), ",",
                print >>f, '\n'
                             
    elif state != 0x0600:
        print "Internal error in Emotiv Engine ! "
# -------------------------------------------------------------------------
f.close()
libEDK.IEE_EngineDisconnect()
libEDK.IEE_EmoStateFree(eState)
libEDK.IEE_EmoEngineEventFree(eEvent)
