//
//  VROTriangle.h
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

#ifndef VROLINESEGMENT_H_
#define VROLINESEGMENT_H_

#include "VROVector3f.h"

enum class VROOrientation {
    Colinear = 0,
    Left = 1,
    Right = -1
};

class VROLineSegment {
public:

    /*
     Construct a new {@link LineSegment}.
     
     @param A
                First point.
     @param B
                Second point.
     */
    VROLineSegment(VROVector3f A, VROVector3f B);

    /*
     Get the start and end of this segment.
     */
    VROVector3f getA() const;
    VROVector3f getB() const;

    /*
     Get a ray representation of the segment, the midpoint or the length.
     */
    VROVector3f ray() const;
    VROVector3f midpoint() const;
    float length() const;
    float lengthApprox() const;

    /*
     Flip the orientation of this segment (set A to B and vice-versa).
     */
    VROLineSegment flip() const;

    /*
     Returns the orientation (as a VROOrientation) of the direction of the point p relative
     to this segment.
     */
    VROOrientation orientationOfPoint(VROVector3f q) const;

    /*
     Test if this segment intersects the given 2D segment or 2D box.
     */
    bool intersectsSegment2D(VROLineSegment other) const;
    bool intersectsSegment2D(VROLineSegment other, VROVector3f *result) const;
    bool intersectsBox2D(float left, float right, float bottom, float top) const;

    /*
     Test if the line represented by this segment intersects the line represented by the
     given segment. This function treats both segments as infinitely extending lines.
     */
    bool intersectsLine(VROLineSegment other, VROVector3f *result) const;
    
    /*
     Test if this line segment intersects plane defined by the given point and normal.
     Return true if so, storing the intersection point in the given out vector.
     */
    bool intersectsPlane(VROVector3f point, VROVector3f normal, VROVector3f *outIntersectionPoint) const;

    /*
     Get the angle this segment makes with the given other line-segment,
     normalized to the range [0, PI/2]. Note that this method ignores the
     <i>direction</i> of the line-segment (we do not treat line-segments like
     rays).
     */
    float angleWithSegment(VROLineSegment other) const;
    float angle2DWithSegment(VROLineSegment other) const;

    /*
     Get the angle this segment makes with the given other line-segment,
     normalized to the range [-PI, PI]. Note that this method takes into
     account the <i>direction</i> of the line-segment. A positive angle is
     taken to mean the other segment is that many radians
     <i>counter-clockwise</i> from this segment. Likewise, a negative angle
     implies <i>clockwise</i> rotation.
     
     For example, the angle between the segment [(0, -1) -> (0, 0)] and [(0,
     0) -> (1,1)] is -45 degrees, as the latter is 45 degrees clockwise from
     the former.
     */
    float directedAngleWithSegment(VROLineSegment other) const;

    /*
     Compute the angle, in the same manner as directedAngleWithSegment, that this
     segment makes with the given ray.
     */
    float directedAngleWithRay(VROVector3f ray) const;
    
    /*
     Translate the segment by the given amount.
     */
    VROLineSegment translate(VROVector3f translation) const;

    /*
     Rotate this line segment counter-clockwise about its center by the given
     amount and return the result. Does not mutate this segment.
     */
    VROLineSegment rotate(float radians) const;

    /*
     Rotate this line segment counter-clockwise about its start point, by the
     given number of radians, and store the result. Does not mutate this
     segment.
     */
    VROLineSegment pivot(float radians) const;

    /*
     Scale this segment out by the given factor. This will essentially just
     grow the segment, keeping its angle and midpoint the same.
     */
    VROLineSegment scale(float scale) const;

    /*
     Get the normal unit vector in the positive or negative
     direction for this line segment. This is a 2D calculation,
     so the z-values in the line segment are ignored and the
     z-value in the returned vector will be zero.

     The returned vector will be <0, 0, 0> if this line
     segment has an 2D-length of 0.
     */
    VROVector3f normal2DUnitVector(bool positive) const;

    /*
     Extend this segment out by the given amount. This
     essentially just pushes out the B endpoint in the direction of the segment.
     The second function will push out the A endpoint in the reverse direction
     of the segment, and the third does both.
     */
    VROLineSegment extend(float amount) const;
    VROLineSegment extendBackwards(float amount) const;
    VROLineSegment extendForwardAndBackward(float amount) const;
    
    /*
     Shift the line segment by the given amount. This pushes out the A
     and B endpoints in the direction of the segment.
     */
    VROLineSegment shift(float amount) const;

    /*
     Traverse the line-segment by the given distance from the start-point and
     return the result. If we overshoot either endpoint of the segment, just
     interpolate away. The distance may be negative to go backward (from B to A
     instead of A to B).
     */
    VROVector3f traverseFromStart(float distance) const;
    VROVector3f traverseFromEnd(float distance) const;

    /*
     Returns the 't' that gives the closest point on this line to
     the given point 'p' for the vector operation 'AB * t + A', note
     that this may be off of the actual line segment if t is not in
     [0, 1]
     
     see: * http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
     */
    float parameterOfClosestPoint(VROVector3f p) const;

    /*
     Find the point on the line segment defined that is closest to
     the given point. Call the closest point on the line P. Observe
     that the vector formed by P and the given point is orthogonal
     to the line defined by p1 and p2. Thus the dot product of these
     two vectors is zero.  From there we can calculate the x, y, and
     z of the point P (after a lot of hairy algebra).
     */
    VROVector3f pointOnLineSegmentClosestTo(VROVector3f p) const;
    float distanceToPoint(VROVector3f v) const;
    float distanceToPointSigned(VROVector3f v) const;

    /*
     Find the point on the infinite line given by this line segment
     that is closest to the given point 'p'.
     */
    VROVector3f pointOnLineClosestTo(VROVector3f p) const;

    /*
     Offset this line-segment by the given amount, and store the result in the
     given segment. Positive distance offsets to the left, negative to the right.
     */
    void offsetByDistance(double distance, VROLineSegment *result) const;

    /*
     Return a string representation of this segment.
     */
    std::string toString() const;

    inline bool operator==(const VROLineSegment &other) const {
        return other.__A == __A && other.__B == __B;
    }

private:
    
    // Note: we use double underscores here (__A, __B, etc.) because _B is a
    //       reserved macro on some platforms (Android NDK)
    
    /*
     The two points joined by this segment.
     */
    VROVector3f __A, __B;

    /*
     Useful values for intermediate calculations.
     */
    float __ABx, __ABy, __ABz;
    
    /*
     Square of the length.
     */
    float _lengthSq;

};

#endif /* VROLINESEGMENT_H_ */
