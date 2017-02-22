//
//  VRTSoundModule.mm
//  ViroReact
//
//  Created by Andy Chu on 2/2/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTSoundModule.h"
#import <React/RCTLog.h>

@interface VRTSoundModule ()

@end

@implementation VRTSoundModule {

    std::map<std::string, std::shared_ptr<VROSoundData>> _preloadedSounds;

}

RCT_EXPORT_MODULE()

RCT_EXPORT_METHOD(preloadSounds:(NSDictionary *)soundDict) {
    for (id key in soundDict) {
        NSString *valueString = (NSString *)[soundDict objectForKey:key];
        std::string cValueString = std::string([valueString UTF8String]);
        std::string cKeyString = std::string([((NSString *) key) UTF8String]);
        std::shared_ptr<VROSoundDataGVR> data = VROSoundDataGVR::create(cValueString, false);
        _preloadedSounds[cKeyString] = data;
    }
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
