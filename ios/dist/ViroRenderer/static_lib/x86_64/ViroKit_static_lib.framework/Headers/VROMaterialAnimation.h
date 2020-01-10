//
//  VROMaterialAnimation.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 2/14/17.
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

#ifndef VROMaterialAnimation_h
#define VROMaterialAnimation_h

#include <memory>
#include "VROLazyMaterial.h"

class VROMaterial;

/*
 Represents a single material animation.
 */
class VROMaterialAnimation {
    
public:
    
    VROMaterialAnimation(int index, std::shared_ptr<VROLazyMaterial> lazyMaterial) :
        _index(index),
        _lazyMaterial(lazyMaterial) {}
    virtual ~VROMaterialAnimation() {}
    
    int getIndex() const { return _index; }
    std::shared_ptr<VROMaterial> getMaterial() const { return _lazyMaterial->get(); }
    
private:
    
    const int _index;
    std::shared_ptr<VROLazyMaterial> _lazyMaterial;
    
};

#endif /* VROMaterialAnimation_h */
