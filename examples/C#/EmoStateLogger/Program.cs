/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - EmoStateLogger
** This example demonstrates the use of the core Emotiv API functions.
** It logs all Emotiv detection results for the attached users after
** successfully establishing a connection to Emotiv EmoEngineTM or
** EmoComposerTM
****************************************************************************/

using System;
using System.Collections.Generic;
using Emotiv;
using System.IO;
using System.Threading;

namespace EmoStateLogger
{
    class EmoStateLogger
    {
        static System.IO.StreamWriter engineLog = new System.IO.StreamWriter("engineLog.log");
        static System.IO.StreamWriter expLog = new System.IO.StreamWriter("FacialExpression.log");
        static System.IO.StreamWriter cogLog = new System.IO.StreamWriter("MentalCommand.log");

        static Boolean enableLoger = false;

        static void engine_EmoEngineConnected(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("Emoengine connected");
        }

        static void engine_EmoEngineDisconnected(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("Emoengine disconnected");
        }
        static void engine_UserAdded(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("user added ({0})", e.userId);
        }
        static void engine_UserRemoved(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("user removed");
        }

        static void engine_EmoStateUpdated(object sender, EmoStateUpdatedEventArgs e)
        {
            EmoState es = e.emoState;

            Single timeFromStart = es.GetTimeFromStart();
            //Console.WriteLine("new emostate {0}", timeFromStart);
        }

        static void engine_EmoEngineEmoStateUpdated(object sender, EmoStateUpdatedEventArgs e)
        {
            EmoState es = e.emoState;

            Single timeFromStart = es.GetTimeFromStart();

            Int32 headsetOn = es.GetHeadsetOn();

            EdkDll.IEE_SignalStrength_t signalStrength = es.GetWirelessSignalStatus();
            Int32 chargeLevel = 0;
            Int32 maxChargeLevel = 0;
            es.GetBatteryChargeLevel(out chargeLevel, out maxChargeLevel);

            engineLog.Write(
                "{0},{1},{2},{3},{4},",
                timeFromStart,
                headsetOn, signalStrength, chargeLevel, maxChargeLevel);
            
            engineLog.WriteLine("");
            engineLog.Flush();
        }  

        static void engine_MentalCommandEmoStateUpdated(object sender, EmoStateUpdatedEventArgs e)
        {
            EmoState es = e.emoState;

            Single timeFromStart = es.GetTimeFromStart();

            EdkDll.IEE_MentalCommandAction_t cogAction = es.MentalCommandGetCurrentAction();
            Single power = es.MentalCommandGetCurrentActionPower();
            Boolean isActive = es.MentalCommandIsActive();

            cogLog.WriteLine("{0},{1},{2},{3}", timeFromStart, cogAction, power, isActive);
            cogLog.Flush();
            if (enableLoger)
                Console.WriteLine("{0},{1},{2},{3}", timeFromStart, cogAction, power, isActive);
        }

        static void engine_FacialExpressionEmoStateUpdated(object sender, EmoStateUpdatedEventArgs e)
        {
            EmoState es = e.emoState;

            Single timeFromStart = es.GetTimeFromStart();

            EdkDll.IEE_FacialExpressionAlgo_t[] expAlgoList = { 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_BLINK, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_CLENCH, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_SUPRISE, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_FROWN, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_HORIEYE, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_NEUTRAL, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_SMILE, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_WINK_LEFT, 
                                                      EdkDll.IEE_FacialExpressionAlgo_t.FE_WINK_RIGHT
                                                      };
            Boolean[] isExpActiveList = new Boolean[expAlgoList.Length];

            Boolean isBlink = es.FacialExpressionIsBlink();
            Boolean isLeftWink = es.FacialExpressionIsLeftWink();
            Boolean isRightWink = es.FacialExpressionIsRightWink();
            Boolean isEyesOpen = es.FacialExpressionIsEyesOpen();
            Boolean isLookingUp = es.FacialExpressionIsLookingUp();
            Boolean isLookingDown = es.FacialExpressionIsLookingDown();
            Single leftEye = 0.0F;
            Single rightEye = 0.0F;
            Single x = 0.0F;
            Single y = 0.0F;
            es.FacialExpressionGetEyelidState(out leftEye, out rightEye);
            es.FacialExpressionGetEyeLocation(out x, out y);
            Single eyebrowExtent = es.FacialExpressionGetEyebrowExtent();
            Single smileExtent = es.FacialExpressionGetSmileExtent();
            Single clenchExtent = es.FacialExpressionGetClenchExtent();
            EdkDll.IEE_FacialExpressionAlgo_t upperFaceAction = es.FacialExpressionGetUpperFaceAction();
            Single upperFacePower = es.FacialExpressionGetUpperFaceActionPower();
            EdkDll.IEE_FacialExpressionAlgo_t lowerFaceAction = es.FacialExpressionGetLowerFaceAction();
            Single lowerFacePower = es.FacialExpressionGetLowerFaceActionPower();
            for (int i = 0; i < expAlgoList.Length; ++i)
            {
                isExpActiveList[i] = es.FacialExpressionIsActive(expAlgoList[i]);
            }

            expLog.Write(
                "{0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15},{16},",
                timeFromStart,
                isBlink, isLeftWink, isRightWink, isEyesOpen, isLookingUp,
                isLookingDown, leftEye, rightEye,
                x, y, eyebrowExtent, smileExtent, upperFaceAction,
                upperFacePower, lowerFaceAction, lowerFacePower);
            for (int i = 0; i < expAlgoList.Length; ++i)
            {
                expLog.Write("{0},", isExpActiveList[i]);
            }
            expLog.WriteLine("");
            expLog.Flush();
        }

        static void engine_MentalCommandTrainingStarted(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("Start MentalCommand Training");
        }

        static void engine_MentalCommandTrainingSucceeded(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("MentalCommand Training Success. (A)ccept/Reject?");
            ConsoleKeyInfo cki = Console.ReadKey(true);
            if (cki.Key == ConsoleKey.A)
            {
                Console.WriteLine("Accept!!!");
                EmoEngine.Instance.MentalCommandSetTrainingControl(0, EdkDll.IEE_MentalCommandTrainingControl_t.MC_ACCEPT);
            }
            else
            {
                EmoEngine.Instance.MentalCommandSetTrainingControl(0, EdkDll.IEE_MentalCommandTrainingControl_t.MC_REJECT);
            }
        }

        static void engine_MentalCommandTrainingCompleted(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("MentalCommand Training Completed.");
        }

        static void engine_MentalCommandTrainingRejected(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("MentalCommand Training Rejected.");
        }

        static void engine_FacialExpressionTrainingStarted(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("Start FacialExpression Training");
        }

        static void engine_FacialExpressionTrainingSucceeded(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("FacialExpression Training Success. (A)ccept/Reject?");
            ConsoleKeyInfo cki = Console.ReadKey(true);
            if (cki.Key == ConsoleKey.A)
            {
                Console.WriteLine("Accept!!!");
                EmoEngine.Instance.FacialExpressionSetTrainingControl(0, EdkDll.IEE_FacialExpressionTrainingControl_t.FE_ACCEPT);
            }
            else
            {
                EmoEngine.Instance.FacialExpressionSetTrainingControl(0, EdkDll.IEE_FacialExpressionTrainingControl_t.FE_REJECT);
            }
        }

        static void engine_FacialExpressionTrainingCompleted(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("FacialExpressione Training Completed.");
        }

        static void engine_FacialExpressionTrainingRejected(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("FacialExpression Training Rejected.");
        }

        static void keyHandler(ConsoleKey key)
        {
            switch (key)
            {
                case ConsoleKey.F1:                    					
					ulong action1 = (ulong)EdkDll.IEE_MentalCommandAction_t.MC_LEFT;
                    ulong action2 = (ulong)EdkDll.IEE_MentalCommandAction_t.MC_RIGHT;
                    ulong listAction = action1 | action2;
                    EmoEngine.Instance.MentalCommandSetActiveActions(0, listAction);					
                    Console.WriteLine("Setting MentalCommand active actions for user");
                    break;
                case ConsoleKey.F2:
                    EmoEngine.Instance.MentalCommandSetTrainingAction(0, EdkDll.IEE_MentalCommandAction_t.MC_NEUTRAL);
                    EmoEngine.Instance.MentalCommandSetTrainingControl(0, EdkDll.IEE_MentalCommandTrainingControl_t.MC_START);
                    break;
                case ConsoleKey.F3:
                    EmoEngine.Instance.MentalCommandSetTrainingAction(0, EdkDll.IEE_MentalCommandAction_t.MC_RIGHT);
                    EmoEngine.Instance.MentalCommandSetTrainingControl(0, EdkDll.IEE_MentalCommandTrainingControl_t.MC_START);
                    break;
                case ConsoleKey.F4:
                    EmoEngine.Instance.MentalCommandSetTrainingAction(0, EdkDll.IEE_MentalCommandAction_t.MC_LEFT);
                    EmoEngine.Instance.MentalCommandSetTrainingControl(0, EdkDll.IEE_MentalCommandTrainingControl_t.MC_START);
                    break;
                case ConsoleKey.F5:
                    EmoEngine.Instance.MentalCommandSetActivationLevel(0, 2);
                    Console.WriteLine("MentalCommand Activateion level set to {0}", EmoEngine.Instance.MentalCommandGetActivationLevel(0));
                    break;
                case ConsoleKey.F6:
                    Console.WriteLine("MentalCommand Activateion level is {0}", EmoEngine.Instance.MentalCommandGetActivationLevel(0));
                    break;
                case ConsoleKey.F7:
                    Console.WriteLine("Get the current overall skill rating: {0}", EmoEngine.Instance.MentalCommandGetOverallSkillRating(0));
                    break;
                case ConsoleKey.F8:
                    EmoEngine.Instance.FacialExpressionSetTrainingAction(0, EdkDll.IEE_FacialExpressionAlgo_t.FE_CLENCH);
                    EmoEngine.Instance.FacialExpressionSetTrainingControl(0, EdkDll.IEE_FacialExpressionTrainingControl_t.FE_START);
                    break;
                case ConsoleKey.F9:                    
                    String version;
                    UInt32 buildNum;
                    EmoEngine.Instance.SoftwareGetVersion(out version, out buildNum);
                    Console.WriteLine("Software Version: {0}, {1}", version, buildNum);
                    break;
                case ConsoleKey.F10:
                    enableLoger = !enableLoger;
                    break;

                default:
                    break;
            }
        }

        static void Main(string[] args)
        {
            EmoEngine engine = EmoEngine.Instance;

            engine.EmoEngineConnected +=
                new EmoEngine.EmoEngineConnectedEventHandler(engine_EmoEngineConnected);
            engine.EmoEngineDisconnected +=
                new EmoEngine.EmoEngineDisconnectedEventHandler(engine_EmoEngineDisconnected);
            engine.UserAdded +=
                new EmoEngine.UserAddedEventHandler(engine_UserAdded);
            engine.UserRemoved +=
                new EmoEngine.UserRemovedEventHandler(engine_UserRemoved);
            engine.EmoStateUpdated +=
                new EmoEngine.EmoStateUpdatedEventHandler(engine_EmoStateUpdated);
            engine.FacialExpressionEmoStateUpdated +=
                new EmoEngine.FacialExpressionEmoStateUpdatedEventHandler(engine_FacialExpressionEmoStateUpdated);
            engine.MentalCommandEmoStateUpdated +=
                new EmoEngine.MentalCommandEmoStateUpdatedEventHandler(engine_MentalCommandEmoStateUpdated);            
            engine.EmoEngineEmoStateUpdated +=
                new EmoEngine.EmoEngineEmoStateUpdatedEventHandler(engine_EmoEngineEmoStateUpdated);
            engine.MentalCommandTrainingStarted +=
                new EmoEngine.MentalCommandTrainingStartedEventEventHandler(engine_MentalCommandTrainingStarted);
            engine.MentalCommandTrainingSucceeded +=
                new EmoEngine.MentalCommandTrainingSucceededEventHandler(engine_MentalCommandTrainingSucceeded);
            engine.MentalCommandTrainingCompleted +=
                new EmoEngine.MentalCommandTrainingCompletedEventHandler(engine_MentalCommandTrainingCompleted);
            engine.MentalCommandTrainingRejected +=
                new EmoEngine.MentalCommandTrainingRejectedEventHandler(engine_MentalCommandTrainingRejected);
            engine.FacialExpressionTrainingStarted +=
                new EmoEngine.FacialExpressionTrainingStartedEventEventHandler(engine_FacialExpressionTrainingStarted);
            engine.FacialExpressionTrainingSucceeded +=
                new EmoEngine.FacialExpressionTrainingSucceededEventHandler(engine_FacialExpressionTrainingSucceeded);
            engine.FacialExpressionTrainingCompleted +=
                new EmoEngine.FacialExpressionTrainingCompletedEventHandler(engine_FacialExpressionTrainingCompleted);
            engine.FacialExpressionTrainingRejected +=
                new EmoEngine.FacialExpressionTrainingRejectedEventHandler(engine_FacialExpressionTrainingRejected);

            engine.Connect();

            Console.WriteLine("===========================================================================");
            Console.WriteLine("Example to show how to log the EmoState from EmoEngine.");
            Console.WriteLine("===========================================================================");

            ConsoleKeyInfo cki = new ConsoleKeyInfo();

            while (true)
            {
                try
                {
                    if (Console.KeyAvailable)
                    {
                        cki = Console.ReadKey(true);
                        keyHandler(cki.Key);

                        if (cki.Key == ConsoleKey.X)
                        {
                            break;
                        }
                    }
                    engine.ProcessEvents(5);
                }
                catch (EmoEngineException e)
                {
                    Console.WriteLine("{0}", e.ToString());
                }
                catch (Exception e)
                {
                    Console.WriteLine("{0}", e.ToString());
                }
            }
            engine.Disconnect();
        }
    }
}
