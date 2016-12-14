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
  
  // TODO: check scheme to see if equal to 'file, 'data' or 'http' to handle streaming models.
  _url = [RCTConvert NSURL:path];
  // NSString *scheme = _url.scheme.lowercaseString;
  
  downloadDataWithURL(_url, ^(NSData *data, NSError *error) {
    if (error.code == NSURLErrorCancelled){
      return;
    }
    
    if (!error) {
      NSString *fileName = [NSString stringWithFormat:@"%@_%@", [[NSProcessInfo processInfo] globallyUniqueString], @"model.obj"];
      NSURL *fileURL = [NSURL fileURLWithPath:[NSTemporaryDirectory() stringByAppendingPathComponent:fileName]];
      
      [data writeToURL:fileURL atomically:NO];
      
      std::shared_ptr<VRONode> modelNode =  VROLoader::loadURL(fileURL)[0];
      [self node]->setGeometry(modelNode->getGeometry());
      [self applyMaterials];
      
      NSError *deleteError = nil;
      [[NSFileManager defaultManager] removeItemAtURL:fileURL error:&deleteError];
    }
  });
  

}

@end
