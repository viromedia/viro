//
//  VROHoverListener.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/25/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROHoverListener_h
#define VROHoverListener_h

#include <functional>
#include <memory>

class VRONode;

/*
 Attaches to a VROHoverController to respond to hover events.
 */
class VROHoverDelegate {
    
public:
    
    VROHoverDelegate(bool hitTestBoundsOnly,
                     std::function<bool(std::shared_ptr<VRONode> node)> isHoverable,
                     std::function<void(std::shared_ptr<VRONode> node)> hoverOn,
                     std::function<void(std::shared_ptr<VRONode> node)> hoverOff) :
        _hitTestBoundsOnly(hitTestBoundsOnly),
        _isHoverable(isHoverable),
        _hoverOn(hoverOn),
        _hoverOff(hoverOff)
    {}
    
    bool isHitTestBoundsOnly() const {
        return _hitTestBoundsOnly;
    }
    
    bool isHoverable(std::shared_ptr<VRONode> node) const {
        return _isHoverable(node);
    }
    void hoverOn(std::shared_ptr<VRONode> node) {
        _hoverOn(node);
    }
    void hoverOff(std::shared_ptr<VRONode> node) {
        _hoverOff(node);
    }
    
private:
    
    bool _hitTestBoundsOnly;
    
    std::function<bool(std::shared_ptr<VRONode> node)> _isHoverable;
    std::function<void(std::shared_ptr<VRONode> node)> _hoverOn, _hoverOff;
    
};

#endif /* VROHoverListener_hpp */
