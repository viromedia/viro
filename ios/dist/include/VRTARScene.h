//
//  VRTARScene.h
//  ViroReact
//
//  Created by Andy Chu on 6/13/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#import <Foundation/Foundation.h>
#import <React/RCTImageSource.h>
#import "VRTScene.h"
#import "VRTImageAsyncLoaderEventDelegate.h"


@interface VRTARScene : VRTScene <VROARSceneDelegateProtocol, VRTImageAsyncLoaderEventDelegate>

@property (nonatomic, copy, nullable) NSArray<NSString *> *anchorDetectionTypes;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onTrackingUpdatedViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onAmbientLightUpdateViro;

@property (nonatomic, copy, nullable) RCTDirectEventBlock onAnchorFoundViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onAnchorUpdatedViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onAnchorRemovedViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onCameraARHitTestViro;
@property (nonatomic, copy, nullable) RCTDirectEventBlock onARPointCloudUpdateViro;

@property (nonatomic, assign) BOOL canCameraARHitTest;
@property (nonatomic, assign) BOOL canARPointCloudUpdate;
@property (nonatomic, assign) BOOL displayPointCloud;
@property (nonatomic, copy, nullable) RCTImageSource *pointCloudImage;

@property (nonatomic, copy, nullable) NSArray<NSNumber *> *pointCloudScale;
@property (nonatomic, copy, nullable) NSNumber *pointCloudMaxPoints;

- (instancetype)initWithBridge:(RCTBridge *)bridge;

- (void)initSceneController;
- (void)onCameraHitTest:(int)source results:(std::vector<std::shared_ptr<VROARHitTestResult>>)results;

@end
