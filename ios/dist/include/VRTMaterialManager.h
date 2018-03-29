//
//  VRTMaterialManager.h
//  React
//
//  Created by Vik Advani on 1/20/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

