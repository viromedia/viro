//
//  VROCrossLayout.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/11/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROCrossLayout_h
#define VROCrossLayout_h

#include "VROLayer.h"
#include "VROScene.h"
#include <memory>

class VROCrossLayoutDelegate {
    
public:

    virtual std::shared_ptr<VROLayer> getCenterLayer() = 0;
    virtual std::shared_ptr<VROLayer> getTopLayer()    = 0;
    virtual std::shared_ptr<VROLayer> getBottomLayer() = 0;
    virtual std::shared_ptr<VROLayer> getLeftLayer()   = 0;
    virtual std::shared_ptr<VROLayer> getRightLayer()  = 0;
    
};

class VROCrossLayout {
    
public:
    
    VROCrossLayout(std::shared_ptr<VROScene> scene);
    virtual ~VROCrossLayout();
    
    void setDelegate(std::shared_ptr<VROCrossLayoutDelegate> delegate) {
        _delegate = delegate;
    }
    std::shared_ptr<VROCrossLayoutDelegate> getDelegate() {
        return _delegate.lock();
    }
    
    void layout();
    
private:
    
    std::weak_ptr<VROCrossLayoutDelegate> _delegate;
    std::weak_ptr<VROScene> _scene;
    
};

#endif /* VROCrossLayout_h */
