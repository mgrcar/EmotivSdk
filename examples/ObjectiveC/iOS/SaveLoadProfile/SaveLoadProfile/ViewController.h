//
//  ViewController.h
//  SaveLoadProfile
//
//  Created by nvtu on 11/19/15.
//  Copyright © 2015 nvtu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
{


}
@property (weak, nonatomic) IBOutlet UILabel *labelStatus;
@property (weak, nonatomic) IBOutlet UITextField *textName;
@property (weak, nonatomic) IBOutlet UITextField *textPass;
@property (weak, nonatomic) IBOutlet UIButton *btSave;
@property (weak, nonatomic) IBOutlet UIButton *btLoad;
@property (weak, nonatomic) IBOutlet UIButton *btLogin;

- (IBAction)login:(id)sender;
- (IBAction)saveProfile:(id)sender;
- (IBAction)loadProfile:(id)sender;
@end

