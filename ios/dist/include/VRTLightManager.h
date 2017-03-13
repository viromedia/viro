//
//  VRTLightManager.h
//  React
//
//  Created by Vik Advani on 1/8/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ViroViewManager.h"

@interface VRTLightManager : ViroViewManager

@end

@interface VRTDirectionalLightManager : VRTLightManager

@end

@interface VRTAmbientLightManager : VRTLightManager

@end

@interface VRTOmniLightManager : VRTLightManager

@end

@interface VRTSpotLightManager : VRTLightManager

@end




