//
//  VROTypeface.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/24/16.
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

#ifndef VROTypeface_h
#define VROTypeface_h

#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
#include <climits>
#include <map>
#include "VROLog.h"
#include "VROAllocationTracker.h"
#include "VROSparseBitSet.h"

class VROGlyph;
class VRODriver;
class VROGlyphAtlas;
struct FT_FaceRec_;

enum class VROGlyphRenderMode;
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
    
    /*
     Construct a new typeface with the given family name, size, style, and weight.
     If the provided family name is empty, then the system default font will be
     used.
     */
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
    
    /*
     Load the face data from the underlying font system. This must be invoked
     before loading any other data.
     */
    void loadFace();
    
    /*
     Returns true if this typeface has the character with the given code-point
     and variation. If the variation selector is 0, then we will not check for
     the variation.
     */
    bool hasCharacter(uint32_t codePoint, uint32_t variationSelector) const;
    
    /*
     Get the glyph for the given character. If renderMode is Bitmap, then the
     texture (and related bitmap properties) in the VROGlyph will be populated;
     if renderMode is Vector, then the triangles in the glyph will be populated.
     
     Glyphs are cached (once for each render mode) when they are retrieved for the
     first time, so that future retrievals are faster.
     
     If the variant selector is 0, then we assume this is not a variation sequence.
     
     The outlineWidth parameter determines the size of the outline to use. This is
     only valid for VROGlyphRenderMode::OutlinedBitmap.
     */
    std::shared_ptr<VROGlyph> getGlyph(uint32_t codePoint, uint32_t variantSelector,
                                       uint32_t outlineWidth, VROGlyphRenderMode renderMode);
    
    /*
     Refresh the texture of all glyph atlases used by this typeface.
     */
    void refreshGlyphAtlases(std::shared_ptr<VRODriver> driver);
    
    /*
     Preload the glyphs in the given string, caching them with this typeface.
     */
    void preloadGlyphs(std::string chars);
    
    /*
     Get the line height of this typeface.
     */
    virtual float getLineHeight() const = 0;
    
protected:
    
    virtual FT_FaceRec_ *loadFTFace() = 0;
    virtual std::shared_ptr<VROGlyph> loadGlyph(uint32_t charCode, uint32_t variantSelector,
                                                uint32_t outlineWidth, VROGlyphRenderMode renderMode) = 0;
    
    std::string _name;
    std::vector<std::shared_ptr<VROGlyphAtlas>> _glyphAtlases;
    std::map<int, std::vector<std::shared_ptr<VROGlyphAtlas>>> _outlineAtlases;
    
    // TODO VIRO-3239 Move these to VROFont. VROTypeface is essentially a "font family"
    int _size;
    VROFontStyle _style;
    VROFontWeight _weight;
    
    /*
     Returns the languages the given face was designed for, and the languages the face
     supports, both as comma-separated strings of BCP-47 identifiers.
     */
    std::pair<std::string, std::string> getLanguages(FT_FaceRec_* face);
    
private:
    
    VROSparseBitSet _coverage;
    std::vector<std::unique_ptr<VROSparseBitSet>> _variationCoverage;
    std::map<std::string, std::shared_ptr<VROGlyph>> _bitmapGlyphCache, _vectorGlyphCache;
    
    /*
     Compute the charmap coverage of this typeface.
     */
    void computeCoverage(FT_FaceRec_* face);
    
};

#endif /* VROTypeface_h */
