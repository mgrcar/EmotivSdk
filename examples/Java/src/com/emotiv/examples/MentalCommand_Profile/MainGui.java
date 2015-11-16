package com.emotiv.examples.MentalCommand_Profile;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.xml.soap.MessageFactory;

import com.emotiv.Iedk.Edk;
import com.emotiv.Iedk.EdkErrorCode;
import com.emotiv.Iedk.EmoState;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;

//import statements
//Check if window closes automatically. Otherwise add suitable code
public class MainGui extends JFrame {
	public static JButton trainBtt, saveBtt, loadBtt;
	public static JComboBox comboBox;
	public static int[] MentalCommandActionList = {
			EmoState.IEE_MentalCommandAction_t.MC_NEUTRAL.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_PUSH.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_PULL.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_LIFT.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_DROP.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_LEFT.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_RIGHT.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_ROTATE_LEFT.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_ROTATE_RIGHT.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_ROTATE_CLOCKWISE.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_ROTATE_COUNTER_CLOCKWISE
					.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_ROTATE_FORWARDS.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_ROTATE_REVERSE.ToInt(),
			EmoState.IEE_MentalCommandAction_t.MC_DISAPPEAR.ToInt() };
	public static Boolean[] MentalCommandActionsEnabled = new Boolean[MentalCommandActionList.length];

	// / <summary>
	// / Start traning MentalCommand action
	// / </summary>
	// / <param name="MentalCommandAction">MentalCommand Action</param>
	public static void StartTrainingMentalCommand(
			EmoState.IEE_MentalCommandAction_t MentalCommandAction) {
		if (MentalCommandAction == EmoState.IEE_MentalCommandAction_t.MC_NEUTRAL) {
			Edk.INSTANCE.IEE_MentalCommandSetTrainingAction(0,
					EmoState.IEE_MentalCommandAction_t.MC_NEUTRAL.ToInt());
			Edk.INSTANCE.IEE_MentalCommandSetTrainingControl(0,
					Edk.IEE_MentalCommandTrainingControl_t.MC_START.getType());
		} else
			for (int i = 1; i < MentalCommandActionList.length; i++) {
				if (MentalCommandAction.ToInt() == MentalCommandActionList[i]) {

					if (MentalCommandActionsEnabled[i]) {
						Edk.INSTANCE.IEE_MentalCommandSetTrainingAction(0,
								MentalCommandAction.ToInt());
						Edk.INSTANCE.IEE_MentalCommandSetTrainingControl(0,
								Edk.IEE_MentalCommandTrainingControl_t.MC_START
										.getType());
					}

				}
			}

	}

	// / <summary>
	// / Enable MentalCommand action in arraylist
	// / </summary>
	// / <param name="MentalCommandAction">MentalCommand Action</param>
	// / <param name="iBool">True = Enable/False = Disable</param>
	public static void EnableMentalCommandAction(
			EmoState.IEE_MentalCommandAction_t MentalCommandAction,
			Boolean iBool) {
		for (int i = 1; i < MentalCommandActionList.length; i++) {
			if (MentalCommandAction.ToInt() == MentalCommandActionList[i]) {
				MentalCommandActionsEnabled[i] = iBool;

			}
		}

	}

	public static void EnableMentalCommandActionsList() {
		long MentalCommandActions = 0x0000;
		for (int i = 1; i < MentalCommandActionList.length; i++) {
			if (MentalCommandActionsEnabled[i]) {
				MentalCommandActions = MentalCommandActions
						| ((long) MentalCommandActionList[i]);

			}
		}
		Edk.INSTANCE.IEE_MentalCommandSetActiveActions(0, MentalCommandActions);

	}

	public static void main(String args[]) {
		new MainGui();
		Pointer eEvent = Edk.INSTANCE.IEE_EmoEngineEventCreate();
		Pointer eState = Edk.INSTANCE.IEE_EmoStateCreate();
		IntByReference userID = null;
		short composerPort = 1726;
		int option = 1;
		int state = 0;

		userID = new IntByReference(0);

		switch (option) {
		case 1: {
			if (Edk.INSTANCE.IEE_EngineConnect("Emotiv Systems-5") != EdkErrorCode.EDK_OK
					.ToInt()) {
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

		while (true) {
			state = Edk.INSTANCE.IEE_EngineGetNextEvent(eEvent);

			// New event needs to be handled
			if (state == EdkErrorCode.EDK_OK.ToInt()) {

				int eventType = Edk.INSTANCE.IEE_EmoEngineEventGetType(eEvent);
				Edk.INSTANCE.IEE_EmoEngineEventGetUserId(eEvent, userID);
				if (eventType == Edk.IEE_Event_t.IEE_UserAdded.ToInt()) {
					EmoProfileManagement.AddNewProfile("3");
					JOptionPane.showMessageDialog(new JFrame(), "User add",
							"Dialog", JOptionPane.ERROR_MESSAGE);
				}

				if (eventType == Edk.IEE_Event_t.IEE_MentalCommandEvent.ToInt()) {
					int mcType = Edk.INSTANCE
							.IEE_MentalCommandEventGetType(eEvent);

					if (mcType == Edk.IEE_MentalCommandEvent_t.IEE_MentalCommandTrainingStarted
							.getType()) {
						JOptionPane.showMessageDialog(new JFrame(),
								"MentalCommand Training Start", "Dialog",
								JOptionPane.ERROR_MESSAGE);
					}
					if (mcType == Edk.IEE_MentalCommandEvent_t.IEE_MentalCommandTrainingCompleted
							.getType()) {
						JOptionPane.showMessageDialog(new JFrame(),
								"MentalCommand Training Complete", "Dialog",
								JOptionPane.ERROR_MESSAGE);
					}
					if (mcType == Edk.IEE_MentalCommandEvent_t.IEE_MentalCommandTrainingSucceeded
							.getType()) {
						Edk.INSTANCE
								.IEE_MentalCommandSetTrainingControl(
										0,
										Edk.IEE_MentalCommandTrainingControl_t.MC_ACCEPT
												.getType());
						JOptionPane.showMessageDialog(new JFrame(),
								"MentalCommand Training Succeeded", "Dialog",
								JOptionPane.ERROR_MESSAGE);
					}
					if (mcType == Edk.IEE_MentalCommandEvent_t.IEE_MentalCommandTrainingFailed
							.getType()) {
						JOptionPane.showMessageDialog(new JFrame(),
								"MentalCommand Training Failed", "Dialog",
								JOptionPane.ERROR_MESSAGE);
					}
					if (mcType == Edk.IEE_MentalCommandEvent_t.IEE_MentalCommandTrainingRejected
							.getType()) {
						JOptionPane.showMessageDialog(new JFrame(),
								"MentalCommand Training Rejected", "Dialog",
								JOptionPane.ERROR_MESSAGE);
					}
				}
				if (eventType == Edk.IEE_Event_t.IEE_EmoStateUpdated.ToInt()) {
					Edk.INSTANCE.IEE_EmoEngineEventGetEmoState(eEvent, eState);

					// {
					int action = EmoState.INSTANCE
							.IS_MentalCommandGetCurrentAction(eState);
					double power = EmoState.INSTANCE
							.IS_MentalCommandGetCurrentActionPower(eState);
					if (power != 0) {
						System.out.println("Action:" + action);
						System.out.println("Power:" + power);
					}
					// }
				}
			} else if (state != EdkErrorCode.EDK_NO_EVENT.ToInt()) {
				System.out.println("Internal error in Emotiv Engine!");
				break;
			}
		}

		Edk.INSTANCE.IEE_EngineDisconnect();
		System.out.println("Disconnected!");
	}

	MainGui() {
		MentalCommandActionsEnabled[0] = true;
		for (int i = 1; i < MentalCommandActionList.length; i++) {
			MentalCommandActionsEnabled[i] = false;
		}
		this.setSize(600, 300);
		setVisible(true);
		Container content = getContentPane();
		content.setBackground(Color.white);
		content.setLayout(new FlowLayout());

		String[] options = { "Neutral", "Push", "Lift" };
		comboBox = new JComboBox(options);

		add(comboBox);
		trainBtt = new JButton("Train");
		trainBtt.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {

				int index = comboBox.getSelectedIndex();
				if (index == 0) {
					Edk.INSTANCE.IEE_MentalCommandSetTrainingAction(0,
							EmoState.IEE_MentalCommandAction_t.MC_NEUTRAL
									.ToInt());
					Edk.INSTANCE.IEE_MentalCommandSetTrainingControl(0,
							Edk.IEE_MentalCommandTrainingControl_t.MC_START
									.getType());
				}
				if (index == 1) {
					try {
						EnableMentalCommandAction(
								EmoState.IEE_MentalCommandAction_t.MC_PUSH,
								true);
						EnableMentalCommandActionsList();
						StartTrainingMentalCommand(EmoState.IEE_MentalCommandAction_t.MC_PUSH);
					} catch (Exception ex) {
						ex.printStackTrace();
					}
				}
				if (index == 2) {
					try {
						EnableMentalCommandAction(
								EmoState.IEE_MentalCommandAction_t.MC_LIFT,
								true);
						EnableMentalCommandActionsList();
						StartTrainingMentalCommand(EmoState.IEE_MentalCommandAction_t.MC_LIFT);
					} catch (Exception ex) {
						ex.printStackTrace();
					}
				}
			}
		});

		add(trainBtt);

		// / Save Profile handle
		saveBtt = new JButton("Save Profile");
		saveBtt.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {

				EmoProfileManagement.SaveCurrentProfile();
				EmoProfileManagement.SaveProfilesToFile();
			}
		});
		add(saveBtt);

		// / Load Profile handle
		loadBtt = new JButton("Load Profile");
		loadBtt.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				EmoProfileManagement.LoadProfilesFromFile();
				EmoProfileManagement.SetUserProfile("1");
				String actionList = EmoProfileManagement.CheckCurrentProfile();
				long MentalCommandActions = Long.valueOf(actionList);
				Edk.INSTANCE.IEE_MentalCommandSetActiveActions(0,
						MentalCommandActions);
			}
		});
		add(loadBtt);
	}

}