//
//  VRTHUD.h
//  React
//
//  Created by Vik Advani on 1/29/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VRTNode.h"

@interface VRTHUD : VRTNode {
  NSMutableArray *_hudChildren;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge;
- (NSArray *)getChildren;

@property (nonatomic, assign) BOOL reticle;

@end
