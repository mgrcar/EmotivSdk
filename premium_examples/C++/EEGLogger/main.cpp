/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - EEGLogger
** This Example to show how to log the EmoState from EmoEngine
** It works with Emotiv premium libraries.
****************************************************************************/

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
#include "IEegData.h"
#include "IedkErrorCode.h"

void logEmoState(std::ostream& os, unsigned int userID,
                 EmoStateHandle eState, bool withHeader = false);

#ifdef __linux__
    int _kbhit(void);
#endif

int main() 
{
	IEE_DataChannel_t EpocChannelList[] = {

		IED_COUNTER, IED_INTERPOLATED,
		IED_AF3, IED_F7, IED_F3, IED_FC5, IED_T7,
		IED_P7, IED_O1, IED_O2, IED_P8, IED_T8,
		IED_FC6, IED_F4, IED_F8, IED_AF4, IED_RAW_CQ,
		IED_GYROX, IED_GYROY, IED_MARKER, IED_TIMESTAMP
	};

	IEE_DataChannel_t InsightChannelList[] = {

		IED_COUNTER,
        IED_INTERPOLATED,
        IED_RAW_CQ,
        IED_AF3,
        IED_T7,
        IED_Pz,
        IED_T8,
        IED_AF4,
        IED_TIMESTAMP,
        IED_MARKER,
        IED_SYNC_SIGNAL
	};

	const char header[] = "	IED_COUNTER, IED_INTERPOLATED, IED_AF3, IED_F7, IED_F3, IED_FC5, IED_T7, "
		"IED_P7, IED_O1, IED_O2, IED_P8, IED_T8, IED_FC6, IED_F4, IED_F8, IED_AF4, IED_RAW_CQ, "
		"IED_GYROX, IED_GYROY, IED_MARKER, IED_TIMESTAMP ";

    EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
	EmoStateHandle eState       = IEE_EmoStateCreate();
	unsigned int userID         = 0;
	bool readytocollect         = false;
	float secs = 1;	
	int option = 0;
	int state  = 0;

	std::ofstream ofs("EEGLogger.csv", std::ios::trunc);	
	ofs << header << std::endl;
	
	try {
		std::cout << "==================================================================="
			      << std::endl;
		std::cout << "Example to show how to log the EmoState from EmoEngine."
			      << std::endl;
		std::cout << "==================================================================="
			      << std::endl;

		if (IEE_EngineConnect() != EDK_OK) {
			throw std::runtime_error("Emotiv Driver start up failed.");
		}

		DataHandle hData = IEE_DataCreate();
		IEE_DataSetBufferSizeInSec(secs);

		while (!_kbhit()) {
			state = IEE_EngineGetNextEvent(eEvent);

			if (state == EDK_OK) {	
				IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
				IEE_EmoEngineEventGetUserId(eEvent, &userID);

				if (eventType == IEE_UserAdded) {
					std::cout << "User added" << std::endl;
					IEE_DataAcquisitionEnable(userID, true);
					readytocollect = true;
				}
			}

			if (readytocollect) {
				IEE_DataUpdateHandle(0, hData);
				unsigned int nSamplesTaken = 0;
				IEE_DataGetNumberOfSample(hData, &nSamplesTaken);
				
				std::cout << "Updated " << nSamplesTaken << std::endl;
				
				if (nSamplesTaken != 0) {
					double* data = new double[nSamplesTaken];
					for (int sampleIdx = 0; sampleIdx < (int)nSamplesTaken; ++sampleIdx) {
						for (int i = 0; i < sizeof(EpocChannelList) / sizeof(IEE_DataChannel_t); i++) {
							IEE_DataGet(hData, EpocChannelList[i], data, nSamplesTaken);
							ofs << data[sampleIdx] << ",";
						}
						
						ofs << std::endl;
					}
					
					delete[] data;
				}
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}			
		}

		ofs.close();
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
