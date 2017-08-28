//
//  VRT3DObject.m
//  React
//
//  Created by Vik Advani on 4/14/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTConvert.h>
#import "VRT3DObject.h"
#import "VRTMaterialManager.h"
#import "VRTUtils.h"
#import "VRTManagedAnimation.h"

@interface VRT3DObjectAnimation : VRTManagedAnimation

@property (readwrite, nonatomic) std::string animationKey;

- (std::shared_ptr<VROExecutableAnimation>)loadAnimation;

@end

@implementation VRT3DObjectAnimation

- (std::shared_ptr<VROExecutableAnimation>)loadAnimation {
    std::shared_ptr<VRONode> node = self.node.lock();
    if (!node) {
        return nullptr;
    }
    
    if (!self.animationKey.empty()) {
        return node->getAnimation(self.animationKey, true);
    }
    else {
        return nullptr;
    }
}

- (void)setAnimationKey:(std::string)key {
    _animationKey = key;
    [super updateAnimation];
}

@end

@interface VRT3DObject ()

@property (readwrite, nonatomic) NSString *animationName;
@property (readwrite, nonatomic) VRT3DObjectAnimation *managedAnimation;

@end

@implementation VRT3DObject {
    
    NSURL *_url;
    std::shared_ptr<VROMaterial> _objMaterial;
    BOOL _sourceChanged;
    
}

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
    self = [super initWithBridge:bridge];
    _sourceChanged = NO;
    self.managedAnimation = [[VRT3DObjectAnimation alloc] init];
    self.managedAnimation.node = self.node;
    
    return self;
}

- (void)setSource:(NSDictionary *)source {
    _source = source;
    _sourceChanged = YES;
}

- (void)updateAnimation {
    /*
     Get all the animations loaded from the object.
     */
    std::set<std::string> animationKeys = self.node->getAnimationKeys(true);
    if (animationKeys.empty()) {
        return;
    }
    
    /*
     If an animation to run was specified (animation.name), then run that animation;
     otherwise just run the first animation.
     */
    std::string key;
    if (self.animationName == nil) {
        key = *animationKeys.begin();
    }
    else {
        auto it = animationKeys.find(std::string([self.animationName UTF8String]));
        if (it != animationKeys.end()) {
            key = *it;
        }
        else {
            RCTLogWarn(@"Animation %@ cannot be run: was not found on object!", self.animationName);
        }
    }
    self.managedAnimation.animationKey = key;
}

- (void)setAnimation:(NSDictionary *)animation {
    [self.managedAnimation parseFromDictionary:animation];
    self.animationName = [animation objectForKey:@"name"];
    [self updateAnimation];
}

- (void)setOnAnimationStartViro:(RCTDirectEventBlock)onAnimationStartViro {
    self.managedAnimation.onStart = onAnimationStartViro;
}

- (void)setOnAnimationFinishViro:(RCTDirectEventBlock)onAnimationFinishViro {
    self.managedAnimation.onFinish = onAnimationFinishViro;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if (![NSThread isMainThread]) {
        RCTLogWarn(@"Calling [RCTConvert didSetProps:] on a background thread is not recommended");
        dispatch_sync(dispatch_get_main_queue(), ^{
            [self didSetProps:changedProps];
        });
        
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
    
    BOOL isOBJ = NO;
    std::string urlPath = std::string([[_url path] UTF8String]);
    VROStringUtil::toLowerCase(urlPath);
    if (VROStringUtil::endsWith(urlPath, "obj")) {
        isOBJ = YES;
    }
    
    std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish =
    [self](std::shared_ptr<VRONode> node, bool success) {
        if (success) {
            // The geometry is set for OBJ models
            self.node->setGeometry(node->getGeometry());
            
            // The children are set for FBX models (in FBX, the root node is a dummy node)
            for (std::shared_ptr<VRONode> child : node->getChildNodes()) {
                self.node->addChildNode(child);
            }
            
            if (self.materials) {
                [self applyMaterials];
            }
            [self updateAnimation];
        }
        
        // TODO: remove this once we do shadows better!
        [self setSelfShadowProps:node];
        
        if (self.onLoadEndViro) {
            self.onLoadEndViro(nil);
        }
        if (!success) {
            if (self.onErrorViro) {
                self.onErrorViro(@{ @"error": @"model failed to load" });
            }
        }
    };
    
    if (isOBJ) {
        VROOBJLoader::loadOBJFromURL(url, base, true, onFinish);
    }
    else {
        VROFBXLoader::loadFBXFromURL(url, base, true, onFinish);
    }
    _sourceChanged = NO;
}

/*
 This function sets this object's shadow props on the given vroNode and its
 children recursively. This is needed because the shadow props aren't currently
 inheirited and so to make shadows appear for FBX objects which can define multiple
 nodes, we need to recursively go down the node tree and set the shadow properties.
 TODO: remove this once the shadow properties are inheirited. 
 */
- (void)setSelfShadowProps:(std::shared_ptr<VRONode>)vroNode {
    vroNode->setShadowCastingBitMask([self shadowCastingBitMask]);
    vroNode->setLightBitMask([self lightBitMask]);
    for (std::shared_ptr<VRONode> child : vroNode->getChildNodes()) {
        [self setSelfShadowProps:child];
    }
}

- (void)setLightBitMask:(int)lightBitMask {
    [super setLightBitMask:lightBitMask];
    [self setSelfShadowProps:[self node]];
}

- (void)setShadowCastingBitMask:(int)shadowCastingBitMask {
    [super setShadowCastingBitMask:shadowCastingBitMask];
    [self setSelfShadowProps:[self node]];
}

@end
