#import "StarsRenderLoop.h"

// Flag to control if rendering is performed on a background thread or on the main thread.
static const BOOL kRenderInBackgroundThread = YES;

@implementation StarsRenderLoop {
  NSThread *_renderThread;
  CADisplayLink *_displayLink;
  BOOL _paused;
}

- (instancetype)initWithRenderTarget:(id)target selector:(SEL)selector {
  if (self = [super init]) {
    _displayLink = [CADisplayLink displayLinkWithTarget:target selector:selector];

    if (kRenderInBackgroundThread) {
      _renderThread = [[NSThread alloc] initWithTarget:self
                                              selector:@selector(threadMain)
                                                object:nil];
      [_renderThread start];
    } else {
      [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    }

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationWillResignActive:)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(applicationDidBecomeActive:)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
  }
  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - Public

- (void)invalidate {
  if (kRenderInBackgroundThread) {
    [self performSelector:@selector(renderThreadStop)
                 onThread:_renderThread
               withObject:nil
            waitUntilDone:NO];
  } else {
    [_displayLink invalidate];
    _displayLink = nil;
  }
}

- (BOOL)paused {
  return _paused;
}

- (void)setPaused:(BOOL)paused {
  _paused = paused;
  _displayLink.paused = paused;
}

- (NSTimeInterval)nextFrameTime {
  return _displayLink.timestamp + (_displayLink.duration * _displayLink.frameInterval);
}

#pragma mark - NSNotificationCenter

- (void)applicationWillResignActive:(NSNotification *)notification {
  if (kRenderInBackgroundThread) {
    [self performSelector:@selector(threadPause)
                 onThread:_renderThread
               withObject:nil
            waitUntilDone:YES];
  } else {
    [self threadPause];
  }
}

- (void)applicationDidBecomeActive:(NSNotification *)notification {
  _displayLink.paused = _paused;
}

#pragma mark - Background thread rendering.

- (void)threadMain {
  [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];

  CFRunLoopRun();
}

- (void)threadPause {
  _displayLink.paused = YES;
}

- (void)renderThreadStop {
  [_displayLink invalidate];
  _displayLink = nil;

  CFRunLoopStop(CFRunLoopGetCurrent());

  // Ensure we release the last reference to self on the main thread. The ivar: _renderThread in the
  // dispatch block implicitly retains a strong reference to self.
  // See 'The deallocation problem' in:
  // https://developer.apple.com/library/ios/technotes/tn2109/_index.html
  dispatch_async(dispatch_get_main_queue(), ^{
    [_renderThread cancel];
    _renderThread = nil;
  });
}

@end
