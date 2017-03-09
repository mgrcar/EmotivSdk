
% function [AllOK output_matrix nS] = eeglogger(rectime,acqtime,lib_flag_popup, plot_popup)
%1. Load library, see list of fuction in the SDK
%2. calllib : pass value in correct format-> you have to convert data in C,
% c++to matlab data type
%3. check dongle, headset
%4. Get data then save to csv file
% data structures, copied and pasted from epocmfile.m
%created by Binh Nguyen Emotiv; binh@emotiv.com
%%%%%%%%%%%%%%%%%%%%%%
w = warning ('off','all');
enuminfo.IEE_DataChannels_enum = struct('IED_COUNTER', 0, 'IED_INTERPOLATED', 1, 'IED_RAW_CQ', 2,'IED_AF3', 3, 'IED_F7',4, 'IED_F3', 5, 'IED_FC5', 6, 'IED_T7', 7,'IED_P7', 8, 'IED_Pz', 9,'IED_O2', 10, 'IED_P8', 11, 'IED_T8', 12, 'IED_FC6', 13, 'IED_F4', 14, 'IED_F8', 15, 'IED_AF4', 16, 'IED_GYROX', 17,'IED_GYROY', 18, 'IED_TIMESTAMP', 19,'IED_MARKER_HARDWARE', 20, 'IED_ES_TIMESTAMP',21, 'IED_FUNC_ID', 22, 'IED_FUNC_VALUE', 23, 'IED_MARKER', 24,'IED_SYNC_SIGNAL', 25);
enuminfo.IEE_MentalCommandTrainingControl_enum = struct('MC_NONE',0,'MC_START',1,'MC_ACCEPT',2,'MC_REJECT',3,'MC_ERASE',4,'MC_RESET',5);

DataChannels = enuminfo.IEE_DataChannels_enum;
DataChannelsNames = {'IED_COUNTER','IED_INTERPOLATED','IED_AF3','IED_T7','IED_Pz','IED_T8','IED_AF4','IED_GYROX','IED_GYROY','IED_TIMESTAMP','IED_ES_TIMESTAMP'};
DataChannelsNamesfull ={'IED_COUNTER','IED_INTERPOLATED','IED_RAW_CQ','IED_AF3','IED_F7','IED_F3','IED_FC5','IED_T7','IED_P7','IED_Pz','IED_O2','IED_P8','IED_T8','IED_FC6','IED_F4','IED_F8','IED_AF4','IED_GYROX','IED_GYROY','IED_TIMESTAMP','IED_MARKER_HARDWARE','IED_ES_TIMESTAMP','IED_FUNC_ID','IED_FUNC_VALUE','IED_MARKER','IED_SYNC_SIGNAL'};
clc
    loadlibrary('../../bin/win32/edk.dll','../../include/Iedk.h','addheader','IedkErrorCode.h','addheader','IEmoStateDLL.h','addheader','FacialExpressionDetection.h','addheader','MentalCommandDetection.h','addheader','IEmotivProfile.h','addheader','EmotivLicense.h','alias','libs'); 
    loadlibrary('../../bin/win32/edk.dll','../../include/IEegData.h','addheader','Iedk.h','alias','libD');

fid = fopen('eegloger.csv','wt');

strfull = 'IED_COUNTER;IED_INTERPOLATED;IED_RAW_CQ;IED_AF3;IED_F7;IED_F3;IED_FC5;IED_T7;IED_P7;IED_Pz;IED_O2;IED_P8;IED_T8;IED_FC6;IED_F4;IED_F8;IED_AF4;IED_GYROX;IED_GYROY;IED_TIMESTAMP;IED_MARKER_HARDWARE;IED_ES_TIMESTAMP;IED_FUNC_ID;IED_FUNC_VALUE;IED_MARKER;IED_SYNC_SIGNAL\n';
fprintf(fid,strfull);
fclose(fid);

eEvent = calllib('libs','IEE_EmoEngineEventCreate');%1
default = int8(['Emotiv Systems-5' 0]);
AllOK = calllib('libs','IEE_EngineConnect', default); % success means this value is 0;2
sampFreq = 128;
EDK_OK=0;
rectime=1;
hData = calllib('libD','IEE_DataCreate');
calllib('libD','IEE_DataSetBufferSizeInSec',rectime)
readytocollect = false;
cnt = 0;
acqtime=20;
% initialize outputs:


dataflag=false;
tic
mycolumn=numel(DataChannelsNamesfull);
TB=['Getting eeg Data in:',num2str(acqtime),' second...'];
disp(TB);
while (toc<acqtime)
    
    state = calllib('libs','IEE_EngineGetNextEvent',eEvent); % state = 0 if everything's OK
    
    if(state==EDK_OK)
        
        eventType = calllib('libs','IEE_EmoEngineEventGetType',eEvent);
        
        userID=libpointer('uint32Ptr',0);
        calllib('libs','IEE_EmoEngineEventGetUserId',eEvent, userID);
        
        
        
        if (strcmp(eventType,'IEE_UserAdded') == true)
            userID_value = get(userID,'value');
            calllib('libD','IEE_DataAcquisitionEnable',userID_value,true);      
            readytocollect = true;
          
        end
    end
    
    if(readytocollect)
        
        result =calllib('libD','IEE_DataUpdateHandle', 0, hData);
    
        if(result~=0)
          continue;
        end
        nSamples = libpointer('uint32Ptr',0);
        calllib('libD','IEE_DataGetNumberOfSample', hData, nSamples);
        nSamplesTaken = get(nSamples,'value');
          
        if (nSamplesTaken ~= 0)
            dataflag=true;
            data = libpointer('doublePtr', zeros(1, nSamplesTaken));
            data2=zeros(nSamplesTaken,numel(DataChannelsNames));
                           
                for i = 1:mycolumn
                    calllib('libD', 'IEE_DataGet', hData, DataChannels.([DataChannelsNamesfull{i}]), data, uint32(nSamplesTaken));
                    data_value = get(data,'value');
                    for k=1: nSamplesTaken
                        data2(k,i)=data_value(k);    
                      
                    end
                                  
                     disp(data_value);
                end
                  
                for i=1:nSamplesTaken
                    dlmwrite('eegloger.csv',data2(i,1:mycolumn),'-append','delimiter',';','precision','%.2f');

                end
         end            
        
    end
    
end
delete data2;
sampRateOutPtr = libpointer('uint32Ptr',0);
calllib('libD','IEE_DataGetSamplingRate',0,sampRateOutPtr);
calllib('libD','IEE_DataFree',hData);
filename = 'eegloger.csv';
delimiterIn = ';';
headerlinesIn = 1;

if(dataflag)
A = importdata(filename,delimiterIn,headerlinesIn);

Gyrox_col =18;
% tinh phuong sai do lech chuan
numcol=numel(A.data(:,Gyrox_col));
    figure;    
plot(0:1/sampFreq:(numel(A.data(:,Gyrox_col))-1)/sampFreq,A.data(:,Gyrox_col));
         xlabel('time (s)');
  %  plot([0:1/sampFreq:(end_time-1)/sampFreq],output_matrix(1:end_time,18))

axis([0 numcol/sampFreq 0 20000]);
   legend('GyroX','GyroZ');

end
if(dataflag==0)
disp('no data getted');
end
unloadlibrary libs
unloadlibrary libD
disp('finish');

