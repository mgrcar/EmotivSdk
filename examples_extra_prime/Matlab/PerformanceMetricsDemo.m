
function PerformanceMetricsDemo()
% Copyright 2017 by Emotiv. All rights reserved
% ** PerformanceMetric Demo
% ** This example receives the PerformanceMetric scores
% ** then calculates the scaled scores
% ** and writes them to the data file( .csv file ...ie)
% ** It works if you have license subscription Performance Metrics
% ** Example ActivateLicense need to run at least one time on your computer to active your license before run this example.
% done with matlab 32 bit
%created by Binh Nguyen Emotiv; binh@emotiv.com
%--------------------------------------------------------------------------
warning ('off','all');
clc;
    loadlibrary('../../bin/win32/edk.dll','../../include/Iedk.h','addheader','IedkErrorCode.h','addheader','IEmoStateDLL.h','addheader','FacialExpressionDetection.h','addheader','IEmotivProfile.h','addheader','EmotivLicense.h','alias','libs'); 
    loadlibrary('../../bin/win32/edk.dll','../../include/IEegData.h','addheader','Iedk.h','alias','libD');
    loadlibrary('../../bin/win32/edk.dll','../../include/IEmoStatePerformanceMetric.h','addheader','Iedk.h','alias','libP');
    
fid = fopen('PerformanceMetrics.csv','wt');
strfull = ['Time;UserID;Stress raw score;Stress min score;Stress max score;'...
'Stress scaled score;Engagement boredom raw score;Engagement boredom min score;Engagement boredom max score;'...
        'Engagement boredom scaled score;'...
        'Relaxation raw score;'...
        'Relaxation min score;'...
         'Relaxation max score;'...
        'Relaxation scaled score;'...
        'Excitement raw score;'...
        'Excitement min score;'...
        'Excitement max score;'...
        'Excitement scaled score;'...
        'Interest raw score;'...
        'Interest min score;'...
        'Interest max score;'...
        'Interest scaled score;\n'];
fprintf(fid,strfull);
eEvent = calllib('libs','IEE_EmoEngineEventCreate');%1
eState = calllib('libs','IEE_EmoStateCreate');
default = int8(['Emotiv Systems-5' 0]);
calllib('libs','IEE_EngineConnect', default); % success means this value is 0;2

EDK_OK=0;
rectime=1;

calllib('libD','IEE_DataSetBufferSizeInSec',rectime)

acqtime=20; % time (second) for getting performance metrics data
tic
TB=['Getting Performance Metrics Data in:',num2str(acqtime),' second...'];
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
            
          
        end  
   if (strcmp(eventType,'IEE_EmoStateUpdated') == true)
           calllib('libs','IEE_EmoEngineEventGetEmoState',eEvent,eState);
        
        time=calllib('libs','IS_GetTimeFromStart',eState);
        fprintf(fid,'%6.3f;%d;',time,get(userID,'Value'));
        %IS_GetTimeFromStart(EmoStateHandle state);
        rawScore = libpointer('doublePtr',0);
        minScale = libpointer('doublePtr',0);
        maxScale = libpointer('doublePtr',0);
        %scaledScore = libpointer('doublePtr',0);
       calllib('libP','IS_PerformanceMetricGetStressModelParams',eState,rawScore,minScale,maxScale);
        raw=get(rawScore,'Value');
        min=get(minScale,'Value');
        max=get(maxScale,'Value');
        fprintf(fid,'%6.2f;%6.2f;%6.2f;',raw,min,max);
            if (min == max)
        
            fprintf(fid,'undefined;');
        %os << "undefined" << ",";
        
            else
            scaledScore = CaculateScale(raw, max, min);
            fprintf(fid,'%6.6f;',scaledScore);
            %os << scaledScore << ",";
            end;
            
       
      calllib('libP','IS_PerformanceMetricGetEngagementBoredomModelParams',eState,rawScore,minScale,maxScale);
        r=get(rawScore,'Value');
        min=get(minScale,'Value');
        max=get(maxScale,'Value');
        fprintf(fid,'%6.6f;%6.6f;%6.6f;',r,min,max);
            if (min == max)
        
            fprintf(fid,'undefined;');
        %os << "undefined" << ",";
        
            else
            scaledScore = CaculateScale(raw, max, min);
            fprintf(fid,'%6.6f;',scaledScore);
            %os << scaledScore << ",";
            end;
       
             calllib('libP','IS_PerformanceMetricGetRelaxationModelParams',eState,rawScore,minScale,maxScale);
        raw=get(rawScore,'Value');
        min=get(minScale,'Value');
        max=get(maxScale,'Value');
        fprintf(fid,'%6.6f;%6.6f;%6.6f;',raw,min,max);
            if (min == max)
        
            fprintf(fid,'undefined;');
        %os << "undefined" << ",";
        
            else
            scaledScore = CaculateScale(raw, max, min);
            fprintf(fid,'%6.6f;',scaledScore);
            %os << scaledScore << ",";
            end;
            
        calllib('libP','IS_PerformanceMetricGetInstantaneousExcitementModelParams',eState,rawScore,minScale,maxScale);
        raw=get(rawScore,'Value');
        min=get(minScale,'Value');
        max=get(maxScale,'Value');
        fprintf(fid,'%6.6f;%6.6f;%6.6f;',raw,min,max);
            if (min == max)        
            fprintf(fid,'undefined;');
            else
            scaledScore = CaculateScale(raw, max, min);
            fprintf(fid,'%6.6f;',scaledScore);            
            end;           
            
        calllib('libP','IS_PerformanceMetricGetInterestModelParams',eState,rawScore,minScale,maxScale);
        raw=get(rawScore,'Value');
        min=get(minScale,'Value');
        max=get(maxScale,'Value');
        fprintf(fid,'%6.6f;%6.6f;%6.6f;',raw,min,max);
            if (min == max)        
            fprintf(fid,'undefined;');     
            else
            scaledScore = CaculateScale(raw, max, min);
            fprintf(fid,'%6.6f;',scaledScore);
            end;
            fprintf(fid,'\n');
   end
    end  
end
fclose(fid);
disp('finish');

function y=CaculateScale(raw, max,min)
    
if(raw<min)
    
        y =0;
    elseif (raw>max)
    
        y = 1;
    
    else
        y = (raw-min)/(max-min);
end

