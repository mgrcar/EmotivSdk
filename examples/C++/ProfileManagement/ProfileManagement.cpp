/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - Profile Management
** This example demonstrates the API functions that can be used to manage
** a user’s profile within Emotiv EmoEngineTM.
****************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdio.h>

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"
#include "ProfileControl.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

ExternalProfileManager profileManager;
std::map<unsigned int,std::string> headsetProfileMap;
unsigned char* baseProfile = 0;
unsigned int baseProfileSize = 0;

bool handleUserInput() {

	static std::string inputBuffer;

    char c = _getch();

	if (c == '\r') {
		std::cout << std::endl;
		std::string command;

		const size_t len = inputBuffer.length();
		command.reserve(len);

		// Convert the input to lower case first
		for (size_t i=0; i < len; i++) {
			command.append(1, tolower(inputBuffer.at(i)));
		}

		inputBuffer.clear();

		bool success = parseCommand(command, std::cout,
							profileManager, headsetProfileMap,
							baseProfile, baseProfileSize);
		std::cout << "ProfileManagement> ";
		return success;

		return true;
	}
	else {
		if (c == '\b') { // Backspace key
			if (inputBuffer.length()) {
				putchar(c);
				putchar(' ');
				putchar(c);
				inputBuffer.erase(inputBuffer.end()-1);
			}
		}
		else {
			inputBuffer.append(1,c);
			std::cout << c;
		}
	}	

	return true;
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

int main(int argc, char** argv) {

	EmoEngineEventHandle eEvent		= IEE_EmoEngineEventCreate();
	EmoEngineEventHandle eProfile	= IEE_ProfileEventCreate();
	EmoStateHandle eState			= IEE_EmoStateCreate();
	unsigned int userID				= 0;

	profileManager.load(DEFAULT_PROFILE_NAME);
	
	try {

        // connect directly to EmoEngine
        if (IEE_EngineConnect() != EDK_OK ) {
            throw std::runtime_error("Emotiv Engine start up failed.");
		}
		else {
			std::cout << "Emotiv Engine started!" << std::endl;
		}

        std::cout << "Type \"exit\" to quit, \"help\" to list available commands..."
                  << std::endl;

		// Retrieve the base profile and save it in buffer
		IEE_GetBaseProfile(eProfile);
		profileHandleToByteArray(eProfile, &baseProfile, &baseProfileSize);

		while (true) {
			
			// Handle the user input
			if (_kbhit()) {
				if (!handleUserInput()) {
					break;
				}
			}

            int state = IEE_EngineGetNextEvent(eEvent);

			// New event needs to be handled
			if (state == EDK_OK) {

				IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
				IEE_EmoEngineEventGetUserId(eEvent, &userID);

				switch (eventType) {

                // New headset connected
                case IEE_UserAdded:
                {
                    std::cout << "New user " << userID << " added."
                              << std::endl;

                    // add the headset
                    headsetProfileMap.insert(
                        std::pair<unsigned int,std::string>(userID,
                                                            std::string())
                    );
                    break;
                }

                // Headset disconnected
                case IEE_UserRemoved:
                {
                    std::cout << "User " << userID << " has been removed."
                              << std::endl;

                    // Profile will be returned as well,
                    // we need to save the profile according
                    // to the associated headset ID

                    unsigned char* profileBuffer = 0;
                    unsigned int   profileSize   = 0;

                    if (profileHandleToByteArray(eEvent, &profileBuffer,
                                                 &profileSize)) {

                        std::map<unsigned int, std::string>::iterator iter
                            = headsetProfileMap.find(userID);

                        if (iter != headsetProfileMap.end()) {
                            const std::string& profileName = iter->second;

                            if (!profileName.empty()) {
                                profileManager.insertProfile(profileName,
                                                             profileBuffer,
                                                             profileSize);
                            }

                            // Remove the headset ID from the mapping table
                            headsetProfileMap.erase(iter);
                        }
                    }

                    if (profileBuffer) {
                        delete [] profileBuffer, profileBuffer = 0;
                    }

                    break;
                }

                // Handle EmoState update
                case IEE_EmoStateUpdated:
                {
                    IEE_EmoEngineEventGetEmoState(eEvent, eState);
                    break;
                }

                default:
                    break;
				}
			}
			else if (state != EDK_NO_EVENT) {
                std::cout << "Internal error in Emotiv Engine!"
                          << std::endl;
				break;
			}

#ifdef _WIN32
            Sleep(15);
#endif
#ifdef __linux__
            sleep(1);
#endif
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cout << "Press any keys to exit..." << std::endl;
		getchar();
	}

	// Save the binary form of all profiles into a physical file
	profileManager.save(DEFAULT_PROFILE_NAME);

	// Free up resources
	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
	IEE_EmoEngineEventFree(eProfile);

	if (baseProfile) {
		delete [] baseProfile, baseProfile = 0;
	}

	return 0;
}
