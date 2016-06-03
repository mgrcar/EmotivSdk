//
//  ViewController.m
//  SaveLoadProfile
//
//  Created by nvtu on 11/19/15.
//  Copyright © 2015 nvtu. All rights reserved.
//

#import "ViewController.h"
#import <edk_ios/Iedk.h>
#import <edk_ios/EmotivCloudClient.h>
#import <edk_ios/EmotivCloudErrorCode.h>

@interface ViewController ()

@end

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
    
    [self connectEngine];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)login:(id)sender {
    [self.textName resignFirstResponder];
    [self.textPass resignFirstResponder];
    if(!cloudConnected) {
        cloudConnected = (EC_Connect() == EC_OK );
        if(!cloudConnected) {
            [self.labelStatus setText:@"Status : Please check internet connection and connect again"];
            return;
        }
    }
    if(self.textName.text.length == 0 || self.textPass.text.length == 0) {
        [self.labelStatus setText:@"Status : Enter username and password"];
        return;
    }
    if(EC_Login([self.textName.text cStringUsingEncoding:NSUTF8StringEncoding], [self.textPass.text cStringUsingEncoding:NSUTF8StringEncoding]) == EC_OK) {
        [self.labelStatus setText:@"Status : Login successfully"];
        userCloudID = -1;
        if(EC_GetUserDetail(&userCloudID) == EC_OK) {
            self.btSave.enabled = true;
            self.btLoad.enabled = true;
        }
        else {
            [self.labelStatus setText:@"Status : Cant get user detail. Please try again"];
        }
    }
    else {
        [self.labelStatus setText:@"Status : Username or password is wrong. Check again"];
    }
}

- (IBAction)saveProfile:(id)sender {
    if(!headsetConnected) {
        [self.labelStatus setText:@"Status : Connect headset first"];
        return;
    }
    if(userCloudID < 0) {
        [self.labelStatus setText:@"Status : Login first"];
        return;
    }
    
    int error = EC_SaveUserProfile(userCloudID, engineUserID, "test", TRAINING);
    if(error == EC_OK) {
        [self.labelStatus setText:@"Status : Save new profile successfully"];
    }
    else if (error == EC_PROFILE_CLOUD_EXISTED) {
        [self.labelStatus setText:@"Status : Profile is existed "];
    }
    else {
        [self.labelStatus setText:@"Status : Can't create new profile"];
    }
}

- (IBAction)loadProfile:(id)sender {
    if(!headsetConnected) {
        [self.labelStatus setText:@"Status : Connect headset first"];
        return;
    }
    if(userCloudID < 0) {
        [self.labelStatus setText:@"Status : Login first"];
        return;
    }
    int profileID = EC_GetProfileId(userCloudID, "test");
    if ( profileID < 0) {
        [self.labelStatus setText:@"Status : Profile isnt existed"];
        return;
    }
    if(EC_LoadUserProfile(userCloudID, engineUserID, profileID) == EC_OK) {
        [self.labelStatus setText:@"Status : Load profile successfully"];
    }
    else {
        [self.labelStatus setText:@"Status : Cant load this profile"];
    }
}

-(void) connectEngine {
    IEE_EmoInitDevice();
    if(IEE_EngineConnect() != EDK_OK) {
        [self.labelStatus setText:@"Status : Connect Engine failed"];
    }
    else {
        cloudConnected = (EC_Connect() == EC_OK);
        if(!cloudConnected) {
            [self.labelStatus setText:@"Status : Please check internet connection and connect again"];
        }
    }
    
    NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(getNextEvent) userInfo:nil repeats:YES];
    [[NSRunLoop mainRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
}

-(void) getNextEvent {
    int couter_insight = IEE_GetInsightDeviceCount();
    if(couter_insight > 0){
        if(!headsetConnected){
            headsetConnected = IEE_ConnectInsightDevice(0);
        }
    }
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
