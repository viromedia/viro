//
//  VRT360Video.m
//  React
//
//  Created by Vik Advani on 1/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VROUtils.h"
#import "VRT360Video.h"
#include "RCTConvert.h"
#include "RCTImageSource.h"

@implementation VRT360Video {
  std::shared_ptr<VROVideoTexture> _videoTexture;
  BOOL _sphereTextureAddedToScene;
}

@synthesize rotation = _rotation;

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _sphereTextureAddedToScene = NO;
    _muted = NO;
    _loop = NO;
    _volume = 1;
    _paused = NO;
  }
  
  return self;
}

- (void)setPaused:(BOOL)paused {
  _paused = paused;
  if (_videoTexture) {
    _paused ? _videoTexture->pause() : _videoTexture->play();
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
    _videoTexture->setMuted(muted);
  }
}

- (void)setVolume:(float)volume {
  _volume = volume;
  if (_videoTexture) {
    _videoTexture->setVolume(volume);
  }
}

- (void)seekToTime:(NSInteger)time {
  if (_videoTexture) {
    _videoTexture->seekToTime(time);
  }
}

- (void)setSource:(NSDictionary *)source {
  _source = source;
  _sphereTextureAddedToScene = NO;
  [self updateSceneWithSphereTexture];
}

- (void)setRotation:(NSArray<NSNumber *> *)rotation {
  _rotation = [rotation copy];
  if (_sphereTextureAddedToScene) {
    float rotationValues[3] = {0.0f, 0.0f, 0.0f};
    populateFloatArrayFromNSArray(_rotation, rotationValues, 3);
    self.scene->setBackgroundRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});
  }
}

- (void)updateSceneWithSphereTexture {
  if (!self.source) {
    RCTLogError(@"Source should not be nil.");
  } else if (!self.context || !self.driver || _sphereTextureAddedToScene) {
    return;
  }
  
  RCTImageSource *imageSource = [RCTConvert RCTImageSource:self.source];
  NSURL *videoURL = imageSource.request.URL;
  std::string url = std::string([[videoURL description] UTF8String]);

  _videoTexture = std::make_shared<VROVideoTextureiOS>();
  _videoTexture->loadVideo(url, self.context->getFrameSynchronizer(), *self.driver);
  _videoTexture->prewarm();

  self.scene->setBackgroundSphere(_videoTexture);
  float rotationValues[3] = {0.0f, 0.0f, 0.0f};
  populateFloatArrayFromNSArray(_rotation, rotationValues, 3);
  self.scene->setBackgroundRotation({toRadians(rotationValues[0]), toRadians(rotationValues[1]), toRadians(rotationValues[2])});
  _sphereTextureAddedToScene = true;

  if (self.paused) {
    _videoTexture->pause();
  } else {
    _videoTexture->play();
  }

  _videoTexture->setVolume(self.volume);
  _videoTexture->setMuted(self.muted);
  _videoTexture->setLoop(self.loop);
  _videoTexture->setDelegate(std::make_shared<VROVideoDelegateiOS>(self));
}

- (void)viewWillAppear {
  if (!_sphereTextureAddedToScene) {
    [self updateSceneWithSphereTexture];
  }
}

- (void)viewWillDisappear {
  if (_sphereTextureAddedToScene) {
    _videoTexture->pause();
  }
}

#pragma mark VROVideoDelegate implementation

- (void)videoDidFinish {
  if (self.onFinishViro) {
    self.onFinishViro(@{@"finished": @(true),});
  }
}

@end
