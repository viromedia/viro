//
//  VRTTreeNode.h
//  React
//
//  Copyright © 2016 Viro Media, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * Interface for a VRTTreeNodewhich contain a value and an orderedSet of children. This
 * implementation ensures that the list of children are ordered by insertion and are
 * unique based on the value of each node.
 */
@interface VRTTreeNode : NSObject

@property (nonatomic, copy) NSObject *value;

- (id)initWithValue:(NSObject *)value;
- (NSOrderedSet<VRTTreeNode *> *)getChildren;
- (void)addChild:(VRTTreeNode *)child;
- (VRTTreeNode *)childWithValue:(NSObject *)value;

@end
