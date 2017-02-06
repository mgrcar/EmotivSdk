//
//  ViewController.m
//  FFTExample
//
//  Created by Cuong Trinh on 11/19/15.
//  Copyright © 2015 Emotiv. All rights reserved.
//

#import "ViewController.h"
#import <edk/Iedk.h>

#include <fstream>

BOOL isConnected = NO;

IEE_DataChannel_t ChannelList[] = {
    IED_AF3, IED_F7, IED_F3, IED_FC5, IED_T7, IED_P7, IED_O1, IED_O2,
    IED_P8, IED_T8, IED_FC6, IED_F4, IED_F8, IED_AF4
};

const char header[] = "Channel , Theta ,Alpha ,Low beta ,High beta , Gamma ";
std::ofstream ofs;

@implementation ViewController

EmoEngineEventHandle eEvent;
EmoStateHandle eState;

unsigned int userID					= 0;
float secs							= 1;
bool readytocollect					= false;
int state                           = 0;


NSFileHandle *file;
NSMutableData *data;

- (void)viewDidLoad {
    [super viewDidLoad];
    eEvent	= IEE_EmoEngineEventCreate();
    eState	= IEE_EmoStateCreate();
    
    IEE_EmoInitDevice();
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                         NSUserDomainMask,
                                                         YES);
    documentDirectory = [paths lastObject];
    
    name_channel = [[NSArray alloc]initWithObjects:@"AF3",@"F7",@"F3",@"FC5",@"T7"
                    ,@"P7",@"O1",@"O2",@"P8",@"T8",@"FC6",@"F4",@"F8",@"AF4", nil];
    IEE_EmoInitDevice();
    if( IEE_EngineConnect() != EDK_OK ) {
        self.labelStatus.stringValue = @"Can't connect engine";
    }
    
    NSString* fileName = [NSString stringWithFormat:@"%@/BandPowerValue.csv",documentDirectory];
    ofs.open([fileName UTF8String],std::ios::trunc);
    ofs << header << std::endl;
    
    [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(ConnectDevice) userInfo:nil repeats:YES];
    [NSThread detachNewThreadSelector:@selector(getNextEvent) toTarget:self withObject:nil];
    // Do any additional setup after loading the view.
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
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
                IEE_FFTSetWindowingType(userID, IEE_HANN);
                self.labelStatus.stringValue = @"Connected";
                readytocollect = TRUE;
            }
            else if (eventType == IEE_UserRemoved)
            {
                NSLog(@"User Removed");
                isConnected = NO;
                self.labelStatus.stringValue = @"Disconnected";
                readytocollect = FALSE;
            }
            else if (eventType == IEE_EmoStateUpdated)
            {
            
            }
        }
        if (readytocollect)
        {
            double value[5];
            memset(value, 0, 5*sizeof(double));
            for(int i=0 ; i< sizeof(ChannelList)/sizeof(IEE_DataChannel_t) ; ++i)
            {
                int result = IEE_GetAverageBandPowers(userID, ChannelList[i], &value[0], &value[1], &value[2], &value[3], &value[4]);
                if(result == EDK_OK){
                    ofs << [[name_channel objectAtIndex:i]UTF8String] << ",";
                    for(int j =0; j < 5; j++){
                        ofs << value[j] << ",";
                    }
                    ofs << std::endl;
                    NSLog(@"Chanel %@ %f %f %f %f %f",[name_channel objectAtIndex:i],value[0],value[1],value[2],value[3],value[4]);
                
                }
            }

        }
        usleep(2000);
    }
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
