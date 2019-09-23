//
//  VRTTreeNode.m
//  React
//
//  Copyright © 2016 Viro Media, Inc. All rights reserved.
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
