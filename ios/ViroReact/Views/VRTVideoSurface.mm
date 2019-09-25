//
//  VRTVideoSurface.m
//  React
//
//  Created by Vik Advani on 3/24/16.
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

#import <React/RCTImageSource.h>
#import <React/RCTConvert.h>
#import "VRTVideoSurface.h"
#import "VRTUtils.h"

@implementation VRTVideoSurface{
    std::shared_ptr<VROSurface> _surface;
    std::shared_ptr<VROVideoTexture> _videoTexture;
    BOOL _videoSurfaceNeedsUpdate;
    std::shared_ptr<VROVideoDelegateiOS> _videoDelegate;
    NSString *_stereoMode;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _muted = NO;
        _loop = NO;
        _volume = 1;
        _paused = NO;
        _width = 1;
        _height = 1;
        _videoSurfaceNeedsUpdate = NO;
        _videoDelegate = std::make_shared<VROVideoDelegateiOS>(self);
    }
    return self;
}

- (void)seekToTime:(float)time {
    if (_videoTexture) {
        _videoTexture->seekToTime(time);
    }
}

- (void)setPaused:(BOOL)paused {
    _paused = paused;
    if (_videoTexture) {
        (_paused || ![self shouldAppear]) ? _videoTexture->pause() : _videoTexture->play();
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

- (void)setSource:(NSDictionary *)source {
    _source = source;
    _videoSurfaceNeedsUpdate = YES;
}

-(void)setWidth:(float)width {
    _width = width;
    _videoSurfaceNeedsUpdate = YES;
}

-(void)setHeight:(float)height {
    _height = height;
    _videoSurfaceNeedsUpdate = YES;
}

- (void)setStereoMode:(NSString *)mode;{
    _stereoMode = mode;
}

// Override applyMaterials so we can set our video
- (void)applyMaterials {
    [super applyMaterials];
    
    if (_videoTexture && self.node->getGeometry()) {
        self.node->getGeometry()->getMaterials().front()->getDiffuse().setTexture(_videoTexture);
    }
}

- (void)updateSurface {
    if (!_videoSurfaceNeedsUpdate || !self.context || !self.driver) {
        return;
    }
    if (!self.source) {
        RCTLogError(@"ViroVideo source should not be nil");
        return;
    }
    
    // All asset types can be converted into an RCTImageSource(should be renamed to RCTAssetSource) when using the 'require() statement).
    // If an asset is passed with a full uri, that uri is preserved and passed to the video surface
    RCTImageSource *imageSource = [RCTConvert RCTImageSource:self.source];
    NSURL *videoURL = imageSource.request.URL;
    std::string url = std::string([[videoURL description] UTF8String]);
    
    VROStereoMode mode = VROStereoMode::None;
    if (self.stereoMode){
        mode = VROTextureUtil::getStereoModeForString(std::string([self.stereoMode UTF8String]));
    }
    _videoTexture = std::make_shared<VROVideoTextureiOS>(mode);
    _surface = VROSurface::createSurface(_width, _height);
    _videoTexture->loadVideo(url, self.context->getFrameSynchronizer(), self.driver);
   
    [self node]->setGeometry(_surface);
    [self applyMaterials];
        
    _videoTexture->setVolume(self.volume);
    _videoTexture->setMuted(self.muted);
    _videoTexture->setLoop(self.loop);
    _videoTexture->setDelegate(_videoDelegate);

    // set paused again (let the internal logic run).
    [self setPaused:_paused];

    // set that we did in fact update the surface
    _videoSurfaceNeedsUpdate = NO;
}

- (void)setContext:(VRORenderContext *)context {
    [super setContext:context];
    [self updateSurface];
}

- (void)setDriver:(std::shared_ptr<VRODriver>)driver {
    [super setDriver:driver];
    [self updateSurface];
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    [self updateSurface];
}

- (void)sceneWillDisappear {
    if (_videoTexture) {
        _videoTexture->pause();
    }
}

- (void)handleAppearanceChange {
    // set paused again to let the logic re-run & pause if necessary.
    [self setPaused:self.paused];
    [super handleAppearanceChange];
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

- (void)videoDidUpdateTime:(float)currentTimeInSeconds totalTimeInSeconds:(float)totalTime{
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
