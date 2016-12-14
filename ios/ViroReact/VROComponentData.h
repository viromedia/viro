//
//  VROComponentData.h
//  React
//
//  Created by Vik Advani on 11/3/15.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VROComponent.h"
#import "VRTShadowView.h"
#import "ViroViewManager.h"

@class RCTBridge;

@interface VROComponentData : NSObject

@property (nonatomic, readonly) Class managerClass;
@property (nonatomic, copy, readonly) NSString *name;
@property (nonatomic, strong, readonly) ViroViewManager *manager;

- (instancetype)initWithManagerClass:(Class)managerClass
                              bridge:(RCTBridge *)bridge NS_DESIGNATED_INITIALIZER;

- (VRTView  *)createViewWithTag:(NSNumber *)tag;
- (id<VROComponent>)createViewWithTag:(NSNumber *)tag props:(NSDictionary *)props;
- (VRTShadowView *)createShadowViewWithTag:(NSNumber *)tag;
- (void)setProps:(NSDictionary *)props forView:(id<VROComponent>)view;
- (void)setProps:(NSDictionary *)props forShadowView:(VRTShadowView *)shadowView;

- (NSDictionary *)viewConfig;
@end
