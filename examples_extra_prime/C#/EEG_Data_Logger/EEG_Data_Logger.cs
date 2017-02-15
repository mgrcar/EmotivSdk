using System;
using System.Collections.Generic;
using Emotiv;
using System.IO;
using System.Threading;
using System.Reflection;

namespace EEG_Data_Logger
{
    class EEG_Logger
    {
        EmoEngine engine;   // Access to the EDK is via the EmoEngine 
        int userID = -1;    // userID is used to uniquely identify a user's headset
        string filename = "EEG_Data_Logger.csv"; // output filename        

        EEG_Logger()
        {            
            // create the engine
            engine = EmoEngine.Instance;
            engine.UserAdded += new EmoEngine.UserAddedEventHandler(engine_UserAdded_Event);
            
            // connect to Emoengine.            
            engine.Connect();

            // create a header for our output file
            WriteHeader();
        }

        void engine_UserAdded_Event(object sender, EmoEngineEventArgs e)
        {
            Console.WriteLine("User Added Event has occured");

            // record the user 
            userID = (int)e.userId;

            // enable data aquisition for this user.
            engine.DataAcquisitionEnable((uint)userID, true);
            
            // ask for up to 1 second of buffered data
            engine.DataSetBufferSizeInSec(1); 

        }
        void Run()
        {
            // Handle any waiting events
            engine.ProcessEvents();

            // If the user has not yet connected, do not proceed
            if ((int)userID == -1)
                return;

            Dictionary<EdkDll.IEE_DataChannel_t, double[]> data = engine.GetData((uint)userID);

            if (data == null)
            {
                return;
            }

            int _bufferSize = data[EdkDll.IEE_DataChannel_t.IED_TIMESTAMP].Length;

            Console.WriteLine("Writing " + _bufferSize.ToString() + " sample of data ");

            // Write the data to a file
            TextWriter file = new StreamWriter(filename,true);

            for (int i = 0; i < _bufferSize; i++)
            {
                // now write the data
                foreach (EdkDll.IEE_DataChannel_t channel in data.Keys)
                    file.Write(data[channel][i] + ",");
                file.WriteLine("");

            }
            file.Close();

        }

        public void WriteHeader()
        {
            TextWriter file = new StreamWriter(filename, false);

            string header = "COUNTER, INTERPOLATED, RAW_CQ, AF3, F7, F3, FC5, T7, P7, O1, O2, P8," +
                "T8, FC6, F4, F8, AF4, GYROX, GYROY, TIMESTAMP, MARKER_HARDWARE, ES_TIMESTAMP, FUNC_ID, FUNC_VALUE, MARKER, SYNC_SIGNAL";
            
            file.WriteLine(header);
            file.Close();
        }

        static void Main(string[] args)
        {
            Console.WriteLine("EEG Data Reader Example");            

            EEG_Logger p = new EEG_Logger();

            for (int i = 0; i < 5000; i++)
            {
                //Example for set marker to data stream and set sychronization signal
                //if (i % 37 == 0)
                //{
                //    p.engine.DataSetMarker((uint)p.userID, i);
                //    p.engine.DataSetSychronizationSignal((uint)p.userID, i);
                   
                //}
                
                p.Run();
                Thread.Sleep(10);
            }
        }
    }
}
