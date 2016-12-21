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
class VROHoverDelegate : public std::enable_shared_from_this<VROHoverDelegate> {
    
public:
    
    VROHoverDelegate() {}
    virtual ~VROHoverDelegate() {}
    
    virtual bool isHoverTestBoundsOnly() const = 0;
    virtual bool isHoverable(std::shared_ptr<VRONode> node) = 0;
    virtual void hoverOnNode(std::shared_ptr<VRONode> node) = 0;
    virtual void hoverOffNode(std::shared_ptr<VRONode> node) = 0;
    
};

#endif /* VROHoverListener_hpp */
