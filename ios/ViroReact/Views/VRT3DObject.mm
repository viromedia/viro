//
//  VRT3DObject.m
//  React
//
//  Created by Vik Advani on 4/14/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTConvert.h>
#import "VRT3DObject.h"
#import "VRTMaterialManager.h"
#import "VRTUtils.h"

@implementation VRT3DObject {
  
  NSURL *_url;
  std::shared_ptr<VROMaterial> _objMaterial;
  BOOL _sourceChanged;
  
}

- (instancetype)initWithBridge:(RCTBridge *)bridge  {
  self = [super initWithBridge:bridge];
  _sourceChanged = NO;
    
  return self;
}

- (void)setSource:(NSDictionary *)source {
  _source = source;
  _sourceChanged = YES;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
  if (![NSThread isMainThread]) {
    RCTLogWarn(@"Calling [RCTConvert didSetProps:] on a background thread is not recommended");
    dispatch_sync(dispatch_get_main_queue(), ^{
        [self didSetProps:changedProps];
    });
    
    return;
  }
  
  // Only reload the model if its source changed
  if (!_sourceChanged) {
      return;
  }
  
  NSString *path;
  if (!(path = [RCTConvert NSString:self.source[@"uri"]])) {
    RCTLogError(@"Unable to load 3D model object with no path");
  }
  
  _url = [RCTConvert NSURL:path];
  std::string url = std::string([[_url description] UTF8String]);
  std::string base = url.substr(0, url.find_last_of('/'));

  if (self.onLoadStartViro) {
    self.onLoadStartViro(nil);
  }
  
  BOOL isOBJ = NO;
  std::string urlPath = std::string([[_url path] UTF8String]);
  VROStringUtil::toLowerCase(urlPath);
  if (VROStringUtil::endsWith(urlPath, "obj")) {
    isOBJ = YES;
  }
  
  std::function<void(std::shared_ptr<VRONode> node, bool success)> onFinish =
  [self](std::shared_ptr<VRONode> node, bool success) {
    if (success) {
      // The geometry is set for OBJ models
      self.node->setGeometry(node->getGeometry());
      
      // The children are set for FBX models (in FBX, the root node is a dummy node)
      for (std::shared_ptr<VRONode> child : node->getSubnodes()) {
        self.node->addChildNode(child);
      }
      
      if (self.materials) {
        [self applyMaterials];
      }
    }
    
    if (self.onLoadEndViro) {
      self.onLoadEndViro(nil);
    }
    if (!success) {
      if (self.onErrorViro) {
        self.onErrorViro(@{ @"error": @"model failed to load" });
      }
    }
  };
  
  if (isOBJ) {
    VROOBJLoader::loadOBJFromURL(url, base, true, onFinish);
  }
  else {
    VROFBXLoader::loadFBXFromURL(url, base, true, onFinish);
  }
  _sourceChanged = NO;
}

@end
