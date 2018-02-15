//
//  VRORendererConfiguration.h
//  ViroKit
//
//  Created by Raj Advani on 2/1/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VRORendererConfiguration_h
#define VRORendererConfiguration_h

#include <stdio.h>

class VRORendererConfiguration {
public:
    bool enableShadows = true;
    bool enableBloom = true;
    bool enableHDR = true;
    bool enablePBR = true;
};

#endif /* VRORendererConfiguration_h */
