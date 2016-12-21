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
        static string licenseKey = "";           // Your License Key

        static string format = "dd/MM/yyyy";

        void activateLicense()
        {
            int result = EdkDll.IEE_ActivateLicense(licenseKey);
            if (result == EdkDll.EDK_OK || result == EdkDll.EDK_LICENSE_REGISTERED)
            {
                Console.WriteLine("License activated.");
            }
            else Console.WriteLine("License Error. " + result);
        }

        public DateTime FromUnixTime(uint unixTime)
        {
            var epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            return epoch.AddSeconds(unixTime);
        }

        public long ToUnixTime(DateTime date)
        {
            var epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            return Convert.ToInt64((date - epoch).TotalSeconds);
        }

        void licenseInformation()
        {
            EdkDll.IEE_LicenseInfos_t licenseInfos = new EdkDll.IEE_LicenseInfos_t(); 
            int result = EdkDll.IEE_LicenseInformation(ref licenseInfos);

            Console.WriteLine();
            Console.WriteLine("Date From  : " + FromUnixTime(licenseInfos.date_from).ToString(format));
            Console.WriteLine("Date To    : " + FromUnixTime(licenseInfos.date_to).ToString(format));
            Console.WriteLine();

            Console.WriteLine( "Seat number: " + licenseInfos. seat_count );
            Console.WriteLine();

            Console.WriteLine( "Total Quota: " + licenseInfos.quota );
            Console.WriteLine( "Total quota used    : " + licenseInfos.usedQuota );
            Console.WriteLine();

            Console.WriteLine( "Quota limit in day  : " + licenseInfos.quotaDayLimit );
            Console.WriteLine( "Quota used in day   : " + licenseInfos.usedQuotaDay );
            Console.WriteLine();

            Console.WriteLine( "Quota limit in month: " + licenseInfos.quotaMonthLimit );
            Console.WriteLine( "Quota used in month : " + licenseInfos.usedQuotaMonth );
            Console.WriteLine();

            switch ((int)licenseInfos.scopes)
            {
            case (int)EdkDll.IEE_LicenseType_t.IEE_EEG:

                Console.WriteLine( "License type : EEG" );
                Console.WriteLine();
                break;
            case (int)EdkDll.IEE_LicenseType_t.IEE_EEG_PM:

                Console.WriteLine( "License type : EEG + PM" );
                Console.WriteLine();
                break;
            case (int)EdkDll.IEE_LicenseType_t.IEE_PM:
                Console.WriteLine( "License type : PM" );
                Console.WriteLine();
                break;
            default:
                Console.WriteLine( "License type : No type" );
                Console.WriteLine();
                break;
            }
        }

        EEG_Logger()
        {
            // Just 1 time with the Internet connection
            activateLicense(); 

            // create the engine
            engine = EmoEngine.Instance;
            engine.UserAdded += new EmoEngine.UserAddedEventHandler(engine_UserAdded_Event);
            
            // connect to Emoengine.            
            engine.Connect();

            licenseInformation();

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
                "T8, FC6, F4, F8, AF4, GYROX, GYROY, TIMESTAMP, ES_TIMESTAMP, FUNC_ID, FUNC_VALUE, MARKER, SYNC_SIGNAL";
            
            file.WriteLine(header);
            file.Close();
        }

        static void Main(string[] args)
        {
            Console.WriteLine("EEG Data Reader Example");            

            EEG_Logger p = new EEG_Logger();

            for (int i = 0; i < 5000; i++)
            {
                p.Run();
                Thread.Sleep(10);
            }
        }
    }
}
