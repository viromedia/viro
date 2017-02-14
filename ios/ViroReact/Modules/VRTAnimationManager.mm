//
//  VRTAnimationManager.m
//  React
//
//  Created by Vik Advani on 2/17/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTAnimationManager.h"
#import "VRTMaterialManager.h"
#import <ViroKit/ViroKit.h>
#import "RCTLog.h"

class VROLazyMaterialReact : public VROLazyMaterial {
public:
    
    VROLazyMaterialReact(NSString *materialName, VRTMaterialManager *materialManager) :
        _materialName(materialName),
        _materialManager(materialManager) {}
    
    std::shared_ptr<VROMaterial> get() {
        if (_materialManager) {
            return [_materialManager getMaterialByName:_materialName];
        }
        else {
            return nullptr;
        }
    }
    
private:
    
    NSString *_materialName;
    __weak VRTMaterialManager *_materialManager;
    
};

@interface VRTAnimationManager ()

@end

@implementation VRTAnimationManager {
    
    std::map<std::string, std::shared_ptr<VROExecutableAnimation>> _parsedAnimations;
    
}

@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()

RCT_EXPORT_METHOD(setJSAnimations:(NSDictionary *)animationsDict)
{
    self.animations = animationsDict;
    [self parseAnimations];
}

- (instancetype)init {
    self = [super init];
    return self;
}

- (std::shared_ptr<VROExecutableAnimation>)animationForName:(NSString *)name {
    auto kv = _parsedAnimations.find(std::string([name UTF8String]));
    return kv == _parsedAnimations.end() ? nullptr : kv->second;
}

/*
 Parses the animations from self.animations into VROExecutableAnimations, stored
 in _parsedAnimations.
 */
- (void)parseAnimations {
    for (id key in self.animations) {
        std::string name = std::string([(NSString *)key UTF8String]);
        NSObject *animationObj = [self.animations objectForKey:key];
        
        std::shared_ptr<VROExecutableAnimation> animation = [self parseAnimationObject:animationObj
                                                                             execution:VROAnimationChainExecution::Parallel];
        _parsedAnimations[name] = animation;
        NSLog(@"Parsed animation %s", animation->toString().c_str());
    }
}

/*
 Recursive parsing function for building executable animations from the animation 
 dictionary. Animations can be specified in three different ways:
 
 1. String: a name, referring to another animation in self.animations
 2. Dictionary: a fully-specified VROAnimationGroup
 3. Array: a chain, referring to other animation objects (be they named, groups, or chains)
 
 The VROAnimationChainExecution value controls how animations are executed when an
 array is encountered, serially or in parallel. Animations are only executed in parallel
 on the first recursion; afterward, all arrays are treated serially.
 
 For example:
 
 rotateAndMovePicture:[
        ["moveRight"],
        ["rotate", "moveLeft"],
 ]
 
 On the first recursion, VROAnimationChainExecution is Parallel, so ["moveRight"] runs
 in parallel with ["rotate", "moveLeft"]. On the second recursion, execution is serial,
 so "rotate" runs serially with "moveLeft".
 */
-(std::shared_ptr<VROExecutableAnimation>)parseAnimationObject:(id)animationObj execution:(VROAnimationChainExecution)execution {
    if ([animationObj isKindOfClass:[NSString class]]) {
        NSString *name = (NSString *)animationObj;
        return [self parseAnimationObject:[self.animations objectForKey:name]
                                execution:VROAnimationChainExecution::Serial];
    }
    else if ([animationObj isKindOfClass:[NSDictionary class]]) {
        return [self parseAnimationGroup:animationObj];
    }
    else if ([animationObj isKindOfClass:[NSArray class]]) {
        std::vector<std::shared_ptr<VROExecutableAnimation>> chain;
        for (id child : (NSArray *)animationObj) {
            chain.push_back([self parseAnimationObject:child
                                             execution:VROAnimationChainExecution::Serial]);
        }
        
        return std::make_shared<VROAnimationChain>(chain, execution);
    }
    else {
        RCTLogError(@"Invalid animation object received %@", animationObj);
        return {};
    }
}

-(std::shared_ptr<VROAnimationGroup>)parseAnimationGroup:(NSDictionary *)animationDictionary {
    NSDictionary *propertyDict = animationDictionary[@"properties"];
    
    std::map<std::string, std::string> propertyAnimations;
    [self populateValue:@"positionX" properties:propertyDict inMap:propertyAnimations];
    [self populateValue:@"positionY" properties:propertyDict inMap:propertyAnimations];
    [self populateValue:@"positionZ" properties:propertyDict inMap:propertyAnimations];
    [self populateValue:@"scaleX" properties:propertyDict inMap:propertyAnimations];
    [self populateValue:@"scaleY" properties:propertyDict inMap:propertyAnimations];
    [self populateValue:@"scaleZ" properties:propertyDict inMap:propertyAnimations];
    [self populateValue:@"rotateX" properties:propertyDict inMap:propertyAnimations];
    [self populateValue:@"rotateY" properties:propertyDict inMap:propertyAnimations];
    [self populateValue:@"rotateZ" properties:propertyDict inMap:propertyAnimations];
    [self populateValue:@"opacity" properties:propertyDict inMap:propertyAnimations];
    [self populateValue:@"color" properties:propertyDict inMap:propertyAnimations];
    [self populateValue:@"material" properties:propertyDict inMap:propertyAnimations];
    
    VRTMaterialManager *materialManager = [self.bridge moduleForClass:[VRTMaterialManager class]];
    std::vector<std::shared_ptr<VROLazyMaterial>> materialAnimations;
    
    // Currently we only support animating the index 0 material.
    NSString *material = propertyDict[@"material"];
    if (material != nil) {
        materialAnimations.push_back(std::make_shared<VROLazyMaterialReact>(material, materialManager));
    }

    NSNumber *durationMilliseconds = animationDictionary[@"duration"];
    NSNumber *delayMilliseconds = animationDictionary[@"delay"];
    
    NSString *functionType = animationDictionary[@"easing"];
    if (!functionType) {
        functionType = @"Linear";
    }
    
    float durationInSeconds = [durationMilliseconds floatValue] / 1000.0;
    float delayInSeconds = [delayMilliseconds floatValue] / 1000.0;
    return VROAnimationGroup::parse(durationInSeconds, delayInSeconds,
                                    std::string([functionType UTF8String]),
                                    propertyAnimations, materialAnimations);
}

- (void)populateValue:(NSString *)propertyName properties:(NSDictionary *)propertyDictionary
                inMap:(std::map<std::string, std::string> &)map {
    
    id value = propertyDictionary[propertyName];
    if (value == nil) {
        return;
    }

    if ([value isKindOfClass:[NSString class]]) {
        map[std::string([propertyName UTF8String])] = std::string([value UTF8String]);
    }
    else if ([value isKindOfClass:[NSNumber class]]) {
        map[std::string([propertyName UTF8String])] = std::string([[value stringValue] UTF8String]);
    }
}

@end
