
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <stdexcept>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif
#ifdef __linux__
#include <unistd.h>
#endif

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"
#include "IEmoStatePerformanceMetric.h"
#include "EmotivLicense.h"
#include "IEegData.h"


IEE_DataChannel_t EpocChannelList[] = {

    IED_COUNTER,
    IED_INTERPOLATED,
    IED_TIMESTAMP
};


#if __linux__ || __APPLE__
#include <unistd.h>
#include <termios.h>
int _kbhit(void);
int _getch(void);
#endif

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

void CaculateScale(double& rawScore, double& maxScale,
    double& minScale, double& scaledScore){

    if (rawScore<minScale)
    {
        scaledScore = 0;
    }
    else if (rawScore>maxScale)
    {
        scaledScore = 1;
    }
    else{
        scaledScore = (rawScore - minScale) / (maxScale - minScale);
    }
}

bool emostatedUpdated = false;
void logPerformanceMetricScore(std::ostream& os, unsigned int userID,
    EmoStateHandle eState) {

    emostatedUpdated = true;

    // Log the time stamp and user ID
    os << " " << ",";
    os << userID << ",";

    // PerformanceMetric results
    double rawScore = 0;
    double minScale = 0;
    double maxScale = 0;
    double scaledScore = 0;
    IS_PerformanceMetricGetStressModelParams(eState, &rawScore, &minScale,
        &maxScale);
    os << rawScore << ",";
    os << minScale << ",";
    os << maxScale << ",";
    if (minScale == maxScale)
    {
        os << "undefined" << ",";
    }
    else{
        CaculateScale(rawScore, maxScale, minScale, scaledScore);
        os << scaledScore << ",";
    }
    IS_PerformanceMetricGetEngagementBoredomModelParams(eState, &rawScore,
        &minScale, &maxScale);
    os << rawScore << ",";
    os << minScale << ",";
    os << maxScale << ",";
    if (minScale == maxScale)
    {
        os << "undefined" << ",";
    }
    else{
        CaculateScale(rawScore, maxScale, minScale, scaledScore);
        os << scaledScore << ",";
    }
    IS_PerformanceMetricGetRelaxationModelParams(eState, &rawScore,
        &minScale, &maxScale);
    os << rawScore << ",";
    os << minScale << ",";
    os << maxScale << ",";
    if (minScale == maxScale)
    {
        os << "undefined" << ",";
    }
    else{
        CaculateScale(rawScore, maxScale, minScale, scaledScore);
        os << scaledScore << ",";
    }
    IS_PerformanceMetricGetInstantaneousExcitementModelParams(eState,
        &rawScore, &minScale,
        &maxScale);
    os << rawScore << ",";
    os << minScale << ",";
    os << maxScale << ",";
    if (minScale == maxScale)
    {
        os << "undefined" << ",";
    }
    else{
        CaculateScale(rawScore, maxScale, minScale, scaledScore);
        os << scaledScore << ",";
    }
    IS_PerformanceMetricGetInterestModelParams(eState, &rawScore,
        &minScale, &maxScale);
    os << rawScore << ",";
    os << minScale << ",";
    os << maxScale << ",";
    if (minScale == maxScale)
    {
        os << "undefined" << ",";
    }
    else{
        CaculateScale(rawScore, maxScale, minScale, scaledScore);
        os << scaledScore << ",";
    }
    os << std::endl;
}

int main(int argc, char** argv) {

    EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
    EmoStateHandle eState = IEE_EmoStateCreate();
    unsigned int userID = 0;
    const unsigned short composerPort = 1726;
    int option = 0;
    int state = 0;
    float secs = 1;
    std::string input;

    try {
        std::cout << "==================================================================="
            << std::endl;
        std::cout << "Example to show how to log the EmoState from Emotiv Driver/"
            "EmoComposer."
            << std::endl;
        std::cout << "==================================================================="
            << std::endl;

        if (IEE_EngineConnect() != EDK_OK)
            throw std::runtime_error("Emotiv Insight Driver start up failed.");

        IEE_LicenseInfos_t licenseInfos;
        int result = IEE_LicenseInformation(&licenseInfos);

        printLicenseInformation(licenseInfos);

        std::ofstream emostateSave("emostateSave.csv");

        emostateSave << "Time,";
        emostateSave << "UserID,";
        emostateSave << "Stress raw score,";
        emostateSave << "Stress min score,";
        emostateSave << "Stress max score,";
        emostateSave << "Stress scaled score,";
        emostateSave << "Engagement boredom raw score,";
        emostateSave << "Engagement boredom min score,";
        emostateSave << "Engagement boredom max score,";
        emostateSave << "Engagement boredom scaled score,";
        emostateSave << "Relaxation raw score,";
        emostateSave << "Relaxation min score,";
        emostateSave << "Relaxation max score,";
        emostateSave << "Relaxation scaled score,";
        emostateSave << "Excitement raw score,";
        emostateSave << "Excitement min score,";
        emostateSave << "Excitement max score,";
        emostateSave << "Excitement scaled score,";
        emostateSave << "Interest raw score,";
        emostateSave << "Interest min score,";
        emostateSave << "Interest max score,";
        emostateSave << "Interest scaled score,";
        emostateSave << std::endl;

        std::ofstream EEGSave("EEGSave.csv");
        EEGSave << "Time";
        EEGSave << "IED_COUNTER,";
        EEGSave << "IED_INTERPOLATED,";
        EEGSave << std::endl;

        bool readytocollect = false;
        bool emoStateUpdated = false;

        DataHandle hData = IEE_DataCreate();
        IEE_DataSetBufferSizeInSec(1);

        while (!_kbhit()) {

            state = IEE_EngineGetNextEvent(eEvent);

            // New event needs to be handled
            if (state == EDK_OK) {

                IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
                IEE_EmoEngineEventGetUserId(eEvent, &userID);

                if (eventType == IEE_UserAdded) {
                    std::cout << "User added" << std::endl;
                    IEE_DataAcquisitionEnable(userID, true);
                    readytocollect = true;
                }
                if (eventType == IEE_EmoStateUpdated) {

                    IEE_EmoEngineEventGetEmoState(eEvent, eState);                    

                    logPerformanceMetricScore(emostateSave, userID, eState);
                }
                if (eventType == IEE_UserRemoved) {
                    std::cout << "User removed" << std::endl;
                    readytocollect = false;
                }

                // Log the EmoState if it has been updated
                if (readytocollect)
                {
                    int result = IEE_DataUpdateHandle(0, hData);
                    if (result != EDK_OK)
                        continue;

                    unsigned int nSamplesTaken = 0;
                    IEE_DataGetNumberOfSample(hData, &nSamplesTaken);

                    if (nSamplesTaken != 0) {

                        double* data = new double[nSamplesTaken];
                        for (int sampleIdx = 0; sampleIdx < (int)nSamplesTaken; ++sampleIdx) {
                            std::cout << "Sample = " << nSamplesTaken << std::endl;

                            for (int i = 0; i < sizeof(EpocChannelList) / sizeof(IEE_DataChannel_t); i++) {
                                IEE_DataGet(hData, EpocChannelList[i], data, nSamplesTaken);
                                EEGSave << data[sampleIdx] << ",";

                                if (emostatedUpdated && i == 2)
                                {
                                    emostatedUpdated = false;
                                    emostateSave << data[sampleIdx] << std::endl;
                                }

                            }

                            EEGSave << std::endl;
                        }

                        delete[] data;
                    }

                    //std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }

            }
            else if (state != EDK_NO_EVENT) {
                std::cout << "Internal error in Emotiv Engine!" << std::endl;
                break;
            }

            //std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        EEGSave.close();
        emostateSave.close();
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        std::cout << "Press any key to exit..." << std::endl;
        getchar();
    }

    IEE_EngineDisconnect();
    IEE_EmoStateFree(eState);
    IEE_EmoEngineEventFree(eEvent);

    return 0;
}

#ifdef __linux__
int _kbhit(void)
{
    struct timeval tv;
    fd_set read_fd;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&read_fd);
    FD_SET(0, &read_fd);

    if (select(1, &read_fd, NULL, NULL, &tv) == -1)
        return 0;

    if (FD_ISSET(0, &read_fd))
        return 1;

    return 0;
}

int _getch(void)
{
    struct termios oldattr, newattr;
    int ch;

    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return ch;
}
#endif
#ifdef __APPLE__
int _kbhit(void)
{
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&rdfs);
    FD_SET(STDIN_FILENO, &rdfs);

    select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
}

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
