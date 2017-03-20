//
//  VRTMaterialManager.m
//  React
//
//  Created by Vik Advani on 1/20/16.
//  Copyright © 2016 Viro Media.. All rights reserved.
//

#import <React/RCTConvert.h>
#import "VRTMaterialManager.h"
#import "VRTImage.h"
#import "VRTUIImageWrapper.h"
#import <React/RCTUtils.h>
#import <React/RCTImageSource.h>

@implementation RCTBridge (VRTMaterialManager)

- (VRTMaterialManager *)materialManager
{
  return [self moduleForClass:[VRTMaterialManager class]];
}

@end

@interface MaterialWrapper:NSObject {
  
}
- (instancetype)initWithMaterial:(std::shared_ptr<VROMaterial>) material;
- (std::shared_ptr<VROMaterial>)getMaterial;


@end

@implementation MaterialWrapper {
  std::shared_ptr<VROMaterial> _material;
  NSMutableDictionary *_videoTextures;
}

- (instancetype)initWithMaterial:(std::shared_ptr<VROMaterial>) material {
  self = [self init];
  _material = material;
  _videoTextures = [[NSMutableDictionary alloc] init];
  return self;
}

- (void)setMaterialPropertyName:(NSString *)materialPropertyName forVideoTexturePath:(NSString *)path {
  [_videoTextures setObject:path forKey:materialPropertyName];
}

-(NSDictionary *)getVideoTextures {
  return _videoTextures;
}

- (std::shared_ptr<VROMaterial>)getMaterial {
  return _material;
}


@end

@implementation VRTMaterialManager{

    //dictionary of images, so we only load images once.
    NSMutableDictionary * _imageDictionary;
    NSMutableDictionary *_materialDictionary;
}

@synthesize bridge = _bridge;

+ (std::shared_ptr<VROVideoTexture>)createVideoTexture:(NSString *)path
                                         renderContext:(VRORenderContext *)context
                                                driver:(std::shared_ptr<VRODriver>)driver {
  
  NSURL *videoURL = [NSURL URLWithString:path];
  std::string url = std::string([[videoURL description] UTF8String]);
    
  std::shared_ptr<VROVideoTexture> videoTexture = std::make_shared<VROVideoTextureiOS>();
  videoTexture->loadVideo(url, context->getFrameSynchronizer(), driver);
  videoTexture->prewarm();
  
  return videoTexture;
}

RCT_EXPORT_MODULE()

RCT_EXPORT_METHOD(setJSMaterials:(NSDictionary *)materialsDict)
{
  self.materials = materialsDict;
  [self loadMaterials];
}

- (dispatch_queue_t)methodQueue
{
  return dispatch_get_main_queue();
}

- (instancetype)init
{
  self = [super init];
  _imageDictionary = [[NSMutableDictionary alloc] init];
  _materialDictionary = [[NSMutableDictionary alloc] init];
  return self;
}

// Load material images on the main thread, blocking main thread for now.
- (void)loadMaterials {
  for(id key in self.materials) {
    NSDictionary *dictionary = [self.materials objectForKey:key];
    MaterialWrapper *materialWrapper = [self createMaterial:dictionary];
    [_materialDictionary setObject:materialWrapper forKey:key];
  }
}

- (std::shared_ptr<VROTexture>)createTexture2D:(id)json {
  UIImage *image = [self retrieveImage:json];
  VROTextureInternalFormat format = [self parseImageFormat:json];
  VROMipmapMode mipmap = [self parseImageMipmapMode:json];
    
  return std::make_shared<VROTexture>(format,
                                      mipmap,
                                      std::make_shared<VROImageiOS>(image, format));
}

- (std::shared_ptr<VROTexture>)createTextureCubeMap:(NSDictionary *)cubeMapDict {
  if (!cubeMapDict) {
    RCTLogError(@"Error creating cube map: ensure the nx, px, ny, py, nz, and pz params are passed in the body of the cube map texture");
  }
    
  VROTextureInternalFormat format = [self parseImageFormat:cubeMapDict];
  NSMutableDictionary *cubeMapImages = [[NSMutableDictionary alloc] init];
  
  CGFloat cubeSize = -1;
  for(id key in cubeMapDict) {
    UIImage *image = [self retrieveImage:cubeMapDict[key]];
    [cubeMapImages setObject:image forKey:key];

    if (cubeSize < 0) {
      cubeSize = image.size.width;
    }

    if (image.size.width != cubeSize || image.size.height != cubeSize) {
      RCTLogError(@"Error loading cube map. Cube map must be square and uniformly sized");
    }
  }
  
  if((cubeMapImages[@"px"] == nil) ||
     (cubeMapImages[@"nx"] == nil) ||
     (cubeMapImages[@"ny"] == nil) ||
     (cubeMapImages[@"py"] == nil) ||
     (cubeMapImages[@"pz"] == nil) ||
     (cubeMapImages[@"nz"] == nil)) {
    
    RCTLogError(@"Some cube map images are null: please check and fix");
  }
  
  // create cube texture with following images in this order
  std::vector<std::shared_ptr<VROImage>> cubeImages =  {
      std::make_shared<VROImageiOS>(cubeMapImages[@"px"], format),
      std::make_shared<VROImageiOS>(cubeMapImages[@"nx"], format),
      std::make_shared<VROImageiOS>(cubeMapImages[@"py"], format),
      std::make_shared<VROImageiOS>(cubeMapImages[@"ny"], format),
      std::make_shared<VROImageiOS>(cubeMapImages[@"pz"], format),
      std::make_shared<VROImageiOS>(cubeMapImages[@"nz"], format)
  };
  
  return std::make_shared<VROTexture>(format, cubeImages);
}

-(UIImage *)retrieveImage:(id)json {
  NSString *path = [self parseImagePath:json];
  
  UIImage *image = _imageDictionary[path];
  if (!image) {
    image = [self downloadImageSync:json];
    if (!image) {
      VRTUIImageWrapper *uiWrapper = [RCTConvert VRTUIImageWrapper:[json objectForKey:@"source"]];
      if(uiWrapper == nil) {
        NSString *errorMsg = [NSString stringWithFormat:@"Error, unable to create material with source: %@", [json objectForKey:@"source"]];
        RCTLogError(errorMsg);
      }
      image = uiWrapper.image;
    }
    [_imageDictionary setObject:image forKey:path];
  }
  
  return image;
}

- (MaterialWrapper *)createMaterial:(NSDictionary *)material {
  std::shared_ptr<VROMaterial> vroMaterial = std::make_shared<VROMaterial>();
  MaterialWrapper *materialWrapper = [[MaterialWrapper alloc] initWithMaterial:vroMaterial];
  
  for(id key in material) {
    NSString *materialPropertyName = (NSString *)key;
    
    if([materialPropertyName hasSuffix:@"texture"] || [materialPropertyName hasSuffix:@"Texture"]) {
      if([materialPropertyName caseInsensitiveCompare:@"reflectiveTexture"] == NSOrderedSame) {
        std::shared_ptr<VROTexture> texture = [self createTextureCubeMap:material[key]];
        [self setTextureForMaterial:vroMaterial texture:texture name:materialPropertyName];
        
        continue;
      }
      
      NSString *path = [self parseImagePath:material[key]];
      if(path != nil) {
        if([self isVideoTexture:path]) {
          [materialWrapper setMaterialPropertyName:materialPropertyName forVideoTexturePath:path];
        }else {
          std::shared_ptr<VROTexture> texture = [self createTexture2D:material[key]];
          [self setTextureForMaterial:vroMaterial texture:texture name:materialPropertyName];
        }
      }
    }
    else if([materialPropertyName hasSuffix:@"color"] || [materialPropertyName hasSuffix:@"Color"]){
      //We can set either an image texture or texture color not both.
      NSNumber *number = material[key];
      NSUInteger argb = [number unsignedIntegerValue];
      [self setColorForMaterial:vroMaterial color:argb name:(NSString *)materialPropertyName];
    }else {
      if( [@"shininess" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame ) {
        NSNumber *number = material[key];
        vroMaterial->setShininess([number floatValue]);
      }else if([@"fresnelExponent" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame ){
        NSNumber *number =  material[key];
        vroMaterial->setFresnelExponent([number floatValue]);
      }else if([@"lightingModel" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame ){
        VROLightingModel lightingModel = [self convertLightingModel:material[key]];
        vroMaterial->setLightingModel(lightingModel);
      }else if([@"transparencyMode" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame ){
        VROTransparencyMode transparencyMode = [self convertTransparencyMode:material[key]];
        vroMaterial->setTransparencyMode(transparencyMode);
      }else if([@"writesToDepthBuffer" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame ){
        NSNumber *booleanVal = material[key];
        vroMaterial->setWritesToDepthBuffer([booleanVal boolValue]);
      }else if([@"readsFromDepthBuffer" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        NSNumber *booleanVal = material[key];
        vroMaterial->setReadsFromDepthBuffer([booleanVal boolValue]);
      }
      else if ([@"cullMode" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        VROCullMode cullMode = [self convertCullMode:material[key]];
        vroMaterial->setCullMode(cullMode);
      }
    }
  }
  return materialWrapper;
}

- (void)setColorForMaterial:(std::shared_ptr<VROMaterial>)material
                      color:(NSUInteger)color
                       name:(NSString *)materialPropertyName {
  
  //parse the int into seperate color components.
  CGFloat a = ((color >> 24) & 0xFF) / 255.0;
  CGFloat r = ((color >> 16) & 0xFF) / 255.0;
  CGFloat g = ((color >> 8) & 0xFF) / 255.0;
  CGFloat b = (color & 0xFF) / 255.0;
  
  VROVector4f vecColor(r, g, b, a);
  if([@"diffuseColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getDiffuse().setColor(vecColor);
  }else if([@"specularColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getSpecular().setColor(vecColor);
  }else if([@"normalColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getNormal().setColor(vecColor);
  }else if([@"reflectiveColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getReflective().setColor(vecColor);
  }else if([@"emissionColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getEmission().setColor(vecColor);
  }else if([@"transparentColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getTransparent().setColor(vecColor);
  }else if([@"multiplyColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getMultiply().setColor(vecColor);
  }else if([@"ambientOcclusionColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getAmbientOcclusion().setColor(vecColor);
  }else if([@"selfIlluminationColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getSelfIllumination().setColor(vecColor);
  }
  
}

- (void)setTextureForMaterial:(std::shared_ptr<VROMaterial>)material texture:(std::shared_ptr<VROTexture>)texture name:(NSString *)materialPropertyName {
  if([@"diffuseTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getDiffuse().setTexture(texture);
  }else if([@"specularTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getSpecular().setTexture(texture);
  }else if([@"normalTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getNormal().setTexture(texture);
  }else if([@"reflectiveTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getReflective().setTexture(texture);
  }else if([@"emissionTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getEmission().setTexture(texture);
  }else if([@"transparentTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
   material->getTransparent().setTexture(texture);
  }else if([@"multiplyTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getMultiply().setTexture(texture);
  }else if([@"ambientOcclusionTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getAmbientOcclusion().setTexture(texture);
  }else if([@"selfIlluminationTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
    material->getSelfIllumination().setTexture(texture);
  }
}
  
//Convert string to propert VROLightingModel enum,
- (VROLightingModel) convertLightingModel:(NSString *)name {
  
  if([@"Phong" caseInsensitiveCompare:name] == NSOrderedSame){
    return VROLightingModel::Phong;
  }else if([@"Blinn" caseInsensitiveCompare:name] == NSOrderedSame){
    return VROLightingModel::Blinn;
  }else if([@"Lambert" caseInsensitiveCompare:name] == NSOrderedSame){
    return VROLightingModel::Lambert;
  }else if([@"Constant" caseInsensitiveCompare:name] == NSOrderedSame){
    return VROLightingModel::Constant;
  }
  //return default if nothing else matches
  return VROLightingModel::Blinn;
}

- (BOOL)isVideoTexture:(NSString *)path {
  if([path hasSuffix:@"mp4"]){
    return true;
  }
  return false;
}

- (NSString *)parseImagePath:(id)json {
  NSString *path;
  if ([json isKindOfClass:[NSString class]]) {
    path = json;
  }
    
  // The image dictionary is either the top-level element of diffuse-texture,
  // or it's under the 'source' key.
  else if ([json isKindOfClass:[NSDictionary class]]) {
    NSDictionary *dictionary = (NSDictionary *)json;
    if ([dictionary objectForKey:@"source"]) {
      dictionary = [dictionary objectForKey:@"source"];
    }
    
    path = [RCTConvert NSString:dictionary[@"uri"]];
    if (!path) {
      return nil;
    }
  }
  else {
    RCTLogConvertError(json, @"an image");
    return nil;
  }
  return path;
}

- (VROTextureInternalFormat)parseImageFormat:(id)json {
  VROTextureInternalFormat format = VROTextureInternalFormat::RGBA8;
  if ([json isKindOfClass:[NSDictionary class]]) {
    NSDictionary *dictionary = (NSDictionary *)json;
      
    NSObject *formatValue = [dictionary objectForKey:@"format"];
    if (formatValue) {
      format = [RCTConvert VROTextureInternalFormat:[json objectForKey:@"format"]];
    }
  }
  return format;
}

- (VROMipmapMode)parseImageMipmapMode:(id)json {
  VROMipmapMode mipmapMode = VROMipmapMode::Runtime;
  if ([json isKindOfClass:[NSDictionary class]]) {
    NSDictionary *dictionary = (NSDictionary *)json;
      
    NSObject *boolValue = [dictionary objectForKey:@"mipmap"];
    if (boolValue) {
      if (![RCTConvert BOOL:boolValue]) {
        mipmapMode = VROMipmapMode::None;
      }
    }
  }
  return mipmapMode;
}

//Convert string to property VROTransparencyMode enum,
- (VROTransparencyMode)convertTransparencyMode:(NSString *)name {
  if([@"AOne" caseInsensitiveCompare:name] == NSOrderedSame){
    return VROTransparencyMode::AOne;
  }else if([@"RGBZero" caseInsensitiveCompare:name] == NSOrderedSame){
    return VROTransparencyMode::RGBZero;
  }  //return default if nothing else matches
  return VROTransparencyMode::AOne;
}

- (VROCullMode)convertCullMode:(NSString *)name {
  if ([name caseInsensitiveCompare:@"Front"] == NSOrderedSame) {
    return VROCullMode::Front;
  }
  else if ([name caseInsensitiveCompare:@"None"] == NSOrderedSame) {
    return VROCullMode::None;
  }
  else {
    // Default if nothing else matches
    return VROCullMode::Back;
  }
}
  
- (std::shared_ptr<VROMaterial>)getMaterialByName:(NSString *)name {
  MaterialWrapper *materialWrapper = _materialDictionary[name];
  if(materialWrapper != nil){
    return [materialWrapper getMaterial];
  }
  return nil;
}

- (NSDictionary *)getVideoTexturesForMaterialName:(NSString *)name {
  MaterialWrapper *materialWrapper = _materialDictionary[name];
  if(materialWrapper != nil){
    return [materialWrapper getVideoTextures];
  }
  return nil;
}

// DEPRECATED: this is only in place for Beta. This needs to be
//             replaced with asynchronous image loading.
//             Tracked by VIRO-912
- (UIImage *)downloadImageSync:(id)json
{
  VRTUIImageWrapper *uiImageWrapper = [RCTConvert VRTUIImageWrapper:json];
  if(uiImageWrapper != nil){
    return uiImageWrapper.image;
  }
  return nil;
}

@end
