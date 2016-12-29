/****************************************************************************
**
** Copyright 2016 by Emotiv. All rights reserved
** PerformanceMetric Demo
** This example receives the PerformanceMetric scores
** then calculates the scaled scores
** and writes them to the data file( .csv file ...ie)
** It works if you have license subscription Performance Metrics
** Example ActivateLicense need to run at least one time on your computer to active your license before run this example.
****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <stdexcept>

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

void CaculateScale(double& rawScore, double& maxScale,
                   double& minScale, double& scaledScore);
void logPerformanceMetricScore(std::ostream& os, unsigned int userID,
                               EmoStateHandle eState, bool withHeader = false);


#if __linux__ || __APPLE__
    #include <unistd.h>
    #include <termios.h>
    int _kbhit(void);
    int _getch( void );
#endif

int main(int argc, char** argv) {

	EmoEngineEventHandle eEvent			= IEE_EmoEngineEventCreate();
	EmoStateHandle eState				= IEE_EmoStateCreate();
	unsigned int userID					= 0;
	const unsigned short composerPort	= 1726;
	int option = 0;
	int state  = 0;
	std::string input;

	try {
        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Example to show how to log the EmoState from Emotiv Driver/"
                     "EmoComposer."
                  << std::endl;
        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Press '1' to start and connect to the Emotiv Driver "
                  << std::endl;
        std::cout << "Press '2' to connect to the EmoComposer  "
                  << std::endl;
		std::cout << ">> ";

		std::getline(std::cin, input, '\n');
		option = atoi(input.c_str());

		switch (option) {
        case 1:
            {                
                if (IEE_EngineConnect() != EDK_OK)
                    throw std::runtime_error("Emotiv Insight Driver start up failed.");                

                IEE_LicenseInfos_t licenseInfos;
                int result = IEE_LicenseInformation(&licenseInfos);
                                
                break;
            }
        case 2:
            {
                std::cout << "Target IP of EmoComposer? [127.0.0.1] ";
                std::getline(std::cin, input, '\n');

                if (input.empty()) {
                    input = std::string("127.0.0.1");
                }

                if (IEE_EngineRemoteConnect(input.c_str(), composerPort) != EDK_OK) {
                    std::string errMsg = "Cannot connect to EmoComposer on [" + input + "]";
                    throw std::runtime_error(errMsg.c_str());
                }
                break;
            }
        default:
            throw std::runtime_error("Invalid option...");
            break;
		}


        std::cout << "Start receiving PerformanceMetric Score! "
                  << "Press any key to stop logging..."
                  << std::endl;

		std::ofstream ofs("performanceMetricData.csv");
		bool writeHeader = true;

		while (!_kbhit()) {

			state = IEE_EngineGetNextEvent(eEvent);

			// New event needs to be handled
			if (state == EDK_OK) {

				IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
				IEE_EmoEngineEventGetUserId(eEvent, &userID);

				// Log the EmoState if it has been updated
				if (eventType == IEE_EmoStateUpdated) {

					IEE_EmoEngineEventGetEmoState(eEvent, eState);
					const float timestamp = IS_GetTimeFromStart(eState);

                    printf("%10.3fs : New PerformanceMetric score from user %d ...\r",
                           timestamp, userID);

					logPerformanceMetricScore(ofs, userID, eState, writeHeader);
					writeHeader = false;
				}
			}
			else if (state != EDK_NO_EVENT) {
				std::cout << "Internal error in Emotiv Engine!" << std::endl;
				break;
			}

#ifdef _WIN32
			Sleep(0.05);
#endif
#ifdef __linux__
            sleep(1);
#endif
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


void logPerformanceMetricScore(std::ostream& os, unsigned int userID,
    EmoStateHandle eState, bool writeHeader) {

	// Create the top header
    if (writeHeader) {
		os << "Time,";
		os << "UserID,";
		os << "Stress raw score,";
		os << "Stress min score,";
		os << "Stress max score,";
		os << "Stress scaled score,";
		os << "Engagement boredom raw score,";
		os << "Engagement boredom min score,";
		os << "Engagement boredom max score,";
		os << "Engagement boredom scaled score,";
		os << "Relaxation raw score,";
		os << "Relaxation min score,";
		os << "Relaxation max score,";
		os << "Relaxation scaled score,";
		os << "Excitement raw score,";
		os << "Excitement min score,";
		os << "Excitement max score,";
		os << "Excitement scaled score,";
		os << "Interest raw score,";
		os << "Interest min score,";
		os << "Interest max score,";
		os << "Interest scaled score,";
		os << std::endl;
	}
	// Log the time stamp and user ID
	os << IS_GetTimeFromStart(eState) << ",";
	os << userID << ",";

	// PerformanceMetric results
	double rawScore=0;
	double minScale=0;
	double maxScale=0;	
	double scaledScore=0;
    IS_PerformanceMetricGetStressModelParams(eState,&rawScore,&minScale,
                                             &maxScale);
	os << rawScore << ",";
	os << minScale << ",";
	os << maxScale << ",";
	if (minScale==maxScale)
	{
		os << "undefined" << ",";
	}
	else{
		CaculateScale(rawScore,maxScale, minScale,scaledScore);
		os << scaledScore << ",";
	}		
    IS_PerformanceMetricGetEngagementBoredomModelParams(eState,&rawScore,
                                                        &minScale,&maxScale);
	os << rawScore << ",";
	os << minScale << ",";
	os << maxScale << ",";
	if (minScale==maxScale)
	{
		os << "undefined" << ",";
	}
	else{
		CaculateScale(rawScore,maxScale, minScale,scaledScore);
		os << scaledScore << ",";
	}		
    IS_PerformanceMetricGetRelaxationModelParams(eState,&rawScore,
                                                 &minScale,&maxScale);
	os << rawScore << ",";
	os << minScale << ",";
	os << maxScale << ",";
	if (minScale==maxScale)
	{
		os << "undefined" << ",";
	}
	else{
		CaculateScale(rawScore,maxScale, minScale,scaledScore);
		os << scaledScore << ",";
	}		
    IS_PerformanceMetricGetInstantaneousExcitementModelParams(eState,
                                                              &rawScore,&minScale,
                                                              &maxScale);
	os << rawScore << ",";
	os << minScale << ",";
	os << maxScale << ",";
	if (minScale==maxScale)
	{
		os << "undefined" << ",";
	}
	else{
		CaculateScale(rawScore,maxScale, minScale,scaledScore);
		os << scaledScore << ",";
	}
    IS_PerformanceMetricGetInterestModelParams(eState,&rawScore,
                                               &minScale,&maxScale);
	os << rawScore << ",";
	os << minScale << ",";
	os << maxScale << ",";
	if (minScale==maxScale)
	{
		os << "undefined" << ",";
	}
	else{
		CaculateScale(rawScore,maxScale, minScale,scaledScore);
		os << scaledScore << ",";
	}
	os << std::endl;
}
void CaculateScale (double& rawScore, double& maxScale,
                    double& minScale, double& scaledScore){
	
	if (rawScore<minScale)
	{
		scaledScore =0;
	}else if (rawScore>maxScale)
	{
		scaledScore = 1;
	}
	else{
		scaledScore = (rawScore-minScale)/(maxScale-minScale);
	}
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
