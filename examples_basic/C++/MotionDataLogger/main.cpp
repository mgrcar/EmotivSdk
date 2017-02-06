/****************************************************************************
 **
 ** Copyright 2015 by Emotiv. All rights reserved
 ** Example - MotionDataLogger
 ** This example demonstrates how to extract live Motion data using the EmoEngineTM
 ** in C++. Data is read from the headset and sent to an output file for
 ** later analysis
 **
 ****************************************************************************/
#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif
#if __linux__ || __APPLE__
#include <unistd.h>
#endif


namespace {

const IEE_MotionDataChannel_t targetChannelList[] = {
    IMD_COUNTER,
    IMD_GYROX,
    IMD_GYROY,
    IMD_GYROZ,
    IMD_ACCX,
    IMD_ACCY,
    IMD_ACCZ,
    IMD_MAGX,
    IMD_MAGY,
    IMD_MAGZ,
    IMD_TIMESTAMP
};

const char header[] = "COUNTER, GYROX, GYROY, GYROZ, ACCX, ACCY, ACCZ, MAGX, "
    "MAGY, MAGZ, TIMESTAMP";

}

#if __linux__ || __APPLE__
int _kbhit(void);
#endif

using std::cout;
using std::cerr;
using std::unique_ptr;


int main(int argc, char** argv) {
    unique_ptr<void, void (*)(EmoEngineEventHandle)> eEvent(IEE_EmoEngineEventCreate(), &IEE_EmoEngineEventFree);
    unique_ptr<void, void (*)(EmoStateHandle)> eState(IEE_EmoStateCreate(), &IEE_EmoStateFree);
    float secs            = 1;
    bool readytocollect   = false;
    char const * filename = "motionData.csv";

    try {
        cout << "===================================================================\n";
        cout << "Example to show how to log Motion Data from EmoDriver.\n"; 
        cout << "This example is used for single headset connection.\n";
        cout << "===================================================================" << std::endl;

        if (IEE_EngineConnect() != EDK_OK)
            throw std::runtime_error("Emotiv Driver start up failed.");

        cout << "Start receiving Motion Data! "
             << "Press any key to stop logging...\n"
             << std::endl;

        std::ofstream ofs(filename, std::ios::trunc);
        ofs << header << std::endl;

        unique_ptr<void, void (*)(DataHandle)> hMotionData(IEE_MotionDataCreate(), &IEE_MotionDataFree);
        IEE_MotionDataSetBufferSizeInSec(secs);

        cout << "Buffer size in secs:" << secs << std::endl;

        while (!_kbhit()) {
            auto state = IEE_EngineGetNextEvent(eEvent.get());
            if (state == EDK_OK) {

                IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent.get());

                // Log the EmoState if it has been updated
                if (eventType == IEE_UserAdded) {
                    cout << "User added";
                    readytocollect = true;
                }
            }

            if (readytocollect) {
                IEE_MotionDataUpdateHandle(0, hMotionData.get());
                unsigned int nSamplesTaken=0;
                IEE_MotionDataGetNumberOfSample(hMotionData.get(), &nSamplesTaken);
                
                if (nSamplesTaken != 0) {

                    cout << "Updated " << nSamplesTaken << std::endl;

                    std::vector<double> data(nSamplesTaken);
                    for (int sampleIdx=0 ; sampleIdx<(int)nSamplesTaken ; ++ sampleIdx) {
                        for (int i = 0 ;
                             i<sizeof(targetChannelList)/sizeof(IEE_MotionDataChannel_t) ;
                             i++) {

                            IEE_MotionDataGet(hMotionData.get(), targetChannelList[i],
                                              data.data(), data.size());
                            ofs << data[sampleIdx] << ",";
                        }
                        ofs << std::endl;
                    }
                }
            }            
        }

        ofs.close();
    }
    catch (const std::runtime_error& e) {
        cerr << e.what() << std::endl;
        cout << "Press any key to exit..." << std::endl;
        getchar();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    IEE_EngineDisconnect();

    return 0;
}

#ifdef __linux__
int _kbhit(void)
{
    struct timeval tv;
    fd_set read_fd;

    tv.tv_sec=0;
    tv.tv_usec=0;

    FD_ZERO(&read_fd);
    FD_SET(0,&read_fd);

    if(select(1, &read_fd,NULL, NULL, &tv) == -1)
        return 0;

    if(FD_ISSET(0,&read_fd))
        return 1;

    return 0;
}
#endif
#ifdef __APPLE__
int _kbhit (void)
{
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&rdfs);
    FD_SET (STDIN_FILENO, &rdfs);

    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
}
#endif
