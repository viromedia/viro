//
//  VROAnimatedTextureOpenGL.h
//  ViroKit
//
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

#ifndef VROAnimatedTextureOpenGL_h
#define VROAnimatedTextureOpenGL_h

#include "VROFrameListener.h"
#include "VROTexture.h"
#include "VROOpenGL.h"
#include <memory>

class VRORenderContext;
class VROFrameSynchronizer;
class VRODriver;

/*
 VROAnimatedTextureOpenGL handles the construction and playing of animated texture
 files. These files contain textured data with series of images / frames separated
 by a set of predefined time delays, that when played in rapid succession is viewed
 as an animated texture. Currently, we only support GIF-formatted animated textures.
 */
class VROAnimatedTextureOpenGL : public VROTexture, public VROFrameListener {
public:
    VROAnimatedTextureOpenGL(VROStereoMode state = VROStereoMode::None);
    ~VROAnimatedTextureOpenGL();

    /*
     Loads the Animated texture at the given path asynchronously. Currently supports only
     GIF formatted texture data.
     */
    void loadAnimatedSourceAsync(std::string sourcePath,
                                 std::shared_ptr<VRODriver> driver,
                                 std::shared_ptr<VROFrameSynchronizer> frameSynchronizer,
                                 std::function<void(bool, std::string)> callback);
    void onFrameWillRender(const VRORenderContext &context);
    void onFrameDidRender(const VRORenderContext &context);

    /*
     Plays the animated texture. The animation automatically restarts if had previously
     ended and loop is set to true.
     */
    void play();

    /*
     Pauses the animated texture.
     */
    void pause();

    /*
     Enables looping of the animated texture.
     */
    void setLoop(bool loop);

    /*
     Returns the total duration of this AnimatedTexture, in milliseconds.
     */
    int getTotalAnimationDurationMs();

private:
    /*
     Initializes default values and creates underlying texture substrates as needed.
     */
    void init(std::shared_ptr<VRODriver> driver);

    /*
     Parses the raw animated GIF texture file at the given sourcePath.
     Returns true if successful, false otherwise with the reason for failure in errorOut.
     */
    bool parseGIFFile(std::string sourcePath, std::string &errorOut);

    /*
     Called per frame to update the animation states for this animated texture.
     */
    void animateTexture(double globalCurrentTimeMs);

    /*
     Properties for pausing / playing the animated texture.
     */
    bool _loop;
    bool _paused;

    /*
     The parsed total duration of the animated texture in milliseconds.
     */
    double _animatedTotalDuration;

    /*
     The index of the current frame in _animatedFrameRawData that is being rendered.
     */
    int _currentAnimFrame;

    /*
     Time at which the animation had started, in milliseconds. It is redefined as the texture
     is paused / resumed.
     */
    double _processedAnimationStartTime;

    /*
     Time that has passed since this animated texture has last started. It is redefined as the texture
     is paused / resumed.
     */
    double _processedTimeWhenPaused;

    /*
     GL Texture ID used by this VROAnimatedTexture to swap out animated frame data.
     */
    GLuint _initTexture;

    /*
     Struct for storing raw image data properties, per frame.
     */
    struct AnimatedFrame{
        std::shared_ptr<VROData> rawData;
        double timeStamp;
        int top;
        int left;
        int width;
        int height;
    };

    /*
     The vector of AnimatedFrames representing this Animated Texture.
     */
    std::vector<AnimatedFrame> _animatedFrameData;

};
#endif /* VROAnimatedTextureOpenGL_h */
