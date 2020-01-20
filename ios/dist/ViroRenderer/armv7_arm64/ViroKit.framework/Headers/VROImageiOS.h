//
//  VROImageiOS.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/3/16.
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

#ifndef VROImageiOS_h
#define VROImageiOS_h

#import "VROImage.h"
#import <UIKit/UIKit.h>

class VROImageiOS : public VROImage {
    
public:
    
    /*
     Construct a new VROImage from the given UIImage. The data will be extracted
     from the UIImage into a format compatible with the given target internal format.
     */
    VROImageiOS(UIImage *image, VROTextureInternalFormat format);
    virtual ~VROImageiOS();
    
    int getWidth() const {
        return _width;
    }
    int getHeight() const {
        return _height;
    }
    unsigned char *getData(size_t *length);
    
private:
    
    bool hasAlpha(UIImage *image);
    
    int _width, _height;
    int _dataLength;
    unsigned char *_data;
    
};

#endif /* VROImageiOS_h */
