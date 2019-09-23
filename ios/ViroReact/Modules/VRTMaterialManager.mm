//
//  VRTMaterialManager.m
//  React
//
//  Created by Vik Advani on 1/20/16.
//  Copyright © 2016 Viro Media.. All rights reserved.
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

#import <React/RCTConvert.h>
#import "VRTMaterialManager.h"
#import "VRTImage.h"
#import "VRTUIImageWrapper.h"
#import <React/RCTUtils.h>
#import <React/RCTImageSource.h>

@implementation RCTBridge (VRTMaterialManager)

- (VRTMaterialManager *)materialManager {
    return [self moduleForClass:[VRTMaterialManager class]];
}

@end

#pragma mark WeakMaterialChangeListenerContainer
@interface WeakMaterialChangeListenerContainer : NSObject

@property (nonatomic, weak) id listener;

- (id)initWithListener: (id)listener;

@end

@implementation WeakMaterialChangeListenerContainer

@synthesize listener = _listener;

- (instancetype)initWithListener: (id)listener;
{
    if ((self = [super init])) {
        _listener = listener;
    }
    return self;
}

@end

#pragma mark MaterialWrapper
@interface MaterialWrapper : NSObject {
    
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

- (void)setVideoTexturePathForMaterialProp:(NSString *)materialPropertyName path:(NSString *)path {
    [_videoTextures setObject:path forKey:materialPropertyName];
}

-(NSDictionary *)getVideoTextures {
    return _videoTextures;
}

- (std::shared_ptr<VROMaterial>)getMaterial {
    return _material;
}

@end

@implementation VRTMaterialManager {
    
    // Dictionary of images, so we only load images once
    NSMutableDictionary *_imageDictionary;
    NSMutableDictionary *_materialDictionary;
    NSMutableDictionary *_materialChangeListeners;
    
    // This is flipped to true when an EGL context is loaded for the first time;
    // this happens when reloadMaterials is first called. Until we have an EGL
    // context, we do not load materials, we just store them in self.materials.
    BOOL _eglContextLoaded;
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

// Invoked each time a new JS file with materials is loaded. Add the
// materials found to our self.materials array, and load the materials
// for the renderer. We add to self.materials so that we can reload all
// materials at once if we move to a new EGL context
RCT_EXPORT_METHOD(setJSMaterials:(NSDictionary *)materials) {
    [self.materials addEntriesFromDictionary:materials];
    [self loadMaterials:materials];
}

RCT_EXPORT_METHOD(deleteMaterials:(NSArray *)materials) {
    if (_materialDictionary && materials) {
        for (NSString *material in materials) {
            [_materialDictionary removeObjectForKey:material];
        }
    }
}

- (dispatch_queue_t)methodQueue {
    return dispatch_get_main_queue();
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _materials = [[NSMutableDictionary alloc] init];
        _imageDictionary = [[NSMutableDictionary alloc] init];
        _materialDictionary = [[NSMutableDictionary alloc] init];
        _materialChangeListeners = [[NSMutableDictionary alloc] init];
        _eglContextLoaded = NO;
    }
    return self;
}

+ (BOOL)requiresMainQueueSetup {
  return NO;
}

// Reload all materials; typically used when EGL context switches
- (void)reloadMaterials {
    _eglContextLoaded = YES;
    [_materialDictionary removeAllObjects];
    [self loadMaterials:self.materials];
}

- (void)loadMaterials:(NSDictionary *)materials {
    for (id key in materials) {
        NSDictionary *dictionary = [materials objectForKey:key];
        MaterialWrapper *materialWrapper = [self createMaterial:dictionary name:key];
        [_materialDictionary setObject:materialWrapper forKey:key];
    }
}

- (std::shared_ptr<VROTexture>)createTexture2D:(id)json sRGB:(BOOL)sRGB {
    UIImage *image = [self retrieveImage:json];
    VROTextureInternalFormat format = [self parseImageFormat:json];
    VROMipmapMode mipmap = [self parseImageMipmapMode:json];
    
    return std::make_shared<VROTexture>(sRGB, mipmap,
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
    
    return std::make_shared<VROTexture>(true, cubeImages);
}

- (void)loadProperties:(NSDictionary *)properties forTexture:(std::shared_ptr<VROTexture>)texture {
    NSString *wrapS = [properties objectForKey:@"wrapS"];
    if (wrapS != nil) {
        texture->setWrapS([self convertWrapMode:wrapS]);
    }
    
    NSString *wrapT = [properties objectForKey:@"wrapT"];
    if (wrapT != nil) {
        texture->setWrapT([self convertWrapMode:wrapT]);
    }
    
    NSString *minFilter = [properties objectForKey:@"minificationFilter"];
    if (minFilter != nil) {
        texture->setMinificationFilter([self convertFilterMode:minFilter]);
    }
    
    NSString *magFilter = [properties objectForKey:@"magnificationFilter"];
    if (magFilter != nil) {
        texture->setMagnificationFilter([self convertFilterMode:magFilter]);
    }
    
    NSString *mipFilter = [properties objectForKey:@"mipFilter"];
    if (mipFilter != nil) {
        texture->setMipFilter([self convertFilterMode:mipFilter]);
    }
}

- (UIImage *)retrieveImage:(id)json {
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

- (MaterialWrapper *)createMaterial:(NSDictionary *)material name:(NSString *)materialName {
    std::shared_ptr<VROMaterial> vroMaterial = std::make_shared<VROMaterial>();
    vroMaterial->setName([materialName cStringUsingEncoding:NSASCIIStringEncoding]);
    MaterialWrapper *materialWrapper = [[MaterialWrapper alloc] initWithMaterial:vroMaterial];

    for (id key in material) {
        NSString *materialPropertyName = (NSString *)key;
        if ([materialPropertyName hasSuffix:@"texture"] || [materialPropertyName hasSuffix:@"Texture"]) {
            if ([materialPropertyName caseInsensitiveCompare:@"reflectiveTexture"] == NSOrderedSame) {
                std::shared_ptr<VROTexture> texture = [self createTextureCubeMap:material[key]];
                [self loadProperties:material forTexture:texture];
                [self setTextureForMaterial:vroMaterial texture:texture name:materialPropertyName];
                continue;
            } 
            
            NSString *path = [self parseImagePath:material[key]];
            if (path != nil) {
                if ([self isVideoTexture:path]) {
                     std::shared_ptr<VROVideoTextureiOS> texture = std::make_shared<VROVideoTextureiOS>(VROStereoMode::None);
                    NSLog(@"Create material for video texture: %p for mat name: %@", texture.get(), materialName);
                    [materialWrapper setVideoTexturePathForMaterialProp:materialPropertyName path:path];
                    [self setTextureForMaterial:vroMaterial texture:texture name:materialPropertyName];
                    [self loadProperties:material forTexture:texture];
                } else {
                    BOOL sRGB = [materialPropertyName caseInsensitiveCompare:@"diffuseTexture"] == NSOrderedSame
                    || [materialPropertyName caseInsensitiveCompare:@"ambientOcclusionTexture"] == NSOrderedSame;
                    
                    std::shared_ptr<VROTexture> texture = [self createTexture2D:material[key] sRGB:sRGB];
                    [self loadProperties:material forTexture:texture];
                    [self setTextureForMaterial:vroMaterial texture:texture name:materialPropertyName];
                }
            }
        }
        else if ([materialPropertyName hasSuffix:@"color"] || [materialPropertyName hasSuffix:@"Color"]){
            //We can set either an image texture or texture color not both.
            NSNumber *number = material[key];
            NSUInteger argb = [number unsignedIntegerValue];
            [self setColorForMaterial:vroMaterial color:argb name:(NSString *)materialPropertyName];
        }
        else {
            if ([@"shininess" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
                NSNumber *number = material[key];
                vroMaterial->setShininess([number floatValue]);
            } else if ([@"fresnelExponent" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
                NSNumber *number =  material[key];
                vroMaterial->setFresnelExponent([number floatValue]);
            } else if ([@"lightingModel" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
                VROLightingModel lightingModel = [self convertLightingModel:material[key]];
                vroMaterial->setLightingModel(lightingModel);
            } else if ([@"blendMode" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
                VROBlendMode blendMode = [self convertBlendMode:material[key]];
                vroMaterial->setBlendMode(blendMode);
            } else if ([@"transparencyMode" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
                VROTransparencyMode transparencyMode = [self convertTransparencyMode:material[key]];
                vroMaterial->setTransparencyMode(transparencyMode);
            } else if ([@"writesToDepthBuffer" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
                NSNumber *booleanVal = material[key];
                vroMaterial->setWritesToDepthBuffer([booleanVal boolValue]);
            } else if ([@"readsFromDepthBuffer" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
                NSNumber *booleanVal = material[key];
                vroMaterial->setReadsFromDepthBuffer([booleanVal boolValue]);
            } else if ([@"colorWriteMask" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
                NSArray *writeMask = material[key];
                vroMaterial->setColorWriteMask([self convertColorMask:writeMask]);
            } else if ([@"diffuseIntensity" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
                NSNumber *floatVal = material[key];
                vroMaterial->getDiffuse().setIntensity([floatVal floatValue]);
            } else if ([@"cullMode" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
                VROCullMode cullMode = [self convertCullMode:material[key]];
                vroMaterial->setCullMode(cullMode);
            } else if ([@"bloomThreshold" caseInsensitiveCompare:materialPropertyName]  == NSOrderedSame){
                NSNumber *number =  material[key];
                vroMaterial->setBloomThreshold([number floatValue]);
            } else if ([@"metalness" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
                vroMaterial->getMetalness().setColor({ [material[key] floatValue], 1.0, 1.0, 1.0 });
            } else if ([@"roughness" caseInsensitiveCompare:materialPropertyName]  == NSOrderedSame){
                vroMaterial->getRoughness().setColor({ [material[key] floatValue], 1.0, 1.0, 1.0 });
            }
        }
    }
    return materialWrapper;
}

- (void) setPbrPropertyForMaterial:(VROMaterialVisual &)materialVisual
                       property:(id)property {
    if ([property isKindOfClass:[NSNumber class]]){
        materialVisual.setColor({ [property floatValue], 1.0, 1.0, 1.0 });
    } else {
        std::shared_ptr<VROTexture> texture = [self createTexture2D:property sRGB:false];
        materialVisual.setTexture(texture);
    }
}

- (void) setColorForMaterial:(std::shared_ptr<VROMaterial>)material
                      color:(NSUInteger)color
                       name:(NSString *)materialPropertyName {
    
    //parse the int into seperate color components.
    CGFloat a = ((color >> 24) & 0xFF) / 255.0;
    CGFloat r = ((color >> 16) & 0xFF) / 255.0;
    CGFloat g = ((color >> 8) & 0xFF) / 255.0;
    CGFloat b = (color & 0xFF) / 255.0;
    
    VROVector4f vecColor(r, g, b, a);
    if ([@"diffuseColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getDiffuse().setColor(vecColor);
    } else if([@"chromaKeyFilteringColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        VROVector3f vecColorRGB(r, g, b);
        material->setChromaKeyFilteringColor(vecColorRGB);
        material->setChromaKeyFilteringEnabled(true);
    } else if ([@"specularColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getSpecular().setColor(vecColor);
    } else if ([@"normalColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getNormal().setColor(vecColor);
    } else if ([@"reflectiveColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getReflective().setColor(vecColor);
    } else if ([@"emissionColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getEmission().setColor(vecColor);
    } else if ([@"multiplyColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getMultiply().setColor(vecColor);
    } else if ([@"ambientOcclusionColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getAmbientOcclusion().setColor(vecColor);
    } else if ([@"selfIlluminationColor" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getSelfIllumination().setColor(vecColor);
    }
}

- (void)setTextureForMaterial:(std::shared_ptr<VROMaterial>)material texture:(std::shared_ptr<VROTexture>)texture name:(NSString *)materialPropertyName {
    if([@"diffuseTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getDiffuse().setTexture(texture);
    } else if([@"specularTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getSpecular().setTexture(texture);
    } else if([@"normalTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getNormal().setTexture(texture);
    } else if([@"reflectiveTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getReflective().setTexture(texture);
    } else if([@"emissionTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getEmission().setTexture(texture);
    } else if([@"multiplyTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getMultiply().setTexture(texture);
    } else if([@"ambientOcclusionTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getAmbientOcclusion().setTexture(texture);
    } else if([@"selfIlluminationTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame) {
        material->getSelfIllumination().setTexture(texture);
    } else if ([@"metalnessTexture" caseInsensitiveCompare:materialPropertyName] == NSOrderedSame){
        material->getMetalness().setTexture(texture);
    } else if ([@"roughnessTexture" caseInsensitiveCompare:materialPropertyName]  == NSOrderedSame){
        material->getRoughness().setTexture(texture);
    } else if ([@"ambientOcclusionTexture" caseInsensitiveCompare:materialPropertyName]  == NSOrderedSame){
        material->getAmbientOcclusion().setTexture(texture);
    }
}

//Convert string to propert VROLightingModel enum,
- (VROLightingModel)convertLightingModel:(NSString *)name {
    
    if ([@"Phong" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROLightingModel::Phong;
    } else if ([@"Blinn" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROLightingModel::Blinn;
    } else if ([@"Lambert" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROLightingModel::Lambert;
    } else if ([@"Constant" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROLightingModel::Constant;
    } else if ([@"PBR" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROLightingModel::PhysicallyBased;
    }
    //return default if nothing else matches
    return VROLightingModel::Blinn;
}

- (VROFilterMode)convertFilterMode:(NSString *)name {
    if ([@"Nearest" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROFilterMode::Nearest;
    }
    else if([@"Linear" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROFilterMode::Linear;
    }
    else {
        return VROFilterMode::Linear; // Default
    }
}

- (VROWrapMode)convertWrapMode:(NSString *)name {
    if ([@"Clamp" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROWrapMode::Clamp;
    }
    else if([@"Repeat" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROWrapMode::Repeat;
    }
    else if([@"Mirror" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROWrapMode::Mirror;
    }
    else {
        return VROWrapMode::Clamp; // Default
    }
}

- (VROColorMask)convertColorMask:(NSArray *)maskArray {
    VROColorMask mask = VROColorMaskNone;
    for (NSString *maskString : maskArray) {
        if ([maskString caseInsensitiveCompare:@"Red"] == NSOrderedSame) {
            mask = (VROColorMask) (mask | VROColorMaskRed);
        } else if ([maskString caseInsensitiveCompare:@"Green"] == NSOrderedSame) {
            mask = (VROColorMask) (mask | VROColorMaskGreen);
        } else if ([maskString caseInsensitiveCompare:@"Blue"] == NSOrderedSame) {
            mask = (VROColorMask) (mask | VROColorMaskBlue);
        } else if ([maskString caseInsensitiveCompare:@"Alpha"] == NSOrderedSame) {
            mask = (VROColorMask) (mask | VROColorMaskAlpha);
        } else if ([maskString caseInsensitiveCompare:@"All"] == NSOrderedSame) {
            mask = (VROColorMask) (mask | VROColorMaskAll);
        } else if ([maskString caseInsensitiveCompare:@"None"] == NSOrderedSame) {
            mask = (VROColorMask) (mask | VROColorMaskNone);
        }
    }
    return mask;
}

- (BOOL)isVideoTexture:(NSString *)path {
    if ([path containsString:@".mp4"]) {
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

- (VROBlendMode)convertBlendMode:(NSString *)name {
    if ([@"None" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROBlendMode::None;
    } else if([@"Alpha" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROBlendMode::Alpha;
    } else if([@"Add" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROBlendMode::Add;
    } else if([@"Subtract" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROBlendMode::Subtract;
    } else if([@"Multiply" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROBlendMode::Multiply;
    } else if([@"Screen" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROBlendMode::Screen;
    }
    return VROBlendMode::None;
}

- (VROTransparencyMode)convertTransparencyMode:(NSString *)name {
    if ([@"AOne" caseInsensitiveCompare:name] == NSOrderedSame) {
        return VROTransparencyMode::AOne;
    } else if([@"RGBZero" caseInsensitiveCompare:name] == NSOrderedSame) {
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

- (void)loadVideoTextureForMaterial:(NSString *)materialName driver:(std::shared_ptr<VRODriver>)driver context:(VRORenderContext *)context
{
    MaterialWrapper *materialWrapper = _materialDictionary[materialName];
    NSDictionary *videoTextureDict = [materialWrapper getVideoTextures];
    if ([videoTextureDict count] == 0) {
        NSLog(@"Warning: VRTMaterialManager: No video textures found for mat name: %@", materialName);
        return;
    }
    
    std::shared_ptr<VROMaterial> material = [materialWrapper getMaterial];
    std::shared_ptr<VROTexture> texture = material->getDiffuse().getTexture();
    std::shared_ptr<VROVideoTexture> videoTexture = std::dynamic_pointer_cast<VROVideoTextureiOS>(texture);
        RCTImageSource *imageSource = [RCTConvert RCTImageSource:videoTextureDict[@"diffuseTexture"]];
        NSURL *videoURL = imageSource.request.URL;
        std::string url = std::string([[videoURL description] UTF8String]);
        videoTexture->loadVideo(url, context->getFrameSynchronizer(), driver);
        NSLog(@"VRTMaterialManager: Updating video texture, %p for mat name: %@", videoTexture.get(), materialName);
        // update video texture listeners.
        WeakMaterialChangeListenerContainer *weakMaterialChangeListener = _materialChangeListeners[materialName];
    if (weakMaterialChangeListener != nil && weakMaterialChangeListener.listener != nil) {
       id<VRTMaterialChangedDelegate> delegate =   (id<VRTMaterialChangedDelegate>)weakMaterialChangeListener.listener;
        [delegate videoMaterialDidChange:materialName];
    }
}

- (NSDictionary *)getVideoTexturesForMaterialName:(NSString *)name {
    MaterialWrapper *materialWrapper = _materialDictionary[name];
    if (materialWrapper != nil && ([[materialWrapper getVideoTextures] count] > 0) ){
        return [materialWrapper getVideoTextures];
    }
    return nil;
}


- (BOOL)isVideoMaterial:(NSString *)materialName {
    MaterialWrapper *materialWrapper = _materialDictionary[materialName];
    if(materialWrapper != nil && ([[materialWrapper getVideoTextures] count] > 0) ){
        return YES;
    }
    return NO;
}

- (void)addMaterialChangedListener:(NSString *)name listener:(id<VRTMaterialChangedDelegate>)listener {
    WeakMaterialChangeListenerContainer *weakListener = [[WeakMaterialChangeListenerContainer alloc] initWithListener:listener];
    [_materialChangeListeners setObject:weakListener forKey:name];
}

// DEPRECATED: this is only in place for Beta. This needs to be
//             replaced with asynchronous image loading.
//             Tracked by VIRO-912
- (UIImage *)downloadImageSync:(id)json {
    VRTUIImageWrapper *uiImageWrapper = [RCTConvert VRTUIImageWrapper:json];
    if(uiImageWrapper != nil){
        return uiImageWrapper.image;
    }
    return nil;
}

@end
