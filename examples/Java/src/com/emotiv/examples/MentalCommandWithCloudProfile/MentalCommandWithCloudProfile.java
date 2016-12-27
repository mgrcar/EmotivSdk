package com.emotiv.examples.MentalCommandWithCloudProfile;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import com.emotiv.Iedk.*;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.*;


public class MentalCommandWithCloudProfile implements Runnable {
	public static final Pointer eEvent = Edk.INSTANCE.IEE_EmoEngineEventCreate();
	public static final Pointer eState = Edk.INSTANCE.IEE_EmoStateCreate();
	public static BufferedReader in;

	public static int state = 0;
	public static IntByReference engineUserID = null;
	public static IntByReference userCloudID = null;
	public static IntByReference profileID = null;
	public static String profileName = null;
	public static int pending = 0;
	public static String option = null;
	
	@Override
	public void run() {
		while (true) {
			try {
				option = in.readLine();
				pending = 1;
				if (option.equals("q"))
					break;
			} catch (IOException e) {
				pending = 0;
			}
		}
	}

	public static void main(String[] args) throws InterruptedException {

		String userName = "Your EmotivID";
		String password = "Your password";
		profileName = "Your profile name";

		
		engineUserID = new IntByReference(0);
		userCloudID  = new IntByReference(0);
		profileID 	 = new IntByReference(-1);

		if (Edk.INSTANCE.IEE_EngineConnect("Emotiv Systems-5") != EdkErrorCode.EDK_OK.ToInt()) {
			System.out.println("Emotiv Engine start up failed.");
			return;
		}

		if (EmotivCloudClient.INSTANCE.EC_Connect() != EdkErrorCode.EDK_OK.ToInt()) {
			System.out.println("Cannot connect to Emotiv Cloud");
			return;
		}

		if (EmotivCloudClient.INSTANCE.EC_Login(userName, password) != EdkErrorCode.EDK_OK.ToInt()) {
			System.out.println("Your login attempt has failed. The username or password may be incorrect");
			return;
		}

		System.out.println("Logged in as " + userName);

		if (EmotivCloudClient.INSTANCE.EC_GetUserDetail(userCloudID) != EdkErrorCode.EDK_OK.ToInt()) {
			return;
		}

		help();
		
		in = new BufferedReader(new InputStreamReader(System.in));
		Thread t1 = new Thread(new MentalCommandWithCloudProfile());
		t1.start();
		
		startLiveClassificationProcess();
		
		System.out.println("Quitting...");

		Edk.INSTANCE.IEE_EngineDisconnect();
		Edk.INSTANCE.IEE_EmoStateFree(eState);
		Edk.INSTANCE.IEE_EmoEngineEventFree(eEvent);
		
		System.out.println("Wating for the Thread");

	}

	

	public static void startLiveClassificationProcess() {

		while (true) {
			if (pending == 1) {
				if(!handleUserInput(engineUserID, userCloudID, profileName, option))
					break;
				pending = 0;
			}

			state = Edk.INSTANCE.IEE_EngineGetNextEvent(eEvent);

			if (state == EdkErrorCode.EDK_OK.ToInt()) {
				int eventType = Edk.INSTANCE.IEE_EmoEngineEventGetType(eEvent);
				Edk.INSTANCE.IEE_EmoEngineEventGetUserId(eEvent, engineUserID);

				if (eventType == Edk.IEE_Event_t.IEE_UserAdded.ToInt())
					System.out.println("New user " + engineUserID.getValue() + " added");

				else if (eventType == Edk.IEE_Event_t.IEE_UserRemoved.ToInt())
					System.out.println("User " + engineUserID.getValue() + " has been removed.");

				else if (eventType == Edk.IEE_Event_t.IEE_EmoStateUpdated.ToInt()) {
					Edk.INSTANCE.IEE_EmoEngineEventGetEmoState(eEvent, eState); }

				else if (eventType == Edk.IEE_Event_t.IEE_MentalCommandEvent.ToInt()) {
					handleMentalCommandEvent(engineUserID, eEvent);
					System.out.println("MentalCommandEvent arrived");
				}
					
				else if (eventType == Edk.IEE_Event_t.IEE_FacialExpressionEvent.ToInt()) {
					System.out.println("FacialExpressionEvent arrived");
				}

				else {
				}

			} else if (state != EdkErrorCode.EDK_NO_EVENT.ToInt())
				System.out.println("Internal error in Emotiv Engine!");
		}

	}

	private static void SavingLoadingFunction(int userCloudID, int engineUserID, boolean save, String profileName) {
		int getNumberProfile = EmotivCloudClient.INSTANCE.EC_GetAllProfileName(userCloudID);

		int result = EmotivCloudClient.INSTANCE.EC_GetProfileId(userCloudID, profileName, profileID);

		if (save) {

			if (profileID.getValue() >= 0) {
				System.out.println("Profile with " + profileName + " exists.");
				if (EmotivCloudClient.INSTANCE.EC_UpdateUserProfile(userCloudID, engineUserID,
						profileID.getValue()) == EdkErrorCode.EDK_OK.ToInt()) {
					System.out.println("Updating finished");
				} else {
					System.out.println("updating failed");
				}
			} else if (EmotivCloudClient.INSTANCE.EC_SaveUserProfile(userCloudID, engineUserID, profileName,
					EmotivCloudClient.profileType_t.TRAINING.toInt()) == EdkErrorCode.EDK_OK.ToInt())
				System.out.println("Saving finished");
			else
				System.out.println("Saving failed");
			return;
		} else {
			if (getNumberProfile > 0) {
				if (EmotivCloudClient.INSTANCE.EC_LoadUserProfile(userCloudID, engineUserID, profileID.getValue(),
						-1) == EdkErrorCode.EDK_OK.ToInt())
					System.out.println("Loading finished");
				else
					System.out.println("Loading failed");
			}
			return;
		}

	}

	public static void help() {
		System.out.println("=============================================================");
		System.out.println("Please do follow steps:");
		System.out.println("A. Press 1 to Active training session.");
		System.out.println("B. Press 2 to Start training NEUTRAL. When completed, press 3.");
		System.out.println("C. Press 3 to Start training RIGHT. When completed, press 4");
		System.out.println("D. Press 4 to Start training LEFT.");
		System.out.println("E. Press 5 to Set ActivationLevel.");
		System.out.println("=============================================================");
		System.out.println("Otherwise, press key below for more:");
		System.out.println();
		System.out.println("Press 6 to Read ActivationLevel.");
		System.out.println("Press 7 to Get OveralSkillRating.");
		System.out.println("Press 8 to Save Profile Data to the cloud.");
		System.out.println("Press 9 to Load Profile Data from cloud.");
		System.out.println();
		System.out.println("Press 0 for help.");
		System.out.println("Press r to start live classification.");
		System.out.println("Press q to quit.");
		System.out.println("=============================================================");

	}

	private static void handleMentalCommandEvent(IntByReference engineUserID, Pointer cognitiveEvent) {
		int eventType = Edk.INSTANCE.IEE_MentalCommandEventGetType(cognitiveEvent);

		switch (eventType) {

		case 1: {
			System.out.println("MentalCommand training for user " + engineUserID.getValue() + " STARTED!");
			break;
		}

		case 2: {
			System.out.println("MentalCommand training for user " + engineUserID.getValue() + " SUCCEEDED!");
			System.out.println("Accept training!!!");
			Edk.INSTANCE.IEE_MentalCommandSetTrainingControl(engineUserID.getValue(),
					Edk.IEE_MentalCommandTrainingControl_t.MC_ACCEPT.getType());
			break;
		}

		case 3: {
			System.out.println("MentalCommand training for user " + engineUserID.getValue() + " FAILED!");
			break;
		}
		
		case 4: {
			System.out.println("MentalCommand training for user " + engineUserID.getValue() + " COMPLETED!");
			break;
		}

		case 5: {
			System.out.println("MentalCommand training for user " + engineUserID.getValue() + " ERASED!");
			break;
		}

		case 6: {
			System.out.println("MentalCommand training for user " + engineUserID.getValue() + " REJECTED!");
			break;
		}
		
		case 7: {
			System.out.println("MentalCommand training for user " + engineUserID.getValue() + " RESET!");
			break;
		}
		case 8: {
			System.out.println("MentalCommand training for user " + engineUserID.getValue() + " Auto Sampling Neutral Completed!");
			break;
		}

		case 0: {
			break;
		}

		default: {
			break;
		}
		}
	}
	
	public static boolean handleUserInput(IntByReference userId, IntByReference userCloudId, String profileName,
			String line) {
		switch (line) {
		case "1": {
			long action1 = (long) EmoState.IEE_MentalCommandAction_t.MC_RIGHT.ToInt();
			long action2 = (long) EmoState.IEE_MentalCommandAction_t.MC_LEFT.ToInt();
			long listAction = action1 | action2;
			int errorCode = EdkErrorCode.EDK_OK.ToInt();
			errorCode = Edk.INSTANCE.IEE_MentalCommandSetActiveActions(userId.getValue(), listAction);
			if (errorCode == EdkErrorCode.EDK_OK.ToInt())
				System.out.println(
						"Setting MentalCommand active actions (MC_LEFT | MC_RIGHT) for user " + userId.getValue());
			else
				System.out.println("Setting MentalCommand active actions error: " + errorCode);
			
			Edk.INSTANCE.IEE_EngineClearEventQueue(Edk.IEE_Event_t.IEE_EmoStateUpdated.ToInt());

			break;

		}
		case "2": {
			System.out.println("Starting training NEUTRAL!");
			Edk.INSTANCE.IEE_MentalCommandSetTrainingAction(userId.getValue(), EmoState.IEE_MentalCommandAction_t.MC_NEUTRAL.ToInt());
			Edk.INSTANCE.IEE_MentalCommandSetTrainingControl(userId.getValue(), Edk.IEE_MentalCommandTrainingControl_t.MC_START.getType());
			break;
		}
		case "3": {
			System.out.println("Starting training RIGHT!");
			Edk.INSTANCE.IEE_MentalCommandSetTrainingAction(userId.getValue(),
					EmoState.IEE_MentalCommandAction_t.MC_RIGHT.ToInt());
			Edk.INSTANCE.IEE_MentalCommandSetTrainingControl(userId.getValue(),
					Edk.IEE_MentalCommandTrainingControl_t.MC_START.getType());
			break;
		}
		case "4": {
			System.out.println("Starting training LEFT!");
			Edk.INSTANCE.IEE_MentalCommandSetTrainingAction(userId.getValue(),
					EmoState.IEE_MentalCommandAction_t.MC_LEFT.ToInt());
			Edk.INSTANCE.IEE_MentalCommandSetTrainingControl(userId.getValue(),
					Edk.IEE_MentalCommandTrainingControl_t.MC_START.getType());
			break;
		}
		case "5": {
			IntByReference level = new IntByReference(0);
			if (Edk.INSTANCE.IEE_MentalCommandSetActivationLevel(userId.getValue(), 2) == EdkErrorCode.EDK_OK.ToInt())
				Edk.INSTANCE.IEE_MentalCommandGetActivationLevel(userId.getValue(), level);
			else
				System.out.println("Set MentalComand Activation level failed " + level.getValue());
			break;
		}
		case "6": {
			IntByReference level = new IntByReference(0);
			Edk.INSTANCE.IEE_MentalCommandGetActivationLevel(userId.getValue(), level);
			System.out.println("Current MentalCommand Activation level: " + level.getValue());
		}
		case "7": {
			FloatByReference skill = new FloatByReference(0);
			Edk.INSTANCE.IEE_MentalCommandGetOverallSkillRating(userId.getValue(), skill);
			System.out.println("Current overall skill rating: " + skill.getValue());
			break;
		}
		case "8": {
			System.out.println("Saving profile...");
			SavingLoadingFunction(userCloudId.getValue(), userId.getValue(), true, profileName);
			break;
		}
		case "9": {
			System.out.println("Loading profile...");
			SavingLoadingFunction(userCloudId.getValue(), userId.getValue(), false, profileName);
			break;
		}
		case "0": {
			help();
			break;
		}
		case "q": {
			return false;
		}
		case "r": {
			startLiveClassificationProcess();
		}
		default: {
			break;
		}
		}
		return true;
	}
	
	public static boolean checkTrained(NativeLongByReference action, int userId) {
		int result = Edk.INSTANCE.IEE_MentalCommandGetTrainedSignatureActions(userId, action);
		if (result == EdkErrorCode.EDK_OK.ToInt()) {
			return true;
		}
		return false;
	}
}
