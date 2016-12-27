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
    print ("Error: cannot load EDK lib:", e)
    exit()

write = sys.stdout.write
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

IS_GetWirelessSignalStatus = libEDK.IS_GetWirelessSignalStatus
IS_GetWirelessSignalStatus.restype = c_int
IS_GetWirelessSignalStatus.argtypes = [c_void_p]

IS_FacialExpressionIsBlink = libEDK.IS_FacialExpressionIsBlink
IS_FacialExpressionIsBlink.restype = c_int
IS_FacialExpressionIsBlink.argtypes = [c_void_p]

IS_FacialExpressionIsLeftWink = libEDK.IS_FacialExpressionIsLeftWink
IS_FacialExpressionIsLeftWink.restype = c_int
IS_FacialExpressionIsLeftWink.argtypes = [c_void_p]

IS_FacialExpressionIsRightWink = libEDK.IS_FacialExpressionIsRightWink
IS_FacialExpressionIsRightWink.restype = c_int
IS_FacialExpressionIsRightWink.argtypes = [c_void_p]

IS_FacialExpressionGetUpperFaceAction =  \
    libEDK.IS_FacialExpressionGetUpperFaceAction
IS_FacialExpressionGetUpperFaceAction.restype = c_int
IS_FacialExpressionGetUpperFaceAction.argtypes = [c_void_p]

IS_FacialExpressionGetUpperFaceActionPower = \
    libEDK.IS_FacialExpressionGetUpperFaceActionPower
IS_FacialExpressionGetUpperFaceActionPower.restype = c_float
IS_FacialExpressionGetUpperFaceActionPower.argtypes = [c_void_p]

IS_FacialExpressionGetLowerFaceAction = \
    libEDK.IS_FacialExpressionGetLowerFaceAction
IS_FacialExpressionGetLowerFaceAction.restype = c_int
IS_FacialExpressionGetLowerFaceAction.argtypes = [c_void_p]

IS_FacialExpressionGetLowerFaceActionPower = \
    libEDK.IS_FacialExpressionGetLowerFaceActionPower
IS_FacialExpressionGetLowerFaceActionPower.restype = c_float
IS_FacialExpressionGetLowerFaceActionPower.argtypes = [c_void_p]

IS_FacialExpressionGetEyeLocation = libEDK.IS_FacialExpressionGetEyeLocation
IS_FacialExpressionGetEyeLocation.restype = c_float
IS_FacialExpressionGetEyeLocation.argtype = [c_void_p]

IS_FacialExpressionGetEyelidState = libEDK.IS_FacialExpressionGetEyelidState
IS_FacialExpressionGetEyelidState.restype = c_float
IS_FacialExpressionGetEyelidState.argtype = [c_void_p]

EyeX = c_float(0)
EyeY = c_float(0)
EyeLidLeft = c_float(0)
EyeLidRight = c_float(0)

X = pointer(EyeX)
Y = pointer(EyeY)
Left = pointer(EyeLidLeft)
Right = pointer(EyeLidRight)

#Perfomance metrics Model Parameters /long term excitement not present

RawScore = c_double(0)
MinScale = c_double(0)
MaxScale = c_double(0)

Raw = pointer(RawScore)
Min = pointer(MinScale)
Max = pointer(MaxScale)


# short term excitement
#IS_PerformanceMetricGetInstantaneousExcitementModelParams = libEDK.#IS_PerformanceMetricGetInstantaneousExcitementModelParams
#IS_PerformanceMetricGetInstantaneousExcitementModelParams.restype = c_void_p
#IS_PerformanceMetricGetInstantaneousExcitementModelParams.argtypes = [c_void_p]

# relaxation
IS_PerformanceMetricGetRelaxationModelParams = libEDK.IS_PerformanceMetricGetRelaxationModelParams
IS_PerformanceMetricGetRelaxationModelParams.restype = c_void_p
IS_PerformanceMetricGetRelaxationModelParams.argtypes = [c_void_p]

# stress
IS_PerformanceMetricGetStressModelParams = libEDK.IS_PerformanceMetricGetStressModelParams
IS_PerformanceMetricGetStressModelParams.restype = c_void_p
IS_PerformanceMetricGetStressModelParams.argtypes = [c_void_p]

# boredom/engagement
IS_PerformanceMetricGetEngagementBoredomModelParams = libEDK.IS_PerformanceMetricGetEngagementBoredomModelParams
IS_PerformanceMetricGetEngagementBoredomModelParams.restype = c_void_p
IS_PerformanceMetricGetEngagementBoredomModelParams.argtypes = [c_void_p]

# interest
IS_PerformanceMetricGetInterestModelParams = libEDK.IS_PerformanceMetricGetInterestModelParams
IS_PerformanceMetricGetInterestModelParams.restype = c_void_p
IS_PerformanceMetricGetInterestModelParams.argtypes = [c_void_p]

# focus
IS_PerformanceMetricGetFocusModelParams = libEDK.IS_PerformanceMetricGetFocusModelParams
IS_PerformanceMetricGetFocusModelParams.restype = c_void_p
IS_PerformanceMetricGetFocusModelParams.argtypes = [c_void_p]

#Perfomance metrics Normalized Scores

# long term excitement
IS_PerformanceMetricGetExcitementLongTermScore = libEDK.IS_PerformanceMetricGetExcitementLongTermScore
IS_PerformanceMetricGetExcitementLongTermScore.restype = c_float
IS_PerformanceMetricGetExcitementLongTermScore.argtypes = [c_void_p]

# short term excitement
IS_PerformanceMetricGetInstantaneousExcitementScore = libEDK.IS_PerformanceMetricGetInstantaneousExcitementScore
IS_PerformanceMetricGetInstantaneousExcitementScore.restype = c_float
IS_PerformanceMetricGetInstantaneousExcitementScore.argtypes = [c_void_p]

# relaxation
IS_PerformanceMetricGetRelaxationScore = libEDK.IS_PerformanceMetricGetRelaxationScore
IS_PerformanceMetricGetRelaxationScore.restype = c_float
IS_PerformanceMetricGetRelaxationScore.argtypes = [c_void_p]

# stress
IS_PerformanceMetricGetStressScore = libEDK.IS_PerformanceMetricGetStressScore
IS_PerformanceMetricGetStressScore.restype = c_float
IS_PerformanceMetricGetStressScore.argtypes = [c_void_p]

# boredom/engagement
IS_PerformanceMetricGetEngagementBoredomScore = libEDK.IS_PerformanceMetricGetEngagementBoredomScore
IS_PerformanceMetricGetEngagementBoredomScore.restype = c_float
IS_PerformanceMetricGetEngagementBoredomScore.argtypes = [c_void_p]

# interest
IS_PerformanceMetricGetInterestScore = libEDK.IS_PerformanceMetricGetInterestScore
IS_PerformanceMetricGetInterestScore.restype = c_float
IS_PerformanceMetricGetInterestScore.argtypes = [c_void_p]

# focus
IS_PerformanceMetricGetFocusScore = libEDK.IS_PerformanceMetricGetFocusScore
IS_PerformanceMetricGetFocusScore.restype = c_float
IS_PerformanceMetricGetFocusScore.argtypes = [c_void_p]

# -------------------------------------------------------------------------

def logEmoState(userID, eState):
    print >>f, IS_GetTimeFromStart(eState), ",",
    print >>f, userID.value, ",",
    print >>f, IS_GetWirelessSignalStatus(eState), ",",
    print >>f, IS_FacialExpressionIsBlink(eState), ",",
    print >>f, IS_FacialExpressionIsLeftWink(eState), ",",
    print >>f, IS_FacialExpressionIsRightWink(eState), ",",

    FacialExpressionStates = {}
    FacialExpressionStates[FE_FROWN] = 0
    FacialExpressionStates[FE_SUPPRISE] = 0
    FacialExpressionStates[FE_SMILE] = 0
    FacialExpressionStates[FE_CLENCH] = 0

    upperFaceAction = IS_FacialExpressionGetUpperFaceAction(eState)
    upperFacePower = IS_FacialExpressionGetUpperFaceActionPower(eState)
    lowerFaceAction = IS_FacialExpressionGetLowerFaceAction(eState)
    lowerFacePower = IS_FacialExpressionGetLowerFaceActionPower(eState)
    FacialExpressionStates[upperFaceAction] = upperFacePower
    FacialExpressionStates[lowerFaceAction] = lowerFacePower

    print >>f, FacialExpressionStates[FE_SUPPRISE], ",",
    print >>f, FacialExpressionStates[FE_FROWN], ",",
    print >>f, FacialExpressionStates[FE_SMILE], ",",
    print >>f, FacialExpressionStates[FE_CLENCH], ",",

    IS_FacialExpressionGetEyeLocation(eState,X,Y)
    print >>f, EyeX.value, ",",
    print >>f, EyeY.value, ",",
    IS_FacialExpressionGetEyeLocation(eState,Left,Right)
    print >>f, EyeLidLeft.value, ",",
    print >>f, EyeLidRight.value, ",",

    # Performance metrics Suite results
    print >>f, IS_PerformanceMetricGetExcitementLongTermScore(eState), ",",
    print >>f, IS_PerformanceMetricGetInstantaneousExcitementScore(eState), ",",    
    #IS_PerformanceMetricGetInstantaneousExcitementModelParams(eState, Raw, Min, Max)
    print >>f, RawScore.value, ",",
    print >>f, MinScale.value, ",",
    print >>f, MaxScale.value, ",",
    print >>f, IS_PerformanceMetricGetStressScore(eState), ",",
    IS_PerformanceMetricGetStressModelParams(eState, Raw, Min, Max)
    print >>f, RawScore.value, ",",
    print >>f, MinScale.value, ",",
    print >>f, MaxScale.value, ",",
    print >>f, IS_PerformanceMetricGetRelaxationScore(eState), ",",
    IS_PerformanceMetricGetRelaxationModelParams(eState, Raw, Min, Max)
    print >>f, RawScore.value, ",",
    print >>f, MinScale.value, ",",
    print >>f, MaxScale.value, ",",
    print >>f, IS_PerformanceMetricGetEngagementBoredomScore(eState), ",",
    IS_PerformanceMetricGetEngagementBoredomModelParams(eState, Raw, Min, Max)
    print >>f, RawScore.value, ",",
    print >>f, MinScale.value, ",",
    print >>f, MaxScale.value, ",",
    print >>f, IS_PerformanceMetricGetInterestScore(eState), ",",
    IS_PerformanceMetricGetInterestModelParams(eState, Raw, Min, Max)
    print >>f, RawScore.value, ",",
    print >>f, MinScale.value, ",",
    print >>f, MaxScale.value, ",",
    print >>f, IS_PerformanceMetricGetFocusScore(eState), ",",
    IS_PerformanceMetricGetFocusModelParams(eState, Raw, Min, Max)
    print >>f, RawScore.value, ",",
    print >>f, MinScale.value, ",",
    print >>f, MaxScale.value, ",",
    print >>f, '\n',
    
def kbhit():
    ''' Returns True if keyboard character was hit, False otherwise.
    '''
    if sys.platform.startswith('win32'):
        return msvcrt.kbhit()   
    else:
        dr,dw,de = select([sys.stdin], [], [], 0)
        return dr != []

# -------------------------------------------------------------------------

userID = c_uint(0)
user   = pointer(userID)
option = c_int(0)
state  = c_int(0)
composerPort = c_uint(1726)
timestamp    = c_float(0.0)

FE_SUPPRISE = 0x0020 
FE_FROWN    = 0x0040
FE_SMILE    = 0x0080
FE_CLENCH   = 0x0100

PM_EXCITEMENT = 0x0001,
PM_RELAXATION = 0x0002,
PM_STRESS     = 0x0004,
PM_ENGAGEMENT = 0x0008,

PM_INTEREST   = 0x0010,
PM_FOCUS      = 0x0020

# -------------------------------------------------------------------------
header = ['Time', 'UserID', 'Wireless Signal Status', 'Blink', 'Wink Left', 'Wink Right',
          'Surprise', 'Furrow', 'Smile', 'Clench',
          'EyeLocationHoriz', 'EyeLocationVert','EyelidStateLeft', 'EyelidStateRight', 'LongTermExcitementRawNorm', 
          'ShortTermExcitementRawNorm','ShortTermExcitementRaw', 'ShortTermExcitementMin', 'ShortTermExcitementMax',
          'RelaxationRawNorm','RelaxationRaw','RelaxationMin','RelaxationMax',
          'StressRawNorm','StressRaw','StressMin','StressMax', 'EngagementRawNorm','EngagementRaw', 'EngagementMin','EngagementMax',
          'InterestRawNorm','InterestRaw', 'InterestMin', 'InterestMax','FocusRawNorm','FocusRaw','FocusMin','FocusMax']

input = ''
print "==================================================================="
print "Example to show how to log EmoState from EmoEngine/EmoComposer."
print "==================================================================="
print "Press '1' to start and connect to the EmoEngine                    "
print "Press '2' to connect to the EmoComposer                            "
print ">> "


# -------------------------------------------------------------------------

option = int(raw_input())

if option == 1:
    if libEDK.IEE_EngineConnect("Emotiv Systems-5") != 0:
        print "Emotiv Engine start up failed."
elif option == 2:
    if libEDK.IEE_EngineRemoteConnect("127.0.0.1", composerPort) != 0:
        print "Cannot connect to EmoComposer on"
else:
    print "option = ?"


print "Start receiving Emostate! Press any key to stop logging...\n"
f = file('ES.csv', 'w')
f = open('ES.csv', 'w')
print >> f, header

while (1):
    
    if kbhit():
        break
    
    state = libEDK.IEE_EngineGetNextEvent(eEvent)
    if state == 0:
        eventType = libEDK.IEE_EmoEngineEventGetType(eEvent)
        libEDK.IEE_EmoEngineEventGetUserId(eEvent, user)
        if eventType == 64:  # libEDK.IEE_Event_enum.IEE_EmoStateUpdated
            libEDK.IEE_EmoEngineEventGetEmoState(eEvent, eState)
            timestamp = IS_GetTimeFromStart(eState)
            print "%10.3f New EmoState from user %d ...\r" % (timestamp,
                                                              userID.value)
            logEmoState(userID, eState)
    elif state != 0x0600:
        print "Internal error in Emotiv Engine ! "
    time.sleep(0.1)
# -------------------------------------------------------------------------
f.close()
libEDK.IEE_EngineDisconnect()
libEDK.IEE_EmoStateFree(eState)
libEDK.IEE_EmoEngineEventFree(eEvent)