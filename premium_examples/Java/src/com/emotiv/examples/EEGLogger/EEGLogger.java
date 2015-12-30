package com.emotiv.examples.EEGLogger;
import com.emotiv.Iedk.*;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;

public class EEGLogger {
	public static void main(String[] args) {
		Pointer eEvent = Edk.INSTANCE.IEE_EmoEngineEventCreate();
		Pointer eState = Edk.INSTANCE.IEE_EmoStateCreate();
		IntByReference userID = null;
		IntByReference nSamplesTaken = null;
		short composerPort = 1726;
		int option = 1;
		int state = 0;
		float secs = 1;
		boolean readytocollect = false;

		userID = new IntByReference(0);
		nSamplesTaken = new IntByReference(0);

		switch (option) {
		case 1: {
			if (Edk.INSTANCE.IEE_EngineConnect("Emotiv Systems-5") != EdkErrorCode.EDK_OK.ToInt()) {
				System.out.println("Emotiv Engine start up failed.");
				return;
			}
			break;
		}
		case 2: {
			System.out.println("Target IP of EmoComposer: [127.0.0.1] ");

			if (Edk.INSTANCE.IEE_EngineRemoteConnect("127.0.0.1", composerPort,
					"Emotiv Systems-5") != EdkErrorCode.EDK_OK.ToInt()) {
				System.out
						.println("Cannot connect to EmoComposer on [127.0.0.1]");
				return;
			}
			System.out.println("Connected to EmoComposer on [127.0.0.1]");
			break;
		}
		default:
			System.out.println("Invalid option...");
			return;
		}

		Pointer hData = IEegData.INSTANCE.IEE_DataCreate();
		IEegData.INSTANCE.IEE_DataSetBufferSizeInSec(secs);
		System.out.print("Buffer size in secs: ");
		System.out.println(secs);

		System.out.println("Start receiving EEG Data!");
		while (true) {
			state = Edk.INSTANCE.IEE_EngineGetNextEvent(eEvent);

			// New event needs to be handled
			if (state == EdkErrorCode.EDK_OK.ToInt()) {
				int eventType = Edk.INSTANCE.IEE_EmoEngineEventGetType(eEvent);
				Edk.INSTANCE.IEE_EmoEngineEventGetUserId(eEvent, userID);

				// Log the EmoState if it has been updated
				if (eventType == Edk.IEE_Event_t.IEE_UserAdded.ToInt())
					if (userID != null) {
						System.out.println("User added");
						IEegData.INSTANCE.IEE_DataAcquisitionEnable(userID.getValue(), true);
						readytocollect = true;
					}
			} else if (state != EdkErrorCode.EDK_NO_EVENT.ToInt()) {
				System.out.println("Internal error in Emotiv Engine!");
				break;
			}

			if (readytocollect) {
				IEegData.INSTANCE.IEE_DataUpdateHandle(0, hData);

				IEegData.INSTANCE.IEE_DataGetNumberOfSample(hData, nSamplesTaken);

				if (nSamplesTaken != null) {
					if (nSamplesTaken.getValue() != 0) {

						System.out.print("Updated: ");
						System.out.println(nSamplesTaken.getValue());

						double[] data = new double[nSamplesTaken.getValue()];
						for (int sampleIdx = 0; sampleIdx < nSamplesTaken.getValue(); ++sampleIdx) {
							for (int i = 0; i < 17; i++) {

								IEegData.INSTANCE.IEE_DataGet(hData, i, data, nSamplesTaken.getValue());
								
								System.out.print(data[sampleIdx]);
								System.out.print(",");
							}
							System.out.println();
						}
					}
				}
			}
		}

		Edk.INSTANCE.IEE_EngineDisconnect();
		Edk.INSTANCE.IEE_EmoStateFree(eState);
		Edk.INSTANCE.IEE_EmoEngineEventFree(eEvent);
		System.out.println("Disconnected!");
	}
}
