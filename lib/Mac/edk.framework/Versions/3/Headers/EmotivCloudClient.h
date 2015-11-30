/**
 * Emotiv SDK
 * Copyright (c) 2015 Emotiv Inc.
 *
 * This file is part of the Emotiv SDK.
 *
 * The main interface that allows interactions between external programs and Emotiv Cloud.
 *
 * All API calls are blocking calls. Consider using the Emotiv Cloud APIs in a different thread
 * to avoid blocking the main thread.
 *
 * This header file is designed to be included under C and C++ environment.
 *
 */

#ifndef EMOTIVCLOUDCLIENT_H
#define EMOTIVCLOUDCLIENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef EDK_STATIC_LIB
#   ifdef EMOTIVCLOUDCLIENT_EXPORTS
#       ifdef _WIN32
#           define EMOTIVCLOUD_API __declspec(dllexport)
#       else
#           if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER || defined __clang__
#               define EMOTIVCLOUD_API __attribute__ ((visibility("default")))
#           else
#               define EMOTIVCLOUD_API
#           endif
#       endif
#   else
#       ifdef _WIN32
#           define EMOTIVCLOUD_API __declspec(dllimport)
#       else
#           define EMOTIVCLOUD_API
#       endif
#   endif
#else
#   define EMOTIVCLOUD_API extern
#endif

#define MAX_NUM_OF_BACKUP_PROFILE_VERSION 2


    //! Profile types
    typedef enum profileType {
        TRAINING,
        EMOKEY
    } profileFileType;

    
    //! Profile version
    typedef struct profileVerInfo {
        int version;
        char last_modified[30];
    } profileVersionInfo;

    
    //! Initialize the connection to Emotiv Cloud Server
    /*!
        \return bool
                - true if connect successfully
     
        \sa EC_Disconnect()
     */
    EMOTIVCLOUD_API bool
        EC_Connect();

    
    //! Terminate the connection to Emotiv Cloud server
    /*!
        \sa EC_Connect()
     */
    EMOTIVCLOUD_API void
        EC_Disconnect();

    
    //! Login Emotiv Cloud with EmotivID
    /*!
        To register a new EmotivID please visit https://id.emotivcloud.com/ .
     
        \param username  - username
        \param password  - password
        \return bool
                - true if login successfully
     
        \sa EC_Logout()
     */
    EMOTIVCLOUD_API bool
        EC_Login(const char* username, const char* password);

    
    //! Logout Emotiv Cloud
    /*
        \return bool
                - true if logout successfully
     
        \sa EC_Login()
     */
    EMOTIVCLOUD_API bool
        EC_Logout(int userCloudID);

    
    //! Get user ID after login
    /*!
        \param userCloudID - return user ID for subsequence requests
        \return bool
                - true if fetched successfully
     
        \sa EC_Login()
     */
    EMOTIVCLOUD_API bool
        EC_GetUserDetail(int *userCloudID);

    
    //! Save user profile to Emotiv Cloud
    /*!
        \param userCloudID  - user ID from EC_GetUserDetail()
        \param engineUserID - user ID from current EmoEngine (first user will be 0)
        \param profileName  - profile name to be saved as
        \param ptype        - profile type
            
        \return bool
                - true if saved successfully
     
        \sa EC_UpdateUserProfile(), EC_DeleteUserProfile()
     */
    EMOTIVCLOUD_API bool
        EC_SaveUserProfile(int userCloudID, int engineUserID, const char* profileName, profileFileType ptype);
    
    
    //! Update user profile to Emotiv Cloud
    /*!
        \param userCloudID  - user ID from EC_GetUserDetail()
        \param engineUserID - user ID from current EmoEngine (first user will be 0)
        \param profileId    - profile ID to be updated, from EC_GetProfileId()
        \param profileName  - profile name to be saved as

        \return bool 
                - true if updated successfully
     
        \sa EC_SaveUserProfile(), EC_DeleteUserProfile()
     */
    EMOTIVCLOUD_API bool
        EC_UpdateUserProfile(int userCloudID, int engineUserID, int profileId, const char* profileName);
    
    
    //! Delete user profile from Emotiv Cloud
    /*!
        \param userCloudID  - user ID from EC_GetUserDetail()
        \param profileId    - profile ID to be deleted, from EC_GetProfileId()

        \return bool
                - true if updated successfully
     
        \sa EC_SaveUserProfile(), EC_UpdateUserProfile()
     */
    EMOTIVCLOUD_API bool
        EC_DeleteUserProfile(int userCloudID, int profileId);

    
    //! Get profile ID of a user
    /*!
        \param userCloudID  - user ID from EC_GetUserDetail()
        \param profileName  - profile name to look for
        \return int - return profile ID if found, otherwise -1
     */
    EMOTIVCLOUD_API int
        EC_GetProfileId(int userCloudID, const char* profileName);
    

    //! Load profile from Emotiv Cloud
    /*!
        \remark Time to take to load a profile from Emotiv Cloud depends on network speed and profile size.
     
        \param userCloudID  - user ID from EC_GetUserDetail()
        \param engineUserID - user ID from current EmoEngine (first user will be 0)
        \param profileId    - profile ID to be loaded, from EC_GetProfileId()
        \param version      - version of profile to download (default: -1 for lastest version)
        \return bool
                - true if loaded successfully
     */
    EMOTIVCLOUD_API bool
        EC_LoadUserProfile(int userCloudID, int engineUserID, int profileId, int version = -1);

    
    //! Update all the profile info from Emotiv Cloud
    /*!
        This function needs to be called first before calling EC_ProfileIDAtIndex(), EC_ProfileNameAtIndex(),
        EC_ProfileLastModifiedAtIndex(), EC_ProfileTypeAtIndex()
     
        \param userCloudID  - user ID from EC_GetUserDetail()
     
        \return int - number of existing profiles (only latest version for each profile are counted)
     */
    EMOTIVCLOUD_API int
        EC_GetAllProfileName(int userCloudID);

    
    //! Return the profile ID of a profile in cache
    /*!
        \param userCloudID  - user ID from EC_GetUserDetail()
        \param index        - index of profile (starts from 0)
     
        \return int - profile ID
     */
    EMOTIVCLOUD_API int
        EC_ProfileIDAtIndex(int userCloudID, int index);
    
    
    //! Return the profile name of a profile in cache
    /*!
        \param userCloudID  - user ID from EC_GetUserDetail()
        \param index        - index of profile (starts from 0)
        
        \return const char* - profile name
     */
    EMOTIVCLOUD_API const char*
        EC_ProfileNameAtIndex(int userCloudID, int index);
    
    
    //! Return the last modified timestamp of a profile in cache
    /*!
        \param userCloudID  - user ID from EC_GetUserDetail()
        \param index        - index of profile (starts from 0)
     
        \return const char* - last modified timestamp
    */
    EMOTIVCLOUD_API const char*
        EC_ProfileLastModifiedAtIndex(int userCloudID, int index);
    
    
    //! Return the type of a profile in cache
    /*!
        \param userCloudID  - user ID from EC_GetUserDetail()
        \param index        - index of profile (starts from 0)
     
        \return profileFileType - profile type
     */
    EMOTIVCLOUD_API profileFileType
        EC_ProfileTypeAtIndex(int userCloudID, int index);

#ifdef __cplusplus
}
#endif
#endif // EMOTIVCLOUDCLIENT_H
