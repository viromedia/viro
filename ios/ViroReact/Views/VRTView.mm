//
//  VRTView.m
//  React
//
//  Created by Vik Advani on 10/21/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTView.h"
#import "VROUtils.h"
#import "RCTLog.h"
#import "VROMaterialManager.h"
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
    VROMaterialManager *materialManager = [_bridge materialManager];
    NSDictionary *videoTextures= [materialManager getVideoTexturesForMaterialName:materialName];
    std::shared_ptr<VROMaterial> material = [materialManager getMaterialByName:materialName];
    if(videoTextures != nil){
      
      // TODO The render context and driver are NOT set anywhere!
      for(id key in videoTextures){
        NSString *path = videoTextures[(NSString *)key];
        std::shared_ptr<VROVideoTexture> videoTexture = [VROMaterialManager createVideoTextureForMaterial:material videoPath:path name:(NSString *)key renderContext:self.renderContext driver:self.driver];
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
  if (self.scene) {
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

-(void)viewWillDisappear{
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

// Default implementation returns no.
- (BOOL)didViewHit:(VROHitTestResult)hitResult {
  return NO;
}

// Execute the given hit event
- (BOOL)invokeHitEvent {
  // VRTViews don't handle hit events in any scenario.
  return NO;
}

// Whether or not this element is hoverable
- (BOOL)hoverable {
  // VRTViews don't respond to hover events in any scenario.
  return NO;
}

- (VRTView *)findViewHit:(VROHitTestResult)hitResult {
  // TODO: this is currently a N^2 operation because we have no mapping between a VRONode and a VRTNode/VRTView. If we did, this wouldn't even have to be recursive.
  if ([self didViewHit:hitResult]) {
    return self;
  } else {
    if (_childViews && [_childViews count] > 0) {
      VRTView* matchedView;
      for (VRTView* childView in _childViews) {
        matchedView = [childView findViewHit:hitResult];
        if (matchedView) {
          return matchedView;
        }
      }
    }
  }
  // we haven't found a view that matched the hitResult.
  return nil;
}

- (NSArray<VRTView *> *)viewHierarchyOfResult:(VROHitTestResult)hitResult {

  NSMutableArray<VRTView *> *views = [@[self] mutableCopy];//[[NSMutableArray alloc] initWithObjects:@[self]];

  // base case: if I'm the hit result, return myself.
  if ([self didViewHit: hitResult]) {
    return views;
  }

  // recurse: for each child, see if it contains the hit view.
  for (VRTView *childView in _childViews) {
    NSArray<VRTView *> *tempArr = [childView viewHierarchyOfResult:hitResult];
    
    // If one of the children contains the hitResult, append and return the views array
    if (tempArr) {
      [views addObjectsFromArray:tempArr];
      return views;
    }
  }

  // none of the views in this tree contain the hitResult.
  return nil;
}

@end
