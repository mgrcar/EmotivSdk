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
        static System.IO.StreamWriter perLog = new System.IO.StreamWriter("PerformanceMetric.csv");
        static bool writeHeader = false;

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

            Console.WriteLine("{0},{1},{2}", timeFromStart, headsetOn, signalStrength);           
        }

        static void CaculateScale (Double rawScore, Double maxScale, Double minScale, out Double scaledScore)
        {
            if (rawScore < minScale)
                scaledScore = 0;
            else if (rawScore > maxScale)
                scaledScore = 1;
            else
                scaledScore = (rawScore - minScale)/(maxScale - minScale);
        }

        static void engine_PerformanceMetricEmoStateUpdated(object sender, EmoStateUpdatedEventArgs e)
        {
            EmoState eState = e.emoState;

            if (!writeHeader) {
                writeHeader = true;
                perLog.Write("Time, ");
                perLog.Write("Stress raw score, ");
                perLog.Write("Stress min score, ");
                perLog.Write("Stress max score, ");
                perLog.Write("Stress scaled score, ");
                perLog.Write("Engagement boredom raw score,");
                perLog.Write("Engagement boredom min score,");
                perLog.Write("Engagement boredom max score,");
                perLog.Write("Engagement boredom scaled score,");
                perLog.Write("Relaxation raw score,");
                perLog.Write("Relaxation min score,");
                perLog.Write("Relaxation max score,");
                perLog.Write("Relaxation scaled score,");
                perLog.Write("Excitement raw score,");
                perLog.Write("Excitement min score,");
                perLog.Write("Excitement max score,");
                perLog.Write("Excitement scaled score,");
                perLog.Write("Interest raw score,");
                perLog.Write("Interest min score,");
                perLog.Write("Interest max score,");
                perLog.Write("Interest scaled score");
                perLog.WriteLine("");
            }

            Single timeFromStart = eState.GetTimeFromStart();
            perLog.Write(timeFromStart + ",");

            Double rawScore=0;
            Double minScale=0;
            Double maxScale=0;	
            Double scaledScore=0;
            eState.PerformanceMetricGetStressModelParams(out rawScore, out minScale, out maxScale);
            perLog.Write(rawScore + ",");
            perLog.Write(minScale + ",");
            perLog.Write(maxScale + ",");

            if (minScale == maxScale)
            {
                perLog.Write("undefined" + ",");
            }
            else{
                CaculateScale(rawScore, maxScale, minScale, out scaledScore);
                perLog.Write(scaledScore + ",");
            }		
            
            eState.PerformanceMetricGetEngagementBoredomModelParams(out rawScore, out minScale, out maxScale);
            perLog.Write(rawScore + ",");
            perLog.Write(minScale + ",");
            perLog.Write(maxScale + ",");
            
            if (minScale == maxScale)
                perLog.Write("undefined" + ",");
            else{
                CaculateScale(rawScore, maxScale, minScale, out scaledScore);
                perLog.Write(scaledScore + ",");
            }		
            
            eState.PerformanceMetricGetRelaxationModelParams(out rawScore, out minScale, out maxScale);
            perLog.Write(rawScore + ",");
            perLog.Write(minScale + ",");
            perLog.Write(maxScale + ",");
            
            if (minScale == maxScale)
                perLog.Write("undefined" + ",");
            else{
                CaculateScale(rawScore, maxScale, minScale, out scaledScore);
                perLog.Write(scaledScore + ",");
            }		
            
            eState.PerformanceMetricGetInstantaneousExcitementModelParams(out rawScore, out minScale, out maxScale);
            perLog.Write(rawScore + ",");
            perLog.Write(minScale + ",");
            perLog.Write(maxScale + ",");
            
            if (minScale == maxScale)
                perLog.Write("undefined" + ",");
            else{
                CaculateScale(rawScore, maxScale, minScale, out scaledScore);
                perLog.Write(scaledScore + ",");
            }	

            eState.PerformanceMetricGetInterestModelParams(out rawScore, out minScale, out maxScale);
            perLog.Write(rawScore + ",");
            perLog.Write(minScale + ",");
            perLog.Write(maxScale + ",");
            
            if (minScale == maxScale)
                perLog.Write("undefined" + ",");
            else{
                CaculateScale(rawScore, maxScale, minScale, out scaledScore);
                perLog.Write(scaledScore + ",");
            }	

            perLog.WriteLine("");
            perLog.Flush();
        }              

        static void keyHandler(ConsoleKey key)
        {
            switch (key)
            {
                case ConsoleKey.F1:
                    break;
                case ConsoleKey.F9:                    
                    String version;
                    UInt32 buildNum;
                    EmoEngine.Instance.SoftwareGetVersion(out version, out buildNum);
                    Console.WriteLine("Software Version: {0}, {1}", version, buildNum);
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
            engine.PerformanceMetricEmoStateUpdated +=
                new EmoEngine.PerformanceMetricEmoStateUpdatedEventHandler(engine_PerformanceMetricEmoStateUpdated);            
            engine.EmoEngineEmoStateUpdated +=
                new EmoEngine.EmoEngineEmoStateUpdatedEventHandler(engine_EmoEngineEmoStateUpdated);


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
