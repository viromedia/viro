//
//  VROImageiOS.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/3/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROImageiOS_h
#define VROImageiOS_h

#import "VROImage.h"
#import <UIKit/UIKit.h>

class VROImageiOS : public VROImage {
    
public:
    
    VROImageiOS(UIImage *image);
    virtual ~VROImageiOS();
    
    int getWidth() const {
        return _width;
    }
    int getHeight() const {
        return _height;
    }
    unsigned char *extractRGBA8888(size_t *length);
    
private:
    
    int _width, _height;
    int _dataLength;
    unsigned char *_data;
    
};

#endif /* VROImageiOS_h */
