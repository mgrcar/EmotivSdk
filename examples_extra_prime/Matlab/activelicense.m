%this example show you how to active your license key and get information
%
%of the license.
%edit by Nguyen Vinh Binh; binh@emoviv.com 
% example done with matlab 32 bit

%--------------------------------------------------------------------------
clc;
warning ('off','all');
[nf, w] = loadlibrary('../../bin/win32/edk','../../include/Iedk.h','addheader','EmotivLicense.h','alias','libs'); 
     
LICENSE='';
%define license type according to IEE_LicenseType (EmotivLicense.h)
IEE_EEG=1;
IEE_PM=2;
IEE_EEG_PM=3;
result = calllib('libs','IEE_ActivateLicense',LICENSE)  ;   
        
        EDK_LICENSE_NOT_FOUND = hex2dec('2012');         
        EDK_LICENSE_ERROR =hex2dec('2010');
        EDK_LICENSE_EXPIRED = hex2dec('2011');
        EDK_LICENSE_REGISTERED =hex2dec('2020');
        EDK_LICENSE_DEVICE_LIMITED =hex2dec('2019');
        EDK_UNKNOWN_ERROR =hex2dec('0001');
        EDK_OK=0;
         % result=EDK_LICENSE_NOT_FOUND
         
switch (result)
    case EDK_LICENSE_NOT_FOUND
        disp('LICENSE_NOT_FOUND');
    case EDK_LICENSE_ERROR
        disp('LICENSE_ERROR');
    case EDK_LICENSE_EXPIRED
        disp('LICENSE_EXPIRED');
    case EDK_LICENSE_REGISTERED 
        disp('LICENSE_REGISTERED');
    case EDK_LICENSE_DEVICE_LIMITED
        disp('LICENSE_DEVICE_LIMITED');
    case EDK_UNKNOWN_ERROR
        disp('UNKNOWN_ERROR');
    otherwise
        
        disp('UNKNOWN_ERROR');
end 
    
if((result==EDK_OK)||(result ==EDK_LICENSE_REGISTERED))
    
    IEE_LicenseInfos= struct('scopes',10,'date_from',10,'date_to',10,'seat_count',10,'quotaDayLimit',10,'usedQuotaDay',10,'quotaMonthLimit',10,'usedQuotaMonth',10,'usedQuota',10,'quota',10);
    % IEE_LicenseInfos_struct is a struct defined in 'EmotivLicense.h'
    sp = libpointer('IEE_LicenseInfos_struct',IEE_LicenseInfos);
    [xobj,xval]= calllib('libs','IEE_LicenseInformation',sp);
    disp('*****************************');
    disp('LICENSE INFORMATION:');

     licensetype = '';
     if (sp.Value.scopes==IEE_EEG)
         licensetype='EEG';
     elseif (sp.Value.scopes==IEE_PM)
         licensetype='PM';
     elseif (sp.Value.scopes==IEE_EEG_PM)
         licensetype='EEG+PM';
     end
     X=['License type :',licensetype];
     disp(X);
     %convert time
     utc = sp.Value.date_from;
              
     X=['Date From  :  ',datestr(datenum([1970, 1, 1, 0, 0, utc])),' GMT'];
     disp(X);
     utc = sp.Value.date_to;
     X=['Date From  :  ',datestr(datenum([1970, 1, 1, 0, 0, utc])),' GMT'];
     disp(X);
     X=['Seat number  :  ',num2str(sp.Value.seat_count)];
     disp(X);
     X=['Quota limit per day  :  ',num2str(sp.Value.quotaDayLimit)];
     disp(X);
     X=['Quota used in day :  ',num2str(sp.Value.usedQuotaDay)];
     disp(X);
     X=['Quota limit per month :  ',num2str(sp.Value.quotaMonthLimit)];
     disp(X);
     X=['Quota used this month :  ',num2str(sp.Value.usedQuotaMonth)];
     disp(X);
     X=['Total of Quota:  ',num2str(sp.Value.usedQuota)];
     disp(X);
    X=['Quota used:  ',num2str(sp.Value.quota)];
     disp(X);
end
