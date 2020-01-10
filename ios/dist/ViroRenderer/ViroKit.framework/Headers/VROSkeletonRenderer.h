//
//  VROSkeletonRenderer.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/19/19.
//  Copyright © 2019 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
