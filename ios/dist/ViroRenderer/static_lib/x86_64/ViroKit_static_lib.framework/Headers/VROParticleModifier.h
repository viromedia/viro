//
//  VROParticleModifier.h
//  ViroRenderer
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

#ifndef VROParticleModifier_h
#define VROParticleModifier_h

#include <memory>
#include <vector>
#include <algorithm>
#include "VROMatrix4f.h"
#include "VROVector4f.h"
#include "VROOpenGL.h"
#include "VROVector3f.h"
#include "VROParticle.h"
#include "VROMath.h"
#include "VROStringUtil.h"

/*
 VROParticleModifier contains a list of VROModifierIntervals to interpolate against with
 referenceFactor to time, distance or velocity (as defined by a given VROModifierFactor). This
 is used by VROParticleEmitter to pre-define a set of particle property behaviors (color,
 scale, rotation, etc) that can then be applied to any particle.
 */
class VROParticleModifier {
public:
    /*
     VROModifierFactor represents the referenceFactor factor this modifier will interpolate against.
     */
    enum class VROModifierFactor {
        Time,           // Interpolate against the timeSinceSpawnedInMs of a particle.
        Distance,       // Interpolate against the distanceTravelled of a particle.
        Velocity        // Interpolate against the current velocity of a particle.
    };

    static VROModifierFactor getModifierFactorForString(std::string strType) {
        if (VROStringUtil::strcmpinsensitive(strType, "Velocity")) {
            return VROModifierFactor::Velocity;
        } else if (VROStringUtil::strcmpinsensitive(strType, "Distance")) {
            return VROModifierFactor::Distance;
        } else {
            return VROModifierFactor::Time;
        }
    }

    /*
     Interpolation points between which to animate a given initial value (most likely the
     particle's property value) towards.
     */
    struct VROModifierInterval {
        // Property of the particle to interpolate towards for this given VROModifierWindow.
        VROVector3f targetedValue;

        // Start and End factor values. For example, startTime and endTime.
        double startFactor;
        double endFactor;

        bool operator < (const VROModifierInterval& point) const {
            return (endFactor <= point.startFactor);
        }
    };

    VROParticleModifier(VROVector3f constant) {
        init(constant, constant, VROModifierFactor::Time);
    };

    VROParticleModifier(VROVector3f minRange, VROVector3f maxRange) {
        init(minRange, maxRange, VROModifierFactor::Time);
    };

    VROParticleModifier(VROVector3f minRange, VROVector3f maxRange,
                        VROModifierFactor factor, std::vector<VROModifierInterval> values) {
        init(minRange, maxRange, factor);
        setInterpolatedPoints(values);
    };

    virtual ~VROParticleModifier(){};

    VROVector3f getInitialValue() {
        return random(_initialMinValue, _initialMaxValue);
    }

    /*
     Apply the behavior of this modifier (set by VROInterpolateValues) on the given initialValue
     with referenceFactor to the preset _referenceFactor.
     */
    VROVector3f applyModifier(VROParticle &particle, VROVector3f initialValue) {
        if (_modifierInterval.size() <= 0) {
            return initialValue;
        }

        double deltaFactor = getReferenceFactorForParticle(particle);
        return getFinalValue(initialValue, deltaFactor);
    }

private:
    void init(VROVector3f minRange, VROVector3f maxRange, VROModifierFactor factor) {
        _initialMinValue = minRange;
        _initialMaxValue = maxRange;
        _referenceFactor = factor;
    }

    /*
     Min and Max ranges from which randomize across and determine the initial value applied
     by this modifier.
     */
    VROVector3f _initialMinValue;
    VROVector3f _initialMaxValue;

    /*
     Reference factor this modifier will interpolate against (Time, Distance or Velocity).
     */
    VROModifierFactor _referenceFactor;

    /*
     List of VROModifiers with values dictating the behavior of this modifier, along which this
     VROParticleModifier will interpolate against.
     */
    std::vector<VROModifierInterval> _modifierInterval;

    /*
     Sets a list of VROModifierInterval values that will be used by this VROParticleModifier.
     */
    void setInterpolatedPoints(std::vector<VROModifierInterval> values) {
        if (values.size() == 0 ){
            return;
        }

        std::sort(values.begin(), values.end());
        for (int i = 0; i < values.size() - 1; i++) {
            if (values[i].endFactor > values[i + 1].startFactor
                || (values[i].startFactor > values[i].endFactor)) {
                perror("Attempted to set an invalid set of interpolated points for the Particle Emitter!");
                return;
            }
        }

        _modifierInterval = values;
    }

    /*
     Returns the referenceFactor factor value from the provided particle. The type of referenceFactor factor
     is determined by this VROParticleModifier _referenceFactor.
     */
    double getReferenceFactorForParticle(const VROParticle &particle) {
        if (_referenceFactor == VROModifierFactor::Distance) {
            return particle.distanceTraveled;
        } else if (_referenceFactor == VROModifierFactor::Velocity) {
            return particle.velocity;
        }
        return particle.timeSinceSpawnedInMs;
    }

    /*
     With the given particle, and based on this emitter's referenceFactor factor, determine the amount of
     passed time, distance, or velocity that is required to interpolate the desired value.
     */
    VROVector3f getFinalValue(VROVector3f initialValue, double currentFactor) {
        VROVector3f start;
        VROVector3f end;
        for (int i = 0; i < _modifierInterval.size(); i++) {

            // Determine if this VROModifierInterval contains a startFactor and endFactor interval
            // that applies to the current deltaFactor.
            if (currentFactor >= _modifierInterval[i].startFactor && currentFactor <= _modifierInterval[i].endFactor) {
                // If so, calculate progression ratio
                float delta = currentFactor - _modifierInterval[i].startFactor;
                float deltaWindow = _modifierInterval[i].endFactor - _modifierInterval[i].startFactor;
                float ratio = delta / deltaWindow;
                if (i == 0){
                    start = initialValue;
                } else {
                    start = _modifierInterval[i-1].targetedValue;
                }
                end = _modifierInterval[i].targetedValue;


                return interpolatePoint(start, end, ratio);
            }

            // Else, if we can't find a VROModifierInterval that matches the currentFactor,
            // check if the currentFactor may be referring to a time/distance period in between
            // VROModifierIntervals. If so, attempt to find the last known VROModifierInterval
            // that we had applied before and use its ending value.
            if (i != 0 && currentFactor >= _modifierInterval[i-1].endFactor
                          & currentFactor <= _modifierInterval[i].endFactor) {
                if (i - 1 == 0) {
                    start = initialValue;
                } else {
                    start = _modifierInterval[i-2].targetedValue;
                }

                end = _modifierInterval[i-1].targetedValue;
                return interpolatePoint(start, end, 1);
            }

            // Else if this is the last VROModifierInterval, and the currentFactor has progressed
            // beyond it, attempt to use the ending value of the last known VROModifierInterval.
            if (i == _modifierInterval.size() - 1 && currentFactor > _modifierInterval[i].endFactor) {
                if (i != 0) {
                    start = _modifierInterval[i-1].targetedValue;
                } else {
                    start = _modifierInterval[i].targetedValue;
                }
                end = _modifierInterval[i].targetedValue;
                return interpolatePoint(start, end, 1);
            }
        }

        // If no last known windows were yet found, we have yet to encounter our first
        // VROModifierInterval, thus return initialValue.
        return initialValue;
    }

    VROVector3f interpolatePoint(VROVector3f &startValue, VROVector3f &endValue, float ratio) {
        VROVector3f final;
        if (ratio >= 1) {
            final = endValue;
        } else {
            final = startValue.interpolate(endValue, ratio);
        }
        return final;
    }
};

#endif /* VROParticleModifier_h */
