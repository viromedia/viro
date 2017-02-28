//
//  VRTSound.m
//  ViroReact
//
//  Created by Andy Chu on 1/26/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
        _paused = NO;
        _muted = NO;
        _loop = NO;
        _volume = 1;
        _rolloffModel = VROSoundRolloffModel::None;
        _minDistance = 0;
        _maxDistance = 10;
        _shouldReset = YES;
    }
    return self;
}

- (void)setPaused:(BOOL)paused {
    _paused = paused;
    if (_sound) {
        paused ? _sound->pause() : _sound->play();
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

- (void)setRolloffModel:(NSString *)rolloffModel {
    if ([rolloffModel caseInsensitiveCompare:@"None"] == NSOrderedSame) {
        _rolloffModel = VROSoundRolloffModel::None;
    } else if ([rolloffModel caseInsensitiveCompare:@"Linear"] == NSOrderedSame) {
        _rolloffModel = VROSoundRolloffModel::Linear;
    } else if ([rolloffModel caseInsensitiveCompare:@"Logarithmic"] == NSOrderedSame) {
        _rolloffModel = VROSoundRolloffModel::Logarithmic;
    } else {
        RCTLogError(@"Unknown rolloff model: %@", rolloffModel);
    }
    if (_sound) {
        _sound->setDistanceRolloffModel(_rolloffModel, _minDistance, _maxDistance);
    }
}

- (void)setMinDistance:(float)minDistance {
    _minDistance = minDistance;
    if (_sound) {
        _sound->setDistanceRolloffModel(_rolloffModel, _minDistance, _maxDistance);
    }
}

- (void)setMaxDistance:(float)maxDistance {
    _maxDistance = maxDistance;
    if (_sound) {
        _sound->setDistanceRolloffModel(_rolloffModel, _minDistance, _maxDistance);
    }
}

- (void)setDriver:(VRODriver *)driver {
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
        [self createSoundWithData:data local:false];
    } else if([_source objectForKey:kUriKey]) {
        NSString *uri = [_source objectForKey:kUriKey];
        if ([uri hasPrefix:kLocalPrefix]) {
            [self createSoundWithPath:uri local:true];
        } else if ([uri hasPrefix:kWebPrefix]) {
            [self createSoundWithPath:uri local:false];
        } else {
            RCTLogError(@"Unknown path to sound: %@", uri);
        }
    } else {
        RCTLogError(@"Unknown sound source type. %@", _source);
    }

    [self setNativeProps];
}

- (void)createSoundWithPath:(NSString *)path local:(BOOL)local {
    // TODO: VIRO-756 implement loading from local resources
    if (_sound) {
        _sound->pause();
    }
    _sound = self.driver->newSound(std::string([path UTF8String]), self.soundType, local);
    _sound->setDelegate(std::make_shared<VROSoundDelegateiOS>(self));
}

- (void)createSoundWithData:(std::shared_ptr<VROSoundData>)data local:(BOOL)local {
    // TODO: VIRO-756 implement loading from local resources
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
        if (_ready) {
            _paused ? _sound->pause() : _sound->play();
        }
        _sound->setVolume(_volume);
        _sound->setMuted(_muted);
        _sound->setLoop(_loop);
        _sound->setPosition({[_position[0] floatValue],
                             [_position[1] floatValue],
                             [_position[2] floatValue]});
        _sound->setRotation({[_rotation[0] floatValue],
                             [_rotation[1] floatValue],
                             [_rotation[2] floatValue]});
        _sound->setDistanceRolloffModel(_rolloffModel, _minDistance, _maxDistance);
    }
}


- (void)soundIsReady {
    _ready = YES;
    if (_sound) {
        _paused ? _sound->pause() : _sound->play();
    }
}

- (void)soundDidFinish {
    if (self.onFinishViro) {
        self.onFinishViro(@{@"finished": @(true),});
    }
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
    if (_player) {
        paused ? _player->pause() : _player->play();
    }
}

- (void)setMuted:(BOOL)muted {
    if (_player) {
        _player->setMuted(muted);
    }
}

- (void)setLoop:(BOOL)loop {
    if (_player) {
        _player->setLoop(loop);
    }
}

- (void)setVolume:(float)volume {
    if (_player) {
        _player->setVolume(volume);
    }
}

// Override
- (void)createSoundWithPath:(NSString *)path local:(BOOL)local {
    // TODO: VIRO-756 implement loading from local resources
    if (_player) {
        _player->pause();
    }
    _player = self.driver->newAudioPlayer(std::string([path UTF8String]));
    _player->setDelegate(std::make_shared<VROSoundDelegateiOS>(self));
}

- (void)createSoundWithData:(std::shared_ptr<VROSoundData>)data local:(BOOL)local {
    // TODO: VIRO-756 implement loading from local resources
    if (_player) {
        _player->pause();
    }
    _player = self.driver->newAudioPlayer(data);
    _player->setDelegate(std::make_shared<VROSoundDelegateiOS>(self));
}

- (void)setNativeProps {
    if (_player) {
        self.paused ? _player->pause() : _player->play();
        _player->setMuted(self.muted);
        _player->setLoop(self.loop);
        _player->setVolume(self.volume);
    }
}

- (void)soundIsReady {
    [super soundIsReady];
    if (_player) {
        self.paused ? _player->pause() : _player->play();
    }
}

- (void)seekToTime:(NSInteger)time {
    if (_player) {
        _player->seekToTime(time);
    }
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
        _innerSound = [[VRTSpatialSound alloc] initWithBridge:bridge];
    }
    return self;
}

- (void)setDriver:(VRODriver *)driver {
    [_innerSound setDriver:driver];
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

- (void)setRolloffModel:(VROSoundRolloffModel)rolloffModel {
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
