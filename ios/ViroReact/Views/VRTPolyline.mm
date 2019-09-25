//
//  VRTPolyline.mm
//  ViroReact
//
//  Created by Andy Chu on 5/8/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
#import "VRTPolyline.h"


@interface VRTPolyline()

@property (nonatomic, assign) BOOL didSetGeometry;

@end

@implementation VRTPolyline {
    std::shared_ptr<VROPolyline> _polyline;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _didSetGeometry = NO;
        _thickness = 0.1;
    }
    return self;
}

- (void)setPoints:(NSArray<NSArray<NSNumber *> *> *)points {
    if ([points count] < 1) {
        RCTLogError(@"Polyline should contain at least 1 point");
    }
    _points = points;
    _didSetGeometry = NO;
}

- (void)setThickness:(float)thickness {
    if (thickness < 0) {
        RCTLogError(@"Polyline thickness must be >= 0");
    }
    _thickness = thickness;
    if (_didSetGeometry) {
        _polyline->setThickness(thickness);
    }
    
}

- (void)setClosed:(BOOL)closed {
    _closed = closed;
     _didSetGeometry = NO;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    if (!_didSetGeometry) {
        std::vector<VROVector3f> nativePoints;
        
        for (int i = 0; i < [_points count]; i++) {
            NSArray *pointArray = [_points objectAtIndex:i];
            if ([pointArray count] < 3) {
                RCTLogError(@"Polyline points should have at least 3 coordinates.");
            }
            VROVector3f nativePoint = VROVector3f([[pointArray objectAtIndex:0] floatValue], [[pointArray objectAtIndex:1] floatValue], [[pointArray objectAtIndex:2] floatValue]);
            nativePoints.push_back(nativePoint);
        }

        if (self.closed) {
            NSArray *pointArray = [_points objectAtIndex:0];
            if ([pointArray count] < 3) {
                RCTLogError(@"Polyline points should have at least 3 coordinates.");
            }

            VROVector3f nativePoint = VROVector3f([[pointArray objectAtIndex:0] floatValue], [[pointArray objectAtIndex:1] floatValue], [[pointArray objectAtIndex:2] floatValue]);
            nativePoints.push_back(nativePoint);
        }
        
        _polyline = VROPolyline::createPolyline(nativePoints, _thickness);
        
        self.node->setGeometry(_polyline);
        if (self.materials) {
            [self applyMaterials];
        }
        _didSetGeometry = YES;
    }
}

@end
