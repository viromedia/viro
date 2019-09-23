//
//  VRTSoundModule.mm
//  ViroReact
//
//  Created by Andy Chu on 2/2/17.
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

#import "VRTSoundModule.h"
#import <React/RCTLog.h>
#import <React/RCTConvert.h>
#import <React/RCTUtils.h>

@interface VRTSoundModule ()
@end

@implementation VRTSoundModule {
    std::map<std::string, std::shared_ptr<VROSoundData>> _preloadedSounds;
    std::map<std::string, std::shared_ptr<VROSoundDataDelegateiOS>> _preloadCallbacks;
}

RCT_EXPORT_MODULE()
RCT_EXPORT_METHOD(preloadSounds:(NSDictionary *)soundDict
                  resolver:(RCTPromiseResolveBlock)resolve
                  rejecter:(RCTPromiseRejectBlock)reject) {
    for (id key in soundDict) {
        NSString *path = [self parseImagePath:soundDict[key]];
        if (path == nil) {
            RCTLogError(@"Invalid preloaded sound path provided.");
            return;
        }
        std::string cKeyString = std::string([((NSString *) key) UTF8String]);
        std::string cPathString = std::string([((NSString *) path) UTF8String]);
        std::shared_ptr<VROSoundDataGVR> data = VROSoundDataGVR::create(cPathString, VROResourceType::URL);
        _preloadedSounds[cKeyString] = data;
        
        if (resolve) {
            // Create a finish preloading callback with the promise for notifying Javascript with.
            std::function<void(bool success, std::string msg)> onFinish = [self, cKeyString, resolve](bool success, std::string msg) {
                if (_preloadCallbacks.find(cKeyString) != _preloadCallbacks.end()) {
                    _preloadCallbacks.erase(cKeyString);
                }
                resolve(@{
                          @"key" : @(cKeyString.c_str()),
                          @"result" : @(success),
                          @"msg" : @(msg.c_str()),
                          });
            };
            
            // Store a strong reference to the sound delegate callback for the given sound key.
            std::shared_ptr<VROSoundDataDelegateiOS> callback = std::make_shared<VROSoundDataDelegateiOS>(onFinish);
            _preloadCallbacks[cKeyString] = callback;
            data->setDelegate(callback);
        }
    }
}

- (NSString *)parseImagePath:(id)json {
    NSString *path;
    if ([json isKindOfClass:[NSString class]]) {
        path = json;
    } else if ([json isKindOfClass:[NSDictionary class]]) {
        NSDictionary *dictionary = (NSDictionary *)json;
        path = [RCTConvert NSString:dictionary[@"uri"]];
        if (!path) {
            return nil;
        }
    } else {
        return nil;
    }
    return path;
}

RCT_EXPORT_METHOD(unloadSounds:(NSArray<NSString *> *)soundArray) {
    for (id name in soundArray) {
        NSString *nameString = (NSString *)name;
        std::string cName = std::string([nameString UTF8String]);
        auto it = _preloadedSounds.find(cName);
        
        // "Unloading" a sound simply consists of removing it from our map. If something else is still using
        // the sound data, then the data won't actually be unloaded until all references to it are gone.
        if (it != _preloadedSounds.end()) {
            _preloadedSounds.erase(it);
        }
    }
}

- (std::shared_ptr<VROSoundData>)dataForName:(NSString *)name {
    std::string cName = std::string([name UTF8String]);
    auto it = _preloadedSounds.find(cName);
    if (it != _preloadedSounds.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

@end
