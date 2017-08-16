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

@implementation VRTQuadEmitter {
    VRTImageAsyncLoader *_loader;
    std::shared_ptr<VROParticleEmitter> _emitter;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _loader = [[VRTImageAsyncLoader alloc] initWithDelegate:self];
    }
    return self;
}

- (void)setQuad:(NSDictionary *)quad {
    _quad = quad;
}

- (void)setSpawnModifier:(NSDictionary *)spawnModifier {
    _spawnModifier = spawnModifier;
}

- (void)setAppearanceModifier:(NSDictionary *)appearanceModifier {
    _appearanceModifier = appearanceModifier;
}

- (void)setPhysicsModifier:(NSDictionary *)physicsModifier {
    _physicsModifier = physicsModifier;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    [self updateEmitter];
}

- (void)handleAppearanceChange {
    if ([self shouldAppear]) {
        [self updateEmitter];
    }
}

- (void)updateEmitter {
    if (self.driver && self.node && self.quad && self.scene) {
        RCTImageSource *source = [RCTConvert RCTImageSource:[self.quad objectForKey:@"source"]];
        if (source) {
            [_loader loadImage:source];
        }
    }
}

#pragma mark VRTImageAsyncLoaderEventDelegate
- (void)imageLoaderDidStart:(VRTImageAsyncLoader *)loader {
    // no-op
}

- (void)imageLoaderDidEnd:(VRTImageAsyncLoader *)loader success:(BOOL)success image:(UIImage *)image {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.driver && self.node && self.quad && self.scene) {
            NSNumber *width = [self.quad objectForKey:@"width"];
            NSNumber *height = [self.quad objectForKey:@"height"];
            float fwidth = width ? [width floatValue] : 1.0;
            float fheight = height ? [height floatValue] : 1.0;
            std::shared_ptr<VROTexture> texture = std::make_shared<VROTexture>(VROTextureInternalFormat::RGBA8,
                                                                               VROMipmapMode::Runtime,
                                                                               std::make_shared<VROImageiOS>(image, VROTextureInternalFormat::RGBA8),
                                                                               VROStereoMode::None);
            _emitter = std::make_shared<VROParticleEmitter>(self.driver, self.node, texture, fwidth, fheight);
            self.scene->addParticleEmitter(_emitter);
        }
    });
}

@end
