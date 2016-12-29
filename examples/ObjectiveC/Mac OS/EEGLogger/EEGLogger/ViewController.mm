//
//  ViewController.m
//  MotionDataLogger
//
//  Created by emotiv on 4/22/15.
//  Copyright (c) 2015 emotiv. All rights reserved.
//
/** This Example to show how to log the EEG data from EmoEngine
 *  It works if you have license subscription EEG
 */

#import "ViewController.h"
#import <edk/Iedk.h>
#import <edk/IEegData.h>
#import <edk/EmotivLicense.h>

#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <sstream>
#include <fstream>
#include <map>
#define secondsepoch(x) (std::chrono::duration_cast<std::chrono::seconds>(x.time_since_epoch()).count())
#define secondsdiff(x,y)(std::chrono::duration_cast<std::chrono::seconds>(x - y).count())

#if !(defined __linux__ || defined __APPLE__)
inline char* strptime(const char* s, const char* f, struct tm* tm)
{
    std::istringstream input(s);
    input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
    input >> std::get_time(tm, f);
    if (input.fail()) {
        return nullptr;
    }
    return (char*)(s + input.tellg());
}
#endif
/*****************************************************/

/*If headset Insight only 5 channel AF3, AF4, T7, T8, Pz = O1 have value. 
               Another channels EEG is zero */

/*****************************************************/

IEE_DataChannel_t targetChannelList[] = {
    IED_COUNTER,
    IED_AF3, IED_F7, IED_F3, IED_FC5, IED_T7, IED_P7, IED_O1, IED_O2,
    IED_P8, IED_T8, IED_FC6, IED_F4, IED_F8, IED_AF4, IED_GYROX,IED_GYROY,
    IED_TIMESTAMP, IED_FUNC_ID, IED_FUNC_VALUE, IED_MARKER, IED_SYNC_SIGNAL
};

BOOL isConnected = NO;
const char header[] = "COUNTER, AF3, F7, F3, FC5, T7, P7, O1, O2, P8, T8, FC6, F4, F8, AF4, GYROX, GYROY, TIMESTAMP, FUNC_ID, FUNC_VALUE, MARKER, SYNC_SIGNAL";

@implementation ViewController

EmoEngineEventHandle eEvent;
EmoStateHandle eState;
DataHandle hData;

unsigned int userID					= 0;
float secs							= 1;
bool readytocollect					= false;
int state                           = 0;
int licenseType = 0;

std::ofstream ofs;
/**
 * Set your license 
 */
std::string license = "Put-your-license-here";

std::string convertEpochToTime(time_t epochTime)
{
    char timestamp[64] = { 0 };
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&epochTime));
    return timestamp;
}

void printLicenseInformation(IEE_LicenseInfos_t& licenseInfos)
{
    std::cout << std::endl;
    std::cout << "Date From  : " << convertEpochToTime(licenseInfos.date_from) << std::endl;
    std::cout << "Date To    : " << convertEpochToTime(licenseInfos.date_to) << std::endl;
    std::cout << std::endl;
    
    std::cout << "Seat number: " << licenseInfos.seat_count << std::endl;
    std::cout << std::endl;
    
    std::cout << "Total Quota: " << licenseInfos.quota << std::endl;
    std::cout << "Total quota used    : " << licenseInfos.usedQuota << std::endl;
    std::cout << std::endl;
    
    std::cout << "Quota limit in day  : " << licenseInfos.quotaDayLimit << std::endl;
    std::cout << "Quota used in day   : " << licenseInfos.usedQuotaDay << std::endl;
    std::cout << std::endl;
    
    std::cout << "Quota limit in month: " << licenseInfos.quotaMonthLimit << std::endl;
    std::cout << "Quota used in month : " << licenseInfos.usedQuotaMonth << std::endl;
    std::cout << std::endl;
    
    switch (licenseInfos.scopes)
    {
        case IEE_EEG:
            licenseType = IEE_LicenseType_t::IEE_EEG;
            
            std::cout << "License type : " << "EEG" << std::endl;
            std::cout << std::endl;
            break;
        case IEE_EEG_PM:
            licenseType = IEE_LicenseType_t::IEE_EEG_PM;
            
            std::cout << "License type : " << "EEG + PM" << std::endl;
            std::cout << std::endl;
            break;
        case IEE_PM:
            licenseType = IEE_LicenseType_t::IEE_PM;
            std::cout << "License type : " << "PM" << std::endl;
            std::cout << std::endl;
            break;
        default:
            std::cout << "License type : " << "No type" << std::endl;
            std::cout << std::endl;
            break;
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];

    [NSThread detachNewThreadSelector:@selector(startTheBackgroundJob) toTarget:self withObject:nil];
    
    eEvent	= IEE_EmoEngineEventCreate();
    eState	= IEE_EmoStateCreate();
    hData   = IEE_DataCreate();
    
    IEE_EmoInitDevice();
    
    std::string fileName = "EEGDataLogger.csv";
    ofs.open(fileName,std::ios::trunc);
    ofs << header << std::endl;
    IEE_DataSetBufferSizeInSec(secs);
    
    [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(ConnectDevice) userInfo:nil repeats:YES];
    [NSThread detachNewThreadSelector:@selector(getNextEvent) toTarget:self withObject:nil];
    // Do any additional setup after loading the view.
}

-(void) getNextEvent {
    while (true) {
        int state = IEE_EngineGetNextEvent(eEvent);
        unsigned int userID = 0;
    
        if (state == EDK_OK)
        {
        
            IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
            IEE_EmoEngineEventGetUserId(eEvent, &userID);
        
            // Log the EmoState if it has been updated
            if (eventType == IEE_UserAdded)
            {
                NSLog(@"User Added");
                self.labelStatus.stringValue = @"Connected";
                IEE_DataAcquisitionEnable(userID, true);
                readytocollect = TRUE;
            }
            else if (eventType == IEE_UserRemoved)
            {
                NSLog(@"User Removed");
                self.labelStatus.stringValue = @"Disconnected";
                readytocollect = FALSE;
            }
            else if (eventType == IEE_EmoStateUpdated)
            {
            
            }
        }
        if (readytocollect)
        {
            IEE_DataUpdateHandle(0, hData);
        
            unsigned int nSamplesTaken=0;
            IEE_DataGetNumberOfSample(hData,&nSamplesTaken);
        
            std::cout << " Updated " << nSamplesTaken << "\n";
            if (nSamplesTaken != 0)
            {
            
                std::unique_ptr<double> ddata(new double[nSamplesTaken]);
                for (int sampleIdx=0 ; sampleIdx<(int)nSamplesTaken ; ++sampleIdx) {
                    for (int i = 0 ; i<sizeof(targetChannelList)/sizeof(IEE_DataChannel_t) ; i++) {
                        IEE_DataGet(hData, targetChannelList[i], ddata.get(), nSamplesTaken);
                        ofs << ddata.get()[sampleIdx] << ",";
                    }
                    ofs << std::endl;
                }
            }
        }
    }
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (void)startTheBackgroundJob {
    [self performSelectorInBackground:@selector(CallFunc) withObject:nil];
}

-(void)ConnectDevice{
    /**This function to connect headset in mode Bluetooth*/
    /*Connect with Insight headset in mode Bluetooth*/
        int numberDevice = IEE_GetInsightDeviceCount();
        if(numberDevice > 0 && !isConnected) {
            IEE_ConnectInsightDevice(0);
            isConnected = YES;
        }
    /************************************************/
    /*Connect with Epoc Plus headset in mode Bluetooth*/
//    int numberDevice = IEE_GetEpocPlusDeviceCount();
//    if(numberDevice > 0 && !isConnected) {
//        IEE_ConnectEpocPlusDevice(0);
//        isConnected = YES;
//    }
    /************************************************/
    else isConnected = NO;

}

-(void)CallFunc{
    /**
     * Call this fucntion first time run app
     * from second time, no need call it
     * If you have many license on your computer.Call this function to active license you want to use
     */
    int result ;
    result = IEE_ActivateLicense(license.c_str());
    if(result == EDK_OK || result == EDK_LICENSE_REGISTERED)
        NSLog(@"Active license success");
    else
        NSLog(@"License error code: %x",result);
    /*********************************************************/
    if( IEE_EngineConnect() != EDK_OK ) {
        self.labelStatus.stringValue = @"Can't connect engine";
    }

    IEE_LicenseInfos_t licenseInfos;
    memset(&licenseInfos, 0, sizeof(IEE_LicenseInfos_t));
    result = IEE_LicenseInformation(&licenseInfos);
    switch (result)
    {
        case EDK_OVER_QUOTA_IN_DAY:
            NSLog(@"Over quota in day");
            break;
        case EDK_OVER_QUOTA_IN_MONTH:
            NSLog(@"Over quota in month");
            break;
        case EDK_LICENSE_EXPIRED:
            NSLog(@"License is expired");
            break;
        case EDK_OVER_QUOTA:
            NSLog(@"Over quota ");
            break;
        case EDK_ACCESS_DENIED:
            NSLog(@"Access denied");
            break;
        case EDK_LICENSE_ERROR:
            NSLog(@"Licens error");
            break;
        case EDK_NO_ACTIVE_LICENSE:
            NSLog(@"Haven't been active license");
            break;
        case EDK_OK:
            NSLog(@"License is ok");
            break;
        default:
            break;
    }
    printLicenseInformation(licenseInfos);
}

@end
