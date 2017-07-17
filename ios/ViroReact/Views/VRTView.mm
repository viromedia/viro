//
//  VRTView.m
//  React
//
//  Created by Vik Advani on 10/21/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTLog.h>
#import <React/RCTComponent.h>
#import "VRTView.h"
#import "VRTUtils.h"
#import "VRTMaterialManager.h"
#import <UIKit/UIKit.h>
#import <objc/runtime.h>

@implementation VRTView

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super init];
    if (self) {
        _bridge = bridge;
        _childViews = [[NSMutableArray alloc] init];
        _canAppear = NO;
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
     sceneWillAppear on insertion. Note if the scene has not yet appeared, then
     sceneWillAppear will be run when it *does* appear (see VRTScene::sceneWillAppear).
     */
    if (self.scene && self.driver && self.context) {
        view.context = self.context;
        view.driver = self.driver;
        view.scene = self.scene;
        [view sceneWillAppear];
    }
    
    /*
     If adding a child to a view that shouldAppear, then call viewWillAppear
     on the child to let it know that it "canAppear"
     */
    if ([self shouldAppear]) {
        [view viewWillAppear];
    }
}

- (void)removeReactSubview:(UIView *)subview {
    // Subclasses must override to perform physical actions (i.e. removing
    // nodes from the renderer)
    
    VRTView *view = (VRTView *)subview;
    [view viewWillDisappear];
    
    [_childViews removeObject:subview];
    view.superview = NULL;
}

- (NSArray *)reactSubviews {
    return _childViews;
}

- (id<RCTComponent>)reactSuperview {
    return nil;
}

- (NSNumber *)reactTagAtPoint:(CGPoint)point {
    return nil;
}

// View/ShadowView is a root view
- (BOOL)isReactRootView {
    return false;
}

- (BOOL)shouldAppear {
    return self.canAppear;
}

- (void)handleAppearanceChange {
    if ([self shouldAppear]) {
        for (id childView in _childViews) {
            VRTView *view = (VRTView *)childView;
            [view viewWillAppear];
        }
    } else {
        for (id childView in _childViews) {
            VRTView *view = (VRTView *)childView;
            [view viewWillDisappear];
        }
    }
}

- (void)sceneWillAppear {
    for (id childView in _childViews) {
        VRTView *view = (VRTView *)childView;
        [view sceneWillAppear];
    }
}

- (void)sceneWillDisappear {
    for (id childView in _childViews) {
        VRTView *view = (VRTView *)childView;
        [view sceneWillDisappear];
    }
}

- (void)viewWillAppear {
    self.canAppear = true;
    [self handleAppearanceChange];
}

- (void)viewWillDisappear {
    self.canAppear = false;
    [self handleAppearanceChange];
}

- (void)setContext:(VRORenderContext *)context {
    _context = context;
    for (id childView in _childViews) {
        VRTView *view = (VRTView *)childView;
        view.context = _context;
    }
}

- (void)setDriver:(std::shared_ptr<VRODriver>)driver {
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
