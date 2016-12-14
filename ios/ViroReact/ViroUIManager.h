//
//  ViroViewManager.h
//  React
//
//  Created by Vik Advani on 10/20/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>



#import <UIKit/UIKit.h>

#import "RCTBridge.h"
#import "RCTBridgeModule.h"
#import "RCTInvalidating.h"
#import "ViroViewManager.h"


@class VRTView;
@protocol VROComponent;

/**
 * The ViroUIManager is the module responsible for updating the view hierarchy.
 */
@interface ViroUIManager : NSObject <RCTBridgeModule, RCTInvalidating>



/**
 * Register a root view with the RCTUIManager.
 */
- (void)registerRootView:(UIView *)rootView;

/**
 * Gets the view associated with a reactTag.
 */
- (VRTView *)viewForReactTag:(NSNumber *)reactTag;

/**
 * Returns true if the React Tag is a VRTView.
 */
- (BOOL)isReactTagViroView:(NSNumber *)reactTag;

- (void)setChildren:(NSNumber *)containerTag
          reactTags:(NSArray<NSNumber *> *)reactTags;


/**
 * Update the frame of a view. This might be in response to a screen rotation
 * or some other layout event outside of the React-managed view hierarchy.
 */
- (void)setFrame:(CGRect)frame forView:(UIView *)view;

/**
 * Update the background color of a root view. This is usually triggered by
 * manually setting the background color of the root view with native code.
 */
- (void)setBackgroundColor:(UIColor *)color forRootView:(UIView *)rootView;

/**
 * Schedule a block to be executed on the UI thread. Useful if you need to execute
 * view logic after all currently queued view updates have completed.
 */
- (void)addUIBlock:(ViroViewManagerUIBlock)block;

/**
 * The view that is currently first responder, according to the JS context.
 */
+ (UIView *)JSResponder;

/**Viro Added methods **/
-(void)updateView:(NSNumber *)reactTag viewName:(NSString *)viewName props:(NSDictionary *)props;

-(void)createView:(NSNumber *)reactTag
         viewName:(NSString *)viewName
          rootTag:(__unused NSNumber *)rootTag
            props:(NSDictionary *)props;


-(void)manageChildrenUIThread:( NSNumber *)containerReactTag
      moveFromIndices:(NSArray *)moveFromIndices
        moveToIndices:(NSArray *)moveToIndices
    addChildReactTags:(NSArray *)addChildReactTags
         addAtIndices:(NSArray *)addAtIndices
      removeAtIndices:(NSArray *)removeAtIndices;

-(void)manageChildrenShadowThread:( NSNumber *)containerReactTag
              moveFromIndices:(NSArray *)moveFromIndices
                moveToIndices:(NSArray *)moveToIndices
            addChildReactTags:(NSArray *)addChildReactTags
                 addAtIndices:(NSArray *)addAtIndices
              removeAtIndices:(NSArray *)removeAtIndices;

/**
 Returns true if the viewName is a component found in ViroUIManager.
 */
- (bool) hasComponent:(NSString *)viewName;

-(BOOL) isSceneNavigatorView:(id<VROComponent>)viewVRO;

-(UIView *) getRootUIViewFromSceneNav:(id<VROComponent>)viewVRO;

- (void)layoutAndQueueViroUIBlocks;

@end

/**
 * This category makes the current RCTUIManager instance available via the
 * RCTBridge, which is useful for RCTBridgeModules or RCTViewManagers that
 * need to access the RCTUIManager.
 */
@interface RCTBridge (ViroUIManager)

@property (nonatomic, readonly) ViroUIManager *viroUIManager;

@end