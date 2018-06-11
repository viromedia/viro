//
//  VROTypefaceCollection.h
//  ViroKit
//
//  Created by Raj Advani on 3/21/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROTypefaceCollection_h
#define VROTypefaceCollection_h

#include <stdio.h>
#include <memory>
#include <vector>

class VROTypeface;

/*
 The run of a given font over a span of text.
 */
class VROFontRun {
public:
    int start; // inclusive
    int end;   // exclusive
    std::shared_ptr<VROTypeface> typeface;
    
    VROFontRun(int start, int end, std::shared_ptr<VROTypeface> typeface) :
               start(start), end(end), typeface(typeface) {}
};

/*
 VROTypefaceCollection contains a set of typefaces (or font families) that can
 be used in concert to render a block of text. The text will be subdivided
 into "runs", each rendered with the most suitable typeface. This enables
 cross-language text if a single font collection contains typefaces that each
 contain a different character map.
 */
class VROTypefaceCollection {
public:
    
    /*
     Construct a new typeface collection from the given typefaces. Convenience
     constructor is provided for creating a single-typeface collection.
     */
    VROTypefaceCollection(std::shared_ptr<VROTypeface> typeface);
    VROTypefaceCollection(std::vector<std::shared_ptr<VROTypeface>> typefaces);
    virtual ~VROTypefaceCollection();
    
    /*
     Return the typefaces from this collection that are best suited to rendering
     the provided text. The typefaces are returned in runs: each typeface will
     render its designated range. A single typeface may be used in multiple runs
     (e.g. if we switch from one language to another and back).
     */
    std::vector<VROFontRun> computeRuns(std::wstring text);
    
    /*
     Get all the typefaces in this collection.
     */
    const std::vector<std::shared_ptr<VROTypeface>> &getTypefaces() { return _typefaces; }
    
private:
    
    /*
     The typefaces used by this collection.
     */
    std::vector<std::shared_ptr<VROTypeface>> _typefaces;
    
    /*
     Compute the coverage 'score' the typeface earns for the provided glyph.
     The typeface with the highest score will render the glyph.
     */
    uint32_t computeCoverageScore(std::shared_ptr<VROTypeface> &typeface,
                                  uint32_t codePoint, uint32_t variationSelector) const;
    
};

#endif /* VROTypefaceCollection_h */
