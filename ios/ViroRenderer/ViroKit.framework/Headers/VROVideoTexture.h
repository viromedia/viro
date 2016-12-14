//
//  VROVideoTexture.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/8/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROVideoTexture_h
#define VROVideoTexture_h

#import <AVFoundation/AVFoundation.h>
#import <CoreVideo/CVMetalTextureCache.h>
#import "VROFrameListener.h"
#import "VROTexture.h"
#import <memory>
#import "VROVideoDelegate.h"

@class VROVideoCaptureDelegate;
@class VROAVPlayerDelegate;
@class VROVideoNotificationListener;
class VRORenderContext;
class VROFrameSynchronizer;
class VRODriver;
class VROMaterial;
class VROVideoTextureCache;

static const long kInFlightVideoTextures = 3;

class VROVideoTexture : public VROTexture, public VROFrameListener, public std::enable_shared_from_this<VROVideoTexture> {
    
public:
    
    VROVideoTexture();
    ~VROVideoTexture();
    
    /*
     Use this video texture to display the contents of the given URL. The video
     will not run until play() is invoked.
     */
    void loadVideo(NSURL *url,
                   std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                   VRODriver &driver);
    
    /*
     Perform video initialization (which causes a stutter) early.
     */
    void prewarm();
    
    void onFrameWillRender(const VRORenderContext &context);
    void onFrameDidRender(const VRORenderContext &context);
    
    void pause();
    void play();
    bool isPaused();
    void seekToTime(int seconds);

    void setMuted(bool muted);
    void setVolume(float volume);
    void setLoop(bool loop);

    void setDelegate(id <VROVideoDelegate> delegate);
  
    /*
     Internal: invoked by the delegate.
     */
    void displayPixelBuffer(std::unique_ptr<VROTextureSubstrate> substrate);
    
private:
    
    /*
     AVPlayer for recorded video playback.
     */
    AVPlayer *_player;
    bool _paused;
    bool _loop;
    VROAVPlayerDelegate *_avPlayerDelegate;
    VROVideoNotificationListener *_videoNotificationListener;
    __weak id <VROVideoDelegate> _delegate;

};

/*
 Delegate for receiving video output from URLs.
 */
@interface VROAVPlayerDelegate : NSObject <AVPlayerItemOutputPullDelegate>

- (id)initWithVideoTexture:(VROVideoTexture *)texture
                    player:(AVPlayer *)player
                    driver:(VRODriver &)driver;

- (void)renderFrame;

@end

/*
 Simple object to register for AVPlayer notifications
 */
@interface VROVideoNotificationListener : NSObject

- (id)initWithVideoPlayer:(AVPlayer *)player
                     loop:(BOOL)loop
            videoDelegate:(id <VROVideoDelegate>)videoDelegate;

- (void)shouldLoop:(BOOL)loop;

- (void)setDelegate:(id <VROVideoDelegate>)videoDelegate;

@end

#endif /* VROVideoTexture_h */
