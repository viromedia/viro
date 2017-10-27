//
//  ViewController.m
//  ViroExample
//
//  Created by vik.advani on 12/8/16.
//  Copyright © 2016 Vik Advani. All rights reserved.
//

#import "ViewController.h"
#import <React/RCTRootView.h>
#include "VRTBundleURLProvider.h"
#include "VRTNotifications.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    [self.startButton addTarget:self
                         action:@selector(enterVr)
               forControlEvents:UIControlEventTouchUpInside];
    
    // register for the exit notification
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(exitVr)
                                                 name:kVRTOnExitViro
                                               object:nil];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)enterVr {
    
    VRTBundleURLProvider *bundleProvider = [[VRTBundleURLProvider alloc] init];
    NSURL *jsCodeLocation = [bundleProvider jsBundleURLForBundleRoot:@"index" fallbackResource:nil];
    
    RCTRootView *rootView = [[RCTRootView alloc] initWithBundleURL:jsCodeLocation
                                                        moduleName:@"ViroSample"
                                                 initialProperties:nil
                                                     launchOptions:nil];
    
    UIViewController *vrController = [UIViewController new];
    vrController.view = rootView;
    [self presentViewController:vrController animated:YES completion:nil];
}

- (void)exitVr {
    dispatch_async(dispatch_get_main_queue(), ^{
        [self dismissViewControllerAnimated:YES completion:nil];
    });
}

- (BOOL)shouldAutorotate {
    return YES;
}

- (UIInterfaceOrientationMask)supportedInterfaceOrientations{
    return UIInterfaceOrientationMaskLandscape;
}

@end

