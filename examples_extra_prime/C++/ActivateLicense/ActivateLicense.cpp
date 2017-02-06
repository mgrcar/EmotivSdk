/****************************************************************************
**
** Copyright 2016 by Emotiv. All rights reserved
** Example - ActivateLicense
** How to activate a license key and Get infomation of that license 
**
****************************************************************************/

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#endif

#if __linux__ || __APPLE__
#include <unistd.h>
#include <termios.h>
int _getch(void);
#endif

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <ctime>

#include "Iedk.h"
#include "IedkErrorCode.h"

std::string convertEpochToTime(time_t epochTime, std::string format = "%Y-%m-%d %H:%M:%S");
std::string convertEpochToTime(time_t epochTime, std::string format)
{
    if (format == "")
        format = "%Y-%m-%d %H:%M:%S";


    char timestamp[64] = { 0 };
    strftime(timestamp, sizeof(timestamp), format.c_str(), localtime(&epochTime));
    return timestamp;
}

std::string intToHex(int x)
{
    std::stringstream stream;
    stream << std::hex << x;
    std::string result(stream.str());
    result = "0x" + result;

    return result;
}

void printLicenseInformation(IEE_LicenseInfos_t& licenseInfos)
{
    int licenseType = 0;

    std::cout << std::endl;
    std::cout << "Date From  : " << convertEpochToTime(licenseInfos.date_from) << std::endl;
    std::cout << "Date To    : " << convertEpochToTime(licenseInfos.date_to) << std::endl;
    std::cout << std::endl;

    std::cout << "Seat number: " << licenseInfos.seat_count << std::endl;
    std::cout << std::endl;

    std::cout << "Total Quota: " << licenseInfos.quota << std::endl;
    std::cout << "Total quota used    : " << licenseInfos.usedQuota << std::endl;
    std::cout << std::endl;

    std::cout << "Quota limit in day  : " << licenseInfos.quotaDayLimit << std::endl;
    std::cout << "Quota used in day   : " << licenseInfos.usedQuotaDay << std::endl;
    std::cout << std::endl;

    std::cout << "Quota limit in month: " << licenseInfos.quotaMonthLimit << std::endl;
    std::cout << "Quota used in month : " << licenseInfos.usedQuotaMonth << std::endl;
    std::cout << std::endl;

    switch (licenseInfos.scopes)
    {
    case IEE_EEG:
        licenseType = IEE_LicenseType_t::IEE_EEG;

        std::cout << "License type : " << "EEG" << std::endl;
        std::cout << std::endl;
        break;
    case IEE_EEG_PM:
        licenseType = IEE_LicenseType_t::IEE_EEG_PM;

        std::cout << "License type : " << "EEG + PM" << std::endl;
        std::cout << std::endl;
        break;
    case IEE_PM:
        licenseType = IEE_LicenseType_t::IEE_PM;
        std::cout << "License type : " << "PM" << std::endl;
        std::cout << std::endl;
        break;
    default:
        std::cout << "License type : " << "No type" << std::endl;
        std::cout << std::endl;
        break;
    }
}

std::string const LICENSE_KEY = "";

int main(int argc, char** argv)
{

    int result = EDK_OK;

    // Need only 1 time to call with the internet connection
    result = IEE_ActivateLicense(LICENSE_KEY.c_str());    

    switch (result)
    {
        case EDK_LICENSE_NOT_FOUND:
        case EDK_LICENSE_ERROR:
        case EDK_LICENSE_EXPIRED:
        case EDK_LICENSE_REGISTERED:
        case EDK_LICENSE_DEVICE_LIMITED:
        case EDK_UNKNOWN_ERROR:
            break;

        default:
            break;
    }

    if (!(result == EDK_OK || result == EDK_LICENSE_REGISTERED))
        return result;
    
    IEE_LicenseInfos_t licenseInfos;

    // We can call this API any time to check current License information
    result = IEE_LicenseInformation(&licenseInfos);

    std::cout << std::endl << "IEE_LicenseInfomation result = " << intToHex(result) << std::endl;

    switch (result)
    {
        case EDK_OVER_QUOTA_IN_DAY:
        case EDK_OVER_QUOTA_IN_MONTH:
        case EDK_LICENSE_EXPIRED:
        case EDK_OVER_QUOTA:
        case EDK_ACCESS_DENIED:
        case EDK_LICENSE_ERROR:
            break;
        case EDK_NO_ACTIVE_LICENSE:
            break;
        case EDK_OK:
            break;
        default:
            break;
    }

    printLicenseInformation(licenseInfos);

    _getch();

}
#ifdef __APPLE__
int _getch(void)
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0)
    {
        return r;
    }
    else
    {
        return c;
    }
}
#endif
