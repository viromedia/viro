//
//  VRT360Video.m
//  React
//
//  Created by Vik Advani on 1/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

#include <React/RCTConvert.h>
#include <React/RCTImageSource.h>
#import "VRTUtils.h"
#import "VRT360Video.h"


@implementation VRT360Video {
    std::shared_ptr<VROVideoTexture> _videoTexture;
    std::shared_ptr<VROVideoDelegateiOS> _videoDelegate;
    BOOL _sphereTextureAddedToScene;
    BOOL _needsVideoTextureReset;
    NSString *_stereoMode;
}

@synthesize rotation = _rotation;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _sphereTextureAddedToScene = NO;
        _muted = NO;
        _loop = NO;
        _volume = 1;
        _paused = NO;
        _needsVideoTextureReset = NO;
        _videoDelegate = std::make_shared<VROVideoDelegateiOS>(self);
    }
    
    return self;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if (_needsVideoTextureReset) {
        [self updateSceneWithSphereTexture];
    }
    _needsVideoTextureReset = NO;
}

- (void)setPaused:(BOOL)paused {
    _paused = paused;
    if (_videoTexture) {
        _paused ? _videoTexture->pause() : _videoTexture->play();
    }
}

- (void)setLoop:(BOOL)loop {
    _loop = loop;
    if (_videoTexture) {
        _videoTexture->setLoop(loop);
        // If we're not paused, call play again because we could get into the state
        // where we're at the end of a video w/ loop false, if the user sets it to
        // loop true, setPaused could get called first before setLoop and it wont play
        if (!_paused) {
            _videoTexture->play();
        }
    }
}

- (void)setMuted:(BOOL)muted {
    _muted = muted;
    if (_videoTexture) {
        _videoTexture->setMuted(muted);
    }
}

- (void)setVolume:(float)volume {
    _volume = volume;
    if (_videoTexture) {
        _videoTexture->setVolume(volume);
    }
}

- (void)seekToTime:(float)time {
    if (_videoTexture) {
        _videoTexture->seekToTime(time);
    }
}

- (void)setSource:(NSDictionary *)source {
    _source = source;
    _sphereTextureAddedToScene = NO;
    _needsVideoTextureReset = YES;
}

- (void)setStereoMode:(NSString *)mode;{
    _stereoMode = mode;
    _needsVideoTextureReset = YES;
}

- (void)setRotation:(NSArray<NSNumber *> *)rotation {
    _rotation = [rotation copy];
    if (_sphereTextureAddedToScene) {
        float rotationValues[3] = {0.0f, 0.0f, 0.0f};
        populateFloatArrayFromNSArray(_rotation, rotationValues, 3);
        self.node->getParentPortal()->setBackgroundRotation({toRadians(rotationValues[0]),
                                                          toRadians(rotationValues[1]),
                                                          toRadians(rotationValues[2])});
    }
}

- (void)updateSceneWithSphereTexture {
    if (!self.source) {
        RCTLogError(@"Source should not be nil.");
    } else if (!self.context || !self.driver || _sphereTextureAddedToScene || !self.parentHasAppeared) {
        return;
    }
    
    RCTImageSource *imageSource = [RCTConvert RCTImageSource:self.source];
    NSURL *videoURL = imageSource.request.URL;
    std::string url = std::string([[videoURL description] UTF8String]);
    
    VROStereoMode mode = VROStereoMode::None;
    if (self.stereoMode){
        mode = VROTextureUtil::getStereoModeForString(std::string([self.stereoMode UTF8String]));
    }
    
    _videoTexture = std::make_shared<VROVideoTextureiOS>(mode);
    _videoTexture->loadVideo(url, self.context->getFrameSynchronizer(), self.driver);
    _videoTexture->prewarm();
    
    self.node->getParentPortal()->setBackgroundSphere(_videoTexture);
    float rotationValues[3] = {0.0f, 0.0f, 0.0f};
    populateFloatArrayFromNSArray(_rotation, rotationValues, 3);
    self.node->getParentPortal()->setBackgroundRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});
    _sphereTextureAddedToScene = true;
    
    if (self.paused) {
        _videoTexture->pause();
    } else {
        _videoTexture->play();
    }
    
    _videoTexture->setVolume(self.volume);
    _videoTexture->setMuted(self.muted);
    _videoTexture->setLoop(self.loop);
    _videoTexture->setDelegate(_videoDelegate);
}

- (void)sceneWillAppear {
    if (_sphereTextureAddedToScene) {
        [self setPaused:self.paused];
    }
}

- (void)sceneWillDisappear {
    if (_sphereTextureAddedToScene) {
        _videoTexture->pause();
    }
}

- (void)parentDidAppear {
    [super parentDidAppear];
    if (!_sphereTextureAddedToScene) {
        [self updateSceneWithSphereTexture];
    } else {
        [self setPaused:self.paused];
    }
}

- (void)parentDidDisappear {
    if (_sphereTextureAddedToScene) {
        _videoTexture->pause();
    }
}

#pragma mark VROVideoDelegate implementation

- (void)videoWillBuffer {
    if (self.onBufferStartViro) {
        self.onBufferStartViro(nil);
    }
}

- (void)videoDidBuffer {
    if (self.onBufferEndViro) {
        self.onBufferEndViro(nil);
    }
}

- (void)videoDidFinish {
    if (self.onFinishViro) {
        self.onFinishViro(@{@"finished": @(true),});
    }
}

- (void)videoDidUpdateTime:(float)currentTimeInSeconds totalTimeInSeconds:(float)totalTime {
    if (self.onUpdateTimeViro) {
        self.onUpdateTimeViro(@{@"currentTime": @(currentTimeInSeconds),
                                @"totalTime": @(totalTime)});
    }
}

- (void)videoDidFail:(NSString *)error {
    if (self.onErrorViro) {
        self.onErrorViro(@{ @"error": @"Video failed to load" });
    }
}

@end
