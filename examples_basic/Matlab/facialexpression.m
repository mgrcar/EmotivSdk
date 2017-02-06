% this file facialexpression.m shows how to call expressiv functions in matlab. 
% The result is displayed on Command Window ! Ctrl-C to terminate.

structs.InputSensorDescriptor_struct.members = struct('channelId', 'IEE_InputChannels_enum', 'fExists', 'int32', 'pszLabel', 'cstring', 'xLoc', 'double', 'yLoc', 'double', 'zLoc', 'double');
enuminfo.IEE_DataChannels_enum = struct('IED_COUNTER', 0, 'IED_INTERPOLATED', 1, 'IED_RAW_CQ', 2,'IED_AF3', 3, 'IED_F7',4, 'IED_F3', 5, 'IED_FC5', 6, 'IED_T7', 7,'IED_P7', 8, 'IED_Pz', 9,'IED_O2', 10, 'IED_P8', 11, 'IED_T8', 12, 'IED_FC6', 13, 'IED_F4', 14, 'IED_F8', 15, 'IED_AF4', 16, 'IED_GYROX', 17,'IED_GYROY', 18, 'IED_TIMESTAMP', 19, 'IED_ES_TIMESTAMP', 20, 'IED_FUNC_ID', 21, 'IED_FUNC_VALUE', 22, 'IED_MARKER', 23,'IED_SYNC_SIGNAL', 24);
enuminfo.IEE_MentalCommandTrainingControl_enum = struct('MC_NONE',0,'MC_START',1,'MC_ACCEPT',2,'MC_REJECT',3,'MC_ERASE',4,'MC_RESET',5);
enuminfo.IEE_FacialExpressionAlgo_enum = struct('FE_NEUTRAL',1,'FE_BLINK',2,'FE_WINK_LEFT',4,'FE_WINK_RIGHT',8,'FE_HORIEYE',16,'FE_SURPRISE',32,'FE_FROWN',64,'FE_SMILE',128,'FE_CLENCH',256, 'FE_LAUGH', 512, 'FE_SMIRK_LEFT', 1024, 'FE_SMIRK_RIGHT', 2048);
enuminfo.IEE_FacialExpressionTrainingControl_enum = struct('FE_NONE',0,'FE_START',1,'FE_ACCEPT',2,'FE_REJECT',3,'FE_ERASE',4,'FE_RESET',5);
enuminfo.IEE_FacialExpressionThreshold_enum = struct('FE_SENSITIVITY',0);
enuminfo.IEE_MentalCommandEvent_enum = struct('IEE_MentalCommandNoEvent',0,'IEE_MentalCommandTrainingStarted',1,'IEE_MentalCommandTrainingSucceeded',2,'IEE_MentalCommandTrainingFailed',3,'IEE_MentalCommandTrainingCompleted',4,'IEE_MentalCommandTrainingDataErased',5,'IEE_MentalCommandTrainingRejected',6,'IEE_MentalCommandTrainingReset',7,'IEE_MentalCommandAutoSamplingNeutralCompleted',8,'IEE_MentalCommandSignatureUpdated',9);
enuminfo.IEE_EmotivSuite_enum = struct('IEE_FACIALEXPRESSION',0,'IEE_PERFORMANCEMETRIC',1,'IEE_MENTALCOMMAND',2);
enuminfo.IEE_FacialExpressionEvent_enum = struct('IEE_FacialExpressionNoEvent',0,'IEE_FacialExpressionTrainingStarted',1,'IEE_FacialExpressionTrainingSucceeded',2,'IEE_FacialExpressionTrainingFailed',3,'IEE_FacialExpressionTrainingCompleted',4,'IEE_FacialExpressionTrainingDataErased',5,'IEE_FacialExpressionTrainingRejected',6,'IEE_FacialExpressionTrainingReset',7);
enuminfo.IEE_MentalCommandAction_enum = struct('MC_NEUTRAL',1,'MC_PUSH',2,'MC_PULL',4,'MC_LIFT',8,'MC_DROP',16,'MC_LEFT',32,'MC_RIGHT',64,'MC_ROTATE_LEFT',128,'MC_ROTATE_RIGHT',256,'MC_ROTATE_CLOCKWISE',512,'MC_ROTATE_COUNTER_CLOCKWISE',1024,'MC_ROTATE_FORWARDS',2048,'MC_ROTATE_REVERSE',4096,'MC_DISAPPEAR',8192);
enuminfo.IEE_InputChannels_enum = struct('IEE_CHAN_CMS', 0, 'IEE_CHAN_DRL', 1, 'IEE_CHAN_FP1', 2,'IEE_CHAN_AF3', 3, 'IEE_CHAN_F7',4, 'IEE_CHAN_F3', 5, 'IEE_CHAN_FC5', 6, 'IEE_CHAN_T7', 7,'IEE_CHAN_P7', 8, 'IEE_CHAN_Pz', 9,'IEE_CHAN_O2', 10, 'IEE_CHAN_P8', 11, 'IEE_CHAN_T8', 12, 'IEE_CHAN_FC6', 13, 'IEE_CHAN_F4', 14, 'IEE_CHAN_F8', 15, 'IEE_CHAN_AF4', 16, 'IEE_CHAN_FP2', 17);
enuminfo.IEE_FacialExpressionSignature_enum = struct('FE_SIG_UNIVERSAL',0,'FE_SIG_TRAINED',1);
enuminfo.IEE_Event_enum = struct('IEE_UnknownEvent',0,'IEE_EmulatorError',1,'IEE_ReservedEvent',2,'IEE_UserAdded',16,'IEE_UserRemoved',32,'IEE_EmoStateUpdated',64,'IEE_ProfileEvent',128,'IEE_MentalCommandEvent',256,'IEE_FacialExpressionEvent',512,'IEE_InternalStateChanged',1024,'IEE_AllEvent',2032);

% Check to see if library was already loaded
if ~libisloaded('edk')    
    [nf, w] = loadlibrary('../../bin/win32/edk','Iedk',  'addheader', '../../bin/win32/IEmoStateDLL', 'addheader', '../../bin/win32/IedkErrorCode'); 
	disp(['EDK library loaded']);    
else
    disp(['EDK library already loaded']);
    end

libfunctionsview('edk');

default = int8(['Emotiv Systems-5' 0]);
szhost = int8(['127.0.0.1' 0]);
AllOK = calllib('edk','IEE_EngineConnect', default); % success means this value is 0

eEvent = calllib('edk','IEE_EmoEngineEventCreate');
eState = calllib('edk','IEE_EmoStateCreate');

count = 0;
 while ((AllOK == 0) && (count<9999))
    state = calllib('edk','IEE_EngineGetNextEvent',eEvent); % state = 0 if everything's OK
    eventType = calllib('edk','IEE_EmoEngineEventGetType',eEvent);
    %disp(eventType);
    userID=libpointer('uint32Ptr',0);    
    calllib('edk','IEE_EmoEngineEventGetUserId',eEvent, userID);

    if strcmp(eventType,'IEE_EmoStateUpdated') == true        
        
        emoState = calllib('edk','IEE_EmoEngineEventGetEmoState',eEvent,eState);
        if emoState == 0            
            ExpressIsActiv = calllib('edk','IS_FacialExpressionIsBlink',eState)
            smile = calllib('edk','IS_FacialExpressionGetSmileExtent',eState)
            clench = calllib('edk','IS_FacialExpressionGetClenchExtent',eState)
            isBlink = calllib('edk','IS_FacialExpressionIsBlink',eState)
        end    	
    end   
    count = count + 1;
 end

calllib('edk','IEE_EngineDisconnect')
calllib('edk','IEE_EmoStateFree',eState);
calllib('edk','IEE_EmoEngineEventFree',eEvent);


