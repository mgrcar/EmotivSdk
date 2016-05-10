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

IS_GetTimeFromStart = libEDK.IS_GetTimeFromStart
IS_GetTimeFromStart.argtypes = [ctypes.c_void_p]
IS_GetTimeFromStart.restype = c_float

IEE_EmoStateCreate = libEDK.IEE_EmoStateCreate
IEE_EmoStateCreate.restype = c_void_p
eState = IEE_EmoStateCreate()

def kbhit():
    ''' Returns True if keyboard character was hit, False otherwise.
    '''
    if sys.platform.startswith('win32'):
        return msvcrt.kbhit()   
    else:
        dr,dw,de = select([sys.stdin], [], [], 0)
        return dr != []

# -------------------------------------------------------------------------

userID    = c_uint(0)
user      = pointer(userID)
state     = c_int(0)
timestamp = c_float(0.0)

# -------------------------------------------------------------------------

print "==================================================================="
print "Example demonstrates how to connect to two headsets at the same time."
print "==================================================================="

# -------------------------------------------------------------------------

if libEDK.IEE_EngineConnect("Emotiv Systems-5") != 0:
    print "Emotiv Engine start up failed."


while True:    
    
    if kbhit():
        break
    
    state = libEDK.IEE_EngineGetNextEvent(eEvent)
    if state == 0:
        eventType = libEDK.IEE_EmoEngineEventGetType(eEvent)
        libEDK.IEE_EmoEngineEventGetUserId(eEvent, user)
        
        if eventType == 16:  # libEDK.IEE_Event_enum.IEE_UserAdded
            print "User ID: %d have added" % userID.value
            
        if eventType == 32:  # libEDK.IEE_Event_enum.IEE_UserRemoved
            print "User ID: %d have removed" % userID.value
        
        if eventType == 64:  # libEDK.IEE_Event_enum.IEE_EmoStateUpdated            
            libEDK.IEE_EmoEngineEventGetEmoState(eEvent, eState)        
            timestamp = IS_GetTimeFromStart(eState)    
            print "User %d: %f" % (userID.value, timestamp)                                        
            
    elif state != 0x0600:
        print "Internal error in Emotiv Engine ! "
    time.sleep(0.01)
# -------------------------------------------------------------------------
libEDK.IEE_EngineDisconnect()
libEDK.IEE_EmoStateFree(eState)
libEDK.IEE_EmoEngineEventFree(eEvent)
