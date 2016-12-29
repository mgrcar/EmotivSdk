//
//  ViewController.h
//  ProfileCloudExample
//
//  Created by Cuong Trinh on 11/19/15.
//  Copyright © 2015 Emotiv. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ViewController : NSViewController

@property (weak) IBOutlet NSTextField *labelStatus;
@property (weak) IBOutlet NSTextField *userName;
@property (weak) IBOutlet NSSecureTextField *pass;
@property (weak) IBOutlet NSButton *save_profile;
@property (weak) IBOutlet NSButton *load_profile;
@property (weak) IBOutlet NSButton *delete_profile;
@property (weak) IBOutlet NSButton *login;
-(IBAction)Login:(id)sender;
-(IBAction)Save:(id)sender;
-(IBAction)Load:(id)sender;
-(IBAction)Delete:(id)sender;
@end

