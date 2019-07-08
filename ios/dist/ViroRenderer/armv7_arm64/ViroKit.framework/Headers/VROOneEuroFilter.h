//
//  VROOneEuroFilter.h
//  ViroRenderer
//
//  Created by Raj Advani on 3/14/19.
//  Copyright © 2019 Viro Media. All rights reserved.
//

#ifndef VROOneEuroFilter_h
#define VROOneEuroFilter_h

#include "VROVector3f.h"

/*
 Adaptive low-pass filter based on:
 
 Géry Casiez, Nicolas Roussel, Daniel Vogel. 1€ Filter: A Simple Speed-based Low-pass
 Filter for Noisy Input in Interactive Systems. HI’12, the 30th Conference on Human
 Factors in Computing Systems, May 2012, Austin, United States. ACM, pp.2527-2530, 2012,
 <10.1145/2207676.2208639>. <hal-00670496>
 
 Description from the paper:
 
 Low-pass filter, where the cutoff frequency changes according to speed: at low speeds,
 a low cutoff re- duces jitter at the expense of lag, but at high speeds, the cut- off is
 increased to reduce lag rather than jitter. The intuition is that people are very
 sensitive to jitter and not latency when moving slowly, but as movement speed increases,
 people become very sensitive to latency and not jitter.
 */

// -----------------------------------------------------------------

class LowPassFilter {
public:
    LowPassFilter(double alpha, VROVector3f initval = {0, 0, 0});
    
    VROVector3f filter(VROVector3f value, double alpha);
    bool hasLastRawValue(void) const { return _initialized; }
    VROVector3f getLastRawValue(void) const { return _lastRaw; }
    
private:
    VROVector3f _lastRaw, _lastFiltered;
    double _alpha;
    bool _initialized;
};

// -----------------------------------------------------------------

class VROOneEuroFilter {
public:
    
    VROOneEuroFilter(double freq, double mincutoff = 1.0, double beta_ = 0.0, double dcutoff = 1.0);
    ~VROOneEuroFilter();
    
    VROVector3f filter(VROVector3f value, double timestamp, bool debug = false);
    
    void setBeta(float beta) {
        _beta = beta;
    }
    void setFCMin(float fcMin) {
        _minFrequencyCutoff = fcMin;
    }
    
private:
    double _frequency;
    double _minFrequencyCutoff;
    double _beta;
    double _derivativeCutoff;
    LowPassFilter *_x;
    LowPassFilter *_dx;
    double _lastTimestamp;
    
    double computeAlpha(double cutoff);
};

// -----------------------------------------------------------------

class LowPassFilterF {
public:
    LowPassFilterF(double alpha, float initval = 0);
    
    float filter(float value, double alpha);
    bool hasLastRawValue(void) const { return _initialized; }
    float getLastRawValue(void) const { return _lastRaw; }
    
private:
    float _lastRaw, _lastFiltered;
    double _alpha;
    bool _initialized;
};

// -----------------------------------------------------------------

class VROOneEuroFilterF {
public:
    
    VROOneEuroFilterF(double freq, double mincutoff = 1.0, double beta_ = 0.0, double dcutoff = 1.0);
    ~VROOneEuroFilterF();
    
    float filter(float value, double timestamp, bool debug = false);
    
    void setBeta(float beta) {
        _beta = beta;
    }
    void setFCMin(float fcMin) {
        _minFrequencyCutoff = fcMin;
    }
    
private:
    double _frequency;
    double _minFrequencyCutoff;
    double _beta;
    double _derivativeCutoff;
    LowPassFilterF *_x;
    LowPassFilterF *_dx;
    double _lastTimestamp;
    
    double computeAlpha(double cutoff);
};

#endif /* VROOneEuroFilter_h */
