//
//  VROPolyline.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/12/16.
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

#include <memory>
#include <vector>
#include "VROGeometry.h"
#include "VROAnimatable.h"

class VROVector3f;
class VROByteBuffer;
class VROLineSegment;
class VROShaderModifier;

/*
 The style used to join segments. Round uses more triangles but generally looks better
 for all shapes. Bevel means corners are clipped off, and is used for performance
 reasons.
 */
enum class VROPolylineJoinStyle {
    Round,
    Bevel,
};

class VROPolyline : public VROGeometry {
    
public:
    static std::shared_ptr<VROPolyline> createPolyline(std::vector<std::vector<VROVector3f>> &path, float thickness,
                                                       VROPolylineJoinStyle joinStyle = VROPolylineJoinStyle::Round);
    static std::shared_ptr<VROPolyline> createPolyline(std::vector<VROVector3f> &path, float thickness,
                                                       VROPolylineJoinStyle joinStyle = VROPolylineJoinStyle::Round);
    
    VROPolyline();
    virtual ~VROPolyline() {}
    
    /*
     Set the thickness. Animatable.
     */
    void setThickness(float thickness);
    float getThickness() const {
        return _thickness;
    }
    
    /*
     Set the join style, which determines how segments in the polyline are connected
     together.
     */
    void setJoinStyle(VROPolylineJoinStyle joinStyle);
    VROPolylineJoinStyle getJoinStyle() const {
        return _joinStyle;
    }
    
    /*
     Set the paths of this polyline. Each path is a contiguous line. This will reconstruct
     all paths.
     */
    void setPaths(std::vector<std::vector<VROVector3f>> &paths);
    
    /*
     Append the given point the last path in this polyline. This is more efficient
     than invoking setPaths.
     */
    void appendPoint(VROVector3f point);

    virtual void setMaterials(std::vector<std::shared_ptr<VROMaterial>> materials);

private:
    
    float _thickness;
    std::vector<std::vector<VROVector3f>> _paths;
    VROPolylineJoinStyle _joinStyle;
    
    VROPolyline(std::vector<std::vector<VROVector3f>> paths, float thickness, VROPolylineJoinStyle joinStyle);
    
    /*
     Update the underlying geometry using the latest instance variables.
     */
    void update();
    
    /*
     Return true if there are no paths in this polyline.
     */
    bool isEmpty() const;
    
    /*
     Get the last point in the polyline.
     */
    VROVector3f getLastPoint() const;
    
    static void buildGeometry(std::vector<std::vector<VROVector3f>> &paths,
                              VROPolylineJoinStyle joinStyle,
                              std::vector<std::shared_ptr<VROGeometrySource>> &sources,
                              std::vector<std::shared_ptr<VROGeometryElement>> &elements);
    static std::shared_ptr<VROGeometryElement> buildElement(size_t numCorners);
    static size_t encodeLine(const std::vector<VROVector3f> &path, VROPolylineJoinStyle joinStyle, VROByteBuffer &outBuffer);
    static size_t encodeQuad(VROLineSegment segment, bool beginDegenerate, bool endDegenerate, VROByteBuffer &buffer);
    static size_t encodeCircularEndcap(VROVector3f center, VROVector3f direction,
                                       bool beginDegenerate, bool endDegenerate, VROByteBuffer &buffer);

    static void writeCorner(VROVector3f position, VROVector3f direction, VROByteBuffer &buffer);
    static void writeEndcapCorner(VROVector3f position, VROVector3f direction, float rotation, VROByteBuffer &buffer);
    static void createPolylineShaderModifiers();
    
};
