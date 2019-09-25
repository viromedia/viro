//
//  VROTime.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/21/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROTIME_H_
#define VROTIME_H_

#include <time.h>
#include <stdlib.h>
#include <stdint.h>

/*
 Get the current time in seconds or milliseconds.
 */
double VROTimeCurrentSeconds();
double VROTimeCurrentMillis();

/*
 Get the current calendar time (seconds since 1970).
 */
uint64_t VROTimeGetCalendarTime();

uint64_t VRONanoTime();

#endif /* VROTIME_H_ */
