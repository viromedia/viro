//
//  VROLightManager.h
//  React
//
//  Created by Vik Advani on 1/8/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ViroViewManager.h"

@interface VROLightManager : ViroViewManager

@end

@interface VRODirectionalLightManager : VROLightManager

@end

@interface VROAmbientLightManager : VROLightManager

@end

@interface VROOmniLightManager : VROLightManager

@end

@interface VROSpotLightManager : VROLightManager

@end




