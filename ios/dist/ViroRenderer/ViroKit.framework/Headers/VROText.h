//
//  VROText.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/24/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

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

class VROTextLine {
public:
    std::wstring line;
    float spacingRatio;
    
    VROTextLine(std::wstring &line) : line(line), spacingRatio(1) {}
    VROTextLine(std::wstring &line, float spacingRatio) : line(line), spacingRatio(spacingRatio) {}
    virtual ~VROTextLine() {}
};

class VROText : public VROGeometry {
    
public:
    
    /*
     Create a text object for displaying the given string with the given typeface,
     constrained to the bounds defined by the provided width and height, and aligned
     according to the given alignment parameters and linebreak mode. 
     
     The clip mode determines whether the text is clipped to the given bounds.
     
     The maxLines parameter, if set, caps the number of lines; when zero, there is no
     limit to the number of lines generated.
     */
    static std::shared_ptr<VROText> createText(std::wstring text, std::shared_ptr<VROTypeface> typeface, VROVector4f color,
                                               float width, float height,
                                               VROTextHorizontalAlignment horizontalAlignment, VROTextVerticalAlignment verticalAlignment,
                                               VROLineBreakMode lineBreakMode, VROTextClipMode clipMode, int maxLines = 0);
    
    /*
     Helper method to create a single-line text in a horizontal box of the given width.
     The box is centered at the parent node's position, and the text is aligned within the
     box according to the given alignment.
     */
    static std::shared_ptr<VROText> createSingleLineText(std::wstring text, std::shared_ptr<VROTypeface> typeface, VROVector4f color,
                                                         float width, VROTextHorizontalAlignment alignment, VROTextClipMode clipMode);

    /*
     Helper method to create a centered single-line text. The text will be centered (vertically
     and horizontally) about the parent node's position.
     */
    static std::shared_ptr<VROText> createSingleLineText(std::wstring text, std::shared_ptr<VROTypeface> typeface, VROVector4f color);
    
    /*
     Return the width and height of a text object displaying the given string, with the
     given typeface.
     */
    static VROVector3f getTextSize(std::wstring text, std::shared_ptr<VROTypeface> typeface,
                                   float maxWidth, float maxHeight, VROLineBreakMode lineBreakMode,
                                   VROTextClipMode clipMode, int maxLines = 0);

    /*
     Standard constructor. Update() must be invoked from the rendering thread if this constructor
     is used.
     */
    VROText(std::wstring text, std::shared_ptr<VROTypeface> typeface, VROVector4f color,
            float width, float height,
            VROTextHorizontalAlignment horizontalAlignment, VROTextVerticalAlignment verticalAlignment,
            VROLineBreakMode lineBreakMode, VROTextClipMode clipMode, int maxLines);
    virtual ~VROText();

    /*
     Initialize the VROText if any fields have changed. This must be invoked on the rendering
     thread because it creates glyphs.
     */
    void update();
    
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
    void setTypeface(std::shared_ptr<VROTypeface> typeface);
    void setColor(VROVector4f color);
    void setWidth(float width);
    void setHeight(float height);
    void setHorizontalAlignment(VROTextHorizontalAlignment horizontalAlignment);
    void setVerticalAlignment(VROTextVerticalAlignment verticalAlignment);
    void setLineBreakMode(VROLineBreakMode lineBreakMode);
    void setClipMode(VROTextClipMode clipMode);
    void setMaxLines(int maxLines);
    
private:

    std::wstring _text;
    std::shared_ptr<VROTypeface> _typeface;
    VROVector4f _color;
    float _width, _height;
    VROTextHorizontalAlignment _horizontalAlignment;
    VROTextVerticalAlignment _verticalAlignment;
    VROLineBreakMode _lineBreakMode;
    VROTextClipMode _clipMode;
    int _maxLines;

    std::atomic<float> _realizedWidth, _realizedHeight;
    
    VROText(std::vector<std::shared_ptr<VROGeometrySource>> sources,
            std::vector<std::shared_ptr<VROGeometryElement>> elements,
            float width, float height) :
        VROGeometry(sources, elements),
        _width(width),
        _height(height)
    {}
    
    static void buildText(std::wstring &text,
                          std::shared_ptr<VROTypeface> &typeface,
                          VROVector4f color,
                          float width,
                          float height,
                          VROTextHorizontalAlignment horizontalAlignment,
                          VROTextVerticalAlignment verticalAlignment,
                          VROLineBreakMode lineBreakMode,
                          VROTextClipMode clipMode,
                          int maxLines,
                          std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                          std::vector<std::shared_ptr<VROGeometryElement>> &elements,
                          std::vector<std::shared_ptr<VROMaterial>> &materials,
                          float *outRealizedWidth, float *outRealizedHeight);
    
    /*
     Build a standard Viro geometry from the given vertex array and material/indices
     pairs.
     */
    static void buildGeometry(std::vector<VROShapeVertexLayout> &var,
                              std::map<unsigned long, std::pair<std::shared_ptr<VROMaterial>, std::vector<int>>> &materialMap,
                              std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                              std::vector<std::shared_ptr<VROGeometryElement>> &elements,
                              std::vector<std::shared_ptr<VROMaterial>> &materials);
    
    /*
     Write the geometry for the given glyph (at the given position) into the
     provided vertex array, and write the associated indices into the indices 
     array as well.
     */
    static void buildChar(std::shared_ptr<VROGlyph> &glyph,
                          float x, float y, 
                          std::vector<VROShapeVertexLayout> &var,
                          std::vector<int> &indices);
    
    /*
     Simple methods for processing the line-break mode. All of the methods below use a 
     'greedy' algorithm, filling as much space in the current line as possible then moving
     to the next line. These methods also introduce a newline on hard breaks (i.e. whenever
     the '\n' character is encountered). In particular, the wrapByNewlines function *only*
     processes hard breaks; the rest process both hard and soft.
     
     These functions also handle clipping. When char/word wrapping is on, we only have to
     clip text vertically (horizontal edges are implicitly taken care of by the wrapping
     function). When char/word wrapping is off, we also have to clip text horizontally.
     */
    static std::vector<VROTextLine> wrapByWords(std::wstring &text, float maxWidth, float maxHeight, int maxLines,
                                                std::shared_ptr<VROTypeface> &typeface,
                                                VROTextClipMode clipMode,
                                                std::map<unsigned long, std::shared_ptr<VROGlyph>> &glyphMap);
    static std::vector<VROTextLine> wrapByChars(std::wstring &text, float maxWidth, float maxHeight, int maxLines,
                                                std::shared_ptr<VROTypeface> &typeface,
                                                VROTextClipMode clipMode,
                                                std::map<unsigned long, std::shared_ptr<VROGlyph>> &glyphMap);
    static std::vector<VROTextLine> wrapByNewlines(std::wstring &text, float maxWidth, float maxHeight, int maxLines,
                                                   std::shared_ptr<VROTypeface> &typeface,
                                                   VROTextClipMode clipMode,
                                                   std::map<unsigned long, std::shared_ptr<VROGlyph>> &glyphMap);
    
    /*
     Justification routine. Considerably more complex than the greedy algorithms above. Note that
     justification is a word-wrapping technique that reduces the 'raggedness' of the text edges;
     it can be used with left, right, and centered horizontal alignment. To achieve traditional 
     justified text as seen in newspapers, use it with VROTextHorizontalAlignment::Left.
     */
    static std::vector<VROTextLine> justify(std::wstring &text, float maxWidth, float maxHeight, int maxLines,
                                            std::shared_ptr<VROTypeface> &typeface,
                                            VROTextClipMode clipMode,
                                            std::map<unsigned long, std::shared_ptr<VROGlyph>> &glyphMap);
    
    /*
     Helpers for wrapping/clipping.
     */
    static std::vector<std::wstring> divideIntoParagraphs(std::wstring &text);
    static float getLengthOfWord(const std::wstring &word, std::map<unsigned long, std::shared_ptr<VROGlyph>> &glyphMap);

    static bool isAnotherLineAvailable(size_t numLinesNow, float maxHeight, int maxLines,
                                       std::shared_ptr<VROTypeface> &typeface, VROTextClipMode clipMode);
    

};

#endif /* VROText_h */
