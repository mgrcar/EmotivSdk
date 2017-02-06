import sys,os
import time
import ctypes
 
from ctypes import cdll
from ctypes import CDLL
from ctypes import c_int
from ctypes import c_uint
from ctypes import pointer
from ctypes import c_char_p
from ctypes import c_float
from ctypes import c_double
from ctypes import byref

try :
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
except :
    print 'Error : cannot load dll lib' 
    
IED_COUNTER = 0      
IED_INTERPOLATED = 1       
IED_RAW_CQ = 2     
IED_AF3 = 3            
IED_T7 = 7             
IED_Pz = 9              
IED_T8 = 12            
IED_AF4 = 16           
IED_TIMESTAMP = 19     
IED_ES_TIMESTAMP = 20      
IED_FUNC_ID = 21   
IED_FUNC_VALUE = 22        
IED_MARKER = 23     
IED_SYNC_SIGNAL = 24         

targetChannelList = [IED_COUNTER,IED_INTERPOLATED,IED_RAW_CQ,IED_AF3,IED_T7,IED_Pz,IED_T8,IED_AF4,IED_TIMESTAMP,IED_ES_TIMESTAMP,IED_FUNC_ID,IED_FUNC_VALUE,IED_MARKER,IED_SYNC_SIGNAL]
header = ['COUNTER','INTERPOLATED','RAW_CQ','AF3','T7','Pz','T8','AF4','TIMESTAMP','ES_TIMESTAMP','FUNC_ID','FUNC_VALUE','MARKER','SYNC_SIGNAL']

write = sys.stdout.write
eEvent      = libEDK.IEE_EmoEngineEventCreate()
eState      = libEDK.IEE_EmoStateCreate()
userID            = c_uint(0)
nSamples   = c_uint(0)
nSam       = c_uint(0)
nSamplesTaken  = pointer(nSamples)
#da = zeros(128,double)
data     = pointer(c_double(0))
user                    = pointer(userID)
composerPort          = c_uint(1726)
secs      = c_float(1)
datarate    = c_uint(0)
readytocollect    = False
option      = c_int(0)
state     = c_int(0)

input=''
print "==================================================================="
print "Example to show how to log EEG Data from EmoEngine/EmoComposer."
print "==================================================================="
print "Press '1' to start and connect to the EmoEngine                    "
print "Press '2' to connect to the EmoComposer                            "
print ">> "
#------------------------------------------------------------------------------------------------------------------------------------------------------------
option = int(raw_input())


if option == 1:
    print libEDK.IEE_EngineConnect("Emotiv Systems-5")
    if libEDK.IEE_EngineConnect("Emotiv Systems-5") != 0:
        print "Emotiv Engine start up failed."
elif option == 2:
    if libEDK.IEE_EngineRemoteConnect("127.0.0.1", composerPort) != 0:
        print "Cannot connect to EmoComposer on"
else :
    print "option = ?"
    
print "Start receiving EEG Data! Press any key to stop logging...\n"
f = file('EEG.csv', 'w')
f = open('EEG.csv', 'w')
print >> f,header
    
hData = libEDK.IEE_DataCreate()
libEDK.IEE_DataSetBufferSizeInSec(secs)

print "Buffer size in secs:"
while (1):
    state = libEDK.IEE_EngineGetNextEvent(eEvent)
    if state == 0:
        eventType = libEDK.IEE_EmoEngineEventGetType(eEvent)
        libEDK.IEE_EmoEngineEventGetUserId(eEvent, user)
        if eventType == 16: #libEDK.IEE_Event_enum.IEE_UserAdded:
            print "User added"
            libEDK.IEE_DataAcquisitionEnable(userID,True)
            readytocollect = True

    if readytocollect==True:    
        libEDK.IEE_DataUpdateHandle(0, hData)
        libEDK.IEE_DataGetNumberOfSample(hData,nSamplesTaken)
        print "Updated :",nSamplesTaken[0]
        if nSamplesTaken[0] != 0:
            nSam=nSamplesTaken[0]
            arr=(ctypes.c_double*nSamplesTaken[0])()
            ctypes.cast(arr, ctypes.POINTER(ctypes.c_double))
            for sampleIdx in range(nSamplesTaken[0]): 
                for i in range(14): 
                    libEDK.IEE_DataGet(hData,targetChannelList[i],byref(arr), nSam)
                    print >>f,arr[sampleIdx],",",
                print >>f,'\n'
    time.sleep(1)
libEDK.IEE_DataFree(hData)

#---------------------------------------------------------------------------------------------------------------------------------------------------------------
libEDK.IEE_EngineDisconnect()
libEDK.IEE_EmoStateFree(eState)
libEDK.IEE_EmoEngineEventFree(eEvent)
