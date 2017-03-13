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
    
    /*
     Construct a new VROImage from the given UIImage. The data will 
     be extracted from the UIImage into a format compatible with the
     given target internal format.
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
    
    int _width, _height;
    int _dataLength;
    unsigned char *_data;
    
};

#endif /* VROImageiOS_h */
