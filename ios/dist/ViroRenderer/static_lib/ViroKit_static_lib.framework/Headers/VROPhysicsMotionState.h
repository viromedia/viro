//
//  VROPhysicsMotionState.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
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

#ifndef VROPhysicsMotionState_h
#define VROPhysicsMotionState_h

#include <btBulletDynamicsCommon.h>

/*
 VROPhysicsMotionState, when attached to a Bullet body, notifies or grabs
 transformation updates to/from a weakly referenced VROPhysicsBody.
 */
class VROPhysicsMotionState : public btMotionState {
public:
    std::weak_ptr<VROPhysicsBody> _w_physicsBody;
    VROPhysicsMotionState(std::shared_ptr<VROPhysicsBody> body, btTransform transformOffset) {
        _w_physicsBody = body;
        _physicsTransformOffset = transformOffset;
    }

    virtual ~VROPhysicsMotionState(){}

    void getWorldTransform(btTransform& centerOfMassWorldTrans) const {
        std::shared_ptr<VROPhysicsBody> body = _w_physicsBody.lock();
        if (!body) {
            return;
        }

        body->getWorldTransform(centerOfMassWorldTrans);
    }

    void setWorldTransform(const btTransform& centerOfMassWorldTrans) {
        std::shared_ptr<VROPhysicsBody> body = _w_physicsBody.lock();
        if (!body) {
            return;
        }

        body->setWorldTransform(centerOfMassWorldTrans);
    }

    btTransform getPhysicsTransformOffset(){
        return _physicsTransformOffset;
    }

private:
    /*
     The offset from Viro's geometric transform to Bullet's physicsBody transform (center of mass).
     */
    btTransform _physicsTransformOffset;
};
#endif
