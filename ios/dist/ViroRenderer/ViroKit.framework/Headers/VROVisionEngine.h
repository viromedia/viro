//
//  VROVisionEngine.h
//  ViroKit
//
//  Created by Raj Advani on 5/22/19.
//  Copyright © 2019 Viro Media. All rights reserved.
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

#ifndef VROVisionEngine_h
#define VROVisionEngine_h

#include <map>
#include <atomic>
#include <mutex>
#include <memory>
#include "VROCameraTexture.h"
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreML/CoreML.h>
#import <Vision/Vision.h>
#include "VROMatrix4f.h"

// Number of samples to collect when computing FPS
static const int kNeuralFPSMaxSamples = 100;

class VROARFrame;
class VRODriver;
class VROPoseFilter;
class VROOneEuroFilterF;

enum class VROCropAndScaleOption {
    CoreML_Fill,               // Use CoreML's non-aspect-preserving fill scaling
    CoreML_Fit,                // CoreML's aspect-fit (fit long edge of image)
    CoreML_FitCrop,            // CoreML's aspect-fit-crop (fit short edge of image, center-crop long edge)
    Viro_FitCropPad,           // Use dynamic cropping through manual image cropping, and aspect-fit (fit long edge)
    Viro_RegionOfInterest,     // Use dynamic cropping through CoreML's region of interest, and aspect-fit (fit long edge)
};

class VROVisionEngineDelegate {
public:
    
    /*
     Post-process the output from CoreML. This is invoked each frame with the latest image
     data. Optional: return points (in screen coordinates) and confidence scores, from which
     we can derive the crop-box for the next image.
     
     If an empty vector is returned, no crop box will be used.
     
     The given transforms go from vision space [0, 1] to image space [0, 1], and from
     image space to normalized viewport space [0, 1].
     */
    virtual std::vector<std::pair<VROVector3f, float>> processVisionOutput(VNCoreMLFeatureValueObservation *result, VROCameraPosition cameraPosition,
                                                                           VROMatrix4f visionToImageSpace, VROMatrix4f imageToViewportSpace) = 0;
    
};

/*
 Interfaces CoreML models with AR frames.
 */
class VROVisionEngine : public std::enable_shared_from_this<VROVisionEngine> {
public:
    
    VROVisionEngine(MLModel *model, int imageSize, VROCameraPosition position,
                    VROCropAndScaleOption cropAndScaleOption);
    virtual ~VROVisionEngine();
    
    /*
     Process a new frame with the vision engine. This will crop and scale the frame
     appropriately, run it through the ML model, and send the results to the delegate.
     */
    void update(const VROARFrame *frame);
    
    /*
     Set the delegate that will receive the Core ML output each frame.
     */
    void setDelegate(std::shared_ptr<VROVisionEngineDelegate> delegate) {
        _delegate_w = delegate;
    }
    
    /*
     Get the dynamic crop box used for the last render.
     */
    CGRect getDynamicCropBox() const {
        return _dynamicCropBoxViewport;
    }
    
private:
    
    /*
     Core ML parameters.
     */
    MLModel *_model;
    int _imageSize;
    VNCoreMLModel *_coreMLModel;
    VNCoreMLRequest *_visionRequest;
    
    VROCropAndScaleOption _cropAndScaleOption;
    uint8_t *_cropScratchBuffer;
    int _cropScratchBufferLength;
    
    /*
     Delegate that will process finished vision requests.
     */
    std::weak_ptr<VROVisionEngineDelegate> _delegate_w;
    
    /*
     The camera we are using for tracking (front or back).
     */
    VROCameraPosition _cameraPosition;
    
    /*
     Queue on which CoreML is run.
     */
    dispatch_queue_t _visionQueue;
    
    /*
     Transforms used to convert points from vision space (the CoreML input image
     space) into viewport points.
     
     This is a two-step operation: we have to move from vision space into image space
     (e.g. the space of the image before the cropping, scaling, and padding operations
     used for CoreML). Then we need to move from image space space into viewport space
     (inverting ARKit or similar display transforms).
     */
    VROMatrix4f _visionToImageSpace, _imageToViewportSpace;
    
    /*
     The image being processed now. Null if no image is currently being processed.
     */
    CVPixelBufferRef _processingImage;
    
    /*
     The image (along with its ARKit transform and orientation) that is queued to be
     fed into the CoreML model. The ARKit transform moves from image to viewport
     space.
     
     These are all protected by _imageMutex, as they are modified on the rendering thread
     but utlized on the _visionQueue thread.
     */
    CVPixelBufferRef _nextImage;
    VROMatrix4f _nextTransform;
    CGImagePropertyOrientation _nextOrientation;
    std::mutex _imageMutex;
    
    /*
     Variables for neural engine FPS computation. Array of samples taken, index of
     next sample, and sum of samples so far.
     */
    int _fpsTickIndex = 0;
    uint64_t _fpsTickSum = 0;
    uint64_t _fpsTickArray[kNeuralFPSMaxSamples];
    
    /*
     FPS is measured in ticks, which are the number of nanoseconds
     between consecurive calls to processVisionResults().
     */
    uint64_t _nanosecondsLastFrame;
    bool _neuralEngineInitialized;
    
    /*
     Profiling variables for tracking time spent.
     */
    double _startNeural;
    double _startPostProcessing;
    double _betweenImageTime;
    
    /*
     Dampening window milliseconds. If period is set to 0, no dampening will be applied.
     */
    double _dampeningPeriodMs;
    
    /*
     The dynamic cropping window, which is updated each frame to form a bounding
     box around the detected body.
     */
    CGRect _dynamicCropBox, _dynamicCropBoxViewport;
    std::shared_ptr<VROOneEuroFilterF> _dynamicCropXFilter, _dynamicCropYFilter, _dynamicCropWidthFilter, _dynamicCropHeightFilter;
    
    /*
     Move to track the next image, if one is available.
     */
    void nextImage();
    
    /*
     Initiate a VNImageRequest on the provided image, using the given inverse display transform.
     This method also derives the transform that will be needed to go back from vision space [0, 1] to
     viewport space [0, 1].
     
     Invoked on the visionQueue.
     */
    void trackImage(CVPixelBufferRef image, VROMatrix4f transform, CGImagePropertyOrientation orientation);
    
    /*
     Process the result of the last call to trackImage. This will convert the raw output from
     CoreML into the body joints, and then pass the joints through a filter and finally invoke the delegate
     on the rendering thread.
     
     Invoked on the visionQueue.
     */
    void processVisionResults(VNRequest *request, NSError *error);
    
    /*
     Perform a crop and pad of the given image using the dynamic crop box,
     and return the result.
     */
    CVPixelBufferRef performCropAndPad(CVPixelBufferRef image, int *outCropX, int *outCropY,
                                       int *outCropWidth, int *outCropHeight);
    
    /*
     Derive the full body bounds from the given pose. This will be used as the dynamic crop
     box for the next frame. The bounds are expanded in directions where joints were not found,
     to leave room for finding them in successive frames.
     */
    CGRect deriveBounds(const std::pair<VROVector3f, float> *imageSpaceJoints);
    
    /*
     Derive the full body bounds from the given pose, interpolate from the previous
     crop box to this new bounds, and return the result.
     */
    CGRect deriveBoundsSmooth(const std::pair<VROVector3f, float> *imageSpaceJoints);
    
    /*
     Update and get FPS.
     */
    void updateFPS(uint64_t newTick);
    double getFPS() const;
    
};

#endif /* VROVisionEngine_h */
