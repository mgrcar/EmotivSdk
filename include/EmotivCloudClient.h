/**
 * Emotiv Insight SDK
 * Copyright (c) 2015 Emotiv Inc.
 *
 * This file is part of the Emotiv Insight SDK.
 *
 * The main interface that allows interactions between external programs and Emotiv Cloud
 *
 * None of these API functions are thread-safe.
 *
 * This header file is designed to be included under C and C++ environment.
 *
 */

#ifndef EMOTIVCLOUDCLIENT_H
#define EMOTIVCLOUDCLIENT_H

#ifndef EDK_STATIC_LIB

#ifdef EMOTIVCLOUDCLIENT_EXPORTS
#ifdef WIN32
#define EMOTIVCLOUD_API __declspec(dllexport)
#else
#define EMOTIVCLOUD_API
#endif
#else
#ifdef WIN32
#define EMOTIVCLOUD_API __declspec(dllimport)
#else
#define EMOTIVCLOUD_API
#endif
#endif

#else
#define EMOTIVCLOUD_API extern
#endif
#define MAX_NUM_OF_BACKUP_PROFILE_VERSION 2

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum profileType
	{
		TRAINING,
		EMOKEY
	}profileFileType;

	typedef struct profileVerInfo
	{
		int version;
		char last_modified[30];
	}profileVersionInfo;

    //! Initializes the connection to Emotiv Cloud Server
    /*!
        \return bool
                - true if connect successfully
    */
    EMOTIVCLOUD_API bool EC_Connect();

    //! Terminate the connection to Emotiv Cloud server and EmoEngine
    /*!

    */
    EMOTIVCLOUD_API void EC_Disconnect();

    //! Login on Emotiv Cloud with exist account
    /*!
        \param username        - email of account
        \param password     - password of account
        \return bool
                - true if login successfully
    */
    EMOTIVCLOUD_API bool EC_Login(const char*username, const char*password);

    //! Logout
    /*
     \return bool
     - 1 if succes, 0 if failed
     */
    EMOTIVCLOUD_API bool EC_Logout(int userCloudID);

    //! Get detail of user after login successfully
    /*!
         \return bool
                 - true if get detail successfully and get id from userID param
     */
     EMOTIVCLOUD_API bool EC_GetUserDetail(int *userCloudID);

	//! Upload profile of user to cloud
    /*!
		\param profileName         -
		\param filePath
		\param ptype
			
		\return bool s
										- 1 if succes, 0 if failed
    */
    EMOTIVCLOUD_API  bool   EC_SaveUserProfile(int userCloudID, int engineUserID, const char* profileName, profileFileType ptype);
	//! Update exist profile of user to cloud
    /*!
		\param profileId         -
        \param engineUserID
        \param profileName
		\param filePath

		\return bool 
										- 1 if succes, 0 if failed
    */
    EMOTIVCLOUD_API  bool   EC_UpdateUserProfile(int userCloudID, int engineUserID, int profileId, const char* profileName);
	//! Delete exist profile of user from cloud
    /*!
		\param profileId         -

		\return bool 
										- 1 if succes, 0 if failed
    */
	EMOTIVCLOUD_API  bool   EC_DeleteUserProfile(int userCloudID, int profileId);

	//! Get info Profile of user
	/*!
	    \param profileName
	    \ return int 
										- profile id of profile name
										- 0 if request failed, -1 if not existe profile name 
   */
	EMOTIVCLOUD_API  int   EC_GetProfileId(int userCloudID, const char* profileName);

	//! Download profile from cloud
	/*!
	    \param profileId
		\param destPath
		\param version					- default = -1 for download lastest version
		\return bool 
		                                - 1 if succes, 0 if failed
    */
	EMOTIVCLOUD_API  bool   EC_LoadUserProfile(int userCloudID, int engineUserID, int profileId, int version = -1);

	//! Get All Profile Name
	/*!
		\return int 
		                                - Number of Profile Name
    */
	EMOTIVCLOUD_API  int   EC_GetAllProfileName(int userCloudID);

	//! Get ProfileIDNameAtIndex
	/*!
	    \param index 
		\return const char* 
		                                -  Profile Name at index
    */
	
	EMOTIVCLOUD_API  int   EC_ProfileIDAtIndex(int userCloudID, int index);
	//! Get ProfileNameAtIndex
	/*!
	    \param index 
		\return const char* 
		                                -  Profile Name at index
    */
	
	EMOTIVCLOUD_API  const char*   EC_ProfileNameAtIndex(int userCloudID, int index);
	//! Get last modified time of Profile
	/*!
	    \param index 
		\return const char* 
		                                -  last modified time of Profile at index
    */
	
	EMOTIVCLOUD_API  const char*   EC_ProfileLastModifiedAtIndex(int userCloudID, int index);
	//! Get type of Profile
	/*!
	    \param index 
		\return const char* 
		                                -  type of Profile at index
    */
	
	EMOTIVCLOUD_API  profileFileType   EC_ProfileTypeAtIndex(int userCloudID, int index);

#ifdef __cplusplus
}
#endif
#endif // EMOTIVCLOUDCLIENT_H
