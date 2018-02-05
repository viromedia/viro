//
//  VRTARTrackingTargetsModule.mm
//  ViroReact
//
//  Created by Andy Chu on 2/1/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import "VRTARTrackingTargetsModule.h"
#import "VRTUtils.h"

@implementation VRTARTargetPromise {
    NSString *_key;
    RCTImageSource *_source;
    VROImageOrientation _orientation;
    float _physicalWidth;
    std::shared_ptr<VROARImageTarget> _arTarget;
    NSMutableArray *_waitBlocks;
    NSURLSessionDataTask *_downloadTask;
}

- (instancetype)initWithKey:(NSString *)key
                      source:(RCTImageSource *)source
                 orientation:(VROImageOrientation)orientation
               physicalWidth:(float)physicalWidth {
    self = [super init];
    if (self) {
        _key = key;
        _source = source;
        _orientation = orientation;
        _physicalWidth = physicalWidth;
        _ready = NO;
        _waitBlocks = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)wait:(VRTARTargetPromiseCompletion)finishBlock {
    @synchronized (self) {
        if (!_ready) {
            [_waitBlocks addObject:finishBlock];
        } else {
            finishBlock(_key, _arTarget);
        }
    }
}

- (void)fetch {
    if (_arTarget) {
        return;
    }

    NSURL *URL = _source.request.URL;
    NSString *scheme = URL.scheme.lowercaseString;
    if([scheme isEqualToString:@"file"] || [scheme isEqualToString:@"http"] || [scheme isEqualToString:@"data"] ||  [scheme isEqualToString:@"https"]) {
        _downloadTask = downloadDataWithURL(URL, ^(NSData *data, NSError *error) {
            _downloadTask = nil;
            if (!error) {
                UIImage *image = [[UIImage alloc] initWithData:data];
                if (!image) {
                    RCTLogError(@"[VRTARTrackingTargets] Error converting data into image target [%@]", _key);
                    for (VRTARTargetPromiseCompletion completion in _waitBlocks) {
                        completion(_key, nullptr);
                    }
                    return;
                }
                
                // create image target
                _arTarget = std::make_shared<VROARImageTargetiOS>(image, _orientation, _physicalWidth);

                @synchronized(self) {
                    _ready = YES;
                }

                for (VRTARTargetPromiseCompletion completion in _waitBlocks) {
                    completion(_key, _arTarget);
                }

                _waitBlocks = nil; // release all the blocks!
                
            } else {
                RCTLogError(@"[VRTARTrackingTargets] Error downloading image target source [%@]", _key);
                for (VRTARTargetPromiseCompletion completion in _waitBlocks) {
                    completion(_key, nullptr);
                }
            }
        });
    }
}

@end

// VRTARTrackingTargetsModule Implemetation
@implementation VRTARTrackingTargetsModule {
    NSMutableDictionary *_targetsDict;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _targetsDict = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (VRTARTargetPromise *)getARTargetPromise:(NSString *)name {
    return _targetsDict[name];
}

#pragma mark RCT_EXPORT_METHODS

RCT_EXPORT_MODULE()

RCT_EXPORT_METHOD(createTargets:(NSDictionary *)targets) {
    for (NSString *key in targets) {
        NSDictionary *target = [targets objectForKey:key];

        RCTImageSource *source = [RCTConvert RCTImageSource:[target objectForKey:@"source"]];

        NSNumber *width = [target objectForKey:@"physicalWidth"];
        float physicalWidth = [width floatValue];

        VROImageOrientation orientation = VROImageOrientation::Up; // default orientation!
        NSString *strOrientation = [target objectForKey:@"orientation"];
        if (strOrientation) {
            if ([strOrientation caseInsensitiveCompare:@"Up"] == NSOrderedSame) {
                orientation = VROImageOrientation::Up;
            } else if ([strOrientation caseInsensitiveCompare:@"Down"] == NSOrderedSame) {
                orientation = VROImageOrientation::Down;
            } else if ([strOrientation caseInsensitiveCompare:@"Left"] == NSOrderedSame) {
                orientation = VROImageOrientation::Left;
            } else if ([strOrientation caseInsensitiveCompare:@"Right"] == NSOrderedSame) {
                orientation = VROImageOrientation::Right;
            } else {
                RCTLogError(@"[VRTARTrackingTargets] Unknown orientation [%@] for target [%@]", strOrientation, key);
            }
        }

        _targetsDict[key] = [[VRTARTargetPromise alloc] initWithKey:key
                                                             source:source
                                                        orientation:orientation
                                                      physicalWidth:physicalWidth];
        [_targetsDict[key] fetch];
    }
}

RCT_EXPORT_METHOD(deleteTarget:(NSString *)name) {
    [_targetsDict removeObjectForKey:name];
}

@end


// RCTBridge (VRTARTrackingTargetsModule) Category Implementation
@implementation RCTBridge (VRTARTrackingTargetsModule)

- (VRTARTrackingTargetsModule *)trackingTargetsModule {
    return [self moduleForClass:[VRTARTrackingTargetsModule class]];
}

@end
