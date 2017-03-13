%show list of SDK function, depends on your lisence.
% Create by Binh Nguyen, email : binh@emotiv.com
clc
    w = warning ('off','all');
    loadlibrary('../../bin/win32/edk.dll','../../include/Iedk.h','addheader','IedkErrorCode.h','addheader','IEmoStateDLL.h','addheader','FacialExpressionDetection.h','addheader','MentalCommandDetection.h','addheader','IEmotivProfile.h','addheader','EmotivLicense.h','alias','libE'); 
    loadlibrary('../../bin/win32/edk.dll','../../include/IEegData.h','addheader','Iedk.h','alias','libD');

libfunctionsview libE
libfunctionsview libD
unloadlibrary libE
unloadlibrary libD
