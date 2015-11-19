//
//  ViewController.h
//  FFTExample
//
//  Created by Cuong Trinh on 11/19/15.
//  Copyright © 2015 Emotiv. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <edk/Iedk.h>

@interface ViewController : NSViewController{
    NSArray        *name_channel;
    NSString       *documentDirectory   ;
}


@property (weak) IBOutlet NSTextField *labelStatus;
@end

