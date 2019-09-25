//
//  VRTARTrackingTargetsModule.mm
//  ViroReact
//
//  Created by Andy Chu on 2/1/18.
//  Copyright © 2018 Viro Media. All rights reserved.
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

#import "VRTARTrackingTargetsModule.h"
#import "VRTUtils.h"

#pragma mark - VRTARImageTargetPromise Implementation

@implementation VRTARImageTargetPromise {
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

- (void)wait:(VRTARImageTargetPromiseCompletion)finishBlock {
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
                    for (VRTARImageTargetPromiseCompletion completion in _waitBlocks) {
                        completion(_key, nullptr);
                    }
                    return;
                }
                
                // create image target
                _arTarget = std::make_shared<VROARImageTargetiOS>(image, _orientation, _physicalWidth);

                @synchronized(self) {
                    _ready = YES;
                }

                for (VRTARImageTargetPromiseCompletion completion in _waitBlocks) {
                    completion(_key, _arTarget);
                }

                _waitBlocks = nil; // release all the blocks!
                
            } else {
                RCTLogError(@"[VRTARTrackingTargets] Error downloading image target source [%@]", _key);
                for (VRTARImageTargetPromiseCompletion completion in _waitBlocks) {
                    completion(_key, nullptr);
                }
            }
        });
    }
}

@end

#pragma mark - VRTARObjectTargetPromise Implementation

@implementation VRTARObjectTargetPromise {
    NSString *_key;
    NSDictionary *_source;
    std::shared_ptr<VROARObjectTarget> _arTarget;
    NSMutableArray *_waitBlocks;
    NSURLSessionDataTask *_downloadTask;
    BOOL _assetDownloaded;
}

- (instancetype)initWithKey:(NSString *)key
                     source:(NSDictionary *)source {
    self = [super init];
    if (self) {
        _key = key;
        _source = source;
        _ready = NO;
        _assetDownloaded = NO;
        _waitBlocks = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)dealloc {
    // if the asset was downloaded, delete it from the filesystem once this target is destroyed!
    if (_arTarget && _assetDownloaded) {
        std::shared_ptr<VROARObjectTargetiOS> targetiOS = std::dynamic_pointer_cast<VROARObjectTargetiOS>(_arTarget);
        if (targetiOS) {
            NSURL *localFile = targetiOS->getLocalFileUrl();
            NSFileManager *fileManager = [NSFileManager defaultManager];
            NSError *error;
            BOOL success = [fileManager removeItemAtPath:localFile.absoluteString error:&error];
            if (!success) {
                NSLog(@"[Viro] - could not delete file %@, err: %@", localFile.absoluteString, [error localizedDescription]);
            }
        }
    }
}

- (void)wait:(VRTARObjectTargetPromiseCompletion)finishBlock {
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

    NSURL *url = [NSURL URLWithString:[_source objectForKey:@"uri"]];
    NSString *scheme = url.scheme.lowercaseString;
    
    if([scheme isEqualToString:@"file"] || [scheme isEqualToString:@"http"] || [scheme isEqualToString:@"https"]) {
        if (![scheme isEqualToString:@"file"]) {
            _assetDownloaded = YES;
            _downloadTask = downloadDataWithURL(url, ^(NSData *data, NSError *error) {
                _downloadTask = nil;
                if (!error) {
                    
                    // write the data to a local file!
                    NSString *filename = [NSString stringWithFormat:@"%@-%@", [url lastPathComponent], [[NSProcessInfo processInfo] globallyUniqueString]];
                    NSArray *localPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
                    NSString *documentsDirectory = [localPaths objectAtIndex:0];
                    NSString *filePath = [NSString stringWithFormat:@"%@/%@", documentsDirectory, filename];
                    NSError *error;
                    BOOL success = [data writeToFile:filePath options:NSDataWritingAtomic error:&error];
                    if (success) {
                        // create object target
                        _arTarget = std::make_shared<VROARObjectTargetiOS>([NSURL URLWithString:filePath]);
                    } else {
                        NSLog(@"[Viro] error writing obj to local file %@", [error localizedDescription]);
                    }
                    
                    @synchronized(self) {
                        _ready = YES;
                    }
                    
                    for (VRTARObjectTargetPromiseCompletion completion in _waitBlocks) {
                        completion(_key, _arTarget);
                    }
                    
                    _waitBlocks = nil; // release all the blocks!
                    
                } else {
                    RCTLogError(@"[VRTARTrackingTargets] Error downloading object target source [%@]", _key);
                    for (VRTARObjectTargetPromiseCompletion completion in _waitBlocks) {
                        completion(_key, nullptr);
                    }
                }
            });
        } else {
            _arTarget = std::make_shared<VROARObjectTargetiOS>(url);

            @synchronized(self) {
                _ready = YES;
            }
            
            for (VRTARObjectTargetPromiseCompletion completion in _waitBlocks) {
                completion(_key, _arTarget);
            }
            
            _waitBlocks = nil; // release all the blocks!
        }
    }
}

@end

#pragma mark - VRTARTrackingTargetsModule Implemetation

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

+ (BOOL)requiresMainQueueSetup {
  return NO;
}

- (VRTARImageTargetPromise *)getARImageTargetPromise:(NSString *)name {
    NSObject *obj = _targetsDict[name];
    if (obj && [obj isKindOfClass:[VRTARImageTargetPromise class]]) {
        return (VRTARImageTargetPromise *) obj;
    } else {
        return nil;
    }
}

- (VRTARObjectTargetPromise *)getARObjectTargetPromise:(NSString *)name {
    NSObject *obj = _targetsDict[name];
    if (obj && [obj isKindOfClass:[VRTARObjectTargetPromise class]]) {
        return (VRTARObjectTargetPromise *) obj;
    } else {
        return nil;
    }
}

#pragma mark RCT_EXPORT_METHODS

RCT_EXPORT_MODULE()

RCT_EXPORT_METHOD(createTargets:(NSDictionary *)targets) {
    for (NSString *key in targets) {
        NSDictionary *target = [targets objectForKey:key];
        
        NSString *targetType = [target objectForKey:@"type"];

        if (targetType && [targetType caseInsensitiveCompare:@"Object"] == NSOrderedSame) {
            _targetsDict[key] = [[VRTARObjectTargetPromise alloc] initWithKey:key source:[target objectForKey:@"source"]];
            [_targetsDict[key] fetch];
        } else { // Assume the default is an Image target!
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

            _targetsDict[key] = [[VRTARImageTargetPromise alloc] initWithKey:key
                                                                      source:source
                                                                 orientation:orientation
                                                               physicalWidth:physicalWidth];
            [_targetsDict[key] fetch];
        }
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
