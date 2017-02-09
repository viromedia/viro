//
//  VROAnimateMaterialDelegateiOS.h
//  ViroRenderer
//
//  Created by Vik Advani on 2/7/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef  VROAnimateMaterialDelegateiOS_h
#define  VROAnimateMaterialDelegateiOS_h

#include "VROAnimateMaterialDelegateInternal.h"
#include "VROAnimateMaterialDelegate.h"

class VROAnimateMaterialDelegateiOS : public VROAnimateMaterialDelegateInternal {
  
public:
  
  VROAnimateMaterialDelegateiOS(id <VROAnimateMaterialDelegate> delegate) :
  _delegate(delegate)
  {}
  virtual ~VROAnimateMaterialDelegateiOS() {}
  
  void onAnimateMaterial(std::shared_ptr<VRONode> node, std::string materialName) {
    [_delegate onAnimateMaterial:node materialName:materialName];
  }
  
private:
  
  id <VROAnimateMaterialDelegate> _delegate;
  
};

#endif /*  VROAnimateMaterialDelegateiOS_h */

