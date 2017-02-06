/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - EmoStateLogger
** This example demonstrates the use of the core Emotiv API functions.
** It logs all Emotiv detection results for the attached users after
** successfully establishing a connection to Emotiv EmoEngineTM or
** EmoComposerTM
** The Performance Metrics detection works if you have license subscription Performance Metrics
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
#if __linux__ || __APPLE__
    #include <unistd.h>
#endif

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"


#ifdef __cplusplus
extern "C"
{
#endif

#if __linux__ || __APPLE__
int _kbhit(void);
#endif

void logEmoState(std::ostream& os, unsigned int userID,
                 EmoStateHandle eState, bool withHeader = false);


int main(int argc, char** argv) {

	EmoEngineEventHandle eEvent			= IEE_EmoEngineEventCreate();
	EmoStateHandle eState				= IEE_EmoStateCreate();
	unsigned int userID					= 0;
	const unsigned short composerPort	= 1726;
	int option = 0;
	int state  = 0;
	std::string input;
    std::string filename = "emostate_logger.csv";
	try {
		
        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Example to show how to log the EmoState from EmoEngine/EmoComposer."
                  << std::endl;
        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Press '1' to start and connect to the EmoEngine                    "
                  << std::endl;
        std::cout << "Press '2' to connect to the EmoComposer                            "
                  << std::endl;
		std::cout << ">> ";

		std::getline(std::cin, input, '\n');
		option = atoi(input.c_str());

		switch (option) {
        case 1:
        {
            if (IEE_EngineConnect() != EDK_OK) {
                throw std::runtime_error("Emotiv Driver start up failed.");
            }

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
                std::string errMsg = "Cannot connect to EmoComposer on [" +
                                            input + "]";
                throw std::runtime_error(errMsg.c_str());
            }
            break;
        }
        default:
            throw std::runtime_error("Invalid option...");
            break;
		}
		

        std::cout << "Start receiving EmoState! Press any key to stop logging...\n"
                  << std::endl;

        std::ofstream ofs(filename.c_str());
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

                    std::cout << timestamp << ": " << "New EmoState from user " << userID << std::endl;
					
					logEmoState(ofs, userID, eState, writeHeader);
					writeHeader = false;
				}
			}
			else if (state != EDK_NO_EVENT) {
				std::cout << "Internal error in Emotiv Engine!" << std::endl;
				break;
			}

#ifdef _WIN32
            Sleep(1);
#endif
#if __linux__ || __APPLE__
            usleep(10000);
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


void logEmoState(std::ostream& os, unsigned int userID,
                 EmoStateHandle eState, bool withHeader) {

	// Create the top header
	if (withHeader) {
		os << "Time,";
		os << "Wireless Signal Status,";

        os << "AF3, F7, F3, FC5, T7, P7, O1, O2, P8, T8, FC6, F4, F8, AF4,";

        os << "||,";

		os << "Blink,";
		os << "Wink Left,";
		os << "Wink Right,";
		os << "Surprise,";
		os << "Frown,";
		os << "Smile,";
		os << "Clench,";
        
		os << std::endl;
	}

	// Log the time stamp and user ID
	os << IS_GetTimeFromStart(eState) << ",";
	os << static_cast<int>(IS_GetWirelessSignalStatus(eState)) << ",";

    //CQ 
    os << IS_GetContactQuality(eState, IEE_CHAN_AF3) << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_F7)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_F3)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_FC5) << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_T7)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_P7)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_O1)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_O2)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_P8)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_T8)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_FC6) << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_F4)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_F8)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_AF4) << ", ";

    os << "||,";

	// FacialExpression Suite results
	os << IS_FacialExpressionIsBlink(eState) << ",";
	os << IS_FacialExpressionIsLeftWink(eState) << ",";
	os << IS_FacialExpressionIsRightWink(eState) << ",";

	std::map<IEE_FacialExpressionAlgo_t, float> expressivStates;

    IEE_FacialExpressionAlgo_t upperFaceAction =
            IS_FacialExpressionGetUpperFaceAction(eState);
    float upperFacePower  = IS_FacialExpressionGetUpperFaceActionPower(eState);

    IEE_FacialExpressionAlgo_t lowerFaceAction =
            IS_FacialExpressionGetLowerFaceAction(eState);
    float	lowerFacePower  = IS_FacialExpressionGetLowerFaceActionPower(eState);

	expressivStates[ upperFaceAction ] = upperFacePower;
	expressivStates[ lowerFaceAction ] = lowerFacePower;
	
	os << expressivStates[ FE_SURPRISE] << ","; // eyebrow
	os << expressivStates[ FE_FROWN   ] << ","; // furrow
	os << expressivStates[ FE_SMILE   ] << ","; // smile
	os << expressivStates[ FE_CLENCH  ] << ","; // clench

	os << std::endl;
    os.flush();
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
#ifdef __cplusplus
}
#endif
