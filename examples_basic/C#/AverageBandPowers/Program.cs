/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - AverageBandPowers
** The average band power for a specific channel from the latest epoch with
** 0.5 seconds step size and 2 seconds window size
** This example is used for single connection.
**
****************************************************************************/

using System;
using System.Collections.Generic;
using Emotiv;
using System.IO;
using System.Threading;
using System.Reflection;

namespace AverageBandPowers
{
    class AverageBandPowers
    {
        static int userID = -1;
        static string filename = "AverageBandPowers.csv";
        static TextWriter file = new StreamWriter(filename, false);

        static void engine_UserAdded_Event(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("User Added Event has occured");
            userID = (int)e.userId;

            EmoEngine.Instance.IEE_FFTSetWindowingType((uint)userID, EdkDll.IEE_WindowingTypes.IEE_HAMMING);
        }


        static void Main(string[] args)
        {
            Console.WriteLine("===================================================================================");
            Console.WriteLine("Example to get the average band power for a specific channel from the latest epoch.");
            Console.WriteLine("===================================================================================");

            AverageBandPowers p = new AverageBandPowers();

            // create the engine
            EmoEngine engine = EmoEngine.Instance;
            engine.UserAdded += new EmoEngine.UserAddedEventHandler(engine_UserAdded_Event);
            engine.Connect();

            string header = "Theta, Alpha, Low_beta, High_beta, Gamma"; ;
            file.WriteLine(header);
            file.WriteLine("");

            EdkDll.IEE_DataChannel_t[] channelList = new EdkDll.IEE_DataChannel_t[5] { EdkDll.IEE_DataChannel_t.IED_AF3, 
                                                                                       EdkDll.IEE_DataChannel_t.IED_AF4, 
                                                                                       EdkDll.IEE_DataChannel_t.IED_T7, 
                                                                                       EdkDll.IEE_DataChannel_t.IED_T8, 
                                                                                       EdkDll.IEE_DataChannel_t.IED_O1 };

            while (true)
            {
                engine.ProcessEvents(10);

                if (userID < 0) continue;

                if (Console.KeyAvailable)
                    break;
                
                double[] alpha     = new double[1];
                double[] low_beta  = new double[1];
                double[] high_beta = new double[1];
                double[] gamma     = new double[1];
                double[] theta     = new double[1];
                
                for(int i = 0 ; i < 5 ; i ++)
                {
                    Int32 result = engine.IEE_GetAverageBandPowers((uint)userID, channelList[i], theta, alpha, low_beta, high_beta, gamma);
                    if (result == EdkDll.EDK_OK)
                    {
                        file.Write(theta[0] + ",");
                        file.Write(alpha[0] + ",");
                        file.Write(low_beta[0] + ",");
                        file.Write(high_beta[0] + ",");
                        file.WriteLine(gamma[0] + ",");

                        Console.WriteLine("Theta: " + theta[0]);
                    }
                }
            }

            file.Close();
            engine.Disconnect();
        }
    }
}
