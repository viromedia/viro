//
//  VRTMaterialVideo.mm
//  ViroReact
//
//  Created by vik.advani on 3/8/18.
//  Copyright © 2018 Viro Media. All rights reserved.
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
#import <React/RCTUIManager.h>
#import "VRTMaterialVideo.h"

@implementation VRTMaterialVideo {
    std::shared_ptr<VROVideoDelegateiOS> _videoDelegate;
    std::shared_ptr<VROVideoTexture> _videoTexture;
    BOOL _videoTextureNeedsUpdating;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _muted = NO;
        _loop = NO;
        _volume = 1;
        _paused = NO;
        _videoDelegate = std::make_shared<VROVideoDelegateiOS>(self);
        _videoTexture = nil;
        _videoTextureNeedsUpdating = NO;
    }
    return self;
}

- (void)seekToTime:(float)time {
    if (_videoTexture) {
        _videoTexture->seekToTime(time);
    }
}

- (void)setMaterial:(NSString *)materialName {
    _material = materialName;
    VRTMaterialManager *materialManager = [self.bridge moduleForClass:[VRTMaterialManager class]];
    [materialManager addMaterialChangedListener:materialName listener:self];
    NSLog(@"VRTMaterialVideo: Updating materialName: %@", _material);
    _videoTextureNeedsUpdating = YES;
    [self updateMaterialVideoTexture];
}

- (void)setPaused:(BOOL)paused {
    _paused = paused;
    if (_videoTexture) {
        (_paused) ? _videoTexture->pause() : _videoTexture->play();
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
        NSLog(@"VRTMaterialVideo: Updating video texture: %p", _videoTexture.get());
        _videoTexture->setMuted(muted);
    }
}

- (void)setVolume:(float)volume {
    _volume = volume;
    if (_videoTexture) {
        _videoTexture->setVolume(volume);
    }
}

-(void)updateMaterialVideoTexture {
    if (_videoTextureNeedsUpdating && _material != nil) {
        VRTMaterialManager *materialManager = [self.bridge moduleForClass:[VRTMaterialManager class]];
        std::shared_ptr<VROMaterial> videoMaterial = [materialManager getMaterialByName:_material];
        if (videoMaterial) {
            std::shared_ptr<VROTexture> _texture = videoMaterial->getDiffuse().getTexture();
            VROVideoTexture *videoTexture = dynamic_cast<VROVideoTexture *>(_texture.get());
            if(videoTexture == nullptr) {
                RCTLogError(@"Unable to cast base texture to video texture. Ensure texture named %@ is actually video texture", _material);
            }
            //if we have a old video texture. pause it
            if (_videoTexture) {
                _videoTexture->pause();
            }
            _videoTexture = std::dynamic_pointer_cast<VROVideoTextureiOS>(_texture);
            _videoTextureNeedsUpdating = NO;
        }
    }
}

-(void)applyProps {
    //set all variables dependent on new video texture.
    if (_videoTexture) {
        _videoTexture->setVolume(self.volume);
        _videoTexture->setMuted(self.muted);
        _videoTexture->setLoop(self.loop);
        _videoTexture->setDelegate(_videoDelegate);
        [self setPaused:_paused];
    }
}
- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    [self applyProps];
}

#pragma mark VRTMaterialChangedDelegate implementation
- (void)videoMaterialDidChange:(NSString *)materialName {
    if (_material != nil && [_material isEqualToString:materialName]) {
        _videoTextureNeedsUpdating = YES;
        [self updateMaterialVideoTexture];
        [self applyProps];
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
