//
//  ViewController.m
//  FFTExample
//
//  Created by Cuong Trinh on 11/19/15.
//  Copyright © 2015 Emotiv. All rights reserved.
//

#import "ViewController.h"
#import <edk/Iedk.h>


BOOL isConnected = NO;

IEE_DataChannel_t ChannelList[] = {
    IED_AF3, IED_AF4, IED_T7, IED_T8, IED_Pz
};

const char header[] = "Channel , Theta ,Alpha ,Low beta ,High beta , Gamma ";

const char *newLine = "\n";
const char *comma = ",";
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
    
    name_channel = [[NSArray alloc]initWithObjects:@"AF3",@"AF4",@"T7",@"T8",@"Pz", nil];
    IEE_EmoInitDevice();
    if( IEE_EngineConnect() != EDK_OK ) {
        self.labelStatus.stringValue = @"Can't connect engine";
    }
    
    NSString* fileName = [NSString stringWithFormat:@"%@/BandPowerValue.csv",documentDirectory];
    NSString* createFile = @"";
    [createFile writeToFile:fileName atomically:YES encoding:NSUnicodeStringEncoding error:nil];
    
    file = [NSFileHandle fileHandleForUpdatingAtPath:fileName];
    [self saveStr:file data:data value:header];
    [self saveStr:file data:data value:newLine];
    
    //IEE_MotionDataSetBufferSizeInSec(secs);
    
    [NSTimer scheduledTimerWithTimeInterval:0.01 target:self selector:@selector(getNextEvent) userInfo:nil repeats:YES];

    // Do any additional setup after loading the view.
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

-(void) getNextEvent {
    /*Connect with Insight headset in mode Bluetooth*/
    int numberDevice = IEE_GetInsightDeviceCount();
    if(numberDevice > 0 && !isConnected) {
        IEE_ConnectInsightDevice(0);
        isConnected = YES;
    }
    /************************************************/
    //    /*Connect with Epoc Plus headset in mode Bluetooth*/
    //    int numberDevice = IEE_GetEpocPlusDeviceCount();
    //    if(numberDevice > 0 && !isConnected) {
    //        IEE_ConnectEpocPlusDevice(0);
    //        isConnected = YES;
    //    }
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
                [self saveStr:file data:data value:[[name_channel objectAtIndex:i]UTF8String]];
                [self saveStr:file data:data value:comma];
                for(int j =0; j < 5; j++){
                    [self saveDoubleVal:file data:data value:value[j]];
                    [self saveStr:file data:data value:comma];
                }
                [self saveStr:file data:data value:newLine];
                NSLog(@"Chanel %@ %f %f %f %f %f",[name_channel objectAtIndex:i],value[0],value[1],value[2],value[3],value[4]);
                
            }
        }

    }
}


-(void) saveStr : (NSFileHandle * )file data : (NSMutableData *) data value : (const char*) str
{
    [file seekToEndOfFile];
    data = [NSMutableData dataWithBytes:str length:strlen(str)];
    [file writeData:data];
}

-(void) saveDoubleVal : (NSFileHandle * )file data : (NSMutableData *) data value : (const double) val
{
    NSString* str = [NSString stringWithFormat:@"%f",val];
    const char* myValStr = (const char*)[str UTF8String];
    [self saveStr:file data:data value:myValStr];
}

@end
