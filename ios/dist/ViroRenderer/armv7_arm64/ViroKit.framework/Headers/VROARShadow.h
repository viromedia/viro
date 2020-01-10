//
//  VROARTransparentShadow.h
//  ViroKit
//
//  Created by Raj Advani on 8/23/17.
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

#ifndef VROARTShadow_h
#define VROARTShadow_h

#include <memory>

class VROMaterial;
class VROShaderModifier;

/*
 Apply this to any material to turn it into a "transparent shadow"
 material. These materials are transparent but able to receive shadows
 from the shadow-mapping system.
 
 They achieve this through a combination of shader modifiers that:
 
 1. Make the color of the surface black, with alpha 0,
 2. Increase the alpha if the surface is in shadow.
 
 This is used to cast virtual shadows on real-world scenes.
 */
class VROARShadow {
public:
    
    static void apply(std::shared_ptr<VROMaterial> material);
    static void remove(std::shared_ptr<VROMaterial> material);
    
private:
    
    static std::shared_ptr<VROShaderModifier> createSurfaceModifier();
    static std::shared_ptr<VROShaderModifier> createFragmentModifier();
    static std::shared_ptr<VROShaderModifier> createLightingModifier();
    
};

#endif /* VROARShadow_h */
