//
//  VRTView.m
//  React
//
//  Created by Vik Advani on 10/21/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTView.h"
#import "VRTUtils.h"
#import "RCTLog.h"
#import "VRTMaterialManager.h"
#import <UIKit/UIKit.h>
#import <objc/runtime.h>

/*
 TODO
 
 Old code for starting video textures. This needs to be run somewhere if we want
 video textures to run automatically. Place in viewWillAppear:?
 
 VRTView *vroView = (VRTView *)view;

if([subview respondsToSelector:@selector(material)]) {
  NSString *materialName = [vroView material];
  if(materialName != nil){
    VRTMaterialManager *materialManager = [_bridge materialManager];
    NSDictionary *videoTextures= [materialManager getVideoTexturesForMaterialName:materialName];
    std::shared_ptr<VROMaterial> material = [materialManager getMaterialByName:materialName];
    if(videoTextures != nil){
      
      // TODO The render context and driver are NOT set anywhere!
      for(id key in videoTextures){
        NSString *path = videoTextures[(NSString *)key];
        std::shared_ptr<VROVideoTexture> videoTexture = [VRTMaterialManager createVideoTextureForMaterial:material videoPath:path name:(NSString *)key renderContext:self.renderContext driver:self.driver];
        [materialManager setTextureForMaterial:material texture:videoTexture name:materialPropertyName];

      }
    }
  }
}
 */

@implementation VRTView

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super init];
  if (self) {
    _bridge = bridge;
    _childViews = [[NSMutableArray alloc] init];
  }
  
  return self;
}

- (NSNumber *)reactTag {
  return objc_getAssociatedObject(self, _cmd);
}

- (void)setReactTag:(NSNumber *)reactTag {
  objc_setAssociatedObject(self, @selector(reactTag), reactTag, OBJC_ASSOCIATION_COPY_NONATOMIC);
}

- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex {
  // Subclasses must override to perform physical actions (i.e adding
  // nodes in the renderer), then invoke this
  
  VRTView *view = (VRTView *)subview;
  
  [_childViews insertObject:subview atIndex:atIndex];
  view.superview = self;
  
  /*
   If the scene has appeared, meaning the renderer is ready, then invoke 
   viewWillAppear on insertion. Note if the scene has not yet appeared, then
   viewWillAppear will be run when it *does* appear (see VRTScene::sceneWillAppear).
   */
  
  if (self.scene && self.driver && self.context) {
    view.context = self.context;
    view.driver = self.driver;
    view.scene = self.scene;
    [view viewWillAppear];
  }
}

- (void)removeReactSubview:(UIView *)subview {
  // Subclasses must override to perform physical actions (i.e. removing
  // nodes from the renderer), then invoke this
  
  [_childViews removeObject:subview];
  ((VRTView *) subview).superview = NULL;
}

- (NSArray *)reactSubviews {
  return _childViews;
}

- (id<VROComponent>)reactSuperview {
  return nil;
}

- (NSNumber *)reactTagAtPoint:(CGPoint)point {
  return nil;
}

// View/ShadowView is a root view
- (BOOL)isReactRootView {
  return false;
}

- (void)viewWillAppear {
    for (id childView in _childViews) {
      VRTView *view = (VRTView *)childView;
      [view viewWillAppear];
    }
}

-(void)viewWillDisappear {
  for (id childView in _childViews) {
    VRTView *view = (VRTView *)childView;
    [view viewWillDisappear];
  }
}

- (void)setContext:(VRORenderContext *)context {
  _context = context;
  for (id childView in _childViews) {
    VRTView *view = (VRTView *)childView;
    view.context = _context;
  }
}

- (void)setDriver:(VRODriver *)driver {
  _driver = driver;
  for (id childView in _childViews) {
    VRTView *view = (VRTView *)childView;
    view.driver = _driver;
  }
}

- (void)setScene:(std::shared_ptr<VROScene>)scene {
  _scene = scene;
  for (id childView in _childViews) {
    VRTView *view = (VRTView *)childView;
    view.scene = _scene;
  }
}
@end
