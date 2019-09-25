//
//  VRT3DObject.m
//  React
//
//  Created by Vik Advani on 4/14/16.
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

#import <React/RCTConvert.h>
#import "VRT3DObject.h"
#import "VRTMaterialManager.h"
#import "VRTUtils.h"
#import "VRTAnimationManager.h"

@interface VRT3DObjectAnimation : VRTNodeAnimation

@end

@implementation VRT3DObjectAnimation

- (std::shared_ptr<VROExecutableAnimation>)loadAnimation {
    std::shared_ptr<VRONode> node = self.node.lock();
    if (!node) {
        return [super loadAnimation];
    }
    std::set<std::string> animationKeys = node->getAnimationKeys(true);
    if (animationKeys.empty()) {
        return [super loadAnimation];
    }
    
    if (self.animationName) {
        std::string key = std::string([self.animationName UTF8String]);
        
        auto it = animationKeys.find(key);
        if (it != animationKeys.end()) {
            return node->getAnimation(key, true);
        }
        else {
            return [super loadAnimation];
        }
    }
    else {
        return [super loadAnimation];
    }
}

@end

@interface VRT3DObject ()

@end

@implementation VRT3DObject {
    
    NSURL *_url;
    std::shared_ptr<VROMaterial> _objMaterial;
    BOOL _sourceChanged;
    BOOL _modelLoaded;

}

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
    self = [super initWithBridge:bridge];
    _sourceChanged = NO;
    _modelLoaded = NO;
    self.nodeAnimation = [[VRT3DObjectAnimation alloc] init];
    self.nodeAnimation.animationManager = [bridge animationManager];
    self.nodeAnimation.node = self.node;
    
    return self;
}

- (void)setDriver:(std::shared_ptr<VRODriver>)driver {
    [super setDriver:driver];
    [self didSetProps:nil];
}

- (void)setSource:(NSDictionary *)source {
    _source = source;
    _sourceChanged = YES;
}

- (void)setMorphTargets:(NSArray *)morphTargets {
    _morphTargets = morphTargets;
    if (!_modelLoaded){
        return;
    }
    
    for (NSDictionary *target in morphTargets) {
        // Grab the target key and values
        NSObject *targetObject = [target objectForKey:@"target"];
        if (targetObject == NULL){
            RCTLogWarn(@"Incorrectly configured Morph Targets.");
            return;
        }
        NSString *key = (NSString *) targetObject;
        float value = [[target objectForKey:@"weight"] floatValue];
        std::string targetStr = std::string([key UTF8String]);
        
        std::set<std::shared_ptr<VROMorpher>> morphers = self.node->getMorphers(true);
        for (auto morph : morphers) {
            morph->setWeightForTarget(targetStr, value);
        }
    }
}

- (void)updateAnimation {
    /*
     If no animation name was specified, then use the first keyframe animation,
     if available.
     */
    if (!self.nodeAnimation.animationName || self.nodeAnimation.animationName.length == 0) {
        std::set<std::string> animationKeys = self.node->getAnimationKeys(true);
        if (!animationKeys.empty()) {
            self.nodeAnimation.animationName = [NSString stringWithUTF8String:animationKeys.begin()->c_str()];
        }
    }
    
    [self.nodeAnimation updateAnimation];
}

- (void)setAnimation:(NSDictionary *)animation {
    [super setAnimation:animation];
    [self updateAnimation];
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if (![NSThread isMainThread]) {
        RCTLogWarn(@"Calling [RCTConvert didSetProps:] on a background thread is not recommended");
        dispatch_sync(dispatch_get_main_queue(), ^{
            [self didSetProps:changedProps];
        });
        
        return;
    }
    
    // Wait until we have the driver
    if (!self.driver) {
        return;
    }
    
    // Only reload the model if its source changed
    if (!_sourceChanged) {
        return;
    }
    
    NSString *path;
    if (!(path = [RCTConvert NSString:self.source[@"uri"]])) {
        RCTLogError(@"Unable to load 3D model object with no path");
    }
    
    _url = [RCTConvert NSURL:path];
    std::string url = std::string([[_url description] UTF8String]);
    std::string base = url.substr(0, url.find_last_of('/'));
    
    if (self.onLoadStartViro) {
        self.onLoadStartViro(nil);
    }
    
    if (!_type) {
        RCTLogError(@"`type` property not set on Viro3DObject.");
        return;
    }
    
    // Clear all child nodes of this control before loading our 3D models
    for (std::shared_ptr<VRONode> child : self.node->getChildNodes()) {
        child->removeFromParentNode();
    }
    _modelLoaded = NO;
    __weak VRT3DObject *weakSelf = self;
    std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish =
    [weakSelf](std::shared_ptr<VRONode> node, bool success) {
        VRT3DObject *strongSelf = weakSelf;
        if (success && strongSelf) {
            strongSelf->_modelLoaded = YES;
            [strongSelf setMorphTargets:strongSelf->_morphTargets];
            
            if (strongSelf.materials) {
                [strongSelf applyMaterials];
            }
            
            [weakSelf updateAnimation];
        }

        /*
         Once the object is loaded, set the lighting bit masks recursively
         down the tree to the internal FBX nodes.
         */
        if (node != nullptr) {
            node->setLightReceivingBitMask([weakSelf lightReceivingBitMask], true);
            node->setShadowCastingBitMask([weakSelf shadowCastingBitMask], true);
        }
        
        if (weakSelf && weakSelf.onLoadEndViro) {
            weakSelf.onLoadEndViro(nil);
        }

        if (!success) {
            if (weakSelf && weakSelf.onErrorViro) {
                weakSelf.onErrorViro(@{ @"error": @"model failed to load" });
            }
        }
    };

    if ([_type caseInsensitiveCompare:@"OBJ"] == NSOrderedSame) {
        VROOBJLoader::loadOBJFromResource(url, VROResourceType::URL, self.node, self.driver, onFinish);
    } else if ([_type caseInsensitiveCompare:@"VRX"] == NSOrderedSame) {
        VROFBXLoader::loadFBXFromResource(url, VROResourceType::URL, self.node, self.driver, onFinish);
    } else if ([_type caseInsensitiveCompare:@"GLTF"] == NSOrderedSame) {
        VROGLTFLoader::loadGLTFFromResource(url, {},  VROResourceType::URL, self.node, false, self.driver, onFinish);
    } else if ([_type caseInsensitiveCompare:@"GLB"] == NSOrderedSame) {
        VROGLTFLoader::loadGLTFFromResource(url, {},  VROResourceType::URL, self.node, true, self.driver, onFinish);
    } else {
        self.onErrorViro(@{ @"error": @"model failed to load" });
    }
    _sourceChanged = NO;
}

/*
 Set the bit masks recursively for 3D objects because they may have internal
 (FBX) nodes.
 */
- (void)setLightReceivingBitMask:(int)lightReceivingBitMask {
    [super setLightReceivingBitMask:lightReceivingBitMask];
    [self node]->setLightReceivingBitMask(lightReceivingBitMask, true);
}

- (void)setShadowCastingBitMask:(int)shadowCastingBitMask {
    [super setShadowCastingBitMask:shadowCastingBitMask];
    [self node]->setShadowCastingBitMask(shadowCastingBitMask, true);
}

@end
