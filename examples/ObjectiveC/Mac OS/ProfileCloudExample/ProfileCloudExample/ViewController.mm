//
//  ViewController.m
//  ProfileCloudExample
//
//  Created by Cuong Trinh on 11/19/15.
//  Copyright © 2015 Emotiv. All rights reserved.
//

#import "ViewController.h"
#import <edk/Iedk.h>
#import <edk/EmotivCloudClient.h>
@implementation ViewController

EmoEngineEventHandle eEvent			= IEE_EmoEngineEventCreate();
EmoStateHandle eState				= IEE_EmoStateCreate();
int state                           = 0;
unsigned int engineUserID           = 0;
int  userCloudID                    = -1;

bool cloudConnected                 = false;
bool headsetConnected               = false;
bool userAdded                      = false;
- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.
    [self connectEngine];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}
-(IBAction)Login:(id)sender{
    [self.userName resignFirstResponder];
    [self.pass resignFirstResponder];
    if(!cloudConnected) {
        cloudConnected = (EC_Connect() == EDK_OK);
        if(!cloudConnected) {
            self.labelStatus.stringValue = @"Please check internet connection and connect again";
            return;
        }
    }
    if(self.userName.stringValue.length == 0 || self.pass.stringValue.length == 0) {
        self.labelStatus.stringValue = @"Enter username and password";
        return;
    }
    if(EC_Login([self.userName.stringValue cStringUsingEncoding:NSUTF8StringEncoding], [self.pass.stringValue cStringUsingEncoding:NSUTF8StringEncoding]) == EDK_OK) {
        self.labelStatus.stringValue = @"Login successfully";
        userCloudID = -1;
        if(EC_GetUserDetail(&userCloudID) == EDK_OK) {
            self.save_profile.enabled = true;
            self.load_profile.enabled = true;
        }
        else {
            self.labelStatus.stringValue = @"Can't get user detail. Please try again";
        }
    }
    else {
        self.labelStatus.stringValue = @"Username or password is wrong. Check again";
    }

}
-(IBAction)Save:(id)sender{
    if(!headsetConnected) {
        self.labelStatus.stringValue = @"Connect headset first";
        return;
    }
    if(userCloudID < 0) {
        self.labelStatus.stringValue = @"Login first";
        return;
    }
    
    if(EC_SaveUserProfile(userCloudID, engineUserID, "test", TRAINING) == EDK_OK) {
        self.labelStatus.stringValue = @"Save new profile successfully";
    }
    else {
        self.labelStatus.stringValue = @"Profile is existed or can't create new profile";
    }
}
-(IBAction)Load:(id)sender{
    if(!headsetConnected) {
        self.labelStatus.stringValue = @"Connect headset first";
        return;
    }
    if(userCloudID < 0) {
        self.labelStatus.stringValue = @"Login first";
        return;
    }
    int profileID = 0;
    EC_GetProfileId(userCloudID, "test",&profileID);
    if ( profileID < 0) {
        self.labelStatus.stringValue = @"Profile isnt existed";
        return;
    }
    if(EC_LoadUserProfile(userCloudID, engineUserID, profileID) == EDK_OK) {
        self.labelStatus.stringValue = @"Load profile successfully";
    }
    else {
        self.labelStatus.stringValue = @"Can't load this profile";
    }

}
-(IBAction)Delete:(id)sender{
    if(userCloudID < 0) {
        self.labelStatus.stringValue = @"Login first";
        return;
    }
    int profileID = 0;
    EC_GetProfileId(userCloudID, "test",&profileID);
    if ( profileID < 0) {
        self.labelStatus.stringValue = @"Profile isnt existed";
        return;
    }
    if(EC_DeleteUserProfile(userCloudID, profileID) == EDK_OK) {
        self.labelStatus.stringValue = @"Remove profile successfully";
    }
    else {
        self.labelStatus.stringValue = @"Can't remove this profile";
    }
}
-(void) connectEngine {
    IEE_EmoInitDevice();
    if(IEE_EngineConnect() != EDK_OK) {
        self.labelStatus.stringValue =  @"Connect Engine failed";
    }
    else {
        cloudConnected = (EC_Connect() == EDK_OK);
        if(!cloudConnected) {
            self.labelStatus.stringValue = @"Please check internet connection and connect again";
        }
    }
    
    NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(getNextEvent) userInfo:nil repeats:YES];
    [[NSRunLoop mainRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
}

-(void) getNextEvent {
    /*Connect Insight headset in Mode Bluetooth*/
    int couter_insight = IEE_GetInsightDeviceCount();
    if(couter_insight > 0){
        if(!headsetConnected){
            headsetConnected = IEE_ConnectInsightDevice(0);
        }
    }
    /*******************************************/
//    /*Connect Epoc Plus headset in Mode Bluetooth*/
//    int couter_epoc = IEE_GetEpocPlusDeviceCount();
//    if(couter_epoc > 0){
//        if(!headsetConnected){
//            headsetConnected = IEE_ConnectEpocPlusDevice(0);
//        }
//    }
    /*******************************************/
    state = IEE_EngineGetNextEvent(eEvent);
    if(state == EDK_OK)
    {
        IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
        int result = IEE_EmoEngineEventGetUserId(eEvent, &engineUserID);
        
        if (result != EDK_OK) {
            NSLog(@"WARNING : Failed to return a valid user ID for the current event");
        }
        
        if(eventType == IEE_UserAdded)
        {
            NSLog(@"User Added");
            headsetConnected = true;
            userAdded = true;
        }
        if(eventType == IEE_UserRemoved){
            NSLog(@"user remove");
            headsetConnected = false;
            userAdded = false;
        }
    }
}
@end
