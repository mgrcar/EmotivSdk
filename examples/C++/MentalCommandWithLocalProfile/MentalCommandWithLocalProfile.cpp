/****************************************************************************
**
** Copyright 2016 by Emotiv. All rights reserved
** Example - Mental command with  local profile
** This example demonstrates how to detect mental command detection with
** a profile.
** This example work on single connection
****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif

#if __linux__ || __APPLE__
#include <unistd.h>
int _kbhit(void);
#endif

#include <map>
#include <cassert>

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"

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


using namespace std;

typedef unsigned long ulong;

void handleMentalCommandEvent(std::ostream& os, EmoEngineEventHandle MentalCommandEvent);
void loadProfile(int userID);
void showTrainedActions(int userID);
void showCurrentActionPower(EmoStateHandle);
void trainMentalCommandActions(int headsetID);
void setActiveActions(int userID);
void setMentalCommandActions(int, IEE_MentalCommandAction_t);
int actionTraining;

std::string profileNameForLoading = "C:/1/161.emu";
std::string profileNameForSaving = "C:/1/161.emu";

int main(int argc, char** argv) {

	EmoEngineEventHandle eEvent       = IEE_EmoEngineEventCreate();
	EmoStateHandle eState             = IEE_EmoStateCreate();
	unsigned int userID	              = 0;
	const unsigned short composerPort = 1726;
	int option = 0;
	int state  = 0;
	bool isUserAdded = false;
	std::string input;
	actionTraining = 0;

    std::cout << "===================================================================" << "\n";
    std::cout << "   Example to use MentalCommand for training with profile functions" << "\n";
    std::cout << "===================================================================" << "\n";
    std::cout << "1. Load profile and show MentalCommand actions		             " << "\n";
    std::cout << "2. Train MentalCommand Actions						             " << "\n";
	std::cout << ">> ";

	std::getline(std::cin, input, '\n');
	option = atoi(input.c_str());

	if (IEE_EngineConnect() != EDK_OK) {
		throw std::runtime_error("Emotiv Engine start up failed.");
	}
		
	while (!_kbhit()) {
		state = IEE_EngineGetNextEvent(eEvent);

		// New event needs to be handled
		if (state == EDK_OK) {

			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			IEE_EmoEngineEventGetUserId(eEvent, &userID);

			switch (eventType) {
				case IEE_UserAdded:
				{
                    cout << endl << "New user " << userID <<" added"  << endl;

					if (option == 1)
					{
						loadProfile(userID);
                        showTrainedActions(userID);
					}
					if (option == 2)
					{	
                        setActiveActions(userID);
                        setMentalCommandActions(userID, MC_NEUTRAL);
					}
						
					break;
				}
					
				case IEE_UserRemoved:
				{
                    std::cout << std::endl << "User " << userID << " has been removed." << std::endl;
					break;
				}
					
				case IEE_EmoStateUpdated:
				{
					IEE_EmoEngineEventGetEmoState(eEvent, eState);
					if (option == 1)
						showCurrentActionPower(eState);

					break;
				}

				// Handle MentalCommand training related event
				case IEE_MentalCommandEvent:
				{
					handleMentalCommandEvent(std::cout, eEvent);
					//trainMentalCommandActions(userID);
					break;
				}
				default:
					break;
			}
		}
	}

	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);

	return 0;
}

void loadProfile(int userID)
{			
    if (IEE_LoadUserProfile(userID, profileNameForLoading.c_str()) == EDK_OK)
        std::cout << "Load Profile : done" << std::endl;
	else
		throw std::runtime_error("Can't load profile.");
}

const char *byte_to_binary(long x)
{    
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 8192; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

void showTrainedActions(int userID)
{
    unsigned long pTrainedActionsOut = 0;
    IEE_MentalCommandGetTrainedSignatureActions(userID, &pTrainedActionsOut);
    
    std::cout << "Trained Actions" << " : " << byte_to_binary(pTrainedActionsOut) << "\n";
}

void showCurrentActionPower(EmoStateHandle eState)
{
	IEE_MentalCommandAction_t eeAction = IS_MentalCommandGetCurrentAction(eState);
	float actionPower = IS_MentalCommandGetCurrentActionPower(eState);

	switch (eeAction)
	{
    case MC_NEUTRAL: { std::cout << "Neutral" << " : " << actionPower << "; \n"; break; }
    case MC_PUSH:    { std::cout << "Push" << " : " << actionPower << "; \n"; break; }
    case MC_PULL:    { std::cout << "Pull" << " : " << actionPower << "; \n"; break; }
	}	
}

void setActiveActions(int userID)
{
    ulong action1 = (ulong)IEE_MentalCommandAction_t::MC_PUSH;
    ulong action2 = (ulong)IEE_MentalCommandAction_t::MC_PULL;
    ulong listAction = action1 | action2;

    int errorCode = EDK_OK;
    errorCode = IEE_MentalCommandSetActiveActions(userID, listAction);

    if (errorCode == EDK_OK)
    {
        std::cout << "Setting MentalCommand active actions (MC_PUSH | MC_PULL) for user " << userID << std::endl;
    }
    else std::cout << "Setting MentalCommand active actions error: " << errorCode;
}

void setMentalCommandActions(int headsetID,IEE_MentalCommandAction_t action)
{
    int errorCode = IEE_MentalCommandSetTrainingAction(headsetID, action);
    errorCode = IEE_MentalCommandSetTrainingControl(headsetID, MC_START);

	switch (action)
	{
		case MC_NEUTRAL: { std::cout << std::endl <<"TRAINING NEUTRAL" <<std::endl; break; }
		case MC_PUSH:    { std::cout << std::endl <<"TRAINING PUSH" <<std::endl; break; }
		case MC_PULL:    { std::cout << std::endl <<"TRAINING PULL" <<std::endl; break; }
	}
}

void trainMentalCommandActions(int headsetID)
{
	if (actionTraining == 1)
	{
		setMentalCommandActions(headsetID, MC_PUSH);

		actionTraining ++;
		return;
	}

    if (IEE_SaveUserProfile(headsetID, profileNameForSaving.c_str()) == EDK_OK)
	{
		cout<<endl<<"Save Profile: done";
	}
	else cout<<endl<<"Can't save profile";
}

void handleMentalCommandEvent(std::ostream& os, EmoEngineEventHandle MentalCommandEvent) {

	unsigned int userID = 0;
	IEE_EmoEngineEventGetUserId(MentalCommandEvent, &userID);
	IEE_MentalCommandEvent_t eventType = IEE_MentalCommandEventGetType(MentalCommandEvent);


	switch (eventType) {

		case IEE_MentalCommandTrainingStarted:
		{
			os << std::endl << "MentalCommand training for user " << userID << " STARTED!" << std::endl;
			break;
		}

		case IEE_MentalCommandTrainingSucceeded:
		{
			os << std::endl << "MentalCommand training for user " << userID << " SUCCEEDED!" << std::endl;
			IEE_MentalCommandSetTrainingControl(userID, MC_ACCEPT);
#ifdef _WIN32
            Sleep(3000);
#endif
#if linux || __APPLE__
            sleep(3);
#endif
			break;
		}

		case IEE_MentalCommandTrainingFailed:
		{
			os << std::endl << "MentalCommand training for user " << userID << " FAILED!" << std::endl;
			break;
		}

		case IEE_MentalCommandTrainingCompleted:
		{
			os << std::endl << "MentalCommand training for user " << userID << " COMPLETED!" << std::endl;
			actionTraining ++;
			trainMentalCommandActions(userID);
			break;
		}

		case IEE_MentalCommandTrainingRejected:
		{
			os << std::endl << "MentalCommand training for user " << userID << " REJECTED!" << std::endl;
			break;
		}

		case IEE_MentalCommandTrainingReset:
		{
			os << std::endl << "MentalCommand training for user " << userID << " RESET!" << std::endl;
			break;
		}

		case IEE_MentalCommandAutoSamplingNeutralCompleted:
		{
			os << std::endl << "MentalCommand auto sampling neutral for user " << userID << " COMPLETED!" << std::endl;
			break;
		}

		case IEE_MentalCommandSignatureUpdated:
		{
			os << std::endl << "MentalCommand signature for user " << userID << " UPDATED!" << std::endl;
			break;
		}

		case IEE_MentalCommandNoEvent:
			break;

		default:
			//@@ unhandled case
			{
				assert(0);
				break;
			}
	}
}