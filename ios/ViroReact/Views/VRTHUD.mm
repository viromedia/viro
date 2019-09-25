//
//  VRTHUD.m
//  React
//
//  Created by Vik Advani on 1/29/16.
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

#import <React/RCTComponent.h>
#import "VRTHUD.h"
#import "VRTHUDComponent.h"

@implementation VRTHUD

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _hudChildren = [[NSMutableArray alloc] initWithCapacity:100];
    }
    
    return self;
}

- (void)insertReactSubview:(UIView *)subview atIndex:(NSInteger)atIndex {
    if([subview conformsToProtocol:@protocol(VRTHUDComponent)]) {
        VRTHUDLabel<VRTHUDComponent> *hudcomp = (VRTHUDLabel<VRTHUDComponent> *)subview;
        [_hudChildren addObject:[hudcomp getUIView]];
    }
    
    //VA: Uncomment later. [super insertReactSubview:subview atIndex:atIndex];
}

- (NSArray *)getChildren {
    NSArray *array = [_hudChildren copy];
    return array;
}

@end
