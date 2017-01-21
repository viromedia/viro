//
//  VRTMaterialManager.h
//  React
//
//  Created by Vik Advani on 1/20/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ViroKit/ViroKit.h>
#import "RCTBridgeModule.h"
#import "RCTBridge.h"


@interface VRTMaterialManager : NSObject <RCTBridgeModule>

+ (std::shared_ptr<VROVideoTexture>)createVideoTexture:(NSString *)path
                                         renderContext:(VRORenderContext *)context
                                                driver:(VRODriver *)driver;
- (std::shared_ptr<VROTexture>)createTextureCubeMap:(NSDictionary *)cubeMapDict;
- (std::shared_ptr<VROTexture>)createTexture2D:(id)json;

@property (nonatomic, copy) NSDictionary<NSString *, NSDictionary *> *materials;

- (void)loadMaterials;
- (std::shared_ptr<VROMaterial>)getMaterialByName:(NSString *)name;
- (NSDictionary *)getVideoTexturesForMaterialName:(NSString *)name;

- (void)setTextureForMaterial:(std::shared_ptr<VROMaterial>)material
                      texture:(std::shared_ptr<VROTexture>)texture
                         name:(NSString *)materialPropertyName;

@end

@interface RCTBridge (VRTMaterialManager)

- (VRTMaterialManager *)materialManager;

@end

