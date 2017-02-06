/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - ActivateLicense
** How to activate a license key
 ***
****************************************************************************/

using System;
using System.Collections.Generic;
using Emotiv;
using System.IO;
using System.Threading;
using System.Reflection;

namespace ActivateLicense
{
    class ActivateLicense
    {
        static string licenseKey = "";           // Your License Key

        static string format = "dd/MM/yyyy";

        static void activateLicense()
        {
            int result = EdkDll.IEE_ActivateLicense(licenseKey);
            if (result == EdkDll.EDK_OK || result == EdkDll.EDK_LICENSE_REGISTERED)
            {
                Console.WriteLine("License activated.");
            }
            else Console.WriteLine("License Error. " + result);
        }

        static public DateTime FromUnixTime(uint unixTime)
        {
            var epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            return epoch.AddSeconds(unixTime);
        }

        static public long ToUnixTime(DateTime date)
        {
            var epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            return Convert.ToInt64((date - epoch).TotalSeconds);
        }

        static void licenseInformation()
        {
            EdkDll.IEE_LicenseInfos_t licenseInfos = new EdkDll.IEE_LicenseInfos_t();
            int result = EdkDll.IEE_LicenseInformation(ref licenseInfos);

            Console.WriteLine();
            Console.WriteLine("Date From  : " + FromUnixTime(licenseInfos.date_from).ToString(format));
            Console.WriteLine("Date To    : " + FromUnixTime(licenseInfos.date_to).ToString(format));
            Console.WriteLine();

            Console.WriteLine("Seat number: " + licenseInfos.seat_count);
            Console.WriteLine();

            Console.WriteLine("Total Quota: " + licenseInfos.quota);
            Console.WriteLine("Total quota used    : " + licenseInfos.usedQuota);
            Console.WriteLine();

            Console.WriteLine("Quota limit in day  : " + licenseInfos.quotaDayLimit);
            Console.WriteLine("Quota used in day   : " + licenseInfos.usedQuotaDay);
            Console.WriteLine();

            Console.WriteLine("Quota limit in month: " + licenseInfos.quotaMonthLimit);
            Console.WriteLine("Quota used in month : " + licenseInfos.usedQuotaMonth);
            Console.WriteLine();

            switch ((int)licenseInfos.scopes)
            {
                case (int)EdkDll.IEE_LicenseType_t.IEE_EEG:

                    Console.WriteLine("License type : EEG");
                    Console.WriteLine();
                    break;
                case (int)EdkDll.IEE_LicenseType_t.IEE_EEG_PM:

                    Console.WriteLine("License type : EEG + PM");
                    Console.WriteLine();
                    break;
                case (int)EdkDll.IEE_LicenseType_t.IEE_PM:
                    Console.WriteLine("License type : PM");
                    Console.WriteLine();
                    break;
                default:
                    Console.WriteLine("License type : No type");
                    Console.WriteLine();
                    break;
            }
        }

        static void Main(string[] args)
        {
            Console.WriteLine("===========================================");
            Console.WriteLine("The example to activate a license key.");
            Console.WriteLine("===========================================");

            //Need only 1 time to call with the internet connection
            activateLicense();

            //We can call this API any time to check current License information
            licenseInformation();

            Thread.Sleep(5000);
        }
    }
}
