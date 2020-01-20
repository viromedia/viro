//
//  VROText.h
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

#ifndef VROText_h
#define VROText_h

#include <stdio.h>
#include <map>
#include <atomic>
#include "VROGeometry.h"
#include "VROShapeUtils.h"
#include "VROVector4f.h"

class VROMaterial;
class VROTexture;
class VROGlyph;
class VROGlyphAtlas;
class VROGlyphBitmap;
class VROTypefaceCollection;

static const float kTextPointToWorldScale = 0.01;

enum class VROTextOuterStroke {
    None,
    Outline,
    DropShadow
};

enum class VROTextHorizontalAlignment {
    Left,
    Right,
    Center
};

enum class VROTextVerticalAlignment {
    Top,
    Bottom,
    Center
};

enum class VROLineBreakMode {
    WordWrap,
    CharWrap,
    Justify,
    None
};

enum class VROTextClipMode {
    ClipToBounds,
    None
};

class VROTextLayout {
    float width;
    float height;
    VROTextHorizontalAlignment horizontalAlignment;
    VROTextVerticalAlignment verticalAlignment;
};

class VROText : public VROGeometry {
    
public:
    
    /*
     Create a text object for displaying the given string with the typefaces in the
     given collection, constrained to the bounds defined by the provided width and height,
     and aligned according to the given alignment parameters and linebreak mode.
     
     The clip mode determines whether the text is clipped to the given bounds.
     
     The maxLines parameter, if set, caps the number of lines; when zero, there is no
     limit to the number of lines generated.
     
     The typeface to use for each character will be chosen from among the typeface collection.
     */
    static std::shared_ptr<VROText> createText(std::wstring text,
                                               std::string typefaceNames,
                                               int fontSize, VROFontStyle fontStyle, VROFontWeight fontWeight,
                                               VROVector4f color, float extrusion,
                                               float width, float height,
                                               VROTextHorizontalAlignment horizontalAlignment, VROTextVerticalAlignment verticalAlignment,
                                               VROLineBreakMode lineBreakMode, VROTextClipMode clipMode, int maxLines,
                                               std::shared_ptr<VRODriver> driver);
    
    /*
     Helper method to create a single-line text in a horizontal box of the given width.
     The box is centered at the parent node's position, and the text is aligned within the
     box according to the given alignment.
     */
    static std::shared_ptr<VROText> createSingleLineText(std::wstring text,
                                                         std::string typefaceNames,
                                                         int fontSize, VROFontStyle fontStyle, VROFontWeight fontWeight,
                                                         VROVector4f color, float extrusion,
                                                         float width, VROTextHorizontalAlignment alignment, VROTextClipMode clipMode,
                                                         std::shared_ptr<VRODriver> driver);

    /*
     Helper method to create a centered single-line text. The text will be centered (vertically
     and horizontally) about the parent node's position.
     */
    static std::shared_ptr<VROText> createSingleLineText(std::wstring text,
                                                         std::string typefaceNames,
                                                         int fontSize, VROFontStyle fontStyle, VROFontWeight fontWeight,
                                                         VROVector4f color, float extrusion,
                                                         std::shared_ptr<VRODriver> driver);
    
    /*
     Return the width and height of a text object displaying the given string, with the
     given typeface collection.
     */
    static VROVector3f getTextSize(std::wstring text,
                                   std::shared_ptr<VROTypefaceCollection> typefaces,
                                   float maxWidth, float maxHeight, VROLineBreakMode lineBreakMode,
                                   VROTextClipMode clipMode, int maxLines);

    /*
     Standard constructor. Update() must be invoked from the rendering thread if this constructor
     is used.
     */
    VROText(std::wstring text,
            std::string typefaceNames,
            int fontSize, VROFontStyle fontStyle, VROFontWeight fontWeight,
            VROVector4f color, float extrusion,
            VROTextOuterStroke stroke, int strokeWidth, VROVector4f strokeColor,
            float width, float height,
            VROTextHorizontalAlignment horizontalAlignment, VROTextVerticalAlignment verticalAlignment,
            VROLineBreakMode lineBreakMode, VROTextClipMode clipMode, int maxLines,
            std::shared_ptr<VRODriver> driver);
    virtual ~VROText();

    /*
     Initialize the VROText if any fields have changed. This must be invoked on the rendering
     thread because it creates glyphs.
     */
    void update();

    /*
     Return the typeface collection used by this text. This is only valid after update() has
     been called.
     */
    std::shared_ptr<VROTypefaceCollection> getTypefaceCollection() {
        return _typefaceCollection;
    }
    
    /*
     Get the width and height of the text. This is not the width and height of the
     bounds used when the text was created, but the width and height of the actual
     text.
     */
    float getRealizedWidth() const {
        return _realizedWidth;
    }
    float getRealizedHeight() const {
        return _realizedHeight;
    }

    void setText(std::wstring text);
    void setTypefaces(std::string typefaces, int size, VROFontStyle style, VROFontWeight weight);
    void setColor(VROVector4f color);
    void setWidth(float width);
    void setHeight(float height);
    void setHorizontalAlignment(VROTextHorizontalAlignment horizontalAlignment);
    void setVerticalAlignment(VROTextVerticalAlignment verticalAlignment);
    void setLineBreakMode(VROLineBreakMode lineBreakMode);
    void setClipMode(VROTextClipMode clipMode);
    void setMaxLines(int maxLines);
    void setExtrusion(float extrusion);
    void setOuterStroke(VROTextOuterStroke stroke, int strokeWidth, VROVector4f strokeColor);
    void setMaterials(std::vector<std::shared_ptr<VROMaterial>> materials);

private:

    std::wstring _text;
    std::shared_ptr<VROTypefaceCollection> _typefaceCollection;
    std::string _typefaceNames;
    int _size;
    VROFontStyle _fontStyle;
    VROFontWeight _fontWeight;
    VROVector4f _color;
    float _extrusion;
    float _width, _height;
    VROTextHorizontalAlignment _horizontalAlignment;
    VROTextVerticalAlignment _verticalAlignment;
    VROLineBreakMode _lineBreakMode;
    VROTextClipMode _clipMode;
    int _maxLines;
    VROTextOuterStroke _outerStroke;
    int _outerStrokeWidth;
    VROVector4f _outerStrokeColor;
    std::weak_ptr<VRODriver> _driver;

    std::atomic<float> _realizedWidth, _realizedHeight;
    
    VROText(std::vector<std::shared_ptr<VROGeometrySource>> sources,
            std::vector<std::shared_ptr<VROGeometryElement>> elements,
            float width, float height) :
        VROGeometry(sources, elements),
        _width(width),
        _height(height)
    {}
    
    static void buildBitmapText(std::wstring &text,
                                std::shared_ptr<VROTypefaceCollection> &typefaces,
                                VROVector4f color,
                                VROTextOuterStroke outerStroke, int outerStrokeWidth, VROVector4f outerStrokeColor,
                                float width, float height,
                                VROTextHorizontalAlignment horizontalAlignment,
                                VROTextVerticalAlignment verticalAlignment,
                                VROLineBreakMode lineBreakMode,
                                VROTextClipMode clipMode,
                                int maxLines,
                                std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                                std::vector<std::shared_ptr<VROGeometryElement>> &elements,
                                std::vector<std::shared_ptr<VROMaterial>> &materials,
                                float *outRealizedWidth, float *outRealizedHeight,
                                std::shared_ptr<VRODriver> driver);
    
    /*
     Build a standard Viro geometry from the given vertex array and material/indices
     pairs.
     */
    static void buildBitmapGeometry(std::vector<VROShapeVertexLayout> &var,
                                    std::map<std::shared_ptr<VROGlyphAtlas>, std::pair<std::shared_ptr<VROMaterial>, std::vector<int>>> &materialMap,
                                    std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                                    std::vector<std::shared_ptr<VROGeometryElement>> &elements,
                                    std::vector<std::shared_ptr<VROMaterial>> &materials);
    
    /*
     Write the geometry for the given glyph (at the given position) into the
     provided vertex array, and write the associated indices into the indices
     array as well.
     */
    static void buildBitmapChar(const VROGlyphBitmap &bitmap,
                                float x, float y,
                                float offsetX, float offsetY,
                                std::vector<VROShapeVertexLayout> &var,
                                std::vector<int> &indices);
    
    static void buildVectorizedText(std::wstring &text,
                                    std::shared_ptr<VROTypefaceCollection> &typefaces,
                                    VROVector4f color,
                                    float extrusion,
                                    float width,
                                    float height,
                                    VROTextHorizontalAlignment horizontalAlignment,
                                    VROTextVerticalAlignment verticalAlignment,
                                    VROLineBreakMode lineBreakMode,
                                    VROTextClipMode clipMode,
                                    int maxLines,
                                    const std::vector<std::shared_ptr<VROMaterial>> &existingMaterials,
                                    std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                                    std::vector<std::shared_ptr<VROGeometryElement>> &elements,
                                    std::vector<std::shared_ptr<VROMaterial>> &materials,
                                    float *outRealizedWidth, float *outRealizedHeight,
                                    std::shared_ptr<VRODriver> driver);
    
    /*
     Build a standard Viro geometry from the given vertex array and indices.
     */
    static void buildVectorizedGeometry(std::vector<VROShapeVertexLayout> &var,
                                        std::vector<std::vector<int>> indices,
                                        std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                                        std::vector<std::shared_ptr<VROGeometryElement>> &elements);
    
    /*
     Write the geometry for the given glyph (at the given position) into the
     provided vertex array, and write the associated indices into the indices
     array as well.
     */
    static void buildVectorizedChar(std::shared_ptr<VROGlyph> &glyph,
                                    float x, float y, float extrusion,
                                    std::vector<VROShapeVertexLayout> &var,
                                    std::vector<int> &frontIndices,
                                    std::vector<int> &backIndices,
                                    std::vector<int> &sideIndices);

};

#endif /* VROText_h */
