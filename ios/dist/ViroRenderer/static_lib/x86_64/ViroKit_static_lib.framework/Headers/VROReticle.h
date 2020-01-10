//
//  VROReticle.h
//  ViroRenderer
//
//  Created by Raj Advani on 1/12/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

#ifndef VRORecticle_h
#define VRORecticle_h

#include <memory>
#include <vector>
#include "VROSurface.h"
#include "VROTexture.h"
#include "VRONode.h"

class VRONode;
class VROPolyline;
class VROVector3f;
class VRORenderContext;
class VRODriver;
enum class VROEyeType;

class VROReticle {

public:

    VROReticle(std::shared_ptr<VROTexture> icon);
    virtual ~VROReticle();

    void trigger();
    void renderEye(VROEyeType eye, const VRORenderContext &renderContext, std::shared_ptr<VRODriver> &driver);

    void setPosition(VROVector3f position);
    void setRadius(float radius);

    void setEnabled(bool enabled);

    /*
     If pointer is fixed, the reticle will be locked at the center of each eye. This is used for
     VR devices that lack a pointing controller (e.g. Cardboard).
     */
    void setPointerFixed(bool fixed);
    bool isHeadlocked();

    void animateFuse(float fuseRatio);
    void stopFuseAnimation();
    
private:

    /*
     Set to true by controllers if the reticle is fixed at the center
     of each eye. Set to false if the VR platform has a hand-held controller
     that can be used to point.
     */
    bool _isHeadlocked;

    bool _enabled;
    float _size;
    float _radius;
    float _thickness;
    float _endThickness;
    float _fuseScale;

    /*
     Cached x y points describing a circle with kCircleSegments, used
     to draw the reticleLine and fuseLine.
     */
    std::vector<VROVector3f> _cachedCirclePoints;
    std::vector<VROVector3f> createArc(float radius, int numSegments);

    /*
     Helper function to render each node in the reticle.
     */
    void renderNode(std::shared_ptr<VRONode> node, const VRORenderContext &renderContext,
                    std::shared_ptr<VRODriver> &driver);

    /*
     Nodes containing the UI components of the reticle. It can be
     a line or an icon (image) reticle.
     */
    std::shared_ptr<VRONode> _reticleBaseNode;
    std::shared_ptr<VROSurface> _reticleIcon;
    std::shared_ptr<VROPolyline> _reticleLine;

    /*
     Nodes containing the UI components of the reticle when it is fused.
     */
    std::shared_ptr<VRONode> _fuseNode;
    std::shared_ptr<VROPolyline>  _fuseLine;
    std::shared_ptr<VRONode> _fuseBackgroundNode;
    std::shared_ptr<VROPolyline> _fuseBackgroundLine;
    std::shared_ptr<VRONode> _fuseTriggeredNode;
    std::shared_ptr<VROPolyline> _fuseTriggeredLine;

    /*
     True when we are currently animating the reticle with a fuse ratio under
     animateFuse(). False if we've stopped with stopFuseAnimation().
     */
    bool _isFusing;

    /*
     True when we have already ran the fuseTriggered animation for the
     current fuse state.
     */
    bool _fuseTriggered;
    void animateFuseTriggered();
};
#endif
