//
//  VROPostProcessEffectFactory.h
//  ViroKit
//
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

#ifndef VROPostProcessEffectFactory_h
#define VROPostProcessEffectFactory_h

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "VROStringUtil.h"
#include "VROVector3f.h"

class VROShaderProgram;
class VROImagePostProcess;
class VROMaterial;
class VROLight;
class VROShaderModifier;
class VRODriverOpenGL;
class VRODriver;
class VRORenderTarget;
class VROTexture;
class VROGaussianBlurRenderPass;
class VRORenderContext;

enum class VROPostProcessEffect{
    GrayScale,
    Sepia,
    SinCity,
    BarallelDistortion,
    PincushionDistortion,
    Toonify,
    Inverted,
    ThermalVision,
    Pixelated,
    CrossHatch,
    SwirlDistortion,
    ZoomInDistortion,
    None
};

/*
 The PostProcessEffectFactory handles the enabling, disabling and caching of PostProcessEffectPrograms
 that are applied as part of VROChoreographer.renderBasePass().
 */
class VROPostProcessEffectFactory : public std::enable_shared_from_this<VROPostProcessEffectFactory>{
public:
    VROPostProcessEffectFactory();
    virtual ~VROPostProcessEffectFactory();

    /*
     Enables the given VROPostProcessEffect to be applied. Note effects are applied in the order
     upon which they are enabled.
     */
    void enableEffect(VROPostProcessEffect effect, std::shared_ptr<VRODriver> driver);

    /*
     Disables the given VROPostProcessEffect to be applied.
     */
    void disableEffect(VROPostProcessEffect effect);

    /*
     Clears all effects that had been previously applied.
     */
    void clearAllEffects();

    /*
     This method should be invoked by the main render cycle, *after* the scene is
     rendered to an HDR target, but before tone-mapping and gamma. It is here
     that we should perform all post-processing effects. The *final* output of
     post processing should be rendered into the given target.

     Return the render target that contains the final result. Note this will always
     be one of the passed-in render targets (source if no post-processing was required,
     or one of targetA or targetB if there were any post-processing steps).
     */
    std::shared_ptr<VRORenderTarget> handlePostProcessing(std::shared_ptr<VRORenderTarget> source,
                                                          std::shared_ptr<VRORenderTarget> targetA,
                                                          std::shared_ptr<VRORenderTarget> targetB,
                                                          std::shared_ptr<VROTexture> mask,
                                                          VRORenderContext *context,
                                                          std::shared_ptr<VRODriver> driver);

    static VROPostProcessEffect getEffectForString(std::string strEffect){
        VROStringUtil::toLowerCase(strEffect);
        if (strEffect.compare("grayscale") == 0){
            return VROPostProcessEffect::GrayScale;
        } else if (strEffect.compare("sepia") == 0){
            return VROPostProcessEffect::Sepia;
        } else if (strEffect.compare("sincity") == 0){
            return VROPostProcessEffect::SinCity;
        } else if (strEffect.compare("baralleldistortion") == 0){
            return VROPostProcessEffect::BarallelDistortion;
        } else if (strEffect.compare("pincushiondistortion") == 0){
            return VROPostProcessEffect::PincushionDistortion;
        } else if (strEffect.compare("toonify") == 0){
            return VROPostProcessEffect::Toonify;
        } else if (strEffect.compare("inverted") == 0){
            return VROPostProcessEffect::Inverted;
        } else if (strEffect.compare("thermalvision") == 0){
            return VROPostProcessEffect::ThermalVision;
        } else if (strEffect.compare("pixelated") == 0){
            return VROPostProcessEffect::Pixelated;
        } else if (strEffect.compare("crosshatch") == 0){
            return VROPostProcessEffect::CrossHatch;
        }
        return VROPostProcessEffect::None;
    }

    void createPostProcessMask(std::shared_ptr<VRODriver> driver);
    void enableWindowMask(std::shared_ptr<VRODriver> driver);
    void disableWindowMask(std::shared_ptr<VRODriver> driver);
    void setShouldPostProcessWindowMask(bool inverse);
    void updateWindowMask(VROVector3f tl, VROVector3f tr, VROVector3f bl, VROVector3f br);

    // TODO: Create a better way to pass effect-specific parameters to our post process effects.
    void setSwirlSpeedMultiplier(float speed) {
        _swirlSpeedMultiplier = speed;
    }
    void setDistortion(float distortion) {
        _circularDistortion = distortion;
    }

    /*
     Sets the gaussian blur render pass to be used by this post process factory.
     */
    void setGaussianBlurPass(std::shared_ptr<VROGaussianBlurRenderPass> pass);

private:
    std::shared_ptr<VRORenderTarget> renderEffects(std::shared_ptr<VRORenderTarget> input,
                                                   std::shared_ptr<VRORenderTarget> targetA,
                                                   std::shared_ptr<VRORenderTarget> targetB,
                                                   std::shared_ptr<VRODriver> driver);

    /*
     Shader programs cached by their VROPostProcessEffect types.
     */
    std::vector<std::pair<VROPostProcessEffect, std::shared_ptr<VROImagePostProcess>>> _cachedPrograms;

    /*
     Below is a list of post-process specific functions that builds, caches and returns post process
     effects to run.

     TODO VIRO-1634: Optimize PostProcess Effects to negate cost of re-binding shaders when multiple
     effects are applied by having a single postProcess.vsh.
     */
    std::shared_ptr<VROImagePostProcess> createGreyScale(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createSepia(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createSinCity(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createCircularDistortion(std::shared_ptr<VRODriver> driver, float distortion);
    std::shared_ptr<VROImagePostProcess> createToonify(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createInverted(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createThermalVision(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createPixel(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createCrossHatch(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createSwirlEffect(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createZoomEffect(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createEmptyEffect(std::shared_ptr<VRODriver> driver);
    std::vector<std::string> getHBCSModification(float hue, float brightness, float contrast, float saturation);

    /*
     Properties for applying the post process effects within a window mask.
     */
    bool _enabledWindowMask;
    bool _shouldPostProcessWindowMask;
    VROVector3f _maskTl;
    VROVector3f _maskTr;
    VROVector3f _maskBl;
    VROVector3f _maskBr;
    VROVector3f _outputAspectRatio;

    float _swirlSpeedMultiplier;
    float _circularDistortion;
    std::shared_ptr<VROGaussianBlurRenderPass> _gaussianBlurPass;
};

#endif /* VROPostProcessEffectFactory_h */
