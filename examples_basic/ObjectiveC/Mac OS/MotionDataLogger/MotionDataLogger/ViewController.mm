//
//  ViewController.m
//  MotionDataLogger
//
//  Created by emotiv on 4/22/15.
//  Copyright (c) 2015 emotiv. All rights reserved.
//

#import "ViewController.h"
#import <edk/Iedk.h>
#include <iostream>
#include <fstream>

IEE_MotionDataChannel_t targetChannelList[] = {
    IMD_COUNTER,
    IMD_GYROX, IMD_GYROY, IMD_GYROZ, IMD_ACCX,
    IMD_ACCY, IMD_ACCZ, IMD_MAGX,IMD_MAGY,IMD_MAGZ,IMD_TIMESTAMP
};

BOOL isConnected = NO;
std::ofstream ofs;
const char header[] = "COUNTER_MEMS, GYROX, GYROY, GYROZ, ACCX, ACCY, ACCZ, MAGX,MAGY,MAGZ,TIMESTAMP";

@implementation ViewController

EmoEngineEventHandle eEvent;
EmoStateHandle eState;
DataHandle hData;

unsigned int userID					= 0;
float secs							= 1;
bool readytocollect					= false;
int state                           = 0;

- (void)viewDidLoad {
    [super viewDidLoad];

    eEvent	= IEE_EmoEngineEventCreate();
    eState	= IEE_EmoStateCreate();
    hData   = IEE_MotionDataCreate();
    
    IEE_EmoInitDevice();
    
    if( IEE_EngineConnect() != EDK_OK ) {
        self.labelStatus.stringValue = @"Can't connect engine";
    }
    
    std::string fileName = "MotionDataLogger.csv";
    ofs.open(fileName, std::ios::trunc);
    ofs << header << std::endl;
    IEE_MotionDataSetBufferSizeInSec(secs);
    
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
            IEE_MotionDataUpdateHandle(0, hData);
        
            unsigned int nSamplesTaken=0;
            IEE_MotionDataGetNumberOfSample(hData,&nSamplesTaken);
        
            std::cout << " Updated " << nSamplesTaken << "\n";
            if (nSamplesTaken != 0)
            {
            
                std::unique_ptr<double> ddata(new double[nSamplesTaken]);
                for (int sampleIdx=0 ; sampleIdx<(int)nSamplesTaken ; ++sampleIdx) {
                    for (int i = 0 ; i<sizeof(targetChannelList)/sizeof(IEE_MotionDataChannel_t) ; i++) {
                        IEE_MotionDataGet(hData, targetChannelList[i], ddata.get(), nSamplesTaken);
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


@end
