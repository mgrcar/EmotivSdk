/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - SavingAndLoadingProfileCloud
** How to saving and loading prfile from Emotiv Cloud
****************************************************************************/

#include <iostream>
#include <string>

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


void  main() {
	std::string userName = "Your account name";
	std::string password = "Your password";
	std::string profileName = "EmotivProfile";
	int version	= -1; // Lastest version

	int option	= 0;
	std::string input;
	bool ready  = false;

	EmoEngineEventHandle eEvent	= IEE_EmoEngineEventCreate();
	EmoStateHandle eState		= IEE_EmoStateCreate();
	unsigned int engineUserID	= -1;
	int userCloudID		= -1;
	int state			= 0;
		
	if (IEE_EngineConnect() != EDK_OK) {
                throw std::runtime_error(
                            "Emotiv Driver start up failed.");
	}

	std::cout << "==================================================================="
                << std::endl;
    std::cout << "Example to saving and loading profile from Emotiv Cloud "
                << std::endl;
    std::cout << "==================================================================="
                << std::endl;
    std::cout << "Press '1' to saving profile to Emotiv Cloud "
                << std::endl;
    std::cout << "Press '2' to loading profile from Emotiv Cloud "
                << std::endl;
	std::cout << ">> ";

	std::getline(std::cin, input, '\n');
	option = atoi(input.c_str());

	if(!EC_Connect())
	{
		std::cout << "Cannot connect to Emotiv Cloud";
		return;
	}

	if(!EC_Login(userName.c_str(), password.c_str()))
	{
			
		std::cout << "Your login attempt has failed. The username or password may be incorrect";
		_getch();
		return;
	}

	if (!EC_GetUserDetail(&userCloudID))
		return;

	while (!_kbhit())
	{
		state = IEE_EngineGetNextEvent(eEvent);

		if (state == EDK_OK) {

		IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
		IEE_EmoEngineEventGetUserId(eEvent, &engineUserID);

		if (eventType == IEE_UserAdded) {
		std::cout << "User added" << std::endl;
		ready = true;
		}
		}

		if (ready)
		{
			int getNumberProfile = EC_GetAllProfileName(userCloudID);

			switch (option) {
				case 1:{    
					int profileID = EC_GetProfileId(userCloudID, profileName.c_str());

					if (profileID >= 0) {
						std::cout << "Profile with " << profileName << " is existed" << std::endl;
						if (EC_UpdateUserProfile(userCloudID, engineUserID, profileID, profileName.c_str())) {
						std::cout << "Updating finished";      
						}
						else std::cout << "Updating failed";
						}
					else if (EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(),
						profileFileType::TRAINING))
					{
						std::cout << "Saving finished";
					}
					else std::cout << "Saving failed";

					_getch();
					return;
				}
				case 2:{
					if (getNumberProfile > 0)
						if (EC_LoadUserProfile(userCloudID, (int)engineUserID, EC_ProfileIDAtIndex(userCloudID, 0)))
						std::cout << "Loading finished";
						else std::cout << "Loading failed";

					_getch();
					return;
				}
				default:
					throw std::runtime_error("Invalid option...");
					break;
			}
		}

#ifdef _WIN32
	Sleep(1);
#endif
#ifdef linux
	sleep(1);
#endif
	}

	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
}