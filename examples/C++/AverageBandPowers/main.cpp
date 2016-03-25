/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - AverageBandPowers
** The average band power for a specific channel from the latest epoch with
** 0.5 seconds step size and 2 seconds window size
** This example is used for single connection.
**
****************************************************************************/
#ifdef __cplusplus


#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif
#if __linux__ || __APPLE__
    #include <unistd.h>
#endif

#include <iostream>
#include <string>
#include <fstream>

extern "C"
{
#endif

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"

using namespace std;

#if __linux__ || __APPLE__
int _kbhit(void);
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

int  main()
{

	EmoEngineEventHandle eEvent	= IEE_EmoEngineEventCreate();
	EmoStateHandle eState       = IEE_EmoStateCreate();

	unsigned int engineUserID   = -1;
	bool ready = false;
	int state  = 0;

	IEE_DataChannel_t channelList[] = { IED_AF3, IED_AF4, IED_T7, IED_T8, IED_Pz };
#ifndef __APPLE__
	std::string ouputFile = "AverageBandPowers.txt";
#else
    std::string home_path;
    const char* home = getenv("HOME");
    home_path.assign(home);
    home_path.append("/Desktop/AverageBandPowers.csv");
    std::string ouputFile = home_path;
#endif
	const char header[] = "Theta, Alpha, Low_beta, High_beta, Gamma";
    std::ofstream ofs(ouputFile.c_str(), std::ios::trunc);
	ofs << header << std::endl;
		
	if (IEE_EngineConnect() != EDK_OK) {
        std::cout << "Emotiv Driver start up failed.";
       return -1;
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
#if linux || __APPLE__
        usleep(10000);
#endif
	}

	ofs.close();

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

#ifdef __cplusplus
}
#endif
