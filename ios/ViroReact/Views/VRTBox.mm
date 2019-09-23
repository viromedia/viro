//
//  VROBox.m
//  React
//
//  Created by Vik Advani on 12/14/15.
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
#import <ViroKit/ViroKit.h>
#import <React/RCTLog.h>
#import "VRTBox.h"
#import "VRTMaterialManager.h"

@implementation VRTBox {
    std::shared_ptr<VROBox> _box;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _box = VROBox::createBox(1, 1, 1);
        [self node]->setGeometry(_box);
    }
    
    return self;
}

- (void)setWidth:(float)width {
    if (width < 0) {
        RCTLogError(@"Box width must be >= 0");
    }
    _box->setWidth(width);
}

- (void)setHeight:(float)height {
    if (height < 0) {
        RCTLogError(@"Box height must be >= 0");
    }
    _box->setHeight(height);
}

- (void)setLength:(float)length {
    if (length < 0) {
        RCTLogError(@"Box length must be >= 0");
    }
    _box->setLength(length);
}

@end


