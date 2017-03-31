%  ** Copyright 2017 by Emotiv. All rights reserved
%  ** Example - MotionDataLogger
%  ** This example demonstrates how to extract live Motion data using the EmoEngineTM
%  ** Data is read from the headset and sent to an output file for
%  ** later analysis
%  ** check result file :MotionDataLogger.csv
%   created by Binh Nguyen Emotiv; binh@emotiv.com
%%%%%%%%%%%%%%%%%%%%%%
w = warning ('off','all');
enuminfo.IEE_MotionDataChannels_enum = struct('IMD_COUNTER',0,'IMD_GYROX',1,'IMD_GYROY',2,'IMD_GYROZ',3,'IMD_ACCX',4,'IMD_ACCY',5,'IMD_ACCZ',6,'IMD_MAGX',7,'IMD_MAGY',8,'IMD_MAGZ',9,'IMD_TIMESTAMP',10);
MotionDataChannels = enuminfo.IEE_MotionDataChannels_enum;
MotionDataChannelsNamesfull ={'IMD_COUNTER','IMD_GYROX','IMD_GYROY','IMD_GYROZ','IMD_ACCX','IMD_ACCY','IMD_ACCZ','IMD_MAGX','IMD_MAGY','IMD_MAGZ','IMD_TIMESTAMP'};
clc
    loadlibrary('../../bin/win32/edk.dll','../../include/Iedk.h','addheader','IedkErrorCode.h','addheader','IEmoStateDLL.h','addheader','FacialExpressionDetection.h','addheader','MentalCommandDetection.h','addheader','IEmotivProfile.h','addheader','EmotivLicense.h','alias','libs'); 
    loadlibrary('../../bin/win32/edk.dll','../../include/IEegData.h','addheader','Iedk.h','alias','libD');
filename='MotionDatalogger.csv';
fid = fopen(filename,'wt');

strfull = 'IMD_COUNTER,IMD_GYROX,IMD_GYROY,IMD_GYROZ,IMD_ACCX,IMD_ACCY,IMD_ACCZ,IMD_MAGX,IMD_MAGY,IMD_MAGZ,IMD_TIMESTAMP\n';
fprintf(fid,strfull);
fclose(fid);

eEvent = calllib('libs','IEE_EmoEngineEventCreate');%1
eState = calllib('libs','IEE_EmoStateCreate');
default = int8(['Emotiv Systems-5' 0]);
AllOK = calllib('libs','IEE_EngineConnect', default); % success means this value is 0;2
sampFreq = 128;
EDK_OK=0;
rectime=1;
hMotionData = calllib('libD','IEE_MotionDataCreate');
calllib('libD','IEE_DataSetBufferSizeInSec',rectime);
readytocollect = false;
cnt = 0;
acqtime=20;% run in 20 second
sl_col=5;% select col , in this case-> select AccX;
tic
mycolumn=numel(MotionDataChannelsNamesfull);
dataflag=false;
TB=['Getting Motion Data in:',num2str(acqtime),' second...'];
disp(TB);
while (toc<acqtime)
    
    state = calllib('libs','IEE_EngineGetNextEvent',eEvent); % state = 0 if everything's OK
    
    if(state==EDK_OK)
        
        eventType = calllib('libs','IEE_EmoEngineEventGetType',eEvent);
        
        if (strcmp(eventType,'IEE_UserAdded') == true)
            readytocollect = true;
        end
    end
    
    if(readytocollect)
        
        result =calllib('libD','IEE_MotionDataUpdateHandle', 0, hMotionData);
    
        if(result~=0)
          continue;
        end
        nSamples = libpointer('uint32Ptr',0);
        calllib('libD','IEE_MotionDataGetNumberOfSample',hMotionData, nSamples);
        nSamplesTaken = get(nSamples,'value');
          
        if (nSamplesTaken ~= 0)
            disp('Hello');
            data = libpointer('doublePtr', zeros(1, nSamplesTaken));
            data2=zeros(nSamplesTaken,mycolumn);
                           
                for i = 1:mycolumn
                    calllib('libD', 'IEE_DataGet', hMotionData, MotionDataChannels.([MotionDataChannelsNamesfull{i}]), data, uint32(nSamplesTaken));
                    data_value = get(data,'value');
                    for k=1: nSamplesTaken
                        data2(k,i)=data_value(k)                          
                    end                     
                end
                for i=1:nSamplesTaken
                    disp(data2(i,1:mycolumn));
                    dlmwrite(filename,data2(i,1:mycolumn),'-append','delimiter',',','precision','%.3f');
                    dataflag=true;
                end
         end            
        
    end
    
end

delimiterIn = ',';
headerlinesIn = 1;

if(dataflag==true)
    A = importdata(filename,delimiterIn,headerlinesIn);

    figure;    
    plot(0:1/sampFreq:(numel(A.data(:,sl_col))-1)/sampFreq,A.data(:,sl_col));
             xlabel('time (s)');
      %  plot([0:1/sampFreq:(end_time-1)/sampFreq],output_matrix(1:end_time,18))

    maxaccx=max(A.data(:,sl_col));
    minaccx=min(A.data(:,sl_col));
      axis([0 5 minaccx-100 maxaccx+100]);
       legend('GyroX');
end
if(dataflag==0)
disp('no data getted');
end
unloadlibrary libs
unloadlibrary libD
disp('finish');




