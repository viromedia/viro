//
//  VRTCubeMap.m
//  React
//
//  Created by Raj Advani on 10/6/16.
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
