//
//  VROSoundDelegate.h
//  ViroRenderer
//
//  Created by Andy Chu on 1/27/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROSoundDelegate_h
#define VROSoundDelegate_h

@protocol VROSoundDelegate <NSObject>

- (void)soundIsReady;
- (void)soundDidFinish;

@end

#endif /* VROSoundDelegate_h */
