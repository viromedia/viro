//
//  VROSkeletonRenderer.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/19/19.
//  Copyright © 2019 Viro Media. All rights reserved.
//

#ifndef VROSkeletonRenderer_h
#define VROSkeletonRenderer_h

#include "VROBodyTracker.h"
#import "VROViewAR.h"
#import <UIKit/UIKit.h>

@interface VROBodyRecognitionDrawDelegate : NSObject<VRODebugDrawDelegate>
- (void)drawRect;
- (void)setLabels:(std::vector<NSString *>)labels positions:(std::vector<VROVector3f>)positions;
- (void)setBoxes:(std::vector<VROBoundingBox>)boxes;
- (void)setColors:(std::vector<UIColor *>)colors;
- (void)setConfidences:(std::vector<float>)confidences;
- (void)setDynamicCropBox:(CGRect)box;
- (void)setViewWidth:(int)width height:(int)height;
@end

/*
 Renders the skeleton to the screen.
 */
class VROSkeletonRenderer : public VROBodyTrackerDelegate {
  
public:
    
    VROSkeletonRenderer(VROViewAR *view, std::shared_ptr<VROBodyTracker> tracker);
    virtual ~VROSkeletonRenderer();
    
    void onBodyJointsFound(const VROPoseFrame &inferredJoints);
    
private:
    
    VROViewAR *_view;
    std::weak_ptr<VROBodyTracker> _bodyTracker;
    VROBodyRecognitionDrawDelegate *_drawDelegate;

};

#endif /* VROSkeletonRenderer_h */
