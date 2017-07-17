//
//  VRTTreeNode.m
//  React
//
//  Copyright © 2016 Viro Media, Inc. All rights reserved.
//

#import "VRTTreeNode.h"

@interface VRTTreeNode()

@property (nonatomic, copy) NSMutableOrderedSet<VRTTreeNode *> *children;

@end

@implementation VRTTreeNode

- (id)initWithValue:(NSObject *)value {
    self = [super init];
    if (self) {
        _value = value;
        _children = [[NSMutableOrderedSet alloc] init];
    }
    return self;
}

- (NSOrderedSet<VRTTreeNode *> *)getChildren {
    return [_children copy];
}

- (void)addChild:(VRTTreeNode *)child {
    [_children addObject:child];
}

- (VRTTreeNode *)childWithValue:(NSObject *)value {
    NSUInteger index = [_children indexOfObject:value];
    return index != NSNotFound ? [_children objectAtIndex:index] : nil;
}

/**
 * Override the isEquals method to compare the value with the given object.
 */
- (BOOL)isEqual:(id)object {
    if ([object isKindOfClass:[VRTTreeNode class]]) {
        VRTTreeNode *otherNode = (VRTTreeNode *)object;
        return [self.value isEqual:otherNode.value];
    } else {
        return [self.value isEqual:object];
    }
}

/**
 * Override the hash function to hash by the value of the TreeNode.
 */
- (NSUInteger)hash {
    return [_value hash];
}

@end
