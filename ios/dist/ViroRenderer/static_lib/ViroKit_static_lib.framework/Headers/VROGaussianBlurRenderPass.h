//
//  VROGaussianBlurRenderPass.h
//  ViroKit
//
//  Created by Raj Advani on 8/24/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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

#ifndef VROGaussianBlurRenderPass_h
#define VROGaussianBlurRenderPass_h

#include "VRORenderPass.h"
#include "VROVector4f.h"

class VRODriver;
class VROImagePostProcess;
class VROViewport;

/*
 Keys for the gaussian render pass.
 */
const std::string kGaussianInput = "G_Input";
const std::string kGaussianPingPong = "G_PP";

/*
 Implements Gaussian blur for Bloom. Ping-pongs the blur between
 two render targets.
 */
class VROGaussianBlurRenderPass : public VRORenderPass, public std::enable_shared_from_this<VROGaussianBlurRenderPass> {
public:
    
    VROGaussianBlurRenderPass();
    virtual ~VROGaussianBlurRenderPass();

    void render(std::shared_ptr<VROScene> scene,
                std::shared_ptr<VROScene> outgoingScene,
                VRORenderPassInputOutput &inputs,
                VRORenderContext *context, std::shared_ptr<VRODriver> &driver);

    /*
     Functions for handling the render targets associated with this Gaussian Blur.
     */
    void createRenderTargets(std::shared_ptr<VRODriver> &driver);
    void resetRenderTargets();
    void setViewPort(VROViewport viewport, std::shared_ptr<VRODriver> &driver);
    void setClearColor(VROVector4f color, std::shared_ptr<VRODriver> driver);

    /*
     The more iterations, the more blur. Must be an even number.
     */
    void setNumBlurIterations(int numIterations);

    /*
     Sets properties for controlling the blur visually.
     - kernelSize controls the spread of the blur.
     - sigma controls the blur intensity.
     - normalized controls the 'consistent look' of the blur.
     */
    void setBlurKernel(int kernelSize, float sigma, bool normalized = false);
    
    int getKernelSize() const {
        return _kernelSize;
    }
    float getSigma() const {
        return _sigma;
    }

    /*
     Set to True to optimize the blur by reducing the number of texture lookups,
     done by performing bilinear texture lookups in our kernel offsets. This is
     enabled by default.
     */
    void setBilinearTextureLookup(bool enabled);

    /*
     Notifies this blurRenderPass of the current background color configuration of the
     renderer (and more importantly, if there are any semi-transparent pixels).
     */
    void setClearColor(VROVector4f color);

    /*
     A scale factor to reinforce and intensify the rgb values of the input texture before
     applying the blur.
     */
    void setReinforcedIntensity(float intensity) {
        _reinforcedIntensity = intensity;
        resetShaders();
    }

    float getReinforcedIntensity() {
        return _reinforcedIntensity;
    }
    
private:

    /*
     Render targets for ping-ponging the blur operation.
     */
    std::shared_ptr<VRORenderTarget> _blurTargetA;
    std::shared_ptr<VRORenderTarget> _blurTargetB;

    /*
     Sigma controls gaussian "drop-off", and thus the blur intensity. The higher
     the sigma, the faster the drop-off, decreasing the intensity of the blur.
     */
    float _sigma;

    /*
     KernelSize controls how large the convolution window should be. With larger
     sizes, more pixels are included in the blur, thus 'spreading' the blur over
     a wider area.
     */
    int _kernelSize;

    /*
     Set to false if we wish to have a more "feathery" blur. And true if we would
     like the blur distribution to always be the same, no matter the kernel size.
     */
    bool _normalizedKernel;

    /*
     Set to true to optimize the blur by reducing the number of texture lookups,
     done by performing bilinear texture lookups in our kernel offsets.
     */
    bool _bilinearTextureLookup;

    /*
     Flag for tracking the current blur state.
     */
    bool _horizontal;

    /*
     The number of times to perform the blur (both horizontal and vertical).
     */
    int _numBlurIterations;

    /*
     True if we are compositing onto a renderer that has transparent pixels.
     */
    bool _considerTransparentPixels = true;

    /*
     A premultiplied intensity ratio applied to the input texture and it's rgb values
     before bluring.
     */
    float _reinforcedIntensity;

    /*
     The main gaussian blur shader post process program used for blurring the image.
     */
    std::shared_ptr<VROImagePostProcess> _gaussianBlur;

    /*
     Program for pre-processing the image before it is blurred.
     */
    std::shared_ptr<VROImagePostProcess> _preBlurPass;

    /*
     The size of the blur targets relative to the display. Smaller scale leads to
     less accurate but faster blur.
     */
    float _blurScaling;

    /*
     Initializes and replaces _gaussianBlur and _preBlurPass shaders.
     */
    void initPostProcess(std::shared_ptr<VRODriver> driver);
    void initBlurPass(std::shared_ptr<VRODriver> driver);
    void initPreBlurPass(std::shared_ptr<VRODriver> driver);

    /*
     Mathematical helper functions for calculating a kernel that represents a
     weighted gaussian distribution used for bluring the image.
     */
    std::vector<float> calculateKernel(float sigma,
                                       float kernelSize,
                                       float sampleCount);
    float gaussianDistribution(float x,
                               float mu,
                               float sigma);
    float integrateSimphson(std::vector<std::vector<float>> tapSamples);
    std::vector<std::vector<float>> calculateSamplesForRange(float minInclusive,
                                                             float maxInclusive,
                                                             int sampleCount,
                                                             float sigma);
    void convertToLinearKernelSample(std::vector<float> &kernelWeights,
                                     std::vector<float> &kernelOffsets);
    void resetShaders();
};

#endif /* VROGaussianBlurRenderPass_h */
