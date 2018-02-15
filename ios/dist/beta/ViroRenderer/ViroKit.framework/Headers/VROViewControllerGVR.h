//
//  VROViewControllerGVR.h
//  ViroKit
//
//  Created by Raj Advani on 11/30/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <UIKit/UIKit.h>

class VRORendererConfiguration;

@interface VROViewControllerGVR : UIViewController

/*
 Set this to true to force the view into landscape mode
 when it appears.
 */
@property (readwrite, nonatomic) BOOL forceLandscape;

- (id)initWithConfig:(VRORendererConfiguration)config;

@end
