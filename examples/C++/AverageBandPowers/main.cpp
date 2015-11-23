/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - AverageBandPowers
** The average band power for a specific channel from the latest epoch with
** 0.5 seconds step size and 2 seconds window size
** This example is use for single connection.
****************************************************************************/

#include <iostream>
#include <string>
#include <fstream>

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
#include "EmotivCloudClient.h"

using namespace std;

void  main() {

	EmoEngineEventHandle eEvent	= IEE_EmoEngineEventCreate();
	EmoStateHandle eState       = IEE_EmoStateCreate();

	unsigned int engineUserID   = -1;
	bool ready = false;
	int state  = 0;

	IEE_DataChannel_t channelList[] = { IED_AF3, IED_AF4, IED_T7, IED_T8, IED_Pz };

	std::string ouputFile = "AverageBandPowers.txt";
	const char header[] = "Theta, Alpha, Low_beta, High_beta, Gamma";
	std::ofstream ofs(ouputFile, std::ios::trunc);
	ofs << header << std::endl;
		
	if (IEE_EngineConnect() != EDK_OK) {
                throw std::runtime_error(
                            "Emotiv Driver start up failed.");
	}

	std::cout << "==================================================================="
		      << std::endl;
    std::cout << "  Example to get the average band power for a specific channel from \n"
		         "the latest epoch "
		      << std::endl;
    std::cout << "==================================================================="
		      << std::endl;

	while (!_kbhit())
	{
		state = IEE_EngineGetNextEvent(eEvent);

		if (state == EDK_OK) 
		{
		    IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
		    IEE_EmoEngineEventGetUserId(eEvent, &engineUserID);

			if (eventType == IEE_UserAdded) {
		         std::cout << "User added" << std::endl;
				 engineUserID = 0;
				 IEE_FFTSetWindowingType(engineUserID, IEE_HAMMING);

				 std::cout << header << std::endl;
		         ready = true;
			}
		}

		if (ready)
		{
            double alpha, low_beta, high_beta, gamma, theta;
            alpha = low_beta = high_beta = gamma = theta = 0;

            for(int i=0 ; i< sizeof(channelList)/sizeof(channelList[0]) ; ++i)
            {
                int result = IEE_GetAverageBandPowers(engineUserID, channelList[i], &theta, &alpha, 
					                                     &low_beta, &high_beta, &gamma);
                if(result == EDK_OK){
                    ofs << theta << ",";
                    ofs << alpha << ",";
                    ofs << low_beta << ",";
                    ofs << high_beta << ",";
                    ofs << gamma << ",";
                    ofs << std::endl;

					std::cout << theta << "," << alpha << "," << low_beta << ",";
                    std::cout << high_beta << "," << gamma << std::endl;
                }
            }
		}

#ifdef _WIN32
        Sleep(1);
#endif
#ifdef linux
        sleep(1);
#endif
	}

	ofs.close();

	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
}