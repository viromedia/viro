#import "GVRCardboardView.h"

@class StarsRenderLoop;

/** Cardboard Stars renderer. */
@interface StarsRenderer : NSObject<GVRCardboardViewDelegate>

@property(nonatomic, weak) StarsRenderLoop *renderLoop;

@end
