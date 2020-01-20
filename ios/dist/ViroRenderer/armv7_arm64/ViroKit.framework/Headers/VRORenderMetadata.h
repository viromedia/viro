//
//  VRORenderMetadata.h
//  ViroKit
//
//  Created by Raj Advani on 9/14/17.
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

#ifndef VRORenderMetadata_h
#define VRORenderMetadata_h

/*
 The VRORenderMetadata collects scene-wide information about the
 forthcoming render during the updateSortKeys() phase of the render loop.
 
 Enables the choreographer to optimize its render passes based on the
 content of the scene.
 */
class VRORenderMetadata {
public:
    
    VRORenderMetadata() :
            _requiresBloomPass(false),
            _postProcessMaskPass(false) {}

    void setRequiresBloomPass(bool requiresBloomPass) {
        _requiresBloomPass = requiresBloomPass;
    }
    bool requiresBloomPass() const {
        return _requiresBloomPass;
    }

    void setRequiresPostProcessMaskPass(bool requiresPostProcessMaskPass) {
        _postProcessMaskPass = requiresPostProcessMaskPass;
    }
    bool requiresPostProcessMaskPass() const {
        return _postProcessMaskPass;
    }
    
private:
    
    /*
     Set to true to indicate that bloom was used in this render pass, and
     we therefore require a Gaussian blur / accumulation (Bloom) pass.
     */
    bool _requiresBloomPass;

    /*
     True to indicate that a material has a post process mask, and therefore
     to enable post-processing mask pass.
     */
    bool _postProcessMaskPass;
    
};
#endif /* VRORenderMetadata_h */
