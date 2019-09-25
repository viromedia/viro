//
//  VRTSound.m
//  ViroReact
//
//  Created by Andy Chu on 1/26/17.
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

#import <React/RCTLog.h>
#import "VRTSound.h"
#import "VRTSoundModule.h"

static NSString *const kNameKey = @"name";
static NSString *const kUriKey = @"uri";
static NSString *const kLocalPrefix = @"file";
static NSString *const kWebPrefix = @"http";

#pragma mark - Base Sound
@interface VRTBaseSound ()

@property (readwrite, nonatomic) std::shared_ptr<VROSound> sound;
@property (nonatomic, assign) BOOL ready;
@property (nonatomic, assign) BOOL shouldReset;

@end

@implementation VRTBaseSound

- (instancetype)initWithSoundType:(VROSoundType)soundType bridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _soundType = soundType;
        _paused = YES;
        _muted = NO;
        _loop = NO;
        _volume = 1;
        _rolloffModel = @"None";
        _minDistance = 0;
        _maxDistance = 10;
        _shouldReset = YES;
    }
    return self;
}

- (void)setPaused:(BOOL)paused {
    _paused = paused;
    if (_sound) {
        (paused || ![self shouldAppear]) ? _sound->pause() : _sound->play();
    }
}

- (void)setMuted:(BOOL)muted {
    _muted = muted;
    if (_sound) {
        _sound->setMuted(muted);
    }
}

- (void)setLoop:(BOOL)loop {
    _loop = loop;
    if (_sound) {
        // setLoop turns the sound back on if loop is set to true and the sound is NOT paused. Make sure our pause state in the renderer properly reflects our bridge 'pause' property.
        if (_paused) {
            _sound->pause();
        }
        _sound->setLoop(loop);
    }
}

- (void)setVolume:(float)volume {
    _volume = volume;
    if (_sound) {
        _sound->setVolume(volume);
    }
}

- (void)setSource:(NSDictionary *)source {
    _source = source;
    _shouldReset = YES;
    [self resetSound];
}

- (void)setPosition:(NSArray<NSNumber *> *)position {
    _position = position;
    if (_sound) {
        _sound->setPosition({[position[0] floatValue],
            [position[1] floatValue],
            [position[2] floatValue]});
    }
}

- (void)setRotation:(NSArray<NSNumber *> *)rotation {
    _rotation = rotation;
    if (_sound) {
        _sound->setRotation({[rotation[0] floatValue],
            [rotation[1] floatValue],
            [rotation[2] floatValue]});
    }
}

- (VROSoundRolloffModel)parseRolloffModel:(NSString *)rolloffModel {
    if ([rolloffModel caseInsensitiveCompare:@"None"] == NSOrderedSame) {
        return VROSoundRolloffModel::None;
    } else if ([rolloffModel caseInsensitiveCompare:@"Linear"] == NSOrderedSame) {
        return VROSoundRolloffModel::Linear;
    } else if ([rolloffModel caseInsensitiveCompare:@"Logarithmic"] == NSOrderedSame) {
        return VROSoundRolloffModel::Logarithmic;
    } else {
        RCTLogError(@"Unknown rolloff model: %@", rolloffModel);
        return VROSoundRolloffModel::None;
    }
}

- (void)setRolloffModel:(NSString *)rolloffModel {
    _rolloffModel = rolloffModel;
    if (_sound) {
        _sound->setDistanceRolloffModel([self parseRolloffModel:_rolloffModel], _minDistance, _maxDistance);
    }
}

- (void)setMinDistance:(float)minDistance {
    _minDistance = minDistance;
    if (_sound) {
        _sound->setDistanceRolloffModel([self parseRolloffModel:_rolloffModel], _minDistance, _maxDistance);
    }
}

- (void)setMaxDistance:(float)maxDistance {
    _maxDistance = maxDistance;
    if (_sound) {
        _sound->setDistanceRolloffModel([self parseRolloffModel:_rolloffModel], _minDistance, _maxDistance);
    }
}

- (void)setDriver:(std::shared_ptr<VRODriver>)driver {
    [super setDriver:driver];
    [self resetSound];
}

- (void)seekToTime:(NSInteger)time {
    if (_sound) {
        _sound->seekToTime(time);
    }
}

- (void)resetSound {
    if (!self.driver || !_source || !_shouldReset) {
        return;
    }

    _shouldReset = NO;
    
    NSString *name = [_source objectForKey:kNameKey];
    if (name) {
        std::shared_ptr<VROSoundData> data = [self getDataForName:name];
        if (data == nullptr) {
            RCTLogError(@"Sound w/ name [%@] was not preloaded", name);
        }
        else {
            [self createSoundWithData:data local:false];
        }
    } else if([_source objectForKey:kUriKey]) {
        NSString *uri = [_source objectForKey:kUriKey];
        if ([uri hasPrefix:kLocalPrefix]) {
            [self createSoundWithPath:uri resourceType:VROResourceType::LocalFile];
        } else if ([uri hasPrefix:kWebPrefix]) {
            [self createSoundWithPath:uri resourceType:VROResourceType::URL];
        } else {
            [self createSoundWithPath:uri resourceType:VROResourceType::LocalFile];
        }
    } else {
        RCTLogError(@"Unknown sound source type. %@", _source);
    }
    
    [self setNativeProps];
}

- (void)createSoundWithPath:(NSString *)path resourceType:(VROResourceType)resourceType {
    if (_sound) {
        _sound->pause();
    }
    
    _sound = self.driver->newSound(std::string([path UTF8String]), resourceType, self.soundType);
    _sound->setDelegate(std::make_shared<VROSoundDelegateiOS>(self));
}

- (void)createSoundWithData:(std::shared_ptr<VROSoundData>)data local:(BOOL)local {
    if (_sound) {
        _sound->pause();
    }
    
    _sound = self.driver->newSound(data, self.soundType);
    _sound->setDelegate(std::make_shared<VROSoundDelegateiOS>(self));
}

- (std::shared_ptr<VROSoundData>)getDataForName:(NSString *)name {
    VRTSoundModule *soundModule = [self.bridge moduleForClass:[VRTSoundModule class]];
    return [soundModule dataForName:name];
}

- (void)setNativeProps {
    if (_sound) {
        [self setPaused:self.paused];
        _sound->setVolume(_volume);
        _sound->setMuted(_muted);
        _sound->setLoop(_loop);
        _sound->setPosition({[_position[0] floatValue],
            [_position[1] floatValue],
            [_position[2] floatValue]});
        _sound->setRotation({[_rotation[0] floatValue],
            [_rotation[1] floatValue],
            [_rotation[2] floatValue]});
        _sound->setDistanceRolloffModel([self parseRolloffModel:_rolloffModel], _minDistance, _maxDistance);
    }
}

- (void)soundIsReady {
    _ready = YES;
    [self setNativeProps];
    [self setPaused:self.paused];
}

- (void)soundDidFail:(NSString *)error {
    if (self.onErrorViro) {
        self.onErrorViro(@{ @"error": error });
    }
}

- (void)soundDidFinish {
    if (self.onFinishViro) {
        self.onFinishViro(@{@"finished": @(true),});
    }
    // If loop set to true, then seek to 0 and play (incase AVAudioPlayer pauses the video after playing)
    if (_loop) {
        [self seekToTime:0];
        [self setPaused:NO];
    }
}

- (void)sceneWillDisappear {
    if (_sound) {
        _sound->pause();
    }
}

- (void)parentDidDisappear {
    if (_sound) {
        _sound->pause();
    }
}

- (void)handleAppearanceChange {
    if ([self shouldAppear]) {
        if (_sound) {
            _paused ? _sound->pause() : _sound->play();
        }
    } else {
        if (_sound) {
            _sound->pause();
        }
    }
    [super handleAppearanceChange];
}

@end

#pragma mark - Sound

/**
 * Note: The reason we use a VROAudioPlayer as the native component of VRTSound is because gvr's API lacks many of the features that iOS's AVAudioPlayer supports. If/when gvr audio reaches parity, then we can switch over to VROSound (which is backed by gvr's audio API).
 */
@interface VRTSound ()

@property (readwrite, nonatomic) std::shared_ptr<VROAudioPlayer> player;

@end

@implementation VRTSound

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithSoundType:VROSoundType::Normal bridge:bridge];
    return self;
}

- (void)setPaused:(BOOL)paused {
    [super setPaused:paused];
    if (_player) {
        if (paused || ![self shouldAppear])  {
            _player->pause();
        } else {
            _player->play();
        }
    }
}

- (void)setMuted:(BOOL)muted {
    [super setMuted:muted];
    if (_player) {
        _player->setMuted(muted);
    }
}

- (void)setLoop:(BOOL)loop {
    [super setLoop:loop];
    if (_player) {
        //setLoop turns the sound back on if loop is set to true and the sound is NOT paused. Make sure our pause state in the renderer properly reflects our bridge 'pause' property.
        if (self.paused) {
            _player->pause();
        }
        _player->setLoop(loop);
    }
}

- (void)setVolume:(float)volume {
    [super setVolume:volume];
    if (_player) {
        _player->setVolume(volume);
    }
}

// Override
- (void)createSoundWithPath:(NSString *)path resourceType:(VROResourceType)resourceType {
    if (_player) {
        _player->pause();
    }
    
    _player = self.driver->newAudioPlayer(std::string([path UTF8String]), resourceType != VROResourceType::URL);
    _player->setDelegate(std::make_shared<VROSoundDelegateiOS>(self));
    _player->setup();
}

- (void)createSoundWithData:(std::shared_ptr<VROSoundData>)data local:(BOOL)local {
    if (_player) {
        _player->pause();
    }
    _player = self.driver->newAudioPlayer(data);
    _player->setDelegate(std::make_shared<VROSoundDelegateiOS>(self));
    _player->setup();
}

- (void)setNativeProps {
    if (_player) {
        _player->setVolume(self.volume);
        _player->setMuted(self.muted);
        // re-run the setPaused logic to start/stop playback.
        // set pause before setting since player->loop() will check internal pause state and play the sound if sound if pause state is false(default).

        [self setPaused:self.paused];
        _player->setLoop(self.loop);
    }
}

- (void)soundIsReady {
    [super soundIsReady];
    [self setNativeProps];
}

- (void)seekToTime:(NSInteger)time {
    [super seekToTime:time];
    if (_player) {
        _player->seekToTime(time);
    }
}

- (void)sceneWillDisappear {
    if (_player) {
        _player->pause();
    }
}

- (void)handleAppearanceChange {
    if ([self shouldAppear]) {
        if (_player) {
            if (self.paused) {
                _player->pause();
            } else {
                _player->play();
            }
        }
    } else {
        if (_player) {
            _player->pause();
        }
    }
    [super handleAppearanceChange];
}

@end

#pragma mark - Sound Field
@implementation VRTSoundField

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithSoundType:VROSoundType::SoundField bridge:bridge];
    return self;
}

@end

#pragma mark - Spatial Sound

@interface VRTSpatialSound ()

@property (readwrite, nonatomic) std::shared_ptr<VRONode> parentNode;

@end

@implementation VRTSpatialSound

- (instancetype)initWithBridge:(RCTBridge *)bridge node:(std::shared_ptr<VRONode>)node {
    self = [super initWithSoundType:VROSoundType::Spatial bridge:bridge];
    if (self) {
        _parentNode = node;
    }
    return self;
}

- (void)resetSound {
    std::shared_ptr<VROSound> oldNativeSound = self.sound;
    BOOL shouldReset = self.shouldReset;
    [super resetSound];
    
    if (shouldReset && oldNativeSound) {
        _parentNode->removeSound(oldNativeSound);
    }
    if (shouldReset && self.sound) {
        _parentNode->addSound(self.sound);
    }
}

@end

#pragma mark - Spatial Sound Wrapper

@interface VRTSpatialSoundWrapper ()

@property (readwrite, nonatomic) VRTSpatialSound *innerSound;

@end

@implementation VRTSpatialSoundWrapper

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _innerSound = [[VRTSpatialSound alloc] initWithBridge:bridge node:self.node];
    }
    return self;
}

- (void)setDriver:(std::shared_ptr<VRODriver>)driver {
    [_innerSound setDriver:driver];
}

- (void)setParentHasAppeared:(BOOL)parentHasAppeared {
    [_innerSound setParentHasAppeared:parentHasAppeared];
}

- (void)setSource:(NSDictionary *)source {
    [_innerSound setSource:source];
}

- (void)setPaused:(BOOL)paused {
    [_innerSound setPaused:paused];
}

- (void)setVolume:(float)volume {
    [_innerSound setVolume:volume];
}

- (void)setMuted:(BOOL)muted {
    [_innerSound setMuted:muted];
}

- (void)setLoop:(BOOL)loop {
    [_innerSound setLoop:loop];
}

- (void)setPosition:(NSArray<NSNumber *> *)position {
    [_innerSound setPosition:position];
}

- (void)setRolloffModel:(NSString *)rolloffModel {
    [_innerSound setRolloffModel:rolloffModel];
}

- (void)setMinDistance:(float)minDistance {
    [_innerSound setMinDistance:minDistance];
}

- (void)setMaxDistance:(float)maxDistance {
    [_innerSound setMaxDistance:maxDistance];
}

- (void)setOnFinishViro:(RCTDirectEventBlock)onFinishViro {
    [_innerSound setOnFinishViro:onFinishViro];
}

@end
