//
//  VROViewport.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/23/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROViewport_hpp
#define VROViewport_hpp

#include "VRODefines.h"
#include "VROMatrix4f.h"
#include "VROMath.h"

#if VRO_METAL
#include <MetalKit/MetalKit.h>
#endif

class VROViewport {
    
public:
    
    VROViewport() :
        _x(0), _y(0), _width(0), _height(0), _contentScaleFactor(2)
    {}

    VROViewport(int x, int y, int width, int height) :
        _x(x), _y(y), _width(width), _height(height), _contentScaleFactor(2)
    {}
 
    int getX() const { return _x; }
    int getY() const { return _y; }
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }
    
    /*
     On some platforms, there is a difference between screen pixels and
     screen points (e.g. on iOS Retina displays). Viewports are always
     specified in pixels: to get points, divide by the contentScaleFactor.
     */
    float getContentScaleFactor() const { return _contentScaleFactor; };
    
    VROMatrix4f getOrthographicProjection(float near, float far) {
        float left   = _x;
        float right  = _x + _width;
        float bottom = _y;
        float top    = _y + _height;
        
        return VROMathComputeOrthographicProjection(left, right, bottom, top, near, far);
    }
    
    void setViewport(int x, int y, int width, int height) {
        _x = x;
        _y = y;
        _width = width;
        _height = height;
    }
    
    void setContentScaleFactor(float factor) {
        _contentScaleFactor = factor;
    }
    
#if VRO_METAL
    MTLViewport toMetalViewport() const {
        MTLViewport viewport;
        viewport.originX = _x;
        viewport.originY = _y;
        viewport.width   = _width;
        viewport.height  = _height;
        viewport.znear   = 0.0;
        viewport.zfar    = 1.0;
        
        return viewport;
    }
    
    MTLScissorRect toMetalScissor() const {
        MTLScissorRect scissor;
        scissor.x = _x;
        scissor.y = _y;
        scissor.width = _width;
        scissor.height = _height;
        
        return scissor;
    }
#endif
    
    void toArray(int *viewport) const {
        viewport[0] = _x;
        viewport[1] = _y;
        viewport[2] = _width;
        viewport[3] = _height;
    }
    
private:
    
    int _x, _y, _width, _height;
    float _contentScaleFactor;
    
};

#endif /* VROViewport_hpp */
