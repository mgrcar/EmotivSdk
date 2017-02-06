/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - SavingAndLoadingProfileCloud
** How to save and load prfile from Emotiv Cloud
**
****************************************************************************/

using System;
using System.Collections.Generic;
using Emotiv;
using System.IO;
using System.Threading;
using System.Reflection;

namespace SavingAndLoadingProfileCloud
{
    class SavingAndLoadingProfileCloud
    {
        static int engineUserID = -1;
        static int cloudUserID  = 0;
        static string userName  = "your_EmotivID";
        static string password  = "your_password";
        static string profileName = "profile_name";
        static int version = -1; // last version

        static void engine_UserAdded_Event(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("User Added Event has occured");

            // record the user 
            engineUserID = (int)e.userId;
        }


        static void ProfileSaving()
        {
            
            int profileID = -1;
            EmotivCloudClient.EC_GetProfileId(cloudUserID, profileName, ref profileID);
                
            if (profileID >= 0) {
                Console.WriteLine("Updating...");

                Console.WriteLine("Profile with " + profileName + " is existed");
                if (EmotivCloudClient.EC_UpdateUserProfile(cloudUserID, engineUserID, profileID) == EdkDll.EDK_OK ) 
                {
                    Console.WriteLine("Updating finished");
                }
                else Console.WriteLine("Updating failed");                    
            }
            else{
                Console.WriteLine("Saving...");

                if (EmotivCloudClient.EC_SaveUserProfile(cloudUserID, 0, profileName,
                EmotivCloudClient.profileFileType.TRAINING) == EdkDll.EDK_OK)
                {
                    Console.WriteLine("Saving finished");
                }
                else Console.WriteLine("Saving failed");
            }

            Thread.Sleep(5000);
            return;
        }

        static void ProfileLoading()
        {

            Console.WriteLine("Loading...");

            int profileID = -1;
            EmotivCloudClient.EC_GetProfileId(cloudUserID, profileName, ref profileID);

            if (EmotivCloudClient.EC_LoadUserProfile(cloudUserID, 0, profileID, version) == EdkDll.EDK_OK)
                Console.WriteLine("Loading finished");
            else
                Console.WriteLine("Loading failed");

            Thread.Sleep(5000);
            return;                    
        }

        static void Main(string[] args)
        {
            Console.WriteLine("===========================================================================");
            Console.WriteLine("Example to saving and loading profile from Emotiv Cloud.");
            Console.WriteLine("===========================================================================");
            Console.WriteLine("Press 'F1' to saving profile to Emotiv Cloud.");
            Console.WriteLine("Press 'F2' to loading profile from Emotiv Cloud.");

            SavingAndLoadingProfileCloud p = new SavingAndLoadingProfileCloud();

            // create the engine
            EmoEngine engine = EmoEngine.Instance;
            engine.UserAdded += new EmoEngine.UserAddedEventHandler(engine_UserAdded_Event);
            engine.Connect();

            ConsoleKeyInfo cki = new ConsoleKeyInfo();

            if (EmotivCloudClient.EC_Connect() != EdkDll.EDK_OK)
            {
                Console.WriteLine("Cannot connect to Emotiv Cloud.");
                Thread.Sleep(2000);
                return;
            }

            if (EmotivCloudClient.EC_Login(userName, password) != EdkDll.EDK_OK)
            {			
                Console.WriteLine("Your login attempt has failed. The username or password may be incorrect");
                Thread.Sleep(2000);
                return;
            }

            Console.WriteLine("Logged in as " + userName);

            if (EmotivCloudClient.EC_GetUserDetail(ref cloudUserID) != EdkDll.EDK_OK)
                return;

            // Loading information of all profiles from Cloud to buffer. 
            // and return number of profile in the list. 
            int getNumberProfile = EmotivCloudClient.EC_GetAllProfileName(cloudUserID);

            while (true)
            {
                engine.ProcessEvents(10);

                if (!Console.KeyAvailable) continue;
                cki = Console.ReadKey(true);

                if (cki.Key == ConsoleKey.F1)
                {
                    if (engineUserID < 0)
                    {
                        Console.WriteLine("No headset is connected.");
                        continue;
                    }

                    // If your profile is new profile. You need call this function
                    // before want to train Mental Command detections. 
                    ProfileSaving();
                    break;
                }
                if (cki.Key == ConsoleKey.F2)
                {
                    if (engineUserID < 0)
                    {
                        Console.WriteLine("No headset is connected.");
                        continue;
                    }

                    if (getNumberProfile > 0)
                    {
                        ProfileLoading();
                        break;
                    }
                }
            }

            engine.Disconnect();
        }
    }
}
