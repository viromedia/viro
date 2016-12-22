//
//  VRT3DObject.m
//  React
//
//  Created by Vik Advani on 4/14/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRT3DObject.h"
#import "RCTConvert.h"
#import "VROMaterialManager.h"
#import "VROUtils.h"

@implementation VRT3DObject {
  
  NSURL *_url;
  std::shared_ptr<VROMaterial> _objMaterial;
  std::vector<std::shared_ptr<VROMaterial>> _vroMaterialArray;
  
}

@synthesize materials = _materials;

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
  self = [super initWithBridge:bridge];
  
  return self;
}

- (void)setOBJMaterials:(std::shared_ptr<VROGeometry>)geometry {
    if (!geometry) {
        return;
    }
    
    VROMaterialManager *materialManager = [self.bridge moduleForClass:[VROMaterialManager class]];
    
    /*
     The materials set for a 3D model overwrite the materials set by the
     OBJ loader.
     */
    for (int i = 0; i < [self.materials count]; i++) {
        NSString *materialName = [self.materials objectAtIndex:i];
        
        std::shared_ptr<VROMaterial> material = [materialManager getMaterialByName:materialName];
        if (material != NULL) {
            if (i < geometry->getMaterials().size()) {
                geometry->getMaterials()[i] = material;
            }
            else {
                RCTLogError(@"Model has %d elements, material %d [%@] cannot be set",
                            geometry->getMaterials().size(), i, materialName);
            }
        }
        else {
            RCTLogError(@"Unknown material name: \"%@\"", materialName);
        }
    }
}

- (void)setSource:(NSDictionary *)modelDict {
  if (![NSThread isMainThread]) {
    RCTLogWarn(@"Calling [RCTConvert setSource:] on a background thread is not recommended");
    dispatch_sync(dispatch_get_main_queue(), ^{
      [self setSource:modelDict];
    });
    
    return;
  }
  
  NSString *path;
  if (!(path = [RCTConvert NSString:modelDict[@"uri"]])) {
    RCTLogError(@"Unable to load 3D model object with no path");
  }
  
  _url = [RCTConvert NSURL:path];
  std::string url = std::string([[_url description] UTF8String]);
  std::string base = url.substr(0, url.find_last_of('/'));

  VROOBJLoader::loadOBJFromURL(url, base, true,
    [self](std::shared_ptr<VRONode> node, bool success) {
        if (!success) {
            return;
        }
        
        [self setOBJMaterials:node->getGeometry()];
        self.node->setGeometry(node->getGeometry());
    });
}

@end
