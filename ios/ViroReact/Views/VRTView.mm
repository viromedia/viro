//
//  VRTView.m
//  React
//
//  Created by Vik Advani on 10/21/15.
//  Copyright © 2016 Viro Media. All rights reserved.
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
        _parentHasAppeared = NO;
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
     If this view shouldAppear, then let the given child know that its parentDidAppear.
     */
    if ([self shouldAppear]) {
        [view parentDidAppear];
    }
}

- (void)removeReactSubview:(UIView *)subview {
    // Subclasses must override to perform physical actions (i.e. removing
    // nodes from the renderer)
    
    VRTView *view = (VRTView *)subview;
    [view parentDidDisappear];
    
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
    return self.parentHasAppeared;
}

- (void)handleAppearanceChange {
    if ([self shouldAppear]) {
        for (id childView in _childViews) {
            VRTView *view = (VRTView *)childView;
            [view parentDidAppear];
        }
    } else {
        for (id childView in _childViews) {
            VRTView *view = (VRTView *)childView;
            [view parentDidDisappear];
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

- (void)parentDidAppear {
    self.parentHasAppeared = true;
    [self handleAppearanceChange];
}

- (void)parentDidDisappear {
    self.parentHasAppeared = false;
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

-(void)dealloc {
    _superview = nil;
    _childViews = nil;
    _scene = nil;
}

- (void)setScene:(std::shared_ptr<VROScene>)scene {
    _scene = scene;
    for (id childView in _childViews) {
        VRTView *view = (VRTView *)childView;
        view.scene = _scene;
    }
}
@end
