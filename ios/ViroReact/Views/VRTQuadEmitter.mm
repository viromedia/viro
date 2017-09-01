//
//  VRTQuadEmitter.m
//  ViroReact
//
//  Created by Andy Chu on 8/15/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <ViroKit/ViroKit.h>
#import <React/RCTConvert.h>
#import "VRTQuadEmitter.h"
#import "VRTImageAsyncLoader.h"

const int kDefaultSpawnRate = 0;
const int kDefaultParticleLifetime = 0;
const int kDefaultMaxParticles = 500;

@implementation VRTQuadEmitter {
    VRTImageAsyncLoader *_loader;
    std::shared_ptr<VROParticleEmitter> _emitter;
    std::shared_ptr<VROTexture> _particleTexture;
    bool _needsQuadUpdate;
    bool _needsRestart;
    
    std::shared_ptr<VROParticleModifier> _defaultAlphaModifier;
    std::shared_ptr<VROParticleModifier> _defaultColorModifier;
    std::shared_ptr<VROParticleModifier> _defaultScaleModifier;
    std::shared_ptr<VROParticleModifier> _defaultRotationModifier;
    std::shared_ptr<VROParticleModifier> _defaultVelocityModifier;
    std::shared_ptr<VROParticleModifier> _defaultAccelerationModifier;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _loader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
        _particleTexture = nullptr;
        _emitter = nullptr;
        _needsRestart = false;
        _run = false;
        _needsQuadUpdate = false;

        // Set modifier defaults to simulate particle in its original configuration.
        _defaultAlphaModifier = std::make_shared<VROParticleModifier>(VROVector3f(1,0,0));
        _defaultColorModifier = std::make_shared<VROParticleModifier>(VROVector3f(1,1,1));
        _defaultScaleModifier = std::make_shared<VROParticleModifier>(VROVector3f(1,1,1));
        _defaultRotationModifier = std::make_shared<VROParticleModifier>(VROVector3f(0,0,0));
        _defaultVelocityModifier = std::make_shared<VROParticleModifier>(VROVector3f(0,0,0));
        _defaultAccelerationModifier = std::make_shared<VROParticleModifier>(VROVector3f(0,0,0));
    }
    return self;
}

- (void)setDuration:(float)duration {
    _duration = duration;
    _needsRestart = true;
}

- (void)setDelay:(float)delay {
    _delay = delay;
    _needsRestart = true;
}

- (void)setQuad:(NSDictionary *)quad {
    _quad = quad;
    _needsQuadUpdate = true;
}

- (void)setSpawnModifier:(NSDictionary *)spawnModifier {
    _spawnModifier = spawnModifier;
    _needsRestart = true;
}

- (void)setAppearanceModifier:(NSDictionary *)appearanceModifier {
    _appearanceModifier = appearanceModifier;
    _needsRestart = true;
}

- (void)setPhysicsModifier:(NSDictionary *)physicsModifier {
    _physicsModifier = physicsModifier;
    _needsRestart = true;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    [self updateEmitter];
}

- (void)handleAppearanceChange {
    if ([self shouldAppear]) {
        [self updateEmitter];
    } else {
        // if hidden stop emitter.
        if (_emitter != nullptr) {
            _emitter->setRun(false);
        }

        // Else if the emitter view is removed from the scene.
        if (!self.parentHasAppeared){
            [self removeEmitter];
        }
    }
}

- (void)updateEmitter {
    if (!self.driver || !self.node || !self.quad || !self.scene) {
        return;
    }

    // If the image or image size has changed, recreate the emitter.
    [self updateImageIfNeeded];

    // If a particle source image is provided, wait for it to be loaded
    // into the texture before initializing the emitter with it.
    if ([self.quad objectForKey:@"source"] && _particleTexture == nullptr) {
        return;
    }

    // Create the emitter if we haven't yet done so, or if the texture has changed.
    if (_emitter == nullptr) {
        [self createEmitter];
    }

    // Update emitter modifiers
    [self updateSpawnModifier];
    [self updateAppearanceModifier];
    [self updatePhysicsModifier];

    // Finally set emitter states.
    [self updateEmitterState];

    // Restart emitter if required (some properties may need an emitter restart)
    if (_needsRestart) {
        _emitter->resetEmissionCycle(true);
        _needsRestart = false;
    }
}

- (void)updateImageIfNeeded {
    // Don't update if we have already updated the image.
    if (!_needsQuadUpdate) {
        return;
    }

    // If image has changed, redownload it and re-update the emitter once done.
    [self removeEmitter];

    // Schedule image for download if given.
    NSString *nsStringQuadSource = [self.quad objectForKey:@"source"];
    if (nsStringQuadSource != nil) {
        [_loader loadImage:[RCTConvert RCTImageSource:nsStringQuadSource]];
    } else {
        [_loader cancel];
        _loader.tag = nil;
        _particleTexture = nullptr;
    }
    _needsQuadUpdate = false;
}

-(void)createEmitter {
    NSNumber *width = [self.quad objectForKey:@"width"];
    NSNumber *height = [self.quad objectForKey:@"height"];
    float fwidth = width ? [width floatValue] : 1.0;
    float fheight = height ? [height floatValue] : 1.0;
    _emitter = std::make_shared<VROParticleEmitter>(self.driver, self.node, _particleTexture, fwidth, fheight);
   
    if ([self.quad objectForKey:@"bloomThreshold"]){
        float threshold = [[self.quad objectForKey:@"bloomThreshold"] doubleValue];
        self.node->getGeometry()->getMaterials()[0]->setBloomThreshold(threshold);
    }
    
    self.scene->addParticleEmitter(_emitter);
}

-(void)removeEmitter {
    if (_emitter == nullptr) {
        return;
    }

    if (self.scene) {
        self.scene->removeParticleEmitter(_emitter);
    }
    _particleTexture = nullptr;
    _emitter = nullptr;
}

- (void)updateEmitterState {
    _emitter->setDelay(self.delay);
    _emitter->setFixedToEmitter(self.fixedToEmitter);
    _emitter->setDuration(self.duration);
    _emitter->setLoop(self.loop);
    _emitter->setRun(self.run);
}

- (void)updateSpawnModifier {
    if (!_spawnModifier) {
        return;
    }

    NSArray *emissionRatePerSec = [_spawnModifier objectForKey:@"emissionRatePerSecond"];
    if (emissionRatePerSec && [emissionRatePerSec count] == 2) {
        std::pair<int, int> range = std::pair<int, int>([[emissionRatePerSec objectAtIndex:0] intValue],
                                                        [[emissionRatePerSec objectAtIndex:1] intValue]);
        _emitter->setEmissionRatePerSecond(range);
    } else {
        _emitter->setEmissionRatePerSecond(std::pair<int, int>(kDefaultSpawnRate, kDefaultSpawnRate));
    }

    NSArray *emissionRatePerMeter = [_spawnModifier objectForKey:@"emissionRatePerMeter"];
    if (emissionRatePerMeter && [emissionRatePerMeter count] == 2){
        std::pair<int, int> range = std::pair<int, int>([[emissionRatePerMeter objectAtIndex:0] intValue],
                                                        [[emissionRatePerMeter objectAtIndex:1] intValue]);
        _emitter->setEmissionRatePerDistance(range);
    } else {
        _emitter->setEmissionRatePerDistance(std::pair<int, int>(kDefaultSpawnRate, kDefaultSpawnRate));
    }

    NSArray *particleLifeTime = [_spawnModifier objectForKey:@"particleLifetime"];
    if (particleLifeTime && [particleLifeTime count] == 2) {
        std::pair<int, int> range = std::pair<int, int>([[particleLifeTime objectAtIndex:0] intValue],
                                                        [[particleLifeTime objectAtIndex:1] intValue]);
        _emitter->setParticleLifeTime(range);
    } else {
        _emitter->setParticleLifeTime(std::pair<int, int>(kDefaultParticleLifetime, kDefaultParticleLifetime));
    }

    if ([_spawnModifier objectForKey:@"maxParticles"]) {
        int maxParticles = [[_spawnModifier objectForKey:@"maxParticles"] intValue];
        _emitter->setMaxParticles(maxParticles);
    } else {
        _emitter->setMaxParticles(kDefaultMaxParticles);
    }

    if ([_spawnModifier objectForKey:@"emissionBurst"]) {
        std::vector<VROParticleEmitter::VROParticleBurst> bursts;
        NSArray *burstArray = [_spawnModifier objectForKey:@"emissionBurst"];

        // Process each burst
        for (int i = 0; i < [burstArray count]; i ++) {
            VROParticleEmitter::VROParticleBurst burst;
            NSDictionary *burstDictionary = [burstArray objectAtIndex:i];
            if ([burstDictionary objectForKey:@"time"]) {
                burst.referenceFactor = VROParticleModifier::VROModifierFactor::Time;
                burst.referenceValueStart = [[burstDictionary objectForKey:@"time"] doubleValue];
                burst.referenceValueInterval = [[burstDictionary objectForKey:@"cooldownPeriod"] doubleValue];
            } else if ([burstDictionary objectForKey:@"distance"]) {
                burst.referenceFactor = VROParticleModifier::VROModifierFactor::Distance;
                burst.referenceValueStart = [[burstDictionary objectForKey:@"distance"] doubleValue];
                burst.referenceValueInterval = [[burstDictionary objectForKey:@"cooldownDistance"] doubleValue];
            } else {
                RCTLogError(@"Viro: Unsupported modifier factor provided! Must specify either time or distance.");
                continue;
            }
            int min = [[burstDictionary objectForKey:@"min"] intValue];
            int max = [[burstDictionary objectForKey:@"max"] intValue];
            burst.numberOfParticles = std::pair<int, int>(min,max);
            burst.cycles = [[burstDictionary objectForKey:@"cycles"] intValue];
            bursts.push_back(burst);
        }
        _emitter->setParticleBursts(bursts);
    } else {
        _emitter->setParticleBursts(std::vector<VROParticleEmitter::VROParticleBurst>());
    }

    if ([_spawnModifier objectForKey:@"spawnVolume"]) {
        NSDictionary *spawnVolume = [_spawnModifier objectForKey:@"spawnVolume"];
        NSString *stringShapeName = [spawnVolume objectForKey:@"shape"];
        NSArray *shapeParams = [spawnVolume objectForKey:@"params"];

        if (!stringShapeName) {
            RCTLogError(@"Viro: Missing required volume shape type, skipping spawn volume modifier.");
            return;
        }

        VROParticleSpawnVolume vol;
        if ([stringShapeName isEqualToString:@"box"]) {
            vol.shape = VROParticleSpawnVolume::Shape::Box;
        } else if ([stringShapeName isEqualToString:@"sphere"]) {
            vol.shape = VROParticleSpawnVolume::Shape::Sphere;
        } else {
            RCTLogError(@"Viro: Incorrect spawn volume shape type. Must be either sphere or box.");
            return;
        }

        // Grab the current shapeParams
        if (shapeParams) {
            std::vector<double> params = {};
            for (int i = 0; i < [shapeParams count]; i ++) {
                double value = [[shapeParams objectAtIndex:i] doubleValue];
                params.push_back(value);
            }
            vol.shapeParams = params;
        }

        vol.spawnOnSurface = [[spawnVolume objectForKey:@"spawnOnSurface"] boolValue];
        _emitter->setParticleSpawnVolume(vol);
    } else {
        VROParticleSpawnVolume defaultVol;
        defaultVol.shape =  VROParticleSpawnVolume::Shape::Point;
        _emitter->setParticleSpawnVolume(defaultVol);
    }
}

-(void)updateAppearanceModifier {
    if (!_appearanceModifier) {
        _emitter->setAlphaModifier(_defaultAlphaModifier);
        _emitter->setScaleModifier(_defaultScaleModifier);
        _emitter->setRotationModifier(_defaultRotationModifier);
        _emitter->setColorModifier(_defaultColorModifier);
        return;
    }

    std::shared_ptr<VROParticleModifier> alphaModifier = [self getModifier:[_appearanceModifier objectForKey:@"opacity"]
                                                                    isVec3:false
                                                                   isColor:false
                                                                   isFloat:true];
    
    std::shared_ptr<VROParticleModifier> scaleModifier = [self getModifier:[_appearanceModifier objectForKey:@"scale"]
                                                                    isVec3:true
                                                                   isColor:false
                                                                   isFloat:false];
    
    std::shared_ptr<VROParticleModifier> rotationModifier = [self getModifier:[_appearanceModifier objectForKey:@"rotation"]
                                                                    isVec3:true
                                                                   isColor:false
                                                                   isFloat:false];
    
    std::shared_ptr<VROParticleModifier> colorModifier = [self getModifier:[_appearanceModifier objectForKey:@"color"]
                                                                    isVec3:false
                                                                   isColor:true
                                                                   isFloat:false];
    
    _emitter->setAlphaModifier(alphaModifier != nullptr ? alphaModifier : _defaultAlphaModifier);
    _emitter->setScaleModifier(scaleModifier != nullptr ? scaleModifier : _defaultScaleModifier);
    _emitter->setRotationModifier(rotationModifier != nullptr ? rotationModifier : _defaultRotationModifier);
    _emitter->setColorModifier(colorModifier != nullptr ? colorModifier : _defaultColorModifier);
}

-(void)updatePhysicsModifier{
    if (!_physicsModifier) {
        _emitter->setVelocityModifier(_defaultVelocityModifier);
        _emitter->setAccelerationmodifier(_defaultAccelerationModifier);
        _emitter->setInitialExplosion(VROVector3f(0,0,0), -1);
        return;
    }
    
    std::shared_ptr<VROParticleModifier> velocityMod = [self getModifier:[_physicsModifier objectForKey:@"velocity"]
                                                                       isVec3:true
                                                                      isColor:false
                                                                      isFloat:false];
    
    std::shared_ptr<VROParticleModifier> accelMod = [self getModifier:[_physicsModifier objectForKey:@"acceleration"]
                                                                       isVec3:true
                                                                      isColor:false
                                                                      isFloat:false];
    
    _emitter->setVelocityModifier(velocityMod != nullptr ? velocityMod : _defaultVelocityModifier);
    _emitter->setAccelerationmodifier(accelMod != nullptr ? accelMod : _defaultAccelerationModifier);

    if ([_physicsModifier objectForKey:@"initialExplosiveImpulse"]) {
        NSDictionary *explosiveDict = [_physicsModifier objectForKey:@"initialExplosiveImpulse"];
        NSArray *position = [explosiveDict objectForKey:@"position"];
        if ([position count] < 3) {
            RCTLogError(@"Incorrect parameters provided initial explosion impulse");
            return;
        }

        VROVector3f explodedAt = VROVector3f([[position objectAtIndex:0] floatValue],
                                             [[position objectAtIndex:1] floatValue],
                                             [[position objectAtIndex:2] floatValue]);
        float impulse = [[explosiveDict objectForKey:@"impulse"] floatValue];
        float reverseAccel = [[explosiveDict objectForKey:@"deccelerationPeriod"] floatValue];
        if (![explosiveDict objectForKey:@"deccelerationPeriod"]){
            reverseAccel = -1;
        }
        _emitter->setInitialExplosion(explodedAt, impulse, reverseAccel);
    } else {
        _emitter->setInitialExplosion(VROVector3f(0,0,0), -1);
    }
}

- (std::shared_ptr<VROParticleModifier>) getModifier:(NSDictionary *)modDictionary
                                              isVec3:(bool)isVec3
                                             isColor:(bool)isColor
                                             isFloat:(bool)isFloat{
    // If nothing is defined, unset the modifier.
    if (!modDictionary) {
        return nullptr;
    }
    
    NSArray *intervalsDictArray = [modDictionary objectForKey:@"modifier"];
    VROParticleModifier::VROModifierFactor factor = VROParticleModifier::VROModifierFactor::Time;

    // Grab the VROModifierFactor this modifier is interpolating against.
    if ([modDictionary objectForKey:@"factor"]) {
        std::string stringFactor;
        stringFactor = std::string([[modDictionary objectForKey:@"factor"] UTF8String]);
        if (stringFactor == "distance") {
            factor = VROParticleModifier::VROModifierFactor::Distance;
        } else if (stringFactor != "time" && intervalsDictArray && [intervalsDictArray count] > 0) {
            RCTLogError(@"Viro: Provided unknown modifier factor to interpolate against!");
            return nullptr;
        }
    }

    // Parse through the array of interval modifiers.
    std::vector<VROParticleModifier::VROModifierInterval> intervals;
    for (int i = 0; i < [intervalsDictArray count]; i ++) {
        // Ensure we have a valid interval configuration
        NSDictionary *intervalDict = [intervalsDictArray objectAtIndex:i];
        if (![intervalDict objectForKey:@"interval"] || ![intervalDict objectForKey:@"finalValue"]) {
            RCTLogError(@"Viro: Incorrectly configured modifier, skipping modifier.");
            continue;
        }

        // Grab the values representing the interval data, for example scale / velocity. If we
        // fail to grab it, skip to the next modifier.
        VROParticleModifier::VROModifierInterval interval;
        if (![self getVecValueFromDict:intervalDict
                               withKey:@"finalValue"
                                isVec3:isVec3 isColor:isColor isFloat:isFloat
                                outVec:interval.targetedValue]) {
            continue;
        }

        // Also grab the start / end values defining this window (for example, start/end times).
        NSArray *intervalWindow = [intervalDict objectForKey:@"interval"];
        if ([intervalWindow count] != 2) {
            RCTLogError(@"Incorrect parameters provided for interval, expected: [min, max]!");
            continue;
        }
        interval.startFactor = [[intervalWindow objectAtIndex:0] floatValue];
        interval.endFactor = [[intervalWindow objectAtIndex:1] floatValue];
        intervals.push_back(interval);
    }

    // Grab the inital values to be set in the Particle Modifier
    VROVector3f minVec;
    VROVector3f maxVec;
    bool hasValue = [self getVecValueFromDict:modDictionary withKey:@"min"
                                       isVec3:isVec3 isColor:isColor isFloat:isFloat outVec:minVec];
    hasValue = hasValue && [self getVecValueFromDict:modDictionary withKey:@"max"
                                       isVec3:isVec3 isColor:isColor isFloat:isFloat outVec:maxVec];

    if (!hasValue) {
        return nullptr;
    }

    return std::make_shared<VROParticleModifier>(minVec, maxVec, factor, intervals);
}

- (bool)getVecValueFromDict:(NSDictionary *)dict
                    withKey:(NSString *)key
                     isVec3:(bool)isVec3
                    isColor:(bool)isColor
                    isFloat:(bool)isFloat
                     outVec:(VROVector3f &)vec{

    if (![dict objectForKey:key]) {
        RCTLogError(@"Incorrect parameters provided for tag %@!", key);
        return false;
    }

    if (isFloat) {
        float value = [[dict objectForKey:key] floatValue];
        vec = VROVector3f(value,0,0);
        return true;
    }

    if (isColor) {
        UIColor *color = [RCTConvert UIColor:[dict objectForKey:key]];
        if (!color) {
            return false;
        }
        
        CGFloat red,green,blue, alpha;
        [color getRed:&red green:&green blue:&blue alpha:&alpha];
        vec = VROVector3f(red, green, blue);
        return true;
    }

    if (isVec3) {
        NSArray *value = [dict objectForKey:key];
        if ([value count] < 3) {
            RCTLogError(@"Incorrect parameters provided for %@, expected: [x, y, z]!", key);
            return false;
        }
        
        vec = VROVector3f([[value objectAtIndex:0] floatValue],
                          [[value objectAtIndex:1] floatValue],
                          [[value objectAtIndex:2] floatValue]);
        return true;
    }
    return false;
}

#pragma mark VRTImageAsyncLoaderEventDelegate
- (void)imageLoaderDidStart:(VRTImageAsyncLoader *)loader {
    // no-op
}

- (void)imageLoaderDidEnd:(VRTImageAsyncLoader *)loader success:(BOOL)success image:(UIImage *)image {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (success){
            _particleTexture = std::make_shared<VROTexture>(VROTextureInternalFormat::RGBA8, true,
                                                                               VROMipmapMode::Runtime,
                                                                               std::make_shared<VROImageiOS>(image, VROTextureInternalFormat::RGBA8),
                                                                               VROStereoMode::None);
            [self updateEmitter];
        } else {
            perror("Viro: Error loading particle image resource");
        }
    });
}

@end
