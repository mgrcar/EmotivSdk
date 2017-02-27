/****************************************************************************
**
** Copyright 2016 by Emotiv. All rights reserved
** Example - EEGLogger
** This Example to show how to log the EEG data from EmoEngine
** It works if you have license subscription EEG
** Example ActivateLicense need to run at least one time on your computer to active your license before run this example.
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

#if __linux__ || __APPLE__
    #include <unistd.h>
    #include <termios.h>
    int _kbhit(void);
    int _getch(void);
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

	const char header[] = "IED_COUNTER, IED_INTERPOLATED, IED_AF3, IED_F7, IED_F3, IED_FC5, IED_T7, "
		"IED_P7, IED_O1, IED_O2, IED_P8, IED_T8, IED_FC6, IED_F4, IED_F8, IED_AF4, IED_RAW_CQ, "
		"IED_GYROX, IED_GYROY, IED_MARKER, IED_TIMESTAMP ";

    EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
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
        std::cout << "Example to show how to log the EEG data from EmoEngine. \n";
        std::cout << "This example is used for single headset connection.\n";
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
                int result = IEE_DataUpdateHandle(0, hData);

                if (result != EDK_OK)
                    continue;

				unsigned int nSamplesTaken = 0;
				IEE_DataGetNumberOfSample(hData, &nSamplesTaken);							
				
				if (nSamplesTaken != 0) {
                    std::cout << "Updated " << nSamplesTaken << std::endl;

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
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
