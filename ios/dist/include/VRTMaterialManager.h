//
//  VRTMaterialManager.h
//  React
//
//  Created by Vik Advani on 1/20/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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
//

#import <Foundation/Foundation.h>
#import <ViroKit/ViroKit.h>
#import <React/RCTBridgeModule.h>
#import <React/RCTBridge.h>
#import "VRTMaterialChangedDelegate.h"


@interface VRTMaterialManager : NSObject <RCTBridgeModule>

+ (std::shared_ptr<VROVideoTexture>)createVideoTexture:(NSString *)path
                                         renderContext:(VRORenderContext *)context
                                                driver:(std::shared_ptr<VRODriver>)driver;
- (std::shared_ptr<VROTexture>)createTextureCubeMap:(NSDictionary *)cubeMapDict;
- (std::shared_ptr<VROTexture>)createTexture2D:(id)json sRGB:(BOOL)sRGB;

@property (nonatomic, copy) NSMutableDictionary<NSString *, NSDictionary *> *materials;

- (void)reloadMaterials;
- (std::shared_ptr<VROMaterial>)getMaterialByName:(NSString *)name;
- (NSDictionary *)getVideoTexturesForMaterialName:(NSString *)name;
- (BOOL)isVideoMaterial:(NSString *)materialName;
- (void)addMaterialChangedListener:(NSString *)name listener:(id<VRTMaterialChangedDelegate>)listener;
- (void)loadVideoTextureForMaterial:(NSString *)materialName driver:(std::shared_ptr<VRODriver>)driver context:(VRORenderContext *)context;

- (void)setTextureForMaterial:(std::shared_ptr<VROMaterial>)material
                      texture:(std::shared_ptr<VROTexture>)texture
                         name:(NSString *)materialPropertyName;

@end

@interface RCTBridge (VRTMaterialManager)

- (VRTMaterialManager *)materialManager;

@end

