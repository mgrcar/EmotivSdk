/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - MotionDataLogger
** This example demonstrates how to extract live Motion data using the EmoEngineTM
** Data is read from the headset and sent to an output file for
** later analysis
**
****************************************************************************/

using System;
using System.Collections.Generic;
using Emotiv;
using System.IO;
using System.Threading;
using System.Reflection;

namespace MotionDataLogger
{
    class MotionDataLogger
    {
        static int userID = -1;
        static EmoEngine engine;
        static string filename = "motionDataLogger.csv";
        static TextWriter file = new StreamWriter(filename, false);

        static void engine_UserAdded_Event(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("User Added Event has occured");

            // record the user 
            userID = (int)e.userId;

            // ask for up to 1 second of buffered data
            engine.MotionDataSetBufferSizeInSec(1);
        }

        void Run()
        {           
            // If the user has not yet connected, do not proceed
            if ((int)userID == -1)
                return;

            Dictionary<EdkDll.IEE_MotionDataChannel_t, double[]> data = engine.GetMotionData((UInt32)userID);

            if (data == null)
            {
                return;
            }

            int _bufferSize = data[EdkDll.IEE_MotionDataChannel_t.IMD_COUNTER].Length;

            Console.WriteLine("Writing " + _bufferSize.ToString() + " lines of data ");

            // Write the data to a file

            for (int i = 0; i < _bufferSize; i++)
            {
                // now write the data
                foreach (EdkDll.IEE_MotionDataChannel_t channel in data.Keys)
                    file.Write(data[channel][i] + ",");
                file.WriteLine("");
            }
        }

        static void Main(string[] args)
        {
            Console.WriteLine("===========================================================================");
            Console.WriteLine("Example to show how to log Motion Data from EmoDriver/EmoComposer.");
            Console.WriteLine("===========================================================================");

            MotionDataLogger p = new MotionDataLogger();

            // create the engine
            engine = EmoEngine.Instance;
            engine.UserAdded += new EmoEngine.UserAddedEventHandler(engine_UserAdded_Event);

            // connect to Emoengine.            
            engine.Connect();

            string header = "COUNTER, GYROX, GYROY, GYROZ, ACCX, " +
                "ACCY, ACCZ, MAGX, MAGY, MAGZ, TIMESTAMP";

            file.WriteLine(header);

            while (true)
            {
                if (Console.KeyAvailable)
                    break;

                // Handle any waiting events
                engine.ProcessEvents();

                p.Run();
                Thread.Sleep(10);
            }

            file.Close();
            engine.Disconnect();
        }
    }
}
