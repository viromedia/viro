//
//  VRTPolyline.mm
//  ViroReact
//
//  Created by Andy Chu on 5/8/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
        
        _polyline = VROPolyline::createPolyline(nativePoints, _thickness);
        
        self.node->setGeometry(_polyline);
        if (self.materials) {
            [self applyMaterials];
        }
        _didSetGeometry = YES;
    }
}

@end
