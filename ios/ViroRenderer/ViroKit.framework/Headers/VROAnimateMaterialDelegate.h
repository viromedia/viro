//
//  VROAnimateMaterialDelegate.h
//  ViroRenderer
//
//  Created by Vik Advani on 2/7/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROAnimateMaterialDelegate_h
#define VROAnimateMaterialDelegate_h


#import <Foundation/Foundation.h>
#include <memory>
#include <string>
@protocol VROAnimateMaterialDelegate <NSObject>

/**
  Currently materials are loading in a platform specific way and as a result the animation system
  in the renderer doesn't have access to materials, the renderer only knows the material name. In
  order to properly animate the material, this delegate has to be implemented by the iOS bridge layer,
  which can then properly animate the material when onAnimateMaterial is invoked.
 **/
- (void)onAnimateMaterial:(std::shared_ptr<VRONode>) node materialName:(std::string) materialName;

@end

#endif /* VROAnimateMaterialDelegate_h */
