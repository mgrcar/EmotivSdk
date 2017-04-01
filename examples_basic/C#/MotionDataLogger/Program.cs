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

            uint EPOCmode, eegRate, eegRes, memsRate, memsRes;

            engine.GetHeadsetSettings(
                (uint)userID,
                out EPOCmode,
                out eegRate,
                out eegRes,
                out memsRate,
                out memsRes
            );

            //\param EPOCmode
            //--- If 0, then EPOC mode is EPOC.
            //--- If 1, then EPOC mode is EPOC+.
            //\param eegRate
            //--- If 0, then EEG sample rate is 128Hz.
            //--- If 1, then EEG sample rate is 256Hz.
            //\param eegRes
            //--- If 0, then EEG resolution is 14bit.
            //--- If 1, then EEG resolution is 16bit.
            //\param memsRate
            //--- If 0, then motion sample rate is OFF.
            //--- If 1, then motion sample rate is 32Hz.
            //--- If 2, then motion sample rate is 64Hz.
            //--- If 3, then motion sample rate is 128Hz.
            //\param memsRes
            //--- If 0, then motion resolution is 12bit.
            //--- If 1, then motion resolution is 14bit.
            //--- If 2, then motion resolution is 16bit.

            Console.WriteLine("Current config: {0} {1} {2} {3} {4}", EPOCmode, eegRate, eegRes, memsRate, memsRes);

            if (memsRate == 0) 
            {
                Console.WriteLine("*** The motion sensors are disabled. Will attempt to enable them. This will only work if the headset is connected via the USB cable. The application will exit after the reconfig attempt.");
                engine.SetHeadsetSettings(
                    (uint)userID,
                    EPOCmode: EPOCmode,
                    eegRate: eegRate,
                    eegRes: eegRes,
                    memsRate: 1,
                    memsRes: memsRes
                );
                engine.Disconnect();
                Environment.Exit(-1);
            }

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
