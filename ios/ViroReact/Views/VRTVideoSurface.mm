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
  std::shared_ptr<VROVideoSurface> _surface;
  BOOL _didUpdateSurface;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
  if (self) {
    _muted = NO;
    _loop = NO;
    _volume = 1;
    _paused = NO;
    _didUpdateSurface = NO;
  }
  return self;
}

- (void)seekToTime:(NSInteger)time {
  if (_surface) {
    _surface->seekToTime(time);
  }
}

- (void)setPaused:(BOOL)paused {
  _paused = paused;
  if (_surface) {
    _paused ? _surface->pause() : _surface->play();
  }
}

- (void)setLoop:(BOOL)loop {
  _loop = loop;
  if (_surface) {
    _surface->setLoop(loop);
    // If we're not paused, call play again because we could get into the state
    // where we're at the end of a video w/ loop false, if the user sets it to
    // loop true, setPaused could get called first before setLoop and it wont play
    if (!_paused) {
      _surface->play();
    }
  }
}

- (void)setMuted:(BOOL)muted {
  _muted = muted;
  if (_surface) {
    _surface->setMuted(muted);
  }
}

- (void)setVolume:(float)volume {
  _volume = volume;
  if (_surface) {
    _surface->setVolume(volume);
  }
}

- (void)setSource:(NSDictionary *)source {
  _source = source;
  _didUpdateSurface = NO;
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
  
  std::shared_ptr<VROVideoTexture> videoTexture = std::make_shared<VROVideoTextureiOS>();
  _surface = VROVideoSurface::createVideoSurface(1, 1, url,
                                                 self.context->getFrameSynchronizer(),
                                                 videoTexture, *self.driver);
  _surface->getMaterials().front()->setReadsFromDepthBuffer(false);
  
  if (self.paused) {
    _surface->pause();
  } else {
    _surface->play();
  }

  _surface->setVolume(self.volume);
  _surface->setMuted(self.muted);
  _surface->setLoop(self.loop);
  _surface->setDelegate(std::make_shared<VROVideoDelegateiOS>(self));
  
  [self node]->setGeometry(_surface);
}

- (void)viewWillAppear {
  [self updateSurface];
}


- (void)viewWillDisappear {
  if (_surface) {
    _surface->pause();
  }
}

#pragma mark VROVideoDelegate implementation

- (void)videoDidFinish {
  if (self.onFinishViro) {
    self.onFinishViro(@{@"finished": @(true),});
  }
}

@end
