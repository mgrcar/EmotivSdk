package com.emotiv.examples.SavingAndLoadingProfileCloud;

import java.util.Scanner;

import com.emotiv.Iedk.*;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;

public class SavingAndLoadingProfileCloud {
	
	
	public static void main(String[] args) {
		
		String profileName = "EmotivProfile";
		Pointer eEvent = Edk.INSTANCE.IEE_EmoEngineEventCreate();
		Pointer eState = Edk.INSTANCE.IEE_EmoStateCreate();

		Scanner input = new Scanner(System.in);
		
		System.out.println("username: ");
		String userName = input.nextLine();
		
		System.out.println("password: ");
		String password = input.nextLine();
		
		System.out.println("profileName: ");
		profileName = input.nextLine();
		
		input.close();

		
		int version	= -1;        // Lastest version
		
		IntByReference engineUserID  = null;
		IntByReference userCloudID	 = null;
		IntByReference profileID = null;
		int state     = 0;
		boolean ready = false;
		int option    = 2;       // 1 : Saving/ 2 : Loading

		engineUserID  = new IntByReference(0);
		userCloudID   = new IntByReference(0);
		profileID = new IntByReference(-1);
		
		if (Edk.INSTANCE.IEE_EngineConnect("Emotiv Systems-5") != EdkErrorCode.EDK_OK
				.ToInt()) {
			System.out.println("Emotiv Engine start up failed.");
			return;
		}
						
		if(EmotivCloudClient.INSTANCE.EC_Connect() != EdkErrorCode.EDK_OK.ToInt())
		{
			System.out.println("Cannot connect to Emotiv Cloud");
	        return;
		}

		if(EmotivCloudClient.INSTANCE.EC_Login(userName, password) != EdkErrorCode.EDK_OK.ToInt())
		{			
			System.out.println("Your login attempt has failed. The username or password may be incorrect");
	        return;
		}
		
		System.out.println("Logged in as " + userName);

		if (EmotivCloudClient.INSTANCE.EC_GetUserDetail(userCloudID) != EdkErrorCode.EDK_OK.ToInt())
	        return;

		while (true)
		{
			state = Edk.INSTANCE.IEE_EngineGetNextEvent(eEvent);

			if (state == EdkErrorCode.EDK_OK.ToInt()) {

				int eventType = Edk.INSTANCE.IEE_EmoEngineEventGetType(eEvent);
				Edk.INSTANCE.IEE_EmoEngineEventGetUserId(eEvent, engineUserID);

				// Log the EmoState if it has been updated
				if (eventType == Edk.IEE_Event_t.IEE_UserAdded.ToInt())
					if (engineUserID != null) {
						System.out.println("User added");
						ready = true;
					}
			} else if (state != EdkErrorCode.EDK_NO_EVENT.ToInt()) {
				System.out.println("Internal error in Emotiv Engine!");
				break;
			}

			if (ready)
			{
				int getNumberProfile = EmotivCloudClient.INSTANCE.EC_GetAllProfileName(userCloudID.getValue());

				switch (option) {
					case 1:{    
						int result = EmotivCloudClient.INSTANCE.EC_GetProfileId(userCloudID.getValue(), profileName, profileID);

						if (profileID.getValue() >= 0) {
							System.out.println("Profile with " + profileName + " is existed");
							if (EmotivCloudClient.INSTANCE.EC_UpdateUserProfile(userCloudID.getValue(), engineUserID.getValue(), profileID.getValue()) == EdkErrorCode.EDK_OK.ToInt()) 
							{
								System.out.println("Updating finished");  
							}
							else System.out.println("Updating failed");
					    }
						else if (EmotivCloudClient.INSTANCE.EC_SaveUserProfile(userCloudID.getValue(), engineUserID.getValue(), profileName, 0) == EdkErrorCode.EDK_OK.ToInt())  // Training
						     {
							    System.out.println("Saving finished");
						     }
						     else System.out.println("Saving failed");
						
	                    return;
					}
					case 2:{
	                    if (getNumberProfile > 0){
	                    	int result = EmotivCloudClient.INSTANCE.EC_GetProfileId(userCloudID.getValue(), profileName, profileID);

							if (EmotivCloudClient.INSTANCE.EC_LoadUserProfile(userCloudID.getValue(), engineUserID.getValue(), profileID.getValue(), version) == EdkErrorCode.EDK_OK.ToInt())
								System.out.println("Loading finished");
	                        else
	                        	System.out.println("Loading failed");

	                    }

	                    return;
					}
					default:
						System.out.println("Invalid option...");
						break;
				}
			}		
		}

		Edk.INSTANCE.IEE_EngineDisconnect();
		Edk.INSTANCE.IEE_EmoStateFree(eState);
		Edk.INSTANCE.IEE_EmoEngineEventFree(eEvent);
		System.out.println("Disconnected!");
	}
}
