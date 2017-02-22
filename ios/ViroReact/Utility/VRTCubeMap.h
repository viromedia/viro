//
//  VRTCubeMap.h
//  React
//
//  Created by Raj Advani on 10/6/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTConvert.h>
#import <React/RCTImageSource.h>

@interface VRTCubeMap : NSObject

@property (readwrite, nonatomic) RCTImageSource *px;
@property (readwrite, nonatomic) RCTImageSource *nx;
@property (readwrite, nonatomic) RCTImageSource *py;
@property (readwrite, nonatomic) RCTImageSource *ny;
@property (readwrite, nonatomic) RCTImageSource *pz;
@property (readwrite, nonatomic) RCTImageSource *nz;

+ (NSArray <NSString *> *) keys;
- (RCTImageSource *)imageSourceForKey:(NSString *)key;

@end

@interface RCTConvert (VRTCubeMap)

+ (VRTCubeMap *)VRTCubeMap:(id)json;

@end
