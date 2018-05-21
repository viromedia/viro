//
//  VRORenderMetadata.h
//  ViroKit
//
//  Created by Raj Advani on 9/14/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

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
        _requiresBloomPass(false) {}
    
    void setRequiresBloomPass(bool requiresBloomPass) {
        _requiresBloomPass = requiresBloomPass;
    }
    bool requiresBloomPass() const {
        return _requiresBloomPass;
    }
    
private:
    
    /*
     Set to true to indicate that bloom was used in this render pass, and
     we therefore require a Gaussian blur / accumulation (Bloom) pass.
     */
    bool _requiresBloomPass;
    
};
#endif /* VRORenderMetadata_h */
