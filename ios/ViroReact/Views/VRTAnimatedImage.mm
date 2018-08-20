//
//  VRTAnimatedImage.m
//  React
//
//  Copyright © 2018 Viro Media. All rights reserved.
//
#import "VRTAnimatedImage.h"
#import "VRTMaterialManager.h"

@implementation VRTAnimatedImage {
}

static float const kDefaultWidth = 1;
static float const kDefaultHeight = 1;
-(instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _currentPendingImage = 0;
        _paused = false;
        _loop = true;
    }

    return self;
}

- (void)setPaused:(BOOL)paused {
    _paused = paused;
    if (self.texture) {
        std::shared_ptr<VROAnimatedTextureOpenGL> animTexture = std::dynamic_pointer_cast<VROAnimatedTextureOpenGL>(self.texture);
        (_paused || ![self shouldAppear]) ? animTexture->pause() : animTexture->play();
    }
}

- (void)setLoop:(BOOL)loop {
    _loop = loop;
    if (self.texture) {
        std::shared_ptr<VROAnimatedTextureOpenGL> animTexture = std::dynamic_pointer_cast<VROAnimatedTextureOpenGL>(self.texture);
        animTexture->setLoop(loop);
    }
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    [self updatePlaceholderProps:changedProps];
    [self loadAnimatedTexture];
}

-(void)loadAnimatedTexture {
    if (!self.imageNeedsDownload) {
        return;
    }

    if (!self.source || !self.driver || !self.context) {
        return;
    }

    if(self.onLoadStartViro) {
        self.onLoadStartViro(nil);
    }

    self.currentPendingImage++;
    int targetedImage = self.currentPendingImage;
    
    // Construct callback to be triggered upon loading completion of the Animated Texture.
    std::function<void(bool, std::string)> finishCallback =
    [targetedImage, self](bool succeeded, std::string msg) {
        int width = self.texture->getWidth();
        int height = self.texture->getHeight();
        
        dispatch_async(dispatch_get_main_queue(), ^{
            if (targetedImage != self.currentPendingImage){
                return;
            }
            
            if (succeeded) {
                [self updateMainImage:width height:height];
                [self applyMaterials];
                if(self.onLoadEndViro) {
                    self.onLoadEndViro(@{@"success":@(succeeded)});
                }
                [self setPaused:self.paused];
                [self setLoop:self.loop];
            } else {
                if(self.onLoadEndViro) {
                    self.onErrorViro(@{ @"error": @"Image failed to load" });
                }
            }
        });
    };

    // Load the source of the Animated texture asynchronously.
    std::string urlStr = std::string([[self.source.request.URL description] UTF8String]);
    std::shared_ptr<VROAnimatedTextureOpenGL> animTexture = std::make_shared<VROAnimatedTextureOpenGL>();
    animTexture->loadAnimatedSourceAsync(urlStr,  self.driver, self.context->getFrameSynchronizer(), finishCallback);
    self.texture = std::dynamic_pointer_cast<VROTexture>(animTexture);
    self.imageNeedsDownload = NO;
}

- (void)setContext:(VRORenderContext *)context {
    [super setContext:context];
    [self loadAnimatedTexture];
}

- (void)setDriver:(std::shared_ptr<VRODriver>)driver {
    [super setDriver:driver];
    [self loadAnimatedTexture];
}
@end
