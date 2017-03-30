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
#include <ft2build.h>
#include FT_FREETYPE_H
#include "VROLog.h"

class VROGlyph;

class VROTypeface {
    
public:
    
    VROTypeface(std::string name, int size) :
        _name(name),
        _size(size) {
    
        if (FT_Init_FreeType(&_ft)) {
            pabort("Could not initialize freetype library");
        }
    }
    
    virtual ~VROTypeface() {
        FT_Done_Face(_face);
        FT_Done_FreeType(_ft);
    }
    
    std::string getName() const {
        return _name;
    }
    
    void loadFace() {
        _face = loadFace(_name, _size, _ft);
    }
    
    /*
     Get the glyph for the given character. If forRendering is true, then the
     texture in the VROGlyph will be populated; otherwise it is left empty. 
     Glyphs are cached when they are retrieved for the first time (if
     forRendering is true), so that future retrievals are faster.
     */
    std::shared_ptr<VROGlyph> getGlyph(FT_ULong charCode, bool forRendering) {
        auto kv = _glyphCache.find(charCode);
        if (kv != _glyphCache.end()) {
            return kv->second;
        }
        
        std::shared_ptr<VROGlyph> glyph = loadGlyph(charCode, forRendering);
        if (forRendering) {
            _glyphCache[charCode] = glyph;
        }
        return glyph;
    }
    
    /*
     Preload the glyphs in the given string, caching them with this typeface.
     */
    void preloadGlyphs(std::string chars) {
        for (std::string::const_iterator c = chars.begin(); c != chars.end(); ++c) {
            getGlyph(*c, true);
        }
    }
    
    float getLineHeight() const {
        return _face->size->metrics.height >> 6;
    }
    
protected:
    
    virtual FT_Face loadFace(std::string name, int size, FT_Library ft) = 0;
    virtual std::shared_ptr<VROGlyph> loadGlyph(FT_ULong charCode, bool forRendering) = 0;
    
    std::string _name;
    int _size;
    
    FT_Library _ft;
    FT_Face _face;
    
private:
    
    std::map<FT_ULong, std::shared_ptr<VROGlyph>> _glyphCache;
    
};

#endif /* VROTypeface_h */
