/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - SavingAndLoadingProfileCloud
** How to saving and loading prfile from Emotiv Cloud
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
        static int userCloudID = 0;
        static string userName = "Your account name";
        static string password = "Your password";
        static string profileName = "EmotivProfile";
        static int version	= -1; // Lastest version

        static void engine_UserAdded_Event(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("User Added Event has occured");

            // record the user 
            engineUserID = (int)e.userId;
        }


        static void SavingLoadingFunction(int mode)
        {
            int getNumberProfile = EmotivCloudClient.EC_GetAllProfileName(userCloudID);

            if (mode == 0)
            {
                int profileID = EmotivCloudClient.EC_GetProfileId(userCloudID, profileName);
                
                if (profileID >= 0) {
                    Console.WriteLine("Updating...");

                    Console.WriteLine("Profile with " + profileName + " is existed");
                    if (EmotivCloudClient.EC_UpdateUserProfile(userCloudID, engineUserID, profileID) == EmotivCloudClient.EC_OK ) 
                    {
                        Console.WriteLine("Updating finished");
                    }
                    else Console.WriteLine("Updating failed");                    
                }
                else{
                    Console.WriteLine("Saving...");

                    if (EmotivCloudClient.EC_SaveUserProfile(userCloudID, 0, profileName,
                    EmotivCloudClient.profileFileType.TRAINING) == EmotivCloudClient.EC_OK)
                    {
                        Console.WriteLine("Saving finished");
                    }
                    else Console.WriteLine("Saving failed");
                }

                Thread.Sleep(5000);
                return;
            }
            if (mode == 1)
            {
                if (getNumberProfile > 0)
                {
                    Console.WriteLine("Loading...");

                    if (EmotivCloudClient.EC_LoadUserProfile(userCloudID, 0,
                        EmotivCloudClient.EC_GetProfileId(userCloudID, profileName), version) == EmotivCloudClient.EC_OK)
                        Console.WriteLine("Loading finished");
                    else
                        Console.WriteLine("Loading failed");

                }

                Thread.Sleep(5000);
                return;
            }
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

            if (EmotivCloudClient.EC_Connect() != EmotivCloudClient.EC_OK)
            {
                Console.WriteLine("Cannot connect to Emotiv Cloud.");
                Thread.Sleep(2000);
                return;
            }

            if (EmotivCloudClient.EC_Login(userName, password) != EmotivCloudClient.EC_OK)
            {			
                Console.WriteLine("Your login attempt has failed. The username or password may be incorrect");
                Thread.Sleep(2000);
                return;
            }

            Console.WriteLine("Logged in as " + userName);

            if (EmotivCloudClient.EC_GetUserDetail(ref userCloudID) != EmotivCloudClient.EC_OK)
                return;            

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
                    SavingLoadingFunction(0);
                    break;
                }
                if (cki.Key == ConsoleKey.F2)
                {
                    if (engineUserID < 0)
                    {
                        Console.WriteLine("No headset is connected.");
                        continue;
                    }
                    SavingLoadingFunction(1);
                    break;
                }                
            }

            engine.Disconnect();
        }
    }
}
