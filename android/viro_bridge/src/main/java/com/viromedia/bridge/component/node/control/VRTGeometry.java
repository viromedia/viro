//  Copyright © 2018 Viro Media. All rights reserved.
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

package com.viromedia.bridge.component.node.control;

import android.util.Log;

import com.facebook.react.bridge.ReactContext;
import com.viro.core.Geometry;
import com.viro.core.Submesh;
import com.viro.core.Vector;

import java.util.ArrayList;
import java.util.List;

public class VRTGeometry extends VRTControl {

    private Geometry mGeometry;
    private List<Vector> mVertices = new ArrayList<>();
    private List<Vector> mTexcoords = new ArrayList<>();
    private List<Vector> mNormals = new ArrayList<>();
    private List<List<Integer>> mTriangleIndices = new ArrayList<>();
    private boolean mGeometryNeedsUpdate = true;

    public VRTGeometry(ReactContext reactContext) {
        super(reactContext);
        mGeometry = new Geometry();
        super.setGeometry(mGeometry);
    }

    public void setVertices(List<Vector> coords) {
        mVertices = coords;
        mGeometryNeedsUpdate = true;
    }

    public void setNormals(List<Vector> coords) {
        mNormals = coords;
        mGeometryNeedsUpdate = true;

    }

    public void setTexcoords(List<Vector> coords) {
        mTexcoords = coords;
        mGeometryNeedsUpdate = true;
    }

    public void setTriangleIndices(List<List<Integer>> triangleIndices) {
        mTriangleIndices = triangleIndices;
        mGeometryNeedsUpdate = true;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();
        if (mGeometryNeedsUpdate) {
            updateGeometry();
            mGeometryNeedsUpdate = false;
        }
    }

    public void updateGeometry() {
        mGeometry.setVertices(mVertices);
        mGeometry.setTextureCoordinates(mTexcoords);
        mGeometry.setNormals(mNormals);

        List<Submesh> submeshes = new ArrayList<>();
        for (List<Integer> triangles : mTriangleIndices) {
            submeshes.add(Submesh.builder().triangleIndices(triangles).build());
        }
        mGeometry.setSubmeshes(submeshes);
    }

}
