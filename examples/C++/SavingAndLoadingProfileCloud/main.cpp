/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - SavingAndLoadingProfileCloud
** How to saving and loading prfile from Emotiv Cloud
**
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif
#if __linux__ || __APPLE__
    #include <unistd.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if __linux__ || __APPLE__
int _kbhit(void);
#endif

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"
#include "EmotivCloudClient.h"


int  main() {
	std::string userName = "EmotivID";
	std::string password = "Password";
	std::string profileName = "EmotivProfile";
    std::string pathOfProfile;

    int version = -1; // Lastest version

    int option = 0;
    std::string input;
    bool ready = false;

    EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
    EmoStateHandle eState = IEE_EmoStateCreate();
    unsigned int engineUserID = -1;
    int userCloudID = -1;
    int state = 0;

    int result = EDK_OK;

    result = IEE_EngineConnect();
    if (result != EDK_OK) {
        std::cout << "Emotiv Driver start up failed.";
        return result;
    }

    std::cout << "==================================================================="
        << std::endl;
    std::cout << "Example to saving and loading profile from Emotiv Cloud. "
        << std::endl;
    std::cout << "==================================================================="
        << std::endl;
    std::cout << "Press '1' to saving profile to Emotiv Cloud "
        << std::endl;
    std::cout << "Press '2' to loading profile from Emotiv Cloud "
        << std::endl;
    std::cout << "Press '3' to upload a local profile to Emotiv Cloud "
        << std::endl;
    std::cout << "Press '4' to download a profile from Emotiv Cloud"
        << std::endl;
    std::cout << ">> ";

    std::cout << "Option: " << std::endl;
    std::getline(std::cin, input, '\n');
    option = atoi(input.c_str());

    result = EC_Connect();
    if (result != EDK_OK)
    {
        std::cout << "Cannot connect to Emotiv Cloud";
        return result;
    }

    result = EC_Login(userName.c_str(), password.c_str());
    if (result != EDK_OK)
    {
        std::cout << "Your login attempt has failed. The username or password may be incorrect";
#ifdef _WIN32
        _getch();
#endif
        return result;
    }

    std::cout << "Logged in as " << userName << std::endl;

    result = EC_GetUserDetail(&userCloudID);
    if (result != EDK_OK)
        return result;

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

            std::cout << "Number of profiles: " << getNumberProfile << "\n";

            for (int i = 0; i < getNumberProfile; i++)
            {
                std::cout << "Profile Name: " << EC_ProfileNameAtIndex(userCloudID, i) << ", ";

                std::cout << "Profile ID: " << EC_ProfileIDAtIndex(userCloudID, i) << ", ";
                std::cout << "Profile type: " <<
                    ((EC_ProfileTypeAtIndex(userCloudID, i) == profileFileType::TRAINING) ? "TRAINING" : "EMOKEY") << ", ";

                std::cout << EC_ProfileLastModifiedAtIndex(userCloudID, i) << ",\r\n";
            }

            switch (option) {
            case 1:{
                int profileID = -1;
                result = EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);

                if (profileID >= 0) {
                    std::cout << "Profile with " << profileName << " is existed" << std::endl;
                    result = EC_UpdateUserProfile(userCloudID, engineUserID, profileID);

                    if (result == EDK_OK) {
                        std::cout << "Updating finished";
                    }
                    else std::cout << "Updating failed";
                }
                else {
                    result = EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING);

                    if (result == EDK_OK)
                    {
                        std::cout << "Saving finished";
                    }
                    else std::cout << "Saving failed";
                }

#ifdef _WIN32
                _getch();
#endif                    
                return result;
            }
            case 2:{
                if (getNumberProfile > 0){
                    result = EC_LoadUserProfile(userCloudID, (int)engineUserID, EC_ProfileIDAtIndex(userCloudID, 0));

                    if (result == EDK_OK)
                        std::cout << "Loading finished";
                    else
                        std::cout << "Loading failed";
                }
#ifdef _WIN32
                _getch();
#endif
                return result;
            }
            case 3:
            {
                std::cout << "profile name: " << std::endl;
                std::getline(std::cin, profileName);
                std::cout << "profile path: " << std::endl;
                std::getline(std::cin, pathOfProfile);

                bool overwrite_if_exists = true;
                result = EC_UploadProfileFile(userCloudID, profileName.c_str(), pathOfProfile.c_str(), TRAINING, overwrite_if_exists);

                if (result == EDK_OK)
                    std::cout << "Upload finished";
                else
                    std::cout << "Upload failed";

#ifdef _WIN32
                _getch();
#endif
                return result;
            }            
            case 4:
            {
                std::cout << "profile name: " << std::endl;
                std::getline(std::cin, profileName);
                // E:/profile.emu
                std::cout << "profile path to save: " << std::endl;
                std::getline(std::cin, pathOfProfile);

                int profileID = -1;
                result = EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);

                if (profileID >= 0)
                    result = EC_DownloadProfileFile(userCloudID, profileID, pathOfProfile.c_str());

#ifdef _WIN32
                _getch();
#endif
                return result;
            }            
            default:
                std::cout << "Invalid option...";
                break;
            }
        }

#ifdef _WIN32
        Sleep(1);
#endif
#if __linux__ || __APPLE__
        usleep(10000);
#endif
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
