//
//  VROControllerInputType.h
//  ViroRenderer
//
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

#ifndef VROInputType_h
#define VROInputType_h

#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include <set>
#include <float.h>

/**
 * Header file containing sets of input sources that are grouped to
 * a VR platform.
 */
namespace ViroDayDream{
    enum InputSource{
        Controller = 1,
        TouchPad = 2,
        AppButton = 3,
        HomeButton = 4,
        VolUpButton = 5,
        VolDownButton = 6
    };
}

namespace ViroCardBoard{
    enum InputSource{
        Controller=1,
        ViewerButton = 2
    };
}

namespace ViroOculus{
    enum InputSource{
        Controller = 1,
        TouchPad = 2,
        BackButton = 3
    };
}
#endif
