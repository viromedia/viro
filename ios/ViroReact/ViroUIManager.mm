//
//  ViroViewManager.m
//  React
//
//  ViewManager to handle viro rendering based views.
//  Created by Vik Advani on 10/20/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "ViroUIManager.h"

// NOTE: This is based on RCTUIManager.m. The ViroUIManager is invoked from the RCTUIManager. This class takes over the logic of view creation, updates, destruction for Viro components. This is the messiest class Viro currently has. When updating from React, go through the methods in RCTUIManager to ensure the logic matches in each method, especially:
//  1. uiBlockWithLayoutUpdateForRootView
//  2. createView
//
//  There are many commented out methods in this class from RCTUIManager currently as we're not fully sure what we need/don't need just yet.

#import "RCTUIManager.h"

#import <AVFoundation/AVFoundation.h>

#import <CSSLayout/CSSLayout.h>
#import "RCTAccessibilityManager.h"
#import "RCTAnimationType.h"
#import "RCTAssert.h"
#import "RCTBridge.h"
#import "RCTComponent.h"
#import "RCTComponentData.h"
#import "RCTConvert.h"
#import "RCTDefines.h"
#import "RCTEventDispatcher.h"
#import "RCTLog.h"
#import "RCTProfile.h"
#import "RCTRootView.h"
#import "RCTScrollableProtocol.h"
#import "RCTUtils.h"
#import "RCTView.h"
#import "ViroViewManager.h"
#import "VRTView.h"
#import "ViroUIManager.h"
#import "VROComponentData.h"
#import "VRTShadowView.h"
#import "VRTShadowFlexView.h"
#import "VRTSceneShadowView.h"
#import "VRTNodeContainerShadowView.h"
#import "VRTSceneNavigator.h"
#import "UIView+React.h"
#import "VRTNode.h"
#import "VRTSceneNavigatorManager.h"

static void RCTTraverseViewNodes(id<VROComponent> view, void (^block)(id<VROComponent>))
{
  if (view.reactTag) block(view);
  for (id<VROComponent> subview in view.reactSubviews) {
    RCTTraverseViewNodes(subview, block);
  }
}

//NSString *const RCTUIManagerWillUpdateViewsDueToContentSizeMultiplierChangeNotification = @"RCTUIManagerWillUpdateViewsDueToContentSizeMultiplierChangeNotification";
//NSString *const RCTUIManagerDidRegisterRootViewNotification = @"RCTUIManagerDidRegisterRootViewNotification";
//NSString *const RCTUIManagerDidRemoveRootViewNotification = @"RCTUIManagerDidRemoveRootViewNotification";
//NSString *const RCTUIManagerRootViewKey = @"RCTUIManagerRootViewKey";


@interface RCTLayoutAnimation : NSObject

@property (nonatomic, copy) NSDictionary *config;

@property (nonatomic, copy) RCTResponseSenderBlock callback;

@end



@interface ViroUIManager ()

// NOTE: these are properties so that they can be accessed by unit tests
@property (nonatomic, strong) RCTSparseArray *viewManagerRegistry; // RCT thread only

@property (nonatomic, strong) NSMutableDictionary<NSNumber *, VRTShadowView *> *shadowViewRegistry; // VRTShadowView thread only

@property (nonatomic, strong)  NSMutableDictionary<NSNumber *, VRTView *> *viewRegistry; // Main thread only

@end

@implementation ViroUIManager
{
  // every scene in Viro has one root view, a ViroSceneNavigator.
  NSNumber *_rootViewTag;
  NSMutableSet *_rootFlexViewTags;
  NSMutableArray *_pendingUIBlocks;
  NSLock *_pendingUIBlocksLock;

  // Animation
  RCTLayoutAnimation *_layoutAnimation; // Main thread only
  NSMutableSet<UIView *> *_viewsToBeDeleted; // Main thread only
  
  // Keyed by viewName
  NSDictionary *_componentDataByName;
  
 NSMutableSet<id<VROComponent>> *_bridgeTransactionListeners;
}

@synthesize bridge = _bridge;


RCT_EXPORT_MODULE()

/**
 * Declared in RCTBridge.
 */
extern NSString *RCTBridgeModuleNameForClass(Class cls);

- (instancetype)init
{
  if ((self = [super init])) {
    _pendingUIBlocksLock = [NSLock new];
    
    _shadowViewRegistry =  [NSMutableDictionary new];
    _viewRegistry =  [NSMutableDictionary new];
    
    // Internal resources
    _pendingUIBlocks = [NSMutableArray new];
    _rootFlexViewTags = [NSMutableSet new];
    
    _bridgeTransactionListeners = [NSMutableSet new];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(didReceiveNewContentSizeMultiplier)
                                                 name:RCTAccessibilityManagerDidUpdateMultiplierNotification
                                               object:nil];
  
    NSLog(@"ViroUIManager initialized");
  }
  return self;
}

- (void)dealloc
{
  [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)didReceiveNewContentSizeMultiplier
{
  __weak ViroUIManager *weakSelf = self;
  dispatch_async(self.methodQueue, ^{
    ViroUIManager *strongSelf = weakSelf;
    if (strongSelf) {
      [[NSNotificationCenter defaultCenter] postNotificationName:RCTUIManagerWillUpdateViewsDueToContentSizeMultiplierChangeNotification
                                                          object:strongSelf];
      [strongSelf batchDidComplete];
    }
  });
}

- (void)invalidate
{
  NSLog(@"Invalidate called on ViroUIManager");
  /**
   * Called on the JS Thread since all modules are invalidated on the JS thread
   */
  
  dispatch_async(dispatch_get_main_queue(), ^{
    if (_rootViewTag) {
      [(id<RCTInvalidating>)_viewRegistry[_rootViewTag] invalidate];
    }

    _rootViewTag = nil;
    _rootFlexViewTags = nil;
    _shadowViewRegistry = nil;
    _viewRegistry = nil;
    _bridgeTransactionListeners = nil;
    _bridge = nil;
    
    [_pendingUIBlocksLock lock];
    _pendingUIBlocks = nil;
    [_pendingUIBlocksLock unlock];
  });
}


- (void)setBridge:(RCTBridge *)bridge
{
  RCTAssert(_bridge == nil, @"Should not re-use same UIIManager instance");
  
  _bridge = bridge;
  
  _shadowViewRegistry = [NSMutableDictionary new];
  _viewRegistry = [NSMutableDictionary new];
  
  // Internal resources
  _pendingUIBlocks = [NSMutableArray new];
  _rootViewTag = nil;
  _rootFlexViewTags = [NSMutableSet new];
  
  _bridgeTransactionListeners = [NSMutableSet new];
  
  _viewsToBeDeleted = [NSMutableSet new];
  
  // Get view managers from bridge
  NSMutableDictionary *componentDataByName = [NSMutableDictionary new];
  for (Class moduleClass in _bridge.moduleClasses) {
    if ([moduleClass isSubclassOfClass:[ViroViewManager class]]) {
      VROComponentData *componentData = [[VROComponentData alloc] initWithManagerClass:moduleClass
                                                                                bridge:_bridge];
      componentDataByName[componentData.name] = componentData;
    }
    
  }
  _componentDataByName = [componentDataByName copy];
}


- (bool) hasComponent:(NSString *)viewName {
  if(_componentDataByName[viewName] != nil){
    return true;
  }
  return false;
}

- (dispatch_queue_t)methodQueue
{
  return RCTGetUIManagerQueue();
}

- (void)registerRootView:(UIView *)rootView
{
  RCTAssertMainThread();
  
  /**
  NSNumber *reactTag = rootView.reactTag;
  RCTAssert(RCTIsReactRootView(reactTag),
            @"View %@ with tag #%@ is not a root view", rootView, reactTag);
  
  UIView *existingView = _viewRegistry[reactTag];
  RCTAssert(existingView == nil || existingView == rootView,
            @"Expect all root views to have unique tag. Added %@ twice", reactTag);
  
  // Register view
  _viewRegistry[reactTag] = rootView;
  CGRect frame = rootView.frame;
  
  // Register shadow view
  __weak ViroUIManager *weakSelf = self;
  dispatch_async(_shadowQueue, ^{
    ViroUIManager *strongSelf = weakSelf;
    if (!_viewRegistry) {
      return;
    }
    RCTShadowView *shadowView = [RCTShadowView new];
    shadowView.reactTag = reactTag;
    shadowView.frame = frame;
    shadowView.backgroundColor = rootView.backgroundColor;
    shadowView.viewName = NSStringFromClass([rootView class]);
    strongSelf->_shadowViewRegistry[shadowView.reactTag] = shadowView;
    [strongSelf->_rootViewTags addObject:reactTag];
  });
  
  [[NSNotificationCenter defaultCenter] postNotificationName:RCTUIManagerDidRegisterRootViewNotification
                                                      object:self
                                                    userInfo:@{ RCTUIManagerRootViewKey: rootView }];
   
   **/
}

- (VRTView *)viewForReactTag:(NSNumber *)reactTag
{
  //RCTAssertMainThread();
  return _viewRegistry[reactTag];
}

// Invoked from RCTUIManager shadow thread.
- (BOOL)isReactTagViroView:(NSNumber *)reactTag
{
    if(_shadowViewRegistry[reactTag]!= nil){
        return YES;
    }

    return NO;
}

- (void)setFrame:(CGRect)frame forView:(UIView *)view
{
  RCTAssertMainThread();
  
  NSNumber *reactTag = view.reactTag;
  dispatch_async(RCTGetUIManagerQueue(), ^{
    /**VA: RCTShadowView *rootShadowView = _shadowViewRegistry[reactTag];
    RCTAssert(rootShadowView != nil, @"Could not locate root view with tag #%@", reactTag);
    rootShadowView.frame = frame;
    [rootShadowView updateLayout];
    **/
    [self batchDidComplete];
  });
}

- (void)setBackgroundColor:(UIColor *)color forRootView:(UIView *)rootView
{
  RCTAssertMainThread();
  
  NSNumber *reactTag = rootView.reactTag;
  RCTAssert(RCTIsReactRootView(reactTag), @"Specified view %@ is not a root view", reactTag);
  
  __weak ViroUIManager *weakSelf = self;
  dispatch_async(RCTGetUIManagerQueue(), ^{
    ViroUIManager *strongSelf = weakSelf;
    if (!_viewRegistry) {
      return;
    }
    //VA: RCTShadowView *rootShadowView = strongSelf->_shadowViewRegistry[reactTag];
    //RCTAssert(rootShadowView != nil, @"Could not locate root view with tag #%@", reactTag);
    //rootShadowView.backgroundColor = color;
    //[self _amendPendingUIBlocksWithStylePropagationUpdateForRootView:rootShadowView];
    [self flushUIBlocks];
  });
}

/**
 * Unregisters views from registries
 */
- (void)_purgeChildren:(NSArray *)children fromRegistry:(NSMutableDictionary<NSNumber *, id<VROComponent>> *)registry
{
  for (id<VROComponent> child in children) {
    RCTTraverseViewNodes(registry[child.reactTag], ^(id<VROComponent> subview) {
      RCTAssert(![subview isReactRootView], @"Root views should not be unregistered");
      if ([subview conformsToProtocol:@protocol(RCTInvalidating)]) {
        [(id<RCTInvalidating>)subview invalidate];
      }
      registry[subview.reactTag] = nil;
      
      if (registry == _viewRegistry) {
        [_bridgeTransactionListeners removeObject:subview];
      }
    });
  }
}


- (void)addUIBlock:(ViroViewManagerUIBlock)block
{
  //VA: Comment this out until we have a proper shadowQueue
  //RCTAssertThread(_shadowQueue,
  //                @"-[RCTUIManager addUIBlock:] should only be called from the "
  //                "UIManager's _shadowQueue (it may be accessed via `bridge.uiManager.methodQueue`)");
  
  if (!block) {
    return;
  }
  
  if (!_viewRegistry) {
    return;
  }
  
  __weak ViroUIManager *weakViewManager = self;
  dispatch_block_t outerBlock = ^{
    ViroUIManager *strongViewManager = weakViewManager;
    if (strongViewManager && strongViewManager->_viewRegistry) {
      block(strongViewManager, strongViewManager->_viewRegistry);
    }
  };
  
  [_pendingUIBlocksLock lock];
  [_pendingUIBlocks addObject:outerBlock];
  [_pendingUIBlocksLock unlock];
}


- (ViroViewManagerUIBlock)uiBlockWithLayoutUpdateForRootView:(VRTShadowFlexView *)rootShadowView
{
  RCTAssert(!RCTIsMainQueue(), @"Should be called on shadow queue");
  
  // This is nuanced. In the JS thread, we create a new update buffer
  // `frameTags`/`frames` that is created/mutated in the JS thread. We access
  // these structures in the UI-thread block. `NSMutableArray` is not thread
  // safe so we rely on the fact that we never mutate it after it's passed to
  // the main thread.
  NSMutableOrderedSet<VRTShadowView *> *viewsWithNewFrames = [rootShadowView collectViewsWithUpdatedFrames];
  
  if (!viewsWithNewFrames.count) {
    // no frame change results in no UI update block
    return nil;
  }
  
  typedef struct {
    CGRect frame;
    BOOL isNew;
    BOOL parentIsNew;
    BOOL isHidden;
  } RCTFrameData;
  
  // Construct arrays then hand off to main thread
  NSUInteger count = viewsWithNewFrames.count;
  NSMutableArray *reactTags = [[NSMutableArray alloc] initWithCapacity:count];
  NSMutableData *framesData = [[NSMutableData alloc] initWithLength:sizeof(RCTFrameData) * count];
  {
    NSUInteger index = 0;
    RCTFrameData *frameDataArray = (RCTFrameData *)framesData.mutableBytes;
    for (VRTShadowView *shadowView in viewsWithNewFrames) {
      reactTags[index] = shadowView.reactTag;
      frameDataArray[index++] = (RCTFrameData){
        shadowView.frame,
        shadowView.isNewView,
        shadowView.superview.isNewView,
        shadowView.isHidden,
      };
    }
  }
  
  // These are blocks to be executed on each view, immediately after
  // reactSetFrame: has been called. Note that if reactSetFrame: is not called,
  // these won't be called either, so this is not a suitable place to update
  // properties that aren't related to layout.
  NSMutableDictionary<NSNumber *, ViroViewManagerUIBlock> *updateBlocks =
  [NSMutableDictionary new];
  for (VRTShadowView *shadowView in viewsWithNewFrames) {
    
    // We have to do this after we build the parentsAreNew array.
    shadowView.newView = NO;
    
    NSNumber *reactTag = shadowView.reactTag;
    ViroViewManager *manager = [(VROComponentData *)_componentDataByName[shadowView.viewName] manager];
      ViroViewManagerUIBlock block = nil;
    if (block) {
      updateBlocks[reactTag] = block;
    }
    
    if (shadowView.onLayout) {
      CGRect frame = shadowView.frame;
      shadowView.onLayout(@{
                            @"layout": @{
                                @"x": @(frame.origin.x),
                                @"y": @(frame.origin.y),
                                @"width": @(frame.size.width),
                                @"height": @(frame.size.height),
                                },
                            });
    }
    
    //VA: Removed if(RCT(RCTIsReactRootView(reactTag)) statement that is in RCTUIManager.
  }
  
  // Perform layout (possibly animated)
  return ^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    
    const RCTFrameData *frameDataArray = (const RCTFrameData *)framesData.bytes;
    RCTLayoutAnimation *layoutAnimation = uiManager->_layoutAnimation;
    
    __block NSUInteger completionsCalled = 0;
    
    NSInteger index = 0;
    for (NSNumber *reactTag in reactTags) {
      RCTFrameData frameData = frameDataArray[index++];
      
      VRTView *view = viewRegistry[reactTag];
      CGRect frame = frameData.frame;
      
      BOOL isHidden = frameData.isHidden;
      BOOL isNew = frameData.isNew;
      //RCTAnimation *updateAnimation = isNew ? nil : layoutAnimation.updateAnimation;
      BOOL shouldAnimateCreation = isNew && !frameData.parentIsNew;
      //RCTAnimation *createAnimation = shouldAnimateCreation ? layoutAnimation.createAnimation : nil;
      
      void (^completion)(BOOL) = ^(BOOL finished) {
        completionsCalled++;
        if (layoutAnimation.callback && completionsCalled == count) {
          layoutAnimation.callback(@[@(finished)]);
          
          // It's unsafe to call this callback more than once, so we nil it out here
          // to make sure that doesn't happen.
          layoutAnimation.callback = nil;
        }
      };
      //VA: Hidden flag is not yet part of VRTView. Commenting out for now.
      //if (view.isHidden != isHidden) {
      //  view.hidden = isHidden;
      //}
      
      ViroViewManagerUIBlock updateBlock = updateBlocks[reactTag];
      //VA: We removed the createAnimationCode
        // Update without animation
      
      if ([view isKindOfClass:[VRTNode class]]) {
        [((VRTNode *)view) reactSetFrame:frame];
      }
        if (updateBlock) {
          updateBlock(self, viewRegistry);
        }
        completion(YES);
    }
    
    // Clean up
    uiManager->_layoutAnimation = nil;
  };
}

- (void)_amendPendingUIBlocksWithStylePropagationUpdateForRootView:(VRTShadowView *)topView
{
  NSMutableSet *applierBlocks = [NSMutableSet setWithCapacity:1];
  [topView collectUpdatedProperties:applierBlocks parentProperties:@{}];
  
  //VA: Comment out for shadows view
 
  [self addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    for (VRTApplierBlock block in applierBlocks) {
      block(viewRegistry);
    }
  }];
 
}

/**
 * A method to be called from JS, which takes a container ID and then releases
 * all subviews for that container upon receipt.
 */
RCT_EXPORT_METHOD(removeSubviewsFromContainerWithID:(nonnull NSNumber *)containerID)
{
  /**
  //id<VROComponent> container = _shadowViewRegistry[containerID];
  RCTAssert(container != nil, @"container view (for ID %@) not found", containerID);
  
  NSUInteger subviewsCount = [container reactSubviews].count;
  NSMutableArray *indices = [[NSMutableArray alloc] initWithCapacity:subviewsCount];
  for (NSUInteger childIndex = 0; childIndex < subviewsCount; childIndex++) {
    [indices addObject:@(childIndex)];
  }
  
  [self manageChildren:containerID
       moveFromIndices:nil
         moveToIndices:nil
     addChildReactTags:nil
          addAtIndices:nil
       removeAtIndices:indices];
   
   **/
}

/**
 * Disassociates children from container. Doesn't remove from registries.
 * TODO: use [NSArray getObjects:buffer] to reuse same fast buffer each time.
 *
 * @returns Array of removed items.
 */
- (NSArray *)_childrenToRemoveFromContainer:(id<VROComponent>)container
                                  atIndices:(NSArray *)atIndices
{
  // If there are no indices to move or the container has no subviews don't bother
  // We support parents with nil subviews so long as they're all nil so this allows for this behavior
  if (atIndices.count == 0 || [container reactSubviews].count == 0) {
    return nil;
  }

  // Construction of removed children must be done "up front", before indices are disturbed by removals.
  NSMutableArray *removedChildren = [NSMutableArray arrayWithCapacity:atIndices.count];
  RCTAssert(container != nil, @"container view (for ID %@) not found", container);
  for (NSNumber *indexNumber in atIndices) {
    NSUInteger index = indexNumber.unsignedIntegerValue;
    if (index < [container reactSubviews].count) {
      [removedChildren addObject:[container reactSubviews][index]];
    }
  }
  if (removedChildren.count != atIndices.count) {
    //RCTLogMustFix(@"removedChildren count (%tu) was not what we expected (%tu)",
    //              removedChildren.count, atIndices.count);
  }

  return removedChildren;
}

- (void)_removeChildren:(NSArray *)children fromContainer:(id<VROComponent>)container
{
  for (id removedChild in children) {
    [container removeReactSubview:removedChild];
  }
}

RCT_EXPORT_METHOD(removeRootView:(nonnull NSNumber *)rootReactTag)
{
  /**
  RCTShadowView *rootShadowView = _shadowViewRegistry[rootReactTag];
  RCTAssert(rootShadowView.superview == nil, @"root view cannot have superview (ID %@)", rootReactTag);
  [self _purgeChildren:rootShadowView.reactSubviews fromRegistry:_shadowViewRegistry];
  _shadowViewRegistry[rootReactTag] = nil;
  [_rootViewTags removeObject:rootReactTag];
  
  [self addUIBlock:^(ViroUIManager *uiManager, RCTSparseArray *viewRegistry){
    RCTAssertMainThread();
    UIView *rootView = viewRegistry[rootReactTag];
    [uiManager _purgeChildren:rootView.reactSubviews fromRegistry:viewRegistry];
    viewRegistry[rootReactTag] = nil;
    
    [[NSNotificationCenter defaultCenter] postNotificationName:RCTUIManagerDidRemoveRootViewNotification
                                                        object:uiManager
                                                      userInfo:@{ RCTUIManagerRootViewKey: rootView }];
  
  }];
   **/
}

RCT_EXPORT_METHOD(replaceExistingNonRootView:(nonnull NSNumber *)reactTag withView:(nonnull NSNumber *)newReactTag)
{
  /**
  RCTShadowView *shadowView = _shadowViewRegistry[reactTag];
  RCTAssert(shadowView != nil, @"shadowView (for ID %@) not found", reactTag);
  
  RCTShadowView *superShadowView = shadowView.superview;
  RCTAssert(superShadowView != nil, @"shadowView super (of ID %@) not found", reactTag);
  
  NSUInteger indexOfView = [superShadowView.reactSubviews indexOfObject:shadowView];
  RCTAssert(indexOfView != NSNotFound, @"View's superview doesn't claim it as subview (id %@)", reactTag);
  NSArray *removeAtIndices = @[@(indexOfView)];
  NSArray *addTags = @[newReactTag];
  [self manageChildren:superShadowView.reactTag
       moveFromIndices:nil
         moveToIndices:nil
     addChildReactTags:addTags
          addAtIndices:removeAtIndices
       removeAtIndices:removeAtIndices];
   
   **/
}

-(void) setChildren:(nonnull NSNumber *)containerTag
          reactTags:(NSArray<NSNumber *> *)reactTags {
  
    VROSetChildren(containerTag, reactTags, (NSDictionary<NSNumber *, id<VROComponent>> *)_shadowViewRegistry);
       
    [self addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
        VROSetChildren(containerTag, reactTags, (NSDictionary<NSNumber *, id<VROComponent>> *)viewRegistry);
    }];
}

static void VROSetChildren(NSNumber *containerTag,
                           NSArray<NSNumber *> *reactTags,
                           NSDictionary<NSNumber *,
                           id<VROComponent>> *registry) {
  
    id<VROComponent> container = registry[containerTag];
    NSInteger index = 0;
    for (NSNumber *reactTag in reactTags) {
        id<VROComponent> view = registry[reactTag];
        if (view) {
            [container insertReactSubview:view atIndex:index++];
        }
    }
}

-(void)manageChildrenShadowThread:(nonnull NSNumber *)containerReactTag
                  moveFromIndices:(NSArray *)moveFromIndices
                  moveToIndices:(NSArray *)moveToIndices
                  addChildReactTags:(NSArray *)addChildReactTags
                  addAtIndices:(NSArray *)addAtIndices
                  removeAtIndices:(NSArray *)removeAtIndices
{

  
  [self _manageChildren:containerReactTag
        moveFromIndices:moveFromIndices
          moveToIndices:moveToIndices
      addChildReactTags:addChildReactTags
           addAtIndices:addAtIndices
        removeAtIndices:removeAtIndices
               registry:(NSMutableDictionary<NSNumber *, id<VROComponent>> *)_shadowViewRegistry];
  
}

-(void)manageChildrenUIThread:(nonnull NSNumber *)containerReactTag
      moveFromIndices:(NSArray *)moveFromIndices
        moveToIndices:(NSArray *)moveToIndices
    addChildReactTags:(NSArray *)addChildReactTags
         addAtIndices:(NSArray *)addAtIndices
              removeAtIndices:(NSArray *)removeAtIndices {

  RCTAssertMainThread();

  [self _manageChildren:containerReactTag
             moveFromIndices:moveFromIndices
               moveToIndices:moveToIndices
           addChildReactTags:addChildReactTags
                addAtIndices:addAtIndices
             removeAtIndices:removeAtIndices
                    registry:(NSMutableDictionary<NSNumber *, id<VROComponent>> *)_viewRegistry];
}

/*
 TODO We do not yet support the delete animations that are seen in RCTUIManager's manageChildren:
      method. Otherwise this is a replica of RCTUIManager's manageChildren:
 */
- (void)_manageChildren:(NSNumber *)containerReactTag
        moveFromIndices:(NSArray *)moveFromIndices
          moveToIndices:(NSArray *)moveToIndices
      addChildReactTags:(NSArray *)addChildReactTags
           addAtIndices:(NSArray *)addAtIndices
        removeAtIndices:(NSArray *)removeAtIndices
               registry:(NSMutableDictionary<NSNumber *, id<VROComponent>> *)registry
{
  
  id<VROComponent> container = registry[containerReactTag];
  RCTAssert(moveFromIndices.count == moveToIndices.count, @"moveFromIndices had size %tu, moveToIndices had size %tu", moveFromIndices.count, moveToIndices.count);
  RCTAssert(addChildReactTags.count == addAtIndices.count, @"there should be at least one React child to add");
  
  // Removes (both permanent and temporary moves) are using "before" indices
  NSArray *permanentlyRemovedChildren = [self _childrenToRemoveFromContainer:container atIndices:removeAtIndices];
  NSArray *temporarilyRemovedChildren = [self _childrenToRemoveFromContainer:container atIndices:moveFromIndices];
  [self _removeChildren:permanentlyRemovedChildren fromContainer:container];
  [self _removeChildren:temporarilyRemovedChildren fromContainer:container];
  
  [self _purgeChildren:permanentlyRemovedChildren fromRegistry:registry];
  
  // Figure out what to insert - merge temporary inserts and adds
  NSMutableDictionary *destinationsToChildrenToAdd = [NSMutableDictionary dictionary];
  for (NSInteger index = 0, length = temporarilyRemovedChildren.count; index < length; index++) {
    destinationsToChildrenToAdd[moveToIndices[index]] = temporarilyRemovedChildren[index];
  }

  for (NSInteger index = 0, length = addAtIndices.count; index < length; index++) {
    id view = registry[addChildReactTags[index]];
    if (view) {
      destinationsToChildrenToAdd[addAtIndices[index]] = view;
    }
  }
  
  NSArray *sortedIndices = [destinationsToChildrenToAdd.allKeys sortedArrayUsingSelector:@selector(compare:)];
  for (NSNumber *reactIndex in sortedIndices) {
    [container insertReactSubview:destinationsToChildrenToAdd[reactIndex] atIndex:reactIndex.integerValue];
  }
}

-(void)createView:(nonnull NSNumber *)reactTag
                  viewName:(NSString *)viewName
                  rootTag:(__unused NSNumber *)rootTag
                  props:(NSDictionary *)props
{
  VROComponentData *componentData = _componentDataByName[viewName];
  ViroViewManager *viewManager = componentData.manager;

  // Find all the flex views and the single "root" view, which for us, is the VRTSceneNavigator
  if([viewManager isRootFlexBoxPanel]){
    [self->_rootFlexViewTags addObject:reactTag];
  } else if ([viewManager class] == [VRTSceneNavigatorManager class]) {
    self->_rootViewTag = reactTag;
  }
  
  if (componentData == nil) {
    RCTLogError(@"No component found for view with name \"%@\"", viewName);
  }
  
  // Register shadow view
  VRTShadowView *shadowView = [componentData createShadowViewWithTag:reactTag];
  if (shadowView) {
    [componentData setProps:props forShadowView:shadowView];
    _shadowViewRegistry[reactTag] = shadowView;
  }

  //create view, and set properties in main thread.
  [self addUIBlock:^(ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry){
    id<VROComponent> view = [componentData createViewWithTag:reactTag props:props];
    if (view) {
      [componentData setProps:props forView:view];
      if ([view respondsToSelector:@selector(reactBridgeDidFinishTransaction)]) {
        [uiManager->_bridgeTransactionListeners addObject:view];
      }
      //VA: Uncomment this...((NSMutableDictionary<NSNumber *, VRTView *> *)viewRegistry)[reactTag] = view;
    }
  }];
}

-(void)updateView:(nonnull NSNumber *)reactTag
                  viewName:(NSString *)viewName // not always reliable, use shadowView.viewName if available
                  props:(NSDictionary *)props
{

  
  VRTShadowView *shadowView = _shadowViewRegistry[reactTag];
  VROComponentData *componentData = _componentDataByName[shadowView.viewName ?: viewName];
  [componentData setProps:props forShadowView:shadowView];
  

  [self addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    NSLog(@"ViroUIManager, updateView setProps, reactTag: %@, viewName: %@, %@", reactTag, viewName, props);
    VRTView *view = viewRegistry[reactTag];
    //VA: Uncomment this: [componentData setProps:props forView:view];
  }];
  
}

RCT_EXPORT_METHOD(focus:(nonnull NSNumber *)reactTag)
{
  [self addUIBlock:^(__unused ViroUIManager *uiManager,  NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    VRTView *newResponder = viewRegistry[reactTag];
    //VA:[newResponder reactWillMakeFirstResponder];
    //VA: [newResponder becomeFirstResponder];
    //VA: [newResponder reactDidMakeFirstResponder];
  }];
}

RCT_EXPORT_METHOD(blur:(nonnull NSNumber *)reactTag)
{
  [self addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry){
    VRTView *currentResponder = viewRegistry[reactTag];
    //[currentResponder resignFirstResponder];
  }];
}

RCT_EXPORT_METHOD(findSubviewIn:(nonnull NSNumber *)reactTag atPoint:(CGPoint)point callback:(RCTResponseSenderBlock)callback)
{
  [self addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    VRTView *view = viewRegistry[reactTag];
    //VA: UIView *target = [view hitTest:point withEvent:nil];
    //VA: CGRect frame = [target convertRect:target.bounds toView:view];
    
    //while (target.reactTag == nil && target.superview != nil) {
    //  target = target.superview;
    //}
    
    //VA:
    
    //VA: callback(@[
    //           RCTNullIfNil(target.reactTag),
    //           @(frame.origin.x),
    //           @(frame.origin.y),
    //           @(frame.size.width),
    //           @(frame.size.height),
    //           ]);
  }];
}

-(void)batchDidComplete {
  //Do nothing here.
}

// Flex panels that don't have a VRTScene parent or VRTNodeContainer parent are NOT root flex panels, so remove them.
- (void)cullRootFlexViewTags {
  NSArray *tempArray = [_rootFlexViewTags allObjects];
  for (NSNumber *reactTag in tempArray) {
    VRTShadowFlexView *rootView = (VRTShadowFlexView *)_shadowViewRegistry[reactTag];
    if(![rootView.superview isKindOfClass:[VRTSceneShadowView class]] && ![rootView.superview isKindOfClass:[VRTNodeContainerShadowView class]]) {
      [_rootFlexViewTags removeObject:reactTag];
    }
  }
}

//Perform layout for flexbox views and enqueue Viro UI Component tasks to run on the main UI thread.
- (void)layoutAndQueueViroUIBlocks
{
  
  // Cull from the root flex view tag set now that our view hierachy is set.
  [self cullRootFlexViewTags];

  // Perform layout
  for (NSNumber *reactTag in _rootFlexViewTags) {
    VRTShadowFlexView *rootView = (VRTShadowFlexView *)_shadowViewRegistry[reactTag];
    [self addUIBlock:[self uiBlockWithLayoutUpdateForRootView:rootView]];
    [self _amendPendingUIBlocksWithStylePropagationUpdateForRootView:rootView];
  }
  
  [self flushUIBlocks];
}

- (void)flushUIBlocks
{
   //VA: RCTAssertThread(_shadowQueue, @"flushUIBlocks can only be called from the shadow queue");
  
  // First copy the previous blocks into a temporary variable, then reset the
  // pending blocks to a new array. This guards against mutation while
  // processing the pending blocks in another thread.
  NSArray<dispatch_block_t> *previousPendingUIBlocks = _pendingUIBlocks;
  _pendingUIBlocks = [NSMutableArray new];
  
  if (previousPendingUIBlocks.count) {
    // Execute the previously queued UI blocks
    RCTProfileBeginFlowEvent();
    dispatch_async(dispatch_get_main_queue(), ^{
      RCTProfileEndFlowEvent();

      @try {
        for (dispatch_block_t block in previousPendingUIBlocks) {
          block();
        }
      }
      @catch (NSException *exception) {
        RCTLogError(@"Exception thrown while executing UI block: %@", exception);
      }
    });
  }
}

RCT_EXPORT_METHOD(measure:(nonnull NSNumber *)reactTag
                  callback:(RCTResponseSenderBlock)callback)
{
  /**
  [self addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    VRTView *view = viewRegistry[reactTag];
    if (!view) {
      // this view was probably collapsed out
      RCTLogWarn(@"measure cannot find view with tag #%@", reactTag);
      callback(@[]);
      return;
    }
    //VA: CGRect frame = view.frame;
    
    UIView *rootView = view;
    while (rootView && ![rootView isReactRootView]) {
      rootView = rootView.superview;
    }
    
    // TODO: this doesn't work because sometimes view is inside a modal window
    // RCTAssert([rootView isReactRootView], @"React view is not inside a React root view");
    
    // By convention, all coordinates, whether they be touch coordinates, or
    // measurement coordinates are with respect to the root view.
    //VA: CGPoint pagePoint = [view.superview convertPoint:frame.origin toView:rootView];
   
    //VA:
    //callback(@[
    //           @(frame.origin.x),
    //           @(frame.origin.y),
    //           @(frame.size.width),
    //           @(frame.size.height),
    //           @(pagePoint.x),
    //           @(pagePoint.y)
    //           ]);
  }];
   **/
}

static void RCTMeasureLayout(VRTShadowView *view,
                             VRTShadowView *ancestor,
                             RCTResponseSenderBlock callback)
{
  if (!view) {
    return;
  }
  if (!ancestor) {
    return;
  }
  CGRect result = [view measureLayoutRelativeToAncestor:ancestor];
  if (CGRectIsNull(result)) {
    RCTLogError(@"view %@ (tag #%@) is not a decendant of %@ (tag #%@)",
                view, view.reactTag, ancestor, ancestor.reactTag);
    return;
  }
  CGFloat leftOffset = result.origin.x;
  CGFloat topOffset = result.origin.y;
  CGFloat width = result.size.width;
  CGFloat height = result.size.height;
  if (isnan(leftOffset) || isnan(topOffset) || isnan(width) || isnan(height)) {
    RCTLogError(@"Attempted to measure layout but offset or dimensions were NaN");
    return;
  }
  callback(@[@(leftOffset), @(topOffset), @(width), @(height)]);
}

/**
 * Returns the computed recursive offset layout in a dictionary form. The
 * returned values are relative to the `ancestor` shadow view. Returns `nil`, if
 * the `ancestor` shadow view is not actually an `ancestor`. Does not touch
 * anything on the main UI thread. Invokes supplied callback with (x, y, width,
 * height).
 */
RCT_EXPORT_METHOD(measureLayout:(nonnull NSNumber *)reactTag
                  relativeTo:(nonnull NSNumber *)ancestorReactTag
                  errorCallback:(__unused RCTResponseSenderBlock)errorCallback
                  callback:(RCTResponseSenderBlock)callback)
{
  //VA: RCTShadowView *shadowView = _shadowViewRegistry[reactTag];
  //VA:RCTShadowView *ancestorShadowView = _shadowViewRegistry[ancestorReactTag];
  //VA: RCTMeasureLayout(shadowView, ancestorShadowView, callback);
}

/**
 * Returns the computed recursive offset layout in a dictionary form. The
 * returned values are relative to the `ancestor` shadow view. Returns `nil`, if
 * the `ancestor` shadow view is not actually an `ancestor`. Does not touch
 * anything on the main UI thread. Invokes supplied callback with (x, y, width,
 * height).
 */
RCT_EXPORT_METHOD(measureLayoutRelativeToParent:(nonnull NSNumber *)reactTag
                  errorCallback:(__unused RCTResponseSenderBlock)errorCallback
                  callback:(RCTResponseSenderBlock)callback)
{
 //VA:  RCTShadowView *shadowView = _shadowViewRegistry[reactTag];
 //VA:  RCTMeasureLayout(shadowView, shadowView.reactSuperview, callback);
}

/**
 * Returns an array of computed offset layouts in a dictionary form. The layouts are of any React subviews
 * that are immediate descendants to the parent view found within a specified rect. The dictionary result
 * contains left, top, width, height and an index. The index specifies the position among the other subviews.
 * Only layouts for views that are within the rect passed in are returned. Invokes the error callback if the
 * passed in parent view does not exist. Invokes the supplied callback with the array of computed layouts.
 */
RCT_EXPORT_METHOD(measureViewsInRect:(CGRect)rect
                  parentView:(nonnull NSNumber *)reactTag
                  errorCallback:(__unused RCTResponseSenderBlock)errorCallback
                  callback:(RCTResponseSenderBlock)callback)
{
  //VA:
  /**
  RCTShadowView *shadowView = _shadowViewRegistry[reactTag];
  if (!shadowView) {
    RCTLogError(@"Attempting to measure view that does not exist (tag #%@)", reactTag);
    return;
  }
  NSArray *childShadowViews = [shadowView reactSubviews];
  NSMutableArray *results = [[NSMutableArray alloc] initWithCapacity:childShadowViews.count];
  
  [childShadowViews enumerateObjectsUsingBlock:
   ^(RCTShadowView *childShadowView, NSUInteger idx, __unused BOOL *stop) {
     CGRect childLayout = [childShadowView measureLayoutRelativeToAncestor:shadowView];
     if (CGRectIsNull(childLayout)) {
       RCTLogError(@"View %@ (tag #%@) is not a decendant of %@ (tag #%@)",
                   childShadowView, childShadowView.reactTag, shadowView, shadowView.reactTag);
       return;
     }
     
     CGFloat leftOffset = childLayout.origin.x;
     CGFloat topOffset = childLayout.origin.y;
     CGFloat width = childLayout.size.width;
     CGFloat height = childLayout.size.height;
     
     if (leftOffset <= rect.origin.x + rect.size.width &&
         leftOffset + width >= rect.origin.x &&
         topOffset <= rect.origin.y + rect.size.height &&
         topOffset + height >= rect.origin.y) {
       
       // This view is within the layout rect
       NSDictionary *result = @{@"index": @(idx),
                                @"left": @(leftOffset),
                                @"top": @(topOffset),
                                @"width": @(width),
                                @"height": @(height)};
       
       [results addObject:result];
     }
   }];
  callback(@[results]);
  **/
}

RCT_EXPORT_METHOD(setMainScrollViewTag:(nonnull NSNumber *)reactTag)
{
  NSLog(@"Viro setmainsScrollViewTag invoked.");
}

// TODO: we could just pass point property
RCT_EXPORT_METHOD(scrollTo:(nonnull NSNumber *)reactTag
                  withOffsetX:(CGFloat)offsetX
                  offsetY:(CGFloat)offsetY)
{
  [self addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry){
    VRTView *view = viewRegistry[reactTag];
    if ([view conformsToProtocol:@protocol(RCTScrollableProtocol)]) {
      [(id<RCTScrollableProtocol>)view scrollToOffset:(CGPoint){offsetX, offsetY} animated:YES];
    } else {
      RCTLogError(@"tried to scrollToOffset: on non-RCTScrollableProtocol view %@ with tag #%@", view, reactTag);
    }
  }];
}

// TODO: we could just pass point property
RCT_EXPORT_METHOD(scrollWithoutAnimationTo:(nonnull NSNumber *)reactTag
                  offsetX:(CGFloat)offsetX
                  offsetY:(CGFloat)offsetY)
{
  [self addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry){
    VRTView *view = viewRegistry[reactTag];
    if ([view conformsToProtocol:@protocol(RCTScrollableProtocol)]) {
      [(id<RCTScrollableProtocol>)view scrollToOffset:(CGPoint){offsetX, offsetY} animated:NO];
    } else {
      RCTLogError(@"tried to scrollToOffset: on non-RCTScrollableProtocol view %@ with tag #%@", view, reactTag);
    }
  }];
}

RCT_EXPORT_METHOD(zoomToRect:(nonnull NSNumber *)reactTag
                  withRect:(CGRect)rect)
{
  /**
  [self addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry){
    VRTView *view = viewRegistry[reactTag];
    if ([view conformsToProtocol:@protocol(RCTScrollableProtocol)]) {
      [(id<RCTScrollableProtocol>)view zoomToRect:rect animated:YES];
    } else {
      RCTLogError(@"tried to zoomToRect: on non-RCTScrollableProtocol view %@ with tag #%@", view, reactTag);
    }
  }];
   **/
}

/**
 * JS sets what *it* considers to be the responder. Later, scroll views can use
 * this in order to determine if scrolling is appropriate.
 */
RCT_EXPORT_METHOD(setJSResponder:(nonnull NSNumber *)reactTag
                  blockNativeResponder:(__unused BOOL)blockNativeResponder)
{
  /***
  [self addUIBlock:^(__unused ViroUIManager *uiManager, NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    _jsResponder = viewRegistry[reactTag];
    if (!_jsResponder) {
      RCTLogError(@"Invalid view set to be the JS responder - tag %zd", reactTag);
    }
  }];
   **/
}

RCT_EXPORT_METHOD(clearJSResponder)
{
  /**
  [self addUIBlock:^(__unused ViroUIManager *uiManager, __unused NSDictionary<NSNumber *, VRTView *> *viewRegistry) {
    _jsResponder = nil;
  }];
   **/
}

- (NSDictionary *)bubblingEventsConfig
{
  NSMutableDictionary *customBubblingEventTypesConfigs = [NSMutableDictionary new];
  for (RCTComponentData *componentData in _componentDataByName.allValues) {
    RCTViewManager *manager = componentData.manager;
    if (RCTClassOverridesInstanceMethod([manager class], @selector(customBubblingEventTypes))) {
      NSArray *events = [manager customBubblingEventTypes];
      if (RCT_DEBUG) {
        RCTAssert(!events || [events isKindOfClass:[NSArray class]],
                  @"customBubblingEventTypes must return an array, but %@ returned %@",
                  [manager class], [events class]);
      }
      for (NSString *eventName in events) {
        NSString *topName = RCTNormalizeInputEventName(eventName);
        if (!customBubblingEventTypesConfigs[topName]) {
          NSString *bubbleName = [topName stringByReplacingCharactersInRange:(NSRange){0, 3} withString:@"on"];
          customBubblingEventTypesConfigs[topName] = @{
                                                       @"phasedRegistrationNames": @{
                                                           @"bubbled": bubbleName,
                                                           @"captured": [bubbleName stringByAppendingString:@"Capture"],
                                                           }
                                                       };
        }
      }
    }
  };
  
  return customBubblingEventTypesConfigs;
}

- (NSDictionary *)directEventsConfig
{
  NSMutableDictionary *customDirectEventTypes = [NSMutableDictionary new];
  for (RCTComponentData *componentData in _componentDataByName.allValues) {
    RCTViewManager *manager = componentData.manager;
    if (RCTClassOverridesInstanceMethod([manager class], @selector(customDirectEventTypes))) {
      NSArray *events = [manager customDirectEventTypes];
      if (RCT_DEBUG) {
        RCTAssert(!events || [events isKindOfClass:[NSArray class]],
                  @"customDirectEventTypes must return an array, but %@ returned %@",
                  [manager class], [events class]);
      }
      for (NSString *eventName in events) {
        NSString *topName = RCTNormalizeInputEventName(eventName);
        if (!customDirectEventTypes[topName]) {
          customDirectEventTypes[topName] = @{
                                              @"registrationName": [topName stringByReplacingCharactersInRange:(NSRange){0, 3} withString:@"on"],
                                              };
        }
      }
    }
  };
  
  return customDirectEventTypes;
}

- (NSDictionary<NSString *, id> *)constantsToExport
{
  NSMutableDictionary<NSString *, NSDictionary *> *allJSConstants = [NSMutableDictionary new];
  NSMutableDictionary<NSString *, NSDictionary *> *directEvents = [NSMutableDictionary new];
  NSMutableDictionary<NSString *, NSDictionary *> *bubblingEvents = [NSMutableDictionary new];
  
  [_componentDataByName enumerateKeysAndObjectsUsingBlock:
   ^(NSString *name, VROComponentData *componentData, __unused BOOL *stop) {
     
     ViroViewManager *manager = componentData.manager;
     NSMutableDictionary<NSString *, id> *constantsNamespace =
     [NSMutableDictionary dictionaryWithDictionary:allJSConstants[name]];
     
     // Add manager class
     constantsNamespace[@"Manager"] =  RCTBridgeModuleNameForClass(componentData.managerClass);
     
     // Add native props
     NSDictionary<NSString *, id> *viewConfig = [componentData viewConfig];
     constantsNamespace[@"NativeProps"] = viewConfig[@"propTypes"];
     
     // Add direct events
     for (NSString *eventName in viewConfig[@"directEvents"]) {
       if (!directEvents[eventName]) {
         directEvents[eventName] = @{
                                     @"registrationName": [eventName stringByReplacingCharactersInRange:(NSRange){0, 3} withString:@"on"],
                                     };
       }
       if (RCT_DEBUG && bubblingEvents[eventName]) {
         RCTLogError(@"Component '%@' re-registered bubbling event '%@' as a "
                     "direct event", componentData.name, eventName);
       }
     }
     
     // Add bubbling events
     for (NSString *eventName in viewConfig[@"bubblingEvents"]) {
       if (!bubblingEvents[eventName]) {
         NSString *bubbleName = [eventName stringByReplacingCharactersInRange:(NSRange){0, 3} withString:@"on"];
         bubblingEvents[eventName] = @{
                                       @"phasedRegistrationNames": @{
                                           @"bubbled": bubbleName,
                                           @"captured": [bubbleName stringByAppendingString:@"Capture"],
                                           }
                                       };
       }
       if (RCT_DEBUG && directEvents[eventName]) {
         RCTLogError(@"Component '%@' re-registered direct event '%@' as a "
                     "bubbling event", componentData.name, eventName);
       }
     }
     
     allJSConstants[name] = constantsNamespace;
     
     
   }];
  
  //VA: TODO....the below line may break things since we merge this with RCTUIManager exportConstants.
  [allJSConstants addEntriesFromDictionary:@{
                                             @"customBubblingEventTypes": bubblingEvents,
                                             @"customDirectEventTypes": directEvents,
                                             @"Dimensions": @{
                                                 @"window": @{
                                                     @"width": @(RCTScreenSize().width),
                                                     @"height": @(RCTScreenSize().height),
                                                     @"scale": @(RCTScreenScale()),
                                                     },
                                                 @"modalFullscreenView": @{
                                                     @"width": @(RCTScreenSize().width),
                                                     @"height": @(RCTScreenSize().height),
                                                     },
                                                 },
                                             }];
  return allJSConstants;
}

RCT_EXPORT_METHOD(configureNextLayoutAnimation:(NSDictionary *)config
                  withCallback:(RCTResponseSenderBlock)callback
                  errorCallback:(__unused RCTResponseSenderBlock)errorCallback)
{
  NSLog(@"Viro configureNextLayoutAnimation invoked");
}

static UIView *_jsResponder;

+ (UIView *)JSResponder
{
  return _jsResponder;
}

-(bool) isSceneNavigatorView:(id<VROComponent>)viewVRO {
  if([viewVRO isKindOfClass:[VRTSceneNavigator class]]){
    return YES;
  }
  return NO;
}

-(UIView *) getRootUIViewFromSceneNav:(id<VROComponent>)viewVRO {
  VRTSceneNavigator *sceneNav = (VRTSceneNavigator *)viewVRO;
  return [sceneNav rootVROView];
}

@end


@implementation RCTBridge (ViroUIManager)

- (ViroUIManager *)viroUIManager
{
  return [self moduleForClass:[ViroUIManager class]];
}

@end
 

