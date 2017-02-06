/****************************************************************************
**
** Copyright 2016 by Emotiv. All rights reserved
** Example - Mental command with  cloud profile
** This example demonstrates how to detect mental command detection with 
** a profile.
** This example work on single connection
****************************************************************************/

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <thread>

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"
#include "EmotivCloudClient.h"
#include "MentalCommandDetection.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#endif
#if __linux__ || __APPLE__
    #include <unistd.h>
    #include <termios.h>
    int _kbhit(void);
    int _getch( void );
#endif


const std::string userName = "YourEmotivID";
const std::string password = "YourPassword";
const std::string profileName = "Profile_1";

unsigned int userID = 0;
int userCloudID = -1;
bool enableLoger = false;

int version	= -1; // Lastest version
bool running = true;
typedef unsigned long ulong;
void help();

void SavingLoadingFunction(int userCloudID, bool save = true)
{
	int getNumberProfile = EC_GetAllProfileName(userCloudID);
	
	if (save) {    // Save profile to cloud
        int profileID = -1;
        EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);

		if (profileID >= 0) {
			std::cout << "Profile with " << profileName << " is existed." << std::endl;
            if (EC_UpdateUserProfile(userCloudID, userID, profileID) == EDK_OK)
				std::cout << "Updating finished" << std::endl;
			else 
				std::cout << "Updating failed" << std::endl;
		}
        else if (EC_SaveUserProfile(userCloudID, userID, profileName.c_str(), TRAINING) == EDK_OK)
		{
			std::cout << "Saving finished" << std::endl;
		}
		else std::cout << "Saving failed" << std::endl;
		return;
	} else { // Load profile from cloud
		if (getNumberProfile > 0){
            if (EC_LoadUserProfile(userCloudID, userID, EC_ProfileIDAtIndex(userCloudID, 0)) == EDK_OK)
				std::cout << "Loading finished" << std::endl;
			else
				std::cout << "Loading failed" << std::endl;

		}
		return;
	}
}

void handleMentalCommandEvent(unsigned int userID, EmoEngineEventHandle cognitivEvent)
{
    IEE_MentalCommandEvent_t eventType = IEE_MentalCommandEventGetType(cognitivEvent);

	switch (eventType) {

    case IEE_MentalCommandTrainingStarted:
    {
        std::cout<< std::endl << "MentalCommand training for user " << userID
           << " STARTED!" << std::endl;
        break;
    }

    case IEE_MentalCommandTrainingSucceeded:
    {
        std::cout<< std::endl << "MentalCommand training for user " << userID
           << " SUCCEEDED!" << std::endl;		
		std::cout << "Accept Training!!!" << std::endl;
		IEE_MentalCommandSetTrainingControl(userID, MC_ACCEPT);
		// Reject Training:
		// IEE_MentalCommandSetTrainingControl(userID, MC_REJECT);
        break;
    }

    case IEE_MentalCommandTrainingFailed:
    {
        std::cout<< std::endl << "MentalCommand training for user " << userID
           << " FAILED!" << std::endl;
        break;
    }

    case IEE_MentalCommandTrainingCompleted:
    {
        std::cout<< std::endl << "MentalCommand training for user " << userID
           << " COMPLETED!" << std::endl;
        break;
    }

    case IEE_MentalCommandTrainingDataErased:
    {
        std::cout<< std::endl << "MentalCommand training data for user " << userID
           << " ERASED!" << std::endl;
        break;
    }

    case IEE_MentalCommandTrainingRejected:
    {
        std::cout<< std::endl << "MentalCommand training for user " << userID
           << " REJECTED!" << std::endl;
        break;
    }

    case IEE_MentalCommandTrainingReset:
    {
        std::cout<< std::endl << "MentalCommand training for user " << userID
           << " RESET!" << std::endl;
        break;
    }

    case IEE_MentalCommandAutoSamplingNeutralCompleted:
    {
        std::cout<< std::endl << "MentalCommand auto sampling neutral for user "
           << userID << " COMPLETED!" << std::endl;
        break;
    }

    case IEE_MentalCommandSignatureUpdated:
    {
        std::cout << std::endl << "MentalCommand signature for user " << userID
           << " UPDATED!" << std::endl;
        break;
    }

    case IEE_MentalCommandNoEvent:
        break;

    default:
        break;
	}
}

static bool handleUserInput()
{
	static std::string inputBuffer;
	char key = _getch();

	switch (key)
	{
	case '1':
	{
        int getNumberProfile = EC_GetAllProfileName(userCloudID);

        int profileOK = false;

        int profileID = -1;
        EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);

        if (profileID >= 0) {
            if (EC_LoadUserProfile(userCloudID, userID, profileID) == EDK_OK) {
                profileOK = true;
            }
        }
        if (profileOK == false) {
            if (EC_SaveUserProfile(userCloudID, userID, profileName.c_str(), TRAINING) == EDK_OK)
            {
                std::cout << "Saving finished" << std::endl;
            }
            else std::cout << "Saving failed" << std::endl;
        }

		ulong action1 = (ulong)IEE_MentalCommandAction_t::MC_LEFT;
		ulong action2 = (ulong)IEE_MentalCommandAction_t::MC_RIGHT;
		ulong listAction = action1 | action2;
		int errorCode = EDK_OK;
		errorCode = IEE_MentalCommandSetActiveActions(userID, listAction);
		if (errorCode == EDK_OK)
		{
			std::cout << "Setting MentalCommand active actions (MC_LEFT | MC_RIGHT) for user " << userID << std::endl;			
		}
		else std::cout << "Setting MentalCommand active actions error: " << errorCode;
	}
	break;
	case '2':
		std::cout << std::endl << "Starting training NEUTRAL !" << std::endl;
		IEE_MentalCommandSetTrainingAction(userID, MC_NEUTRAL);
		IEE_MentalCommandSetTrainingControl(userID, MC_START);
		break;
	case '3':
		std::cout << std::endl << "Starting training RIGHT !" << std::endl;
		int errorCode;
		errorCode = IEE_MentalCommandSetTrainingAction(userID, MC_RIGHT);
		errorCode = IEE_MentalCommandSetTrainingControl(userID, MC_START);
		break;
	case '4':
		std::cout << std::endl << "Starting training LEFT !" << std::endl;
		IEE_MentalCommandSetTrainingAction(userID, MC_LEFT);
		IEE_MentalCommandSetTrainingControl(userID, MC_START);
		break;
	case '5':
	{
		int level = 0;
		if (IEE_MentalCommandSetActivationLevel(userID, 2) == EDK_OK) {
			IEE_MentalCommandGetActivationLevel(userID, &level);
			std::cout << "Set MentalCommand Activation level to " << level << std::endl;
		}
		else {
			std::cout << "Set MentalCommand Activation level failured " << level << std::endl;
		}
	}
	break;
	case '6':
	{
		int level = 0;
		IEE_MentalCommandGetActivationLevel(userID, &level);
		std::cout << "Current MentalCommand Activation level: " << level << std::endl;
	}
		break;
	case '7':
	{
		float skill = 0;
		IEE_MentalCommandGetOverallSkillRating(userID, &skill);
		std::cout << "Current overall skill rating: " << skill << std::endl;
	}
	break;
	case '8':
		std::cout << "Saving Profile ... " <<  std::endl;
		SavingLoadingFunction(userCloudID, true);
		break;
	case '9':
		std::cout << "Loading Profile ... " << std::endl;
		SavingLoadingFunction(userCloudID, false);
		break;
	case '0':
		help();
		break;
	case '`':
		return false;
		break;
	default:
		break;
	}
	return true;
}



int main(int argc, char** argv)
{
	int option	= 0;
	std::string input;
	bool ready  = false;

	EmoEngineEventHandle eEvent	= IEE_EmoEngineEventCreate();
	EmoStateHandle eState		= IEE_EmoStateCreate();
	
	int state			        = 0;		
		
	if (IEE_EngineConnect() != EDK_OK) {
        std::cout << "Emotiv Driver start up failed.";
        return -1;
	}

	std::cout << "===================================================================" << std::endl;
    std::cout << "Example to show how to detect mental command detection with a profile. " << std::endl;
    std::cout << "===================================================================" << std::endl;
	std::cout << "Connecting to cloud ..." << std::endl;

    if(EC_Connect() != EDK_OK)
	{
		std::cout << "Cannot connect to Emotiv Cloud";
        return -1;
	}

    if(EC_Login(userName.c_str(), password.c_str()) != EDK_OK)
	{			
		std::cout << "Your login attempt has failed. The username or password may be incorrect" << std::endl;
#ifdef _WIN32
        _getch();
#endif
        return -1;
	}

	std::cout<<"Logged in as " << userName << std::endl;

    if (EC_GetUserDetail(&userCloudID) != EDK_OK) {
        std::cout << "Error: Cannot get detail info, exit " << userName << std::endl;
        return -1;
    }

	help();

	while (true) {
		
		if (_kbhit()) {
			if (!handleUserInput()) {
				break;
			}
		}

		int state = IEE_EngineGetNextEvent(eEvent);

		if (state == EDK_OK) {

			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			IEE_EmoEngineEventGetUserId(eEvent, &userID);

			switch (eventType) 
			{
			    case IEE_UserAdded: 
					std::cout << std::endl << "New user " << userID << " added" << std::endl;
					break;
				case IEE_UserRemoved:
					std::cout << std::endl << "User " << userID << " has been removed." << std::endl;
					break;
				case IEE_EmoStateUpdated:
					IEE_EmoEngineEventGetEmoState(eEvent, eState);
					break;
				case IEE_MentalCommandEvent:
					handleMentalCommandEvent(userID, eEvent);
				case IEE_FacialExpressionEvent:

					break;
				default:
					break;
			}
		}
		else if (state != EDK_NO_EVENT) {
			std::cout << "Internal error in Emotiv Engine!" << std::endl;
			break;
		}

#ifdef _WIN32
    Sleep(5);
#endif
#if __linux__ || __APPLE__
        usleep(5000);
#endif
	}
    
	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
    return -1;
}

void help()
{
    std::cout << "===================================================================" << std::endl;
    std::cout << "Please do follow steps:" << std::endl;
    std::cout << "A. Press 1 to Active training session." << std::endl;
    std::cout << "B. Press 2 to Start training NEUTRAL. When completed, press 3 ." << std::endl;
    std::cout << "C. Press 3 to Start training RIGHT. When completed, press 4 ." << std::endl;
    std::cout << "D. Press 4 to Start training LEFT." << std::endl;
    std::cout << "E. Press 5 to Set ActivationLevel." << std::endl;
    std::cout << "===================================================================" << std::endl;
    std::cout << "Otherwise, press key below for more: " << std::endl;
    std::cout << std::endl;
    std::cout << "Press 6 to Read ActivationLevel." << std::endl;
    std::cout << "Press 7 to Get OverallSkillRating." << std::endl;
    std::cout << "Press 8 to Save Profile Data to cloud." << std::endl;
    std::cout << "Press 9 to Load Profile Data from cloud." << std::endl;
    std::cout << std::endl;
    std::cout << "Press 0 for help." << std::endl;
    std::cout << "Press ` to quit." << std::endl;
    std::cout << "===================================================================" << std::endl;
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

int _getch( void )
{
   struct termios oldattr, newattr;
   int ch;

   tcgetattr( STDIN_FILENO, &oldattr );
   newattr = oldattr;
   newattr.c_lflag &= ~( ICANON | ECHO );
   tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
   ch = getchar();
   tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );

   return ch;
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

int _getch( void )
{
    int r;
    unsigned char c;
    if((r = read(0, &c, sizeof(c))) < 0 )
    {
        return r;
    }
    else
    {
        return c;
    }
}
#endif

