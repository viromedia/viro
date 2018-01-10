//
//  VROPostProcessEffectFactory.h
//  ViroKit
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPostProcessEffectFactory_h
#define VROPostProcessEffectFactory_h

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "VROStringUtil.h"

class VROShaderProgram;
class VROImagePostProcess;
class VROMaterial;
class VROLight;
class VROShaderModifier;
class VRODriverOpenGL;
class VRODriver;
class VRORenderTarget;

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
    None
};

/*
 The PostProcessEffectFactory handles the enabling, disabling and caching of PostProcessEffectPrograms
 that are applied as part of VROChoreographer.renderBasePass().
 */
class VROPostProcessEffectFactory {
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

     Return true if anything was written to the destination. False if there was
     no post-processing done.
     */
    bool handlePostProcessing(std::shared_ptr<VRORenderTarget> source,
                              std::shared_ptr<VRORenderTarget> destination,
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
private:

    void renderEffects(std::shared_ptr<VRORenderTarget> input,
                       std::shared_ptr<VRORenderTarget> output,
                       std::shared_ptr<VRODriver> driver);

    /*
     Shader programs cached by their VROPostProcessEffect types.
     */
    std::vector<std::pair<VROPostProcessEffect,std::shared_ptr<VROImagePostProcess>>> _cachedPrograms;

    /*
     Below is a list of post-process specific functions that builds, caches and returns post process
     effects to run.

     TODO VIRO-1634: Optimize PostProcess Effects to negate cost of re-binding shaders when multiple
     effects are applied by having a single postProcess.vsh.
     */
    std::shared_ptr<VROImagePostProcess> createGreyScale(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createSepia(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createSinCity(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createBarallelDistortion(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createPinCusionDistortion(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createToonify(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createInverted(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createThermalVision(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createPixel(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createCrossHatch(std::shared_ptr<VRODriver> driver);
    std::shared_ptr<VROImagePostProcess> createEmptyEffect(std::shared_ptr<VRODriver> driver);
    std::vector<std::string> getHBCSModification(float hue, float brightness, float contrast, float saturation);
};

#endif /* VROPostProcessEffectFactory_h */
