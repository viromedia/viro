//
//  VROAnimateMaterialDelegateInternal_h
//  ViroRenderer
//
//  Created by Vik Advani on 2/6/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROAnimateMaterialDelegateInternal_h
#define VROAnimateMaterialDelegateInternal_h

#include <string>
#include <memory>
class VRONode;
class VROAnimateMaterialDelegateInternal {
  
public:
  VROAnimateMaterialDelegateInternal() {}
  virtual ~VROAnimateMaterialDelegateInternal() {}
  
  virtual void onAnimateMaterial(std::shared_ptr<VRONode> node, std::string materialName) = 0;
  
};

#endif /* VROAnimateMaterialDelegateInternal_h */



