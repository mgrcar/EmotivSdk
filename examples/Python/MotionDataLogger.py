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

IEE_EmoEngineEventGetEmoState = libEDK.IEE_EmoEngineEventGetEmoState
IEE_EmoEngineEventGetEmoState.argtypes = [c_void_p, c_void_p]
IEE_EmoEngineEventGetEmoState.restype = c_int

IEE_EmoStateCreate = libEDK.IEE_EmoStateCreate
IEE_EmoStateCreate.restype = c_void_p
eState = IEE_EmoStateCreate()

IEE_MotionDataCreate = libEDK.IEE_MotionDataCreate
IEE_MotionDataCreate.restype = c_void_p
hMotionData = IEE_MotionDataCreate();

def kbhit():
    ''' Returns True if keyboard character was hit, False otherwise.
    '''
    if sys.platform.startswith('win32'):
        return msvcrt.kbhit()   
    else:
        dr,dw,de = select([sys.stdin], [], [], 0)
        return dr != []

# -------------------------------------------------------------------------

userID   = c_uint(0)
user     = pointer(userID)
state    = c_int(0)
ready    = 0
datarate = c_uint(0)
secs     = c_float(1)
motionChannelList = [i for i in range(11)]

header = "COUNTER, GYROX, GYROY, GYROZ, ACCX, ACCY, ACCZ, MAGX, MAGY, MAGZ, TIMESTAMP"

# -------------------------------------------------------------------------

print "==================================================================="
print "Example to show how to log Motion Data from EmoDriver."
print "==================================================================="

# -------------------------------------------------------------------------

if libEDK.IEE_EngineConnect("Emotiv Systems-5") != 0:
    print "Emotiv Engine start up failed."
    
print "Start receiving IEEG Data! \n"
print "Press any key to stop logging...\n"

f = file('MotionDataLoggerP.csv', 'w')
f = open('MotionDataLoggerP.csv', 'w')

print >>f, header, "\n"

libEDK.IEE_MotionDataSetBufferSizeInSec(secs);
print "Buffer size in secs: %f \n" % secs.value

while True:    
    
    if kbhit():
        break
    
    state = libEDK.IEE_EngineGetNextEvent(eEvent)
    if state == 0:        
        eventType = libEDK.IEE_EmoEngineEventGetType(eEvent)
        libEDK.IEE_EmoEngineEventGetUserId(eEvent, user)
        
        if eventType == 16:  # libEDK.IEE_Event_enum.IEE_UserAdded
            ready = 1
            print "User added"
        
        if ready == 1:
            
            libEDK.IEE_MotionDataUpdateHandle(userID, hMotionData)
            nSamplesTaken = c_uint(0)
            nSamplesTakenP = pointer(nSamplesTaken)
            
            libEDK.IEE_MotionDataGetNumberOfSample(hMotionData, nSamplesTakenP);
            print "Updated %d " % nSamplesTaken.value
            
            if nSamplesTaken.value > 0:
                dataType = c_double * nSamplesTaken.value
                data = dataType()

                for sampleIdx in range(nSamplesTaken.value):
                    for i in motionChannelList:
                        libEDK.IEE_MotionDataGet(hMotionData, i, data, nSamplesTaken.value)
                        print >>f, data[sampleIdx], ",",
                                                            
                    print >>f, "\n"
            
    elif state != 0x0600:
        print "Internal error in Emotiv Engine ! "
    time.sleep(0.001)
# -------------------------------------------------------------------------
libEDK.IEE_EngineDisconnect()
libEDK.IEE_EmoStateFree(eState)
libEDK.IEE_EmoEngineEventFree(eEvent)
