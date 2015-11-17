//
//  ViewController.h
//  FFTExample
//
//  Created by Cuong Trinh on 11/13/15.
//  Copyright © 2015 Emotiv. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <edk_ios/Iedk.h>

@interface ViewController : UIViewController{
    IBOutlet UIButton *btn_start;
    IBOutlet UIButton *btn_stop;
    
    NSString       *documentDirectory   ;
    NSArray        *name_channel;
    BOOL isConnectBLE;
    BOOL lock ;
    EmoEngineEventHandle eEvent;
    int state;
    BOOL readytocollect;
    unsigned int userid;
}
-(IBAction)Start_click:(id)sender;
-(IBAction)Stop_click:(id)sender;
@end

