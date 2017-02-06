//
//  ViewController.m
//  EmotionalState
//
//  Created by emotiv on 4/23/15.
//  Copyright (c) 2015 emotiv. All rights reserved.
//
/**
 ** This example receives the PerformanceMetric scores
 ** It works if you have license subscription Performance Metrics
 */

#import "ViewController.h"
#import <edk/Iedk.h>
#import <edk/IEmoStatePerformanceMetric.h>

#import <edk/EmotivLicense.h>

#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <sstream>
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
@implementation ViewController

EmoEngineEventHandle eEvent;
EmoStateHandle eState;
NSMutableArray *arrayView;
BOOL isConnected = false;
int licenseType = 0;

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
    arrayView = [[NSMutableArray alloc] init];
    
    for(int i = 0; i < 5; i++)
    {
        GraphView *view = [[GraphView alloc] initWithFrame:CGRectMake(0, 50 + 120*i, self.view.frame.size.width, 120) index:i];
        [self.view addSubview:view];
        [arrayView addObject:view];
    }
    eEvent	= IEE_EmoEngineEventCreate();
    eState	= IEE_EmoStateCreate();
    isRecording = false;
    
    IEE_EmoInitDevice();
    
    [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(getNextEvent) userInfo:nil repeats:YES];
    // Do any additional setup after loading the view.
}

-(void) getNextEvent {
    /*Connect with Insight headset in mode Bluetooth*/
    //    int numberDevice = IEE_GetInsightDeviceCount();
    //    if(numberDevice > 0 && !isConnected) {
    //        IEE_ConnectInsightDevice(0);
    //        isConnected = YES;
    //    }
    /************************************************/
    /*Connect with Epoc Plus headset in mode Bluetooth*/
    int numberDevice = IEE_GetEpocPlusDeviceCount();
    if(numberDevice > 0 && !isConnected) {
        IEE_ConnectEpocPlusDevice(0);
        isConnected = YES;
    }
    /************************************************/
    else isConnected = NO;
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
            isConnected = true;
        }
        else if (eventType == IEE_UserRemoved)
        {
            NSLog(@"User Removed");
            isConnected = false;
        }
        else if (eventType == IEE_EmoStateUpdated)
        {
            IEE_EmoEngineEventGetEmoState(eEvent, eState);
            NSArray *array = [NSArray arrayWithObjects:[NSNumber numberWithFloat:IS_PerformanceMetricGetEngagementBoredomScore(eState)], [NSNumber numberWithFloat:IS_PerformanceMetricGetRelaxationScore(eState)], [NSNumber numberWithFloat:IS_PerformanceMetricGetStressScore(eState)], [NSNumber numberWithFloat:IS_PerformanceMetricGetInstantaneousExcitementScore(eState)], [NSNumber numberWithFloat:IS_PerformanceMetricGetInterestScore(eState)], nil];
            
            [[arrayView objectAtIndex:0] updateValue:IS_PerformanceMetricGetEngagementBoredomScore(eState)];
            [[arrayView objectAtIndex:1] updateValue:IS_PerformanceMetricGetRelaxationScore(eState)];
            [[arrayView objectAtIndex:2] updateValue:IS_PerformanceMetricGetStressScore(eState)];
            [[arrayView objectAtIndex:3] updateValue:IS_PerformanceMetricGetInstantaneousExcitementScore(eState)];
            [[arrayView objectAtIndex:4] updateValue:IS_PerformanceMetricGetInterestScore(eState)];
            
            [self writeData:array];

        }
    }
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (IBAction)clickButton:(id)sender {
    isRecording = !isRecording;
    if(isRecording) {
        NSString* fileName = @"EmoStateLogger.csv";
        NSString* createFile = @"";
        [createFile writeToFile:fileName atomically:YES encoding:NSUnicodeStringEncoding error:nil];
        
        NSArray *arrayTitle = [NSArray arrayWithObjects:@"Engagement", @"Relax", @"Stress", @"Instantaneous Excitement", @"Interest", nil];
        [self writeData:arrayTitle];
        
        self.buttonRecord.title = @"Stop record";
    }
    else
    {
        self.buttonRecord.title = @"Record data";
    }
}

-(void) writeData : (NSArray *) array
{
    if(isRecording)
    {
        NSString *data = [NSString stringWithFormat:@"%@", [array objectAtIndex:0]];
        for (int i = 1; i < [array count]; i++) {
            @autoreleasepool {
                data = [data stringByAppendingFormat:@", %@", [array objectAtIndex:i]];
            }
        }
        data = [data stringByAppendingFormat:@"\n"];
        NSFileHandle *fh = [NSFileHandle fileHandleForWritingAtPath:@"EmoStateLogger.csv"];
        [fh seekToEndOfFile];
        [fh writeData:[data dataUsingEncoding:NSUTF8StringEncoding]];
        [fh closeFile];
    }
}

- (void)startTheBackgroundJob {
    [self performSelectorInBackground:@selector(CallFunc) withObject:nil];
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
    /******************************************************/
    if( IEE_EngineConnect() != EDK_OK ) {
        NSLog(@"Engine connect failed");
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
