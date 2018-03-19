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
#include <climits>
#include <map>
#include "VROLog.h"
#include "VROAllocationTracker.h"

class VROGlyph;
struct FT_FaceRec_;

enum class VROFontStyle {
    Normal,
    Italic
};

enum class VROFontWeight {
    UltraLight = 100,
    Thin = 200,
    Light = 300,
    Regular = 400,
    Medium = 500,
    Semibold = 600,
    Bold = 700,
    Heavy = 800,
    ExtraBlack = 900
};

class VROTypeface {
    
public:
    
    VROTypeface(std::string name, int size, VROFontStyle style, VROFontWeight weight);
    virtual ~VROTypeface();
    
    std::string getName() const {
        return _name;
    }
    int getSize() const {
        return _size;
    }
    VROFontStyle getStyle() const {
        return _style;
    }
    VROFontWeight getWeight() const {
        return _weight;
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
    VROFontStyle _style;
    VROFontWeight _weight;
    
    /*
     Returns the languages the given face was designed for, and the languages the face
     supports, both as comma-separated strings of BCP-47 identifiers.
     */
    std::pair<std::string, std::string> getLanguages(FT_FaceRec_* face);
    
private:
    
    std::map<unsigned long, std::shared_ptr<VROGlyph>> _glyphCache;
    
};

#endif /* VROTypeface_h */
