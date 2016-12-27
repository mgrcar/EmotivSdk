import sys,os
import time
import ctypes
import msvcrt
from threading import Thread
  
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
 
write    = sys.stdout.write
eEvent   = libEDK.IEE_EmoEngineEventCreate()
eState   = libEDK.IEE_EmoStateCreate()
userID   = c_uint(0)
nSamples = c_uint(0)
nSam     = c_uint(0)
nSamplesTaken  = pointer(nSamples)
#da      = zeros(128,double)
data     = pointer(c_double(0))
user     = pointer(userID)
composerPort = c_uint(1726)
option   = c_int(0)
state    = c_int(0)
EmoState = c_int(0)
mental_state = c_int(0)
power    = c_float(0.0)
EngineEventType   = c_int(0)
ActiveActionsOut  = c_ulong(0)
pActiveActionsOut = pointer(ActiveActionsOut)
ActionOut  = c_long(0)
pActionOut = pointer(ActionOut)
TrainingActionOut   = c_int(0)
pTrainingActionOut  = pointer(TrainingActionOut)
NeutralSkillRating  = c_float(0)
pNeutralSkillRating = pointer(NeutralSkillRating)
PushSkillRating     = c_float(0)
pPushSkillRating    = pointer(NeutralSkillRating)
PullSkillRating     = c_float(0)
pPullSkillRating    = pointer(NeutralSkillRating)
NumContact          = 0
chargeLevel         = c_int()
maxChargeLevel      = c_int()

profileID     = c_int(0)
profileIDP    = pointer(profileID)

IS_MentalCommandGetCurrentActionPower = libEDK.IS_MentalCommandGetCurrentActionPower
IS_MentalCommandGetCurrentActionPower.restype = c_float
IS_MentalCommandGetCurrentActionPower.argtypes = [c_void_p]
 
IEE_MentalCommandAction_enum = ["","NEUTRAL","PUSH","","PULL"]
 
IEE_InputChannels_enum = ["IEE_CHAN_CMS",
                          "IEE_CHAN_DRL",
                          "IEE_CHAN_FP1",
                          "IEE_CHAN_AF3",
                          "IEE_CHAN_F7",
                          "IEE_CHAN_F3",
                          "IEE_CHAN_FC5",
                          "IEE_CHAN_T7",
                          "IEE_CHAN_P7",
                          "IEE_CHAN_Pz",
                          "IEE_CHAN_O1",
                          "IEE_CHAN_O2",
                          "IEE_CHAN_P8",
                          "IEE_CHAN_T8",
                          "IEE_CHAN_FC6",
                          "IEE_CHAN_F4",
                          "IEE_CHAN_F8",
                          "IEE_CHAN_AF4",
                          "IEE_CHAN_FP2"]
 
IEE_EEG_ContactQuality_enum = ["No Signal",
                              "Very Bad",
                              "Poor",
                              "Fair",
                              "Good"]
 
IEE_SignalStrength_enum = ["No Signal","Bad Signal","Good Signal"]

running = 0
#------------------------------------------------------------------------------------------------------------------------------------------------------------
 
def handleMentalCommandEvent(cognitiveEvent):
    userID = 0
    eventType = 0
    libEDK.IEE_EmoEngineEventGetUserId(cognitiveEvent, user)
    eventType = libEDK.IEE_MentalCommandEventGetType(cognitiveEvent)

    if eventType == 1: #IEE_MentalCommandTrainingStarted
        print "Mental Command Training for User", userID," STARTED!"
 
    elif eventType == 2: #IEE_MentalCommandTrainingSucceeded
        print "Mental Command Training for User", userID,"SUCCEEDED! Type \"training_accept\" or \"training_reject\""
 
    elif eventType == 3: #IEE_MentalCommandTrainingFailed
        print "Mental Command Training for User", userID," FAILED!"
 
    elif eventType == 4: #IEE_MentalCommandTrainingCompleted
        print "Mental Command Training for User", userID," COMPLETED!"
 
    elif eventType == 5: #IEE_MentalCommandTrainingDataErased
        print "Mental Command Training Data for User", userID," ERASED!"
 
    elif eventType == 6: #IEE_MentalCommandTrainingRejected
        print "Mental Command Training for User", userID," REJECTED!"
 
    elif eventType == 7: #IEE_MentalCommandTrainingReset
        print "Mental Command Training for User", userID," RESET!"
 
    elif eventType == 8: #IEE_MentalCommandTrainingAutoSamplingNeutralCompleted
        print "Mental Command Auto Sampling Neutral for User",userID," COMPLETED!"
 
    elif eventType == 9: #IEE_MentalCommandSignatureUpdated
        print "Mental Command Signature for User", userID," UPDATED!"
 
    elif eventType== 0: #IEE_MentalCommandNoEvent
        print "No Mental Command Event"   
    #time.sleep(.15)
 
    #return eventType
 
     
def parsecommand(inputs):    
    global profileID
    result = 1
    wrongargument = False
    commands = inputs.split()
 
    if commands[0] == "status":
        libEDK.IS_GetBatteryChargeLevel(eState, pointer(chargeLevel), pointer(maxChargeLevel))
        WirelessStrength = libEDK.IS_GetWirelessSignalStatus(eState)
        NumContact = libEDK.IS_GetNumContactQualityChannels(eState)
        print "Battery Level:", chargeLevel.value,"out of", maxChargeLevel.value
        print "Wireless Strength:", IEE_SignalStrength_enum[WirelessStrength]
        print "Number of Contact:",NumContact
        if NumContact != 0:
            for i in range(0,NumContact):
                 
                SignalCondition = libEDK.IS_GetContactQuality(eState, i)
                print IEE_InputChannels_enum[i],"\t :",IEE_EEG_ContactQuality_enum[SignalCondition]
                #print i, "\t", SignalCondition
        else:
            print "Headset Signal not Avaliable"
             
    elif commands[0] == "next":
        print "Moving onto Data Acquisition Mode!"
        wrongargument = False
 
    elif commands[0] == "quit":
        print "BYE!"
        wrongargument = False
 
    elif commands[0] == "help":
        print "Following step for training..."
        print "1. save_profile (create/save profile to Cloud) or load_profile "
        print "2. set_actions"
        print "3. training_action [neutral/push/pull]"
        print "4. training_start"
        print "5. training_accept or training_reject"
        print "Optional: training_erase, status, report"
        wrongargument = False
 
    elif commands[0] == "save_profile":
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
			
        exit()
                
    elif commands[0] == "load_profile":
    
        getNumberProfile = libEDK.EC_GetAllProfileName(userCloudID.value)
		
        if getNumberProfile > 0:
            profileID = libEDK.EC_ProfileIDAtIndex(userCloudID.value, 0)
            if libEDK.EC_LoadUserProfile(userCloudID.value, userEngineID.value, profileID, version) == 0:
                print "Loading finished"
            else:
                print "Loading failed"
                exit()

    elif commands[0] == "set_actions":
        if (libEDK.IEE_MentalCommandSetActiveActions(userID, 0x0002 | 0x0004)== 0): #pull = 0x0004
            print "PUSH and PULL set active for user", userID.value,"."             
        else:
            print "Activating actions FAILED!"
 
    elif commands[0] == "training_action": #Input Ex.: training_action push
        if len(commands) == 2:
            if commands[1] == "neutral":
                if libEDK.IEE_MentalCommandSetTrainingAction(userID, 0x0001) == 0:
                    print "Setting Mental Command Training Action NEUTRAL for user",userID.value,"!"
                    wrongargument = False
                else:
                    print" Failed to set Mental Command Training Action NEUTRAL for user",userID.value,"!"
 
            elif commands[1] == "push":
                if libEDK.IEE_MentalCommandSetTrainingAction(userID, 0x0002) == 0:
                    print "Setting Mental Command Training Action PUSH for user",userID.value,"!"
                    wrongargument = False
                else:
                    print" Failed to set Mental Command Training Action PUSH for user",userID.value,"!"
 
 
            elif commands[1] == "pull": 
                if libEDK.IEE_MentalCommandSetTrainingAction(userID, 0x0004) == 0:
                    print "Setting Mental Command Training Action PULL for user",userID.value,"!"
                    wrongargument = False
                else:
                    print" Failed to set Mental Command Training Action PULL for user",userID.value,"!"
 
            else:
                wrongargument = True
                 
        else:
            print "Action cannot be Trained!"
 
    elif commands[0] == "training_start": #Start Training
        if len(commands) == 1:
            if libEDK.IEE_MentalCommandSetTrainingControl(userID, 1) == 0: #MC_START
                libEDK.IEE_MentalCommandGetTrainingAction(userID, pTrainingActionOut)
                print "Training for", IEE_MentalCommandAction_enum[TrainingActionOut.value],"Start!"
            else:
                print "Training Failed"
        else:
            wrongargument = True
                 
    elif commands[0] == "training_accept": #Accept Training
        if len(commands) == 1:
            if libEDK.IEE_MentalCommandSetTrainingControl(userID, 2) == 0: #MC_ACCEPT
                libEDK.IEE_MentalCommandGetTrainingAction(userID, pTrainingActionOut)
                print "Accepting Training for",IEE_MentalCommandAction_enum[TrainingActionOut.value]
                #while handleMentalCommandEvent(eEvent)!= 4: #Wait for EmoEngine to Confirm Acceptance
                #    pass
                #wrongargument = False
            else:
                print "Training Acceptance Failed"
        else:
            wrongargument = True
             
    elif commands[0] == "training_reject": #Reject Training
        if len(commands) == 1:
            if libEDK.IEE_MentalCommandSetTrainingControl(userID, 3) == 0: #MC_REJECT
                libEDK.IEE_MentalCommandGetTrainingAction(userID, pTrainingActionOut)
                print "Rejecting training for",IEE_MentalCommandAction_enum[TrainingActionOut.value]
                #while handleMentalCommandEvent(eEvent)!= 6: #Wait for EmoEngine to Confirm Rejection
                 #   pass
                #wrongargument = False
            else:
                print "Training Reject Failed"
        else:
            wrongargument = True           
         
    elif commands[0] == "training_erase": #Erase Training Data
        if len(commands) == 1:
            if libEDK.IEE_MentalCommandSetTrainingControl(userID, 4) == 0: #MC_Erase
                if libEDK.IEE_MentalCommandGetTrainingAction(userID, pTrainingActionOut) != 0:
                    #while handleMentalCommandEvent(eEvent)!= 5: #Wait for EmoEngine to Confirm Erase
                     #  pass
                    #print "Training for ",IEE_MentalCommandAction_enum[TrainingActionOut.value]," Erased!"
                    wrongargument = False
            #else:
            #    print "Training Erase Failed"
        else:
            wrongargument = True
			
    elif commands[0] == "running":  
        global running      
        running = 1	
 
    elif commands[0] == "report":
        print "Report:"
        if libEDK.IEE_MentalCommandGetTrainedSignatureActions(userID, pActionOut) == 0:
            print "Trained Action:",ActionOut.value,"\t (Should be 7)"
        else:
            print "Error in Retrieving Trained Signature Actions"
        if libEDK.IEE_MentalCommandGetActionSkillRating(userID,0x0001,pNeutralSkillRating) == 0:
            print "Neutral Skill Rating:",NeutralSkillRating.value
        else:
            print "Error in Retrieving Neutral Skill Rating"
        if libEDK.IEE_MentalCommandGetActionSkillRating(userID,0x0002,pPushSkillRating) == 0:
            print "Push Skill Rating:",PushSkillRating.value
        else:
            print "Error in Retrieving Neutral Skill Rating"           
        if libEDK.IEE_MentalCommandGetActionSkillRating(userID,0x0004,pPullSkillRating) == 0:
            print "Pull Skill Rating:",PullSkillRating.value
        else:
            print "Error in Retrieving Neutral Skill Rating"
 
    else:
        print "Unknown Command", commands[0],"."
 
    if wrongargument == True:
        print "Wrong argument(s) for command ",commands,"."
 
    return
 
 
def kbhit():
    ''' Returns True if keyboard character was hit, False otherwise.
    '''
    if sys.platform.startswith('win32'):
        return msvcrt.kbhit()   
    else:
        dr,dw,de = select([sys.stdin], [], [], 0)
        return dr != []

def getcommand():
    while 1: 
        command = str(raw_input())
        command = command.lower()
        parsecommand(command)
		
# -------------------------------------------------------------------------

userName    = "Your account name"
password    = "Your password"

profileName = "EmotivProfile"
version     = -1    # Lastest version

userEngineID  = c_uint(0)
userEngineIDP = pointer(userEngineID)
userCloudID   = c_uint(0)
userCloudIDP  = pointer(userCloudID)

state         = c_int(0)

#---------------------------------------------------------------------------
input=''
print "========================================================================"
print "Example to show how to train Mental Commands from EmoEngine/EmoComposer."
print "========================================================================"
print "Press '1' to start and connect to the EmoEngine                         "
print "Press '2' to connect to the EmoComposer                                 "
print ">> "
#----------------------------------------------------------------------------

option = int(raw_input())

if option == 1:
    if libEDK.IEE_EngineConnect("Emotiv Systems-5") != 0:
        print "Emotiv Engine start up failed."
elif option == 2:
    if libEDK.IEE_EngineRemoteConnect("127.0.0.1", composerPort) != 0:
        print "Cannot connect to EmoComposer on"
else :
    print "option = ?"

if libEDK.EC_Connect() != 0:
    print "Cannot connect to Emotiv Cloud"
    exit()

if libEDK.EC_Login(userName, password) != 0:
    print "Your login attempt has failed. The username or password may be incorrect"
    exit()

print "Logged in as %s" % userName

if libEDK.EC_GetUserDetail(userCloudIDP) != 0:
    exit()
 
print "Enter your command. Enter \"help\" for commands"
print ">>"
 
thread1 = Thread(target = getcommand)
thread1.start()
 
while 1:
    state = libEDK.IEE_EngineGetNextEvent(eEvent)
    if state == 0: #EDK_OK
        EngineEventType = libEDK.IEE_EmoEngineEventGetType(eEvent)
        libEDK.IEE_EmoEngineEventGetUserId(eEvent, user)
        if EngineEventType == 16: #User Added
            print "New User", userID.value,"added"
 
        if EngineEventType == 32: #User Removed
            print "User ", userID.value," Removed"
 
        if EngineEventType == 64: #EmoState Updated
            libEDK.IEE_EmoEngineEventGetEmoState(eEvent, eState)
            mental_state = libEDK.IS_MentalCommandGetCurrentAction(eState)
            power = libEDK.IS_MentalCommandGetCurrentActionPower(eState)

            if running == 1:
               print "Mental State:",IEE_MentalCommandAction_enum[mental_state],"\t Power:",power
 
        if EngineEventType == 256: #MentalCommandEvent
            handleMentalCommandEvent(eEvent)

     
        time.sleep(.015)
		
#---------------------------------------------------------------------------------------------------------------------------------------------------------------
libEDK.IEE_EmoStateFree(eState)
libEDK.IEE_EmoEngineEventFree(eEvent)
libEDK.IEE_EngineDisconnect()