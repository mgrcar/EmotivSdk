function [AllOK output_matrix nS] = eeglogger(rectime,varargin)
% function [AllOK output_matrix nS] = eeglogger(rectime,acqtime,lib_flag_popup, plot_popup)
%
%
% Francesco Tenore, JHU/APL - April 2016
% 
% Copyright ? April 2016 The Johns Hopkins University Applied Physics 
% Laboratory (JHU/APL).  All Rights Reserved.
% JHU/APL PROVIDES THIS SOFTWARE "AS IS" WITHOUT WARRANTY OF ANY KIND 
% AND IS NOT LIABLE FOR ANY DAMAGES OF ANY KIND ARISING FROM THE USE 
% OF THIS SOFTWARE.

%
% This function uses the Emotiv headset dll (edk.dll) to acquire the 
% data contained in the IEE_DataChannels_enum structure. The function mimics 
% the EEGLogger.exe function that can be compiled and used to acquire the
% data using a C++ compiler.
% Additionally, it checks to make sure that the library hasn't been loaded
% yet. The function requires no inputs (4 optional) and produces 3 outputs
%
% Optional Inputs
% rectime: this is time, in seconds, of the data buffer size, default = 1
% acqtime: acquisition time, in seconds, default = 10 (for testing).
% lib_flag_popup: 1 = activates the libfunctionsview window, useful for 
% looking at all the functions that were loaded from the dll. default = 0.
% plot_popup: 1 = plots the GyroX and GyroY signals after the data was
% recorded; 0 = no plot, default = 1.
%
% Outputs:
% AllOK: Everything worked fine in loading the library. If this is the case
% a ZERO (0) should be returned.
% output_matrix: a 25 (=length(EE_DataChannels_enum) by n matrix where n =
% sampling_frequency * acquisition time. The sampling_frequency, as defined
% by Emotiv, is effectively 128 Hz.
% nS: provides you with the number of samples acquired (equivalent to nSamplesTaken
% in the EEGLogger main.cpp function).

% data structures, copied and pasted from epocmfile.m
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

DataChannels = enuminfo.IEE_DataChannels_enum;
DataChannelsNames = {'IED_COUNTER','IED_INTERPOLATED','IED_AF3','IED_T7','IED_Pz','IED_T8','IED_AF4','IED_GYROX','IED_GYROY','IED_TIMESTAMP','IED_ES_TIMESTAMP'};



optargin = size(varargin,2);
rectime = 1;
acqtime = 10;
lib_flag_popup = 1;
plot_popup = 1;

if optargin > 4
    error('Too many inputs');
    
elseif optargin == 4
	rectime = varargin{1};
    acqtime = varargin{2};
    lib_flag_popup = varargin{3};
    plot_popup = varargin{4};
elseif optargin == 3
	rectime = varargin{1};
    acqtime = varargin{2};
    lib_flag_popup = varargin{3};
elseif optargin == 2    
	rectime = varargin{1};
    acqtime = varargin{2};
elseif optargin == 1
    rectime = varargin{1};
end
% Check to see if library was already loaded
if ~libisloaded('edk')    
    [nf, w] = loadlibrary('../../bin/win32/edk','Iedk',  'addheader', '../../bin/win32/IEmoStateDLL', 'addheader', '../../bin/win32/IedkErrorCode'); 
	disp(['EDK library loaded']);
    if( lib_flag_popup )
        libfunctionsview('edk')
        nf % these should be empty if all went well
        w
    end
else
    disp(['EDK library already loaded']);
end
sampFreq = 128;
default = int8(['Emotiv Systems-5' 0]);
AllOK = calllib('edk','IEE_EngineConnect', default); % success means this value is 0

hData = calllib('edk','IEE_DataCreate');
calllib('edk','IEE_DataSetBufferSizeInSec',rectime);
eEvent = calllib('edk','IEE_EmoEngineEventCreate');
readytocollect = false;
cnt = 0;


% initialize outputs:
output_matrix = zeros(acqtime*sampFreq,length(DataChannelsNames));
nS = zeros(acqtime*sampFreq,1);

% For this next part, see the eeglogger main.cpp file for a better
% understanding of what's happening here.

tic
while(toc < acqtime)
    state = calllib('edk','IEE_EngineGetNextEvent',eEvent); % state = 0 if everything's OK
    eventType = calllib('edk','IEE_EmoEngineEventGetType',eEvent);
    %disp(eventType);
    userID=libpointer('uint32Ptr',0);
    calllib('edk','IEE_EmoEngineEventGetUserId',eEvent, userID);


    if strcmp(eventType,'IEE_UserAdded') == true
        User_added = 1;
        userID_value = get(userID,'value');
        calllib('edk','IEE_DataAcquisitionEnable',userID_value,true);
    	readytocollect = true;
    end
	
	if (readytocollect) 
						
    	calllib('edk','IEE_DataUpdateHandle', 0, hData);
    	nSamples = libpointer('uint32Ptr',0);
        calllib('edk','IEE_DataGetNumberOfSample', hData, nSamples);
        nSamplesTaken = get(nSamples,'value') ;
        if (nSamplesTaken ~= 0)
        	data = libpointer('doublePtr', zeros(1, nSamplesTaken));
                %for sampleIdx=1:nSamplesTaken
            	for i = 1:length(fieldnames(enuminfo.IEE_DataChannels_enum))
                	calllib('edk', 'IEE_DataGet', hData, DataChannels.([DataChannelsNames{i}]), data, uint32(nSamplesTaken));
                    data_value = get(data,'value');
                    %output_matrix(cnt+1,i) = data_value(sampleIdx);                                         
                    output_matrix(cnt+1:cnt+length(data_value),i) = data_value;                    
                end	                
                nS(cnt+1) = nSamplesTaken;
                %cnt = cnt + 1;
                cnt = cnt + length(data_value);
            %end
        end
	end
    %pause(0.1);    % haven't played with this much...
   
end

sampRateOutPtr = libpointer('uint32Ptr',0);
calllib('edk','IEE_DataGetSamplingRate',0,sampRateOutPtr);
sampFreqOut = get(sampRateOutPtr,'value') % in Hz

calllib('edk','IEE_DataFree',hData);
end_time = find(output_matrix(:,20)==0,1) - 1;
if plot_popup
    figure;    
    plot([0:1/sampFreq:(end_time-1)/sampFreq],output_matrix(1:end_time,18))
    hold on
    plot([0:1/sampFreq:(end_time-1)/sampFreq],output_matrix(1:end_time,19),'r')
    xlabel('time (s)')
    title('GyroX GyroZ values') % I call it gyroZ instead of gyroY ...
                                % because if I move the headset up and ...
                                % down it tracks.
    legend('GyroX','GyroZ');
    figure;
    plot3([0:1/sampFreq:(end_time-1)/sampFreq],output_matrix(1:end_time,18),output_matrix(1:end_time,19))
    xlabel('time (s)')
    ylabel('X coord')
    zlabel('Y coord')
end
calllib('edk','IEE_EngineDisconnect');
calllib('edk','IEE_EmoEngineEventFree',eEvent);

% unloadlibrary('edk'); % unload the library after having turned off 
% [int32, uint32Ptr] EE_DataGetSamplingRate (uint32, uint32Ptr)
% int32 EE_DataSetSychronizationSignal (uint32, int32)
% [int32, string] EE_EnableDiagnostics (cstring, int32, int32)

