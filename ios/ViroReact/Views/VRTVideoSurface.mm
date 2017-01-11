//
//  VRTVideoSurface.m
//  React
//
//  Created by Vik Advani on 3/24/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTVideoSurface.h"
#import "RCTImageSource.h"
#import "RCTConvert.h"
#import "VROUtils.h"

@implementation VRTVideoSurface{
  std::shared_ptr<VROSurface> _surface;
  std::shared_ptr<VROVideoTexture> _videoTexture;
  BOOL _didUpdateSurface;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _muted = NO;
    _loop = NO;
    _volume = 1;
    _paused = NO;
    _width = 1;
    _height = 1;
    _didUpdateSurface = NO;
  }
  return self;
}

- (void)seekToTime:(NSInteger)time {
  if (_videoTexture) {
    _videoTexture->seekToTime(time);
  }
}

- (void)setPaused:(BOOL)paused {
  _paused = paused;
  if (_surface) {
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

- (void)setSource:(NSDictionary *)source {
  _source = source;
  _didUpdateSurface = NO;
  [self updateSurface];
}

-(void)setWidth:(float)width {
  _width = width;
  [self updateSurface];
  
}

-(void)setHeight:(float)height {
  _height = height;
  [self updateSurface];
}

- (void)updateSurface {
  if (!self.source) {
    RCTLogError(@"Source should not be nil.");
  } else if (!self.context || !self.driver || _didUpdateSurface) {
    return;
  }
  
  // All asset types can be converted into an RCTImageSource(should be renamed to RCTAssetSource) when using the 'require() statement).
  // If an asset is passed with a full uri, that uri is preserved and passed to the video surface
  RCTImageSource *imageSource = [RCTConvert RCTImageSource:self.source];
  NSURL *videoURL = imageSource.request.URL;
  std::string url = std::string([[videoURL description] UTF8String]);
  
  _videoTexture = std::make_shared<VROVideoTextureiOS>();
  _surface = VROSurface::createSurface(_width, _height);
  _videoTexture->loadVideo(url, self.context->getFrameSynchronizer(), *self.driver);
  
  _surface->getMaterials().front()->getDiffuse().setTexture(_videoTexture);
  if (self.paused) {
    _videoTexture->pause();
  } else {
    _videoTexture->play();
  }

  _videoTexture->setVolume(self.volume);
  _videoTexture->setMuted(self.muted);
  _videoTexture->setLoop(self.loop);
  _videoTexture->setDelegate(std::make_shared<VROVideoDelegateiOS>(self));
  
  [self node]->setGeometry(_surface);

  // set that we did in fact update the surface
  _didUpdateSurface = YES;
}

- (void)viewWillAppear {
  [self updateSurface];
}

- (void)viewWillDisappear {
  if (_videoTexture) {
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
