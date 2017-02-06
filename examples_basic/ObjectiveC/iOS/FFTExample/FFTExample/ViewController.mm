//
//  ViewController.m
//  FFTExample
//
//  Created by Cuong Trinh on 11/13/15.
//  Copyright © 2015 Emotiv. All rights reserved.
//

#import "ViewController.h"
#include <iostream>
#include <fstream>
#include <sstream>
#import <edk_ios/IedkErrorCode.h>

@interface ViewController ()

@end

std::ofstream ofs;

IEE_DataChannel_t ChannelList[] = {
    IED_AF3, IED_AF4, IED_T7, IED_T8, IED_Pz
};

const char header[] = "Channel , Theta ,Alpha ,Low beta ,High beta , Gamma ";
@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    eEvent			= IEE_EmoEngineEventCreate();
    userid			= 0;
    state  = 0;
    readytocollect = NO;
    lock = NO;
    isConnectBLE = NO;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                         NSUserDomainMask,
                                                         YES);
    documentDirectory = [paths lastObject];

    name_channel = [[NSArray alloc]initWithObjects:@"AF3",@"AF4",@"T7",@"T8",@"Pz", nil];
    IEE_EmoInitDevice();
    IEE_EngineConnect();
    [self StartProgram];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
-(IBAction)Start_click:(id)sender{
    if(isConnectBLE) readytocollect = YES;
}
-(IBAction)Stop_click:(id)sender{
    readytocollect = NO;
    if(ofs.is_open()) ofs.close();
}
-(void)getInsightResult{
    while (1) {
        if(!isConnectBLE){
            
            int couter = IEE_GetInsightDeviceCount();
            /********Connect with first Headset Insight scanned*/
            if(couter > 0){
                if(!lock){
                    IEE_ConnectInsightDevice(0);
                    lock =true;
                }
                
            }
            else lock = false;
        }
        /*************************************/
        state = IEE_EngineGetNextEvent(eEvent);
        if(state == EDK_OK){
            IEE_Event_t evenType = IEE_EmoEngineEventGetType(eEvent);
            if(evenType == IEE_UserAdded){
                NSLog(@"User Added");
                IEE_FFTSetWindowingType(userid, IEE_HAMMING);
                isConnectBLE = YES;
                            }
            if(evenType == IEE_UserRemoved){
                NSLog(@"User Removed");
                isConnectBLE = NO;
                readytocollect = NO;
            }
            if(evenType == IEE_EmoStateUpdated){
                
            }
        }
        if(readytocollect){
            double alpha;
            double low_beta;
            double high_beta;
            double gamma;
            double theta;
            alpha = low_beta = high_beta = gamma = theta = 0;
            for(int i=0 ; i< sizeof(ChannelList)/sizeof(IEE_DataChannel_t) ; ++i)
            {
                int result = IEE_GetAverageBandPowers(userid, ChannelList[i], &theta, &alpha, &low_beta, &high_beta, &gamma);
                if(result == EDK_OK){
                    ofs << [[name_channel objectAtIndex:i] UTF8String] << ",";
                    ofs << theta << ",";
                    ofs << alpha << ",";
                    ofs << low_beta << ",";
                    ofs << high_beta << ",";
                    ofs << gamma << ",";
                    NSLog(@"Chanel %@ %f %f %f %f %f",[name_channel objectAtIndex:i],theta,alpha,low_beta,high_beta,gamma);
                    
                    ofs << std::endl;
                }
            }
        }
        usleep(100);
    }
}

-(void)StartProgram{
        NSThread *thread = [[NSThread alloc] initWithTarget:self selector:@selector(getInsightResult) object:nil];
        thread.start;
    
    /*************File .csv stored in folder Document of Application********************/
    NSString *path_file_csv = [NSString stringWithFormat:@"%@/test.csv",documentDirectory];
    NSLog(@"Path to file .csv %@",path_file_csv);
    ofs.open([path_file_csv cStringUsingEncoding:NSUTF8StringEncoding]);
    
    ofs << header << std::endl;
}
@end
