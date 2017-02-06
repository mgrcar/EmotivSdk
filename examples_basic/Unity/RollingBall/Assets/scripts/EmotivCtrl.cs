using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
using Emotiv;

public class EmotivCtrl : MonoBehaviour {
	public GameObject modal;
	public Text message_box;
	public InputField userName;
	public InputField password;
	public InputField profileName;

	public static EmoEngine engine;
	public static int engineUserID = -1;
	public static int userCloudID = 0;
	static int version	= -1; 

	/*
	 * Create instance of EmoEngine and set up his handlers for 
	 * user events, connection events and mental command training events.
	 * Init the connection
	*/
	void Awake () 
	{	
		engine = EmoEngine.Instance;
		engine.UserAdded += new EmoEngine.UserAddedEventHandler (UserAddedEvent);
		engine.UserRemoved += new EmoEngine.UserRemovedEventHandler (UserRemovedEvent);
		engine.EmoEngineConnected += new EmoEngine.EmoEngineConnectedEventHandler (EmotivConnected);
		engine.EmoEngineDisconnected += new EmoEngine.EmoEngineDisconnectedEventHandler (EmotivDisconnected);
		engine.MentalCommandTrainingStarted += new EmoEngine.MentalCommandTrainingStartedEventEventHandler (TrainingStarted);
		engine.MentalCommandTrainingSucceeded += new EmoEngine.MentalCommandTrainingSucceededEventHandler (TrainingSucceeded);
		engine.MentalCommandTrainingCompleted += new EmoEngine.MentalCommandTrainingCompletedEventHandler (TrainingCompleted);
		engine.MentalCommandTrainingRejected += new EmoEngine.MentalCommandTrainingRejectedEventHandler (TrainingRejected);
		engine.MentalCommandTrainingReset += new EmoEngine.MentalCommandTrainingResetEventHandler (TrainingReset);
		engine.Connect ();
	}

	/*
	 * Init the user, password and profile name if you want it
	*/
	void Start(){
		userName.text = "";
		password.text = "";
		profileName.text = "";
	}

	/*
	 * Call the ProcessEvents() method in Update once per frame
	*/
	void Update () {
		engine.ProcessEvents ();
	}

	/*
	 * Close the connection on application exit
	*/
	void OnApplicationQuit() {
		Debug.Log("Application ending after " + Time.time + " seconds");
		engine.Disconnect();
	}

	/*
	 * Several methods for handling the EmoEngine events.
	 * They are self explanatory.
	*/
	void UserAddedEvent(object sender, EmoEngineEventArgs e)
	{
		message_box.text = "User Added";
		engineUserID = (int)e.userId;
	}

	void UserRemovedEvent(object sender, EmoEngineEventArgs e)
	{
		message_box.text = "User Removed";	
	}

	void EmotivConnected(object sender, EmoEngineEventArgs e)
	{
		message_box.text = "Connected!!";
	}

	void EmotivDisconnected(object sender, EmoEngineEventArgs e)
	{
		message_box.text = "Disconnected :(";
	}

	public bool CloudConnected()
	{
		if (EmotivCloudClient.EC_Connect () == Emotiv.EmotivCloudClient.EC_OK) {
			message_box.text = "Connection to server OK";
			if (EmotivCloudClient.EC_Login (userName.text, password.text)== Emotiv.EmotivCloudClient.EC_OK) {
				message_box.text = "Login as " + userName.text;
				if (EmotivCloudClient.EC_GetUserDetail (ref userCloudID) == Emotiv.EmotivCloudClient.EC_OK) {
					message_box.text = "CloudID: " + userCloudID;
					return true;
				}
			} 
			else 
			{
				message_box.text = "Cant login as "+userName.text+", check password is correct";
			}
		} 
		else 
		{
			message_box.text = "Cant connect to server";
		}
		return false;
	}

	public void SaveProfile(){
		if (CloudConnected ()) {
			int profileId = EmotivCloudClient.EC_GetProfileId (userCloudID, profileName.text);
			if (profileId >= 0) {
				if (EmotivCloudClient.EC_UpdateUserProfile (userCloudID, (int)engineUserID, profileId) == Emotiv.EmotivCloudClient.EC_OK) {
					message_box.text = "Profile updated";
				} else {
					message_box.text = "Error saving profile, aborting";
				}
			} else {
				if (EmotivCloudClient.EC_SaveUserProfile (
					userCloudID, engineUserID, profileName.text, 
					EmotivCloudClient.profileFileType.TRAINING) == Emotiv.EmotivCloudClient.EC_OK) {
					message_box.text = "Profiled saved successfully";
				} else {
					message_box.text = "Error saving profile, aborting";
				}
			}
		}

	}

	public void LoadProfile(){
		if (CloudConnected ()) {
			if (EmotivCloudClient.EC_LoadUserProfile (
				userCloudID, (int)engineUserID, 
				EmotivCloudClient.EC_GetProfileId(userCloudID, profileName.text), 
				(int)version) == Emotiv.EmotivCloudClient.EC_OK) {
				message_box.text = "Load finished";
			} 
			else {
				message_box.text = "Problem loading";
			}
		}
	}

	public void TrainPush(){
		engine.MentalCommandSetTrainingAction((uint)engineUserID, EdkDll.IEE_MentalCommandAction_t.MC_PUSH);
		engine.MentalCommandSetTrainingControl((uint)engineUserID, EdkDll.IEE_MentalCommandTrainingControl_t.MC_START);
	}

	public void TrainNeutral(){
		engine.MentalCommandSetTrainingAction ((uint)engineUserID, EdkDll.IEE_MentalCommandAction_t.MC_NEUTRAL);
		engine.MentalCommandSetTrainingControl((uint)engineUserID, EdkDll.IEE_MentalCommandTrainingControl_t.MC_START);
	}

	public void TrainingStarted(object sender, EmoEngineEventArgs e){
		message_box.text = "Trainig started";
	}

	public void TrainingCompleted(object sender, EmoEngineEventArgs e){
		message_box.text = "Training completed!!";
	}

	public void TrainingRejected(object sender, EmoEngineEventArgs e){
		message_box.text = "Trainig rejected";
	}

	public void TrainingSucceeded(object sender, EmoEngineEventArgs e){
		message_box.text = "Training Succeeded!!";
		modal.GetComponent<MessageBox> ().init ("Training Succeeded!!", "Do you want to use this session?", new Decision (AceptTrainig));
	}

	public void AceptTrainig(bool accept){
		if (accept) {
			engine.MentalCommandSetTrainingControl ((uint)engineUserID, EdkDll.IEE_MentalCommandTrainingControl_t.MC_ACCEPT);
		} else {
			engine.MentalCommandSetTrainingControl ((uint)engineUserID, EdkDll.IEE_MentalCommandTrainingControl_t.MC_REJECT);
		}
	}
		
	public void TrainingReset(object sender, EmoEngineEventArgs e){
		message_box.text = "Command reseted";
	}

	public void Close(){
		Application.Quit ();
	}
}
