//
//  VRTParticleEmitter.m
//  ViroReact
//
//  Created by Andy Chu on 8/15/17.
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

#import <ViroKit/ViroKit.h>
#import <React/RCTConvert.h>
#import "VRTParticleEmitter.h"
#import "VRTImageAsyncLoader.h"
#import "VRTQuad.h"

const int kDefaultSpawnRatePerSec = 10;
const int kDefaultSpawnRatePerMeter = 0;
const int kDefaultParticleLifetime = 2000;
const int kDefaultMaxParticles = 500;

@implementation VRTParticleEmitter {
    VRTImageAsyncLoader *_loader;
    std::shared_ptr<VROParticleEmitter> _emitter;
    std::shared_ptr<VROTexture> _particleTexture;
    std::shared_ptr<VROSurface> _particleGeometry;
    bool _needsRestart;
    bool _needsImageUpdate;
    NSString *_currentImageSource;
    
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
        _needsImageUpdate = false;
        _currentImageSource = nil;

        // Set modifier defaults to simulate particle in its original configuration.
        _defaultAlphaModifier = std::make_shared<VROParticleModifier>(VROVector3f(1,0,0));
        _defaultColorModifier = std::make_shared<VROParticleModifier>(VROVector3f(1,1,1));
        _defaultScaleModifier = std::make_shared<VROParticleModifier>(VROVector3f(1,1,1));
        _defaultRotationModifier = std::make_shared<VROParticleModifier>(VROVector3f(0,0,0));
        _defaultVelocityModifier = std::make_shared<VROParticleModifier>(VROVector3f(-0.5,1,0), VROVector3f(0.5,1,0));
        _defaultAccelerationModifier = std::make_shared<VROParticleModifier>(VROVector3f(0,0,0));
        
        // Create a VROSurface, representing the particle geometry template for this emitter.
        _particleGeometry = VROSurface::createSurface(1, 1);
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

- (void)setImage:(NSDictionary *)image {
    _image = image;
    _needsImageUpdate = true;
}

- (void)setSpawnBehavior:(NSDictionary *)spawnBehavior {
    _spawnBehavior = spawnBehavior;
    _needsRestart = true;
}

- (void)setParticleAppearance:(NSDictionary *)particleAppearance {
    _particleAppearance = particleAppearance;
    _needsRestart = true;
}

- (void)setParticlePhysics:(NSDictionary *)particlePhysics {
    _particlePhysics = particlePhysics;
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
    if (!self.driver || !self.node || !self.scene) {
        return;
    }
    
    if (!self.image || ![self.image objectForKey:@"source"]){
        RCTLogError(@"Viro: Missing required Image for a Viro Particle Emitter!");
        return;
    }

    // If the image or image size has changed, recreate the emitter.
    [self downloadImageIfNeeded];

    // If a particle source image is provided, wait for it to be loaded
    // into the texture before initializing the emitter with it.
    if (_particleTexture == nullptr) {
        return;
    }
    
    // Refresh the images set on quad particles if needed.
    [self refreshImageOnParticle];
    
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

- (void)downloadImageIfNeeded {
    // Schedule image for download if given.
    if (![self.image objectForKey:@"source"]){
        return;
    }
    
    // Return if the image for this Image emitter has not changed.
    NSString *nsStringImageSource = [self.image objectForKey:@"source"][@"uri"];
    if ((_currentImageSource && nsStringImageSource && [_currentImageSource isEqualToString:nsStringImageSource])
        || (!_currentImageSource && !nsStringImageSource)) {
        return;
    }
    _currentImageSource = nsStringImageSource;

    // Else, start the download with the newly provided image.
    _particleTexture = nullptr;
    if (nsStringImageSource != nil) {
        [_loader loadImage:[RCTConvert RCTImageSource:nsStringImageSource]];
    } else {
        [_loader cancel];
        _particleTexture = nullptr;
    }
}

-(void)refreshImageOnParticle {
    if (!_needsImageUpdate){
        return;
    }
    NSNumber *width = [self.image objectForKey:@"width"];
    NSNumber *height = [self.image objectForKey:@"height"];
    float fwidth = width ? [width floatValue] : 1.0;
    float fheight = height ? [height floatValue] : 1.0;
    _particleGeometry->setWidth(fwidth);
    _particleGeometry->setHeight(fheight);
    
    // Create a VROSurface, representing the particle geometry template for this emitter.
    std::shared_ptr<VROMaterial> material = _particleGeometry->getMaterials()[0];
    // Apply the particle texture, if given.
    if (_particleTexture) {
        material->getDiffuse().setTexture(_particleTexture);
    }

    if ([self.image objectForKey:@"bloomThreshold"]){
        float threshold = [[self.image objectForKey:@"bloomThreshold"] doubleValue];
        _particleGeometry->getMaterials()[0]->setBloomThreshold(threshold);
    } else {
        _particleGeometry->getMaterials()[0]->setBloomThreshold(-1.0f);
    }

    _particleGeometry->getMaterials()[0]->updateSubstrate();
    _needsImageUpdate = false;
}

-(void)createEmitter {
    _emitter = std::make_shared<VROParticleEmitter>(self.driver, _particleGeometry);
    self.node->setParticleEmitter(_emitter);
}

-(void)removeEmitter {
    if (_emitter == nullptr) {
        return;
    }

    self.node->removeParticleEmitter();
    _particleGeometry = nullptr;
    _particleTexture = nullptr;
    _emitter = nullptr;
}

- (void)updateEmitterState {
    _emitter->setDelay(self.delay);
    _emitter->setFixedToEmitter(self.fixedToEmitter);
    _emitter->setDuration(self.duration);
    _emitter->setLoop(self.loop);
    _emitter->setRun(self.run);
    
    if ([self.image objectForKey:@"blendMode"]){
        std::string stringFactor;
        stringFactor = std::string([[self.image objectForKey:@"blendMode"] UTF8String]);
        VROBlendMode mode = VROMaterial::getBlendModeFromString(stringFactor);
        if (mode == VROBlendMode::None){
            RCTLogError(@"Viro: Attempted to set an invalid Blend mode!");
            return;
        }
        _emitter->setBlendMode(mode);
    } else {
        _emitter->setBlendMode(VROBlendMode::Add);
    }
}

- (void)updateSpawnModifier {
    if (!_spawnBehavior) {
        _emitter->setEmissionRatePerSecond(std::pair<int, int>(kDefaultSpawnRatePerSec, kDefaultSpawnRatePerSec));
        _emitter->setEmissionRatePerDistance(std::pair<int, int>(kDefaultSpawnRatePerMeter, kDefaultSpawnRatePerMeter));
        _emitter->setParticleLifeTime(std::pair<int, int>(kDefaultParticleLifetime, kDefaultParticleLifetime));
        _emitter->setMaxParticles(kDefaultMaxParticles);
        _emitter->setParticleBursts(std::vector<VROParticleEmitter::VROParticleBurst>());

        VROParticleSpawnVolume defaultVol;
        defaultVol.shape =  VROParticleSpawnVolume::Shape::Point;
        _emitter->setParticleSpawnVolume(defaultVol);
        return;
    }

    NSArray *emissionRatePerSec = [_spawnBehavior objectForKey:@"emissionRatePerSecond"];
    if (emissionRatePerSec && [emissionRatePerSec count] == 2) {
        std::pair<int, int> range = std::pair<int, int>([[emissionRatePerSec objectAtIndex:0] intValue],
                                                        [[emissionRatePerSec objectAtIndex:1] intValue]);
        _emitter->setEmissionRatePerSecond(range);
    } else {
        _emitter->setEmissionRatePerSecond(std::pair<int, int>(kDefaultSpawnRatePerSec, kDefaultSpawnRatePerSec));
    }

    NSArray *emissionRatePerMeter = [_spawnBehavior objectForKey:@"emissionRatePerMeter"];
    if (emissionRatePerMeter && [emissionRatePerMeter count] == 2){
        std::pair<int, int> range = std::pair<int, int>([[emissionRatePerMeter objectAtIndex:0] intValue],
                                                        [[emissionRatePerMeter objectAtIndex:1] intValue]);
        _emitter->setEmissionRatePerDistance(range);
    } else {
        _emitter->setEmissionRatePerDistance(std::pair<int, int>(kDefaultSpawnRatePerMeter, kDefaultSpawnRatePerMeter));
    }

    NSArray *particleLifeTime = [_spawnBehavior objectForKey:@"particleLifetime"];
    if (particleLifeTime && [particleLifeTime count] == 2) {
        std::pair<int, int> range = std::pair<int, int>([[particleLifeTime objectAtIndex:0] intValue],
                                                        [[particleLifeTime objectAtIndex:1] intValue]);
        _emitter->setParticleLifeTime(range);
    } else {
        _emitter->setParticleLifeTime(std::pair<int, int>(kDefaultParticleLifetime, kDefaultParticleLifetime));
    }

    if ([_spawnBehavior objectForKey:@"maxParticles"]) {
        int maxParticles = [[_spawnBehavior objectForKey:@"maxParticles"] intValue];
        _emitter->setMaxParticles(maxParticles);
    } else {
        _emitter->setMaxParticles(kDefaultMaxParticles);
    }

    if ([_spawnBehavior objectForKey:@"emissionBurst"]) {
        std::vector<VROParticleEmitter::VROParticleBurst> bursts;
        NSArray *burstArray = [_spawnBehavior objectForKey:@"emissionBurst"];

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
                RCTLogError(@"Viro: Unsupported interpolation factor provided! Must specify either time or distance.");
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

    if ([_spawnBehavior objectForKey:@"spawnVolume"]) {
        NSDictionary *spawnVolume = [_spawnBehavior objectForKey:@"spawnVolume"];
        NSString *stringShapeName = [spawnVolume objectForKey:@"shape"];
        NSArray *shapeParams = [spawnVolume objectForKey:@"params"];

        if (stringShapeName == nil) {
            RCTLogError(@"Viro: Missing required volume shape type, skipping spawn volume interpolation data.");
            return;
        }

        VROParticleSpawnVolume vol;
        if ([stringShapeName caseInsensitiveCompare:@"Box"] == NSOrderedSame) {
            vol.shape = VROParticleSpawnVolume::Shape::Box;
        } else if ([stringShapeName caseInsensitiveCompare:@"Sphere"] == NSOrderedSame) {
            vol.shape = VROParticleSpawnVolume::Shape::Sphere;
        } else {
            RCTLogError(@"Viro: Incorrect spawn volume shape type. Must be either sphere or box.");
            return;
        }

        // Grab the current shapeParams
        if (shapeParams) {
            std::vector<float> params = {};
            for (int i = 0; i < [shapeParams count]; i ++) {
                float value = [[shapeParams objectAtIndex:i] floatValue];
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
    if (!_particleAppearance) {
        _emitter->setAlphaModifier(_defaultAlphaModifier);
        _emitter->setScaleModifier(_defaultScaleModifier);
        _emitter->setRotationModifier(_defaultRotationModifier);
        _emitter->setColorModifier(_defaultColorModifier);
        return;
    }

    std::shared_ptr<VROParticleModifier> alphaModifier = [self getModifier:[_particleAppearance objectForKey:@"opacity"]
                                                                    isVec3:false
                                                                   isColor:false
                                                                   isFloat:true];
    
    std::shared_ptr<VROParticleModifier> scaleModifier = [self getModifier:[_particleAppearance objectForKey:@"scale"]
                                                                    isVec3:true
                                                                   isColor:false
                                                                   isFloat:false];
    
    std::shared_ptr<VROParticleModifier> rotationModifier = [self getModifier:[_particleAppearance objectForKey:@"rotation"]
                                                                    isVec3:true
                                                                   isColor:false
                                                                   isFloat:false];
    
    std::shared_ptr<VROParticleModifier> colorModifier = [self getModifier:[_particleAppearance objectForKey:@"color"]
                                                                    isVec3:false
                                                                   isColor:true
                                                                   isFloat:false];
    
    _emitter->setAlphaModifier(alphaModifier != nullptr ? alphaModifier : _defaultAlphaModifier);
    _emitter->setScaleModifier(scaleModifier != nullptr ? scaleModifier : _defaultScaleModifier);
    _emitter->setRotationModifier(rotationModifier != nullptr ? rotationModifier : _defaultRotationModifier);
    _emitter->setColorModifier(colorModifier != nullptr ? colorModifier : _defaultColorModifier);
}

-(void)updatePhysicsModifier{
    if (!_particlePhysics) {
        _emitter->setVelocityModifier(_defaultVelocityModifier);
        _emitter->setAccelerationmodifier(_defaultAccelerationModifier);
        _emitter->setInitialExplosion(VROVector3f(0,0,0), -1);
        return;
    }
    
    std::shared_ptr<VROParticleModifier> velocityMod = [self getModifier:[_particlePhysics objectForKey:@"velocity"]
                                                                       isVec3:true
                                                                      isColor:false
                                                                      isFloat:false];
    
    std::shared_ptr<VROParticleModifier> accelMod = [self getModifier:[_particlePhysics objectForKey:@"acceleration"]
                                                                       isVec3:true
                                                                      isColor:false
                                                                      isFloat:false];
    
    _emitter->setVelocityModifier(velocityMod != nullptr ? velocityMod : _defaultVelocityModifier);
    _emitter->setAccelerationmodifier(accelMod != nullptr ? accelMod : _defaultAccelerationModifier);

    if ([_particlePhysics objectForKey:@"explosiveImpulse"]) {
        NSDictionary *explosiveDict = [_particlePhysics objectForKey:@"explosiveImpulse"];
        NSArray *position = [explosiveDict objectForKey:@"position"];
        if ([position count] < 3) {
            RCTLogError(@"Incorrect parameters provided explosion impulse");
            return;
        }

        VROVector3f explodedAt = VROVector3f([[position objectAtIndex:0] floatValue],
                                             [[position objectAtIndex:1] floatValue],
                                             [[position objectAtIndex:2] floatValue]);
        float impulse = [[explosiveDict objectForKey:@"impulse"] floatValue];
        float reverseAccel = [[explosiveDict objectForKey:@"decelerationPeriod"] floatValue];
        if (![explosiveDict objectForKey:@"decelerationPeriod"]){
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
    
    NSArray *intervalsDictArray = [modDictionary objectForKey:@"interpolation"];
    VROParticleModifier::VROModifierFactor factor = VROParticleModifier::VROModifierFactor::Time;

    // Grab the VROModifierFactor this modifier is interpolating against.
    if ([modDictionary objectForKey:@"factor"]) {
        std::string stringFactor;
        stringFactor = std::string([[modDictionary objectForKey:@"factor"] UTF8String]);
        if (VROStringUtil::strcmpinsensitive(stringFactor, "Distance")) {
            factor = VROParticleModifier::VROModifierFactor::Distance;
        } else if (!VROStringUtil::strcmpinsensitive(stringFactor, "Time") && intervalsDictArray && [intervalsDictArray count] > 0) {
            RCTLogError(@"Viro: Provided unknown interpolation factor to interpolate against!");
            return nullptr;
        }
    }

    // Parse through the array of interval modifiers.
    std::vector<VROParticleModifier::VROModifierInterval> intervals;
    for (int i = 0; i < [intervalsDictArray count]; i ++) {
        // Ensure we have a valid interval configuration
        NSDictionary *intervalDict = [intervalsDictArray objectAtIndex:i];
        if (![intervalDict objectForKey:@"interval"] || ![intervalDict objectForKey:@"endValue"]) {
            RCTLogError(@"Viro: Incorrectly configured interpolation data, skipping interpolation.");
            continue;
        }

        // Grab the values representing the interval data, for example scale / velocity. If we
        // fail to grab it, skip to the next modifier.
        VROParticleModifier::VROModifierInterval interval;
        if (![self getVecValueFromDict:intervalDict
                               withKey:@"endValue"
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
    std::pair<VROVector3f, VROVector3f> initialVecRange;
    bool hasValue = [self getVecValueFromTuple:modDictionary withKey:@"initialRange"
                                       isVec3:isVec3 isColor:isColor isFloat:isFloat outVec:initialVecRange];

    if (!hasValue) {
        return nullptr;
    }

    return std::make_shared<VROParticleModifier>(initialVecRange.first, initialVecRange.second, factor, intervals);
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

- (bool)getVecValueFromTuple:(NSDictionary *)dict
                    withKey:(NSString *)key
                     isVec3:(bool)isVec3
                    isColor:(bool)isColor
                    isFloat:(bool)isFloat
                      outVec:(std::pair<VROVector3f, VROVector3f> &)vec{
    
    if (![dict objectForKey:key]) {
        RCTLogError(@"Incorrect parameters provided for tag %@!", key);
        return false;
    }
    
    NSArray *tupeValues = [dict objectForKey:key];
    if ([tupeValues count] != 2) {
        RCTLogError(@"Incorrect parameters provided for %@, expected: [min, max]!", key);
        return false;
    }
    
    if (isFloat) {
        float value1 =[[tupeValues objectAtIndex:0] floatValue];
        float value2 =[[tupeValues objectAtIndex:1] floatValue];
        vec = std::pair<VROVector3f, VROVector3f>(VROVector3f(value1,0,0), VROVector3f(value2,0,0));
        return true;
    }
    
    if (isColor) {
        UIColor *color1 =[RCTConvert UIColor:[tupeValues objectAtIndex:0]];
        UIColor *color2 =[RCTConvert UIColor:[tupeValues objectAtIndex:1]];
        
        if (!color1 || !color2) {
            return false;
        }
        
        CGFloat red,green,blue, alpha;
        [color1 getRed:&red green:&green blue:&blue alpha:&alpha];
        
        CGFloat red2,green2,blue2, alpha2;
        [color2 getRed:&red2 green:&green2 blue:&blue2 alpha:&alpha2];
        
        vec = std::pair<VROVector3f, VROVector3f>(VROVector3f(red,green,blue),
                                                  VROVector3f(red2,green2,blue2));
        return true;
    }
    
    if (isVec3) {
        
        NSArray *valueVec1 =[tupeValues objectAtIndex:0];
        NSArray *valueVec2 =[tupeValues objectAtIndex:1];
        
        if ([valueVec1 count] < 3 || [valueVec2 count] < 3) {
            RCTLogError(@"Incorrect parameters provided for %@, expected: [x, y, z]!", key);
            return false;
        }
        
        VROVector3f vec1 = VROVector3f([[valueVec1 objectAtIndex:0] floatValue],
                                      [[valueVec1 objectAtIndex:1] floatValue],
                                      [[valueVec1 objectAtIndex:2] floatValue]);
        
        VROVector3f vec2 = VROVector3f([[valueVec2 objectAtIndex:0] floatValue],
                                       [[valueVec2 objectAtIndex:1] floatValue],
                                       [[valueVec2 objectAtIndex:2] floatValue]);
        vec = std::pair<VROVector3f, VROVector3f>(vec1, vec2);

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
            _particleTexture = std::make_shared<VROTexture>(true,
                                                            VROMipmapMode::Runtime,
                                                            std::make_shared<VROImageiOS>(image, VROTextureInternalFormat::RGBA8),
                                                            VROStereoMode::None);
            _needsImageUpdate = true;
            [self updateEmitter];
        } else {
            perror("Viro: Error loading particle image resource");
        }
    });
}

@end
