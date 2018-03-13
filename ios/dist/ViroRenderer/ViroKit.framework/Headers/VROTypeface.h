//
//  VROTypeface.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/24/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROTypeface_h
#define VROTypeface_h

#include <stdio.h>
#include <string>
#include <memory>
#include <map>
#include "VROLog.h"
#include "VROAllocationTracker.h"

class VROGlyph;

class VROTypeface {
    
public:
    
    VROTypeface(std::string name, int size);
    virtual ~VROTypeface();
    
    std::string getName() const {
        return _name;
    }
    
    void loadFace();
    
    /*
     Get the glyph for the given character. If forRendering is true, then the
     texture in the VROGlyph will be populated; otherwise it is left empty. 
     Glyphs are cached when they are retrieved for the first time (if
     forRendering is true), so that future retrievals are faster.
     */
    std::shared_ptr<VROGlyph> getGlyph(unsigned long charCode, bool forRendering);
    
    /*
     Preload the glyphs in the given string, caching them with this typeface.
     */
    void preloadGlyphs(std::string chars);
    
    virtual float getLineHeight() const = 0;
    
protected:
    
    virtual void loadFace(std::string name, int size) = 0;
    virtual std::shared_ptr<VROGlyph> loadGlyph(unsigned long charCode, bool forRendering) = 0;
    
    std::string _name;
    int _size;
    
private:
    
    std::map<unsigned long, std::shared_ptr<VROGlyph>> _glyphCache;
    
};

#endif /* VROTypeface_h */
