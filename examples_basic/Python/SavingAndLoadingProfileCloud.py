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
    
write = sys.stdout.write

IEE_EmoEngineEventCreate = libEDK.IEE_EmoEngineEventCreate
IEE_EmoEngineEventCreate.restype = c_void_p
eEvent = IEE_EmoEngineEventCreate()

IEE_EmoEngineEventGetEmoState = libEDK.IEE_EmoEngineEventGetEmoState
IEE_EmoEngineEventGetEmoState.argtypes = [c_void_p, c_void_p]
IEE_EmoEngineEventGetEmoState.restype = c_int

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

#userName    = "Your account name"
#password    = "Your password"

profileName = "EmotivProfile"
version     = -1    # Lastest version

option = c_int(0)
ready  = 0

userEngineID  = c_uint(0)
userEngineIDP = pointer(userEngineID)

userCloudID   = c_uint(0)
userCloudIDP  = pointer(userCloudID)

profileID     = c_int(0)
profileIDP    = pointer(profileID)

state         = c_int(0)

# -------------------------------------------------------------------------

print "==================================================================="
print "Example to saving and loading profile from Emotiv Cloud."
print "==================================================================="
print "Press '1' to saving profile to Emotiv Cloud"
print "Press '2' to loading profile from Emotiv Cloud"

# -------------------------------------------------------------------------

if libEDK.IEE_EngineConnect("Emotiv Systems-5") != 0:
    print "Emotiv Engine start up failed."

option = int(raw_input())

if libEDK.EC_Connect() != 0:
    print "Cannot connect to Emotiv Cloud"
    exit()

if libEDK.EC_Login(userName, password) != 0:
    print "Your login attempt has failed. The username or password may be incorrect"
    exit()

print "Logged in as %s" % userName

if libEDK.EC_GetUserDetail(userCloudIDP) != 0:
    exit()

while True:          
    state = libEDK.IEE_EngineGetNextEvent(eEvent)
    if state == 0:
        eventType = libEDK.IEE_EmoEngineEventGetType(eEvent)
        libEDK.IEE_EmoEngineEventGetUserId(eEvent, userEngineIDP)
        
        if eventType == 16:  # libEDK.IEE_Event_enum.IEE_UserAdded
            print "User ID: %d have added" % userEngineID.value
            ready = 1
            break
            
if ready == 1:
    getNumberProfile = libEDK.EC_GetAllProfileName(userCloudID.value)

    if option == 1: 
        getNumberProfile = libEDK.EC_GetAllProfileName(userCloudID.value)
        libEDK.EC_GetProfileId(userCloudID, profileName, profileIDP)       
                
        if profileID.value >= 0:
            print "Profile with %s is existed" % profileName
            if libEDK.EC_UpdateUserProfile(userCloudID.value, userEngineID.value, profileID.value, profileName) == 0:
                print "Updating finished"      
            else:
                print "Updating failed"
                if libEDK.EC_SaveUserProfile(userCloudID.value, userEngineID.value, profileName, 0) == 0:  # 0: libEDK.profileType.TRAINING
                    print "Saving finished"
                else:
                    print "Saving failed"                
                
    if option == 2:                                    
        if getNumberProfile > 0:
            profileID = libEDK.EC_ProfileIDAtIndex(userCloudID.value, 0)
            if libEDK.EC_LoadUserProfile(userCloudID.value, userEngineID.value, profileID, version) == 0:
                print "Loading finished"
            else:
                print "Loading failed"
        exit()
            
time.sleep(5)
# -------------------------------------------------------------------------
libEDK.IEE_EngineDisconnect()
libEDK.IEE_EmoStateFree(eState)
libEDK.IEE_EmoEngineEventFree(eEvent)
