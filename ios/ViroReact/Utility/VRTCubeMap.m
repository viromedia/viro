//
//  VRTCubeMap.m
//  React
//
//  Created by Raj Advani on 10/6/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTCubeMap.h"

@implementation RCTConvert (VRTCubeMap)

+ (VRTCubeMap *)VRTCubeMap:(id)json {
    if (!json) {
        RCTLogError(@"Error setting cube map type, received nil json: %@", json);
        return nil;
    }
    
    if (![json isKindOfClass:[NSDictionary class]]) {
        RCTLogError(@"Error setting cube map type. Dictionary required, recieved: %@", json);
        return nil;
    }
    
    VRTCubeMap *cubeMap = [[VRTCubeMap alloc] init];
    NSDictionary *dict = (NSDictionary *)json;
    
    id pxJson = [dict objectForKey:@"px"];
    if (!pxJson) {
        RCTLogError(@"Error setting cube map type. No px image provided, recieved: %@", json);
    }
    cubeMap.px = [RCTConvert RCTImageSource:pxJson];
    
    id nxJson = [dict objectForKey:@"nx"];
    if (!nxJson) {
        RCTLogError(@"Error setting cube map type. No nx image provided, recieved: %@", json);
    }
    cubeMap.nx = [RCTConvert RCTImageSource:nxJson];
    
    id pyJson = [dict objectForKey:@"py"];
    if (!pyJson) {
        RCTLogError(@"Error setting cube map type. No py image provided, recieved: %@", json);
    }
    cubeMap.py = [RCTConvert RCTImageSource:pyJson];
    
    id nyJson = [dict objectForKey:@"ny"];
    if (!nyJson) {
        RCTLogError(@"Error setting cube map type. No ny image provided, recieved: %@", json);
    }
    cubeMap.ny = [RCTConvert RCTImageSource:nyJson];
    
    id pzJson = [dict objectForKey:@"pz"];
    if (!pzJson) {
        RCTLogError(@"Error setting cube map type. No pz image provided, recieved: %@", json);
    }
    cubeMap.pz = [RCTConvert RCTImageSource:pzJson];
    
    id nzJson = [dict objectForKey:@"nz"];
    if (!nzJson) {
        RCTLogError(@"Error setting cube map type. No nz image provided, recieved: %@", json);
    }
    cubeMap.nz = [RCTConvert RCTImageSource:nzJson];
    return cubeMap;
}

@end

@implementation VRTCubeMap

+ (NSArray <NSString *> *) keys {
    return @[@"px", @"nx", @"py", @"ny", @"pz", @"nz" ];
}

- (RCTImageSource *)imageSourceForKey:(NSString *)key {
    if (key == @"px") {
        return self.px;
    }
    if (key == @"nx") {
        return self.nx;
    }
    if (key == @"py") {
        return self.py;
    }
    if (key == @"ny") {
        return self.ny;
    }
    if (key == @"pz") {
        return self.pz;
    }
    if (key == @"nz") {
        return self.nz;
    }
    [NSException raise:@"Invalid cube key" format:@"Invalid cube key %@", key];
    return nil;
}


@end
