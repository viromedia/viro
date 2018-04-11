//
//  VROARImageTracker.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#if ENABLE_OPENCV

#ifndef VROARImageTracker_h
#define VROARImageTracker_h

#include "VROLog.h"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "VROARImageTarget.h"
#include <memory>
#include <map>
#include <atomic>
#include "VROARCamera.h"
#include "VROVector3f.h"

static const int kMaxInputImageSize = 2073600; // 1920 x 1080

enum class VROARImageTrackerType {
    BRISK,
    ORB,
    ORB4,
};

// TODO: VROARImageTrackerOutput is heavily overloaded.
struct VROARImageTrackerOutput {
    bool found;
    std::vector<cv::Point2f> corners;
    cv::Mat translation;
    cv::Mat rotation;
    
    VROMatrix4f worldTransform;
    std::shared_ptr<VROARImageTarget> target;

    cv::Mat outputImage;
    bool isUpdate;
};

struct VROARImageTargetOpenCV {
    std::shared_ptr<VROARImageTarget> arImageTarget;
    std::vector<cv::KeyPoint> keyPoints;
    cv::Mat descriptors;
    cv::Mat rotation; // the most recent found rotation for this target in the last input image
    cv::Mat translation; // the most recent found translation for this target in the last input image

    // TODO: hold onto shared_ptr<VROARImageTrackerOutput> not just the value!
    std::vector<VROARImageTrackerOutput> rawOutputs; // the raw outputs that we've found so far.
    VROARImageTrackerOutput lastResult; // the most recent output that was returned to the caller for this target.

    // disable tracking once we've found enough rawOutputs - we shouldnt do this anymore (but it's available)
    bool disableTracking;
    std::vector<std::vector<VROARImageTrackerOutput>> similarOutputsList;
};

/*
 For BF-KnnBasedMatching:
 This simple struct relates a match with the ratio of its distance to the next closest match. (it's
 the same ratio that we measure when calculating whether or not it's a good match).

 For FlannBasedMatching: distanceRatio = match.distance
 */
struct VROMatch {
    double distanceRatio;
    cv::DMatch match;
};

class VROARImageTrackerListener {
public:
    virtual ~VROARImageTrackerListener() {}

    virtual void onImageFound(VROARImageTrackerOutput output) = 0;
    virtual void onFindTargetFinished() = 0;
};

// TODO: merge this class into VROARTrackingSession
class VROARImageTracker {
public:
    // helper static function for "legacy"/older tracking test code w/ only 1 target
    static std::shared_ptr<VROARImageTracker> createARImageTracker(std::shared_ptr<VROARImageTarget> arImageTarget);
    
    // helper static function to create a tracker with the best configuration.
    static std::shared_ptr<VROARImageTracker> createDefaultTracker();
    
    // creates an empty, false output
    static VROARImageTrackerOutput createFalseOutput();

    /*
     Constructor - users should use the static constructors to have the type defaulted to
     the optimal version
     */
    VROARImageTracker(VROARImageTrackerType type);

    // Adds/Removes ARImageTarget from tracking
    void addARImageTarget(std::shared_ptr<VROARImageTarget> arImageTarget);
    void removeARImageTarget(std::shared_ptr<VROARImageTarget> arImageTarget);

    // Sets the tracker type we want to use
    void setType(VROARImageTrackerType type);

    // This function takes the input image and detects/computes the keypoints and descriptors (as return arguments)
    void detectKeypointsAndDescriptors(cv::Mat inputImage,
                                       std::vector<cv::KeyPoint> &keypoints,
                                       cv::Mat &descriptors,
                                       bool isTarget);

    /*
     Computes and set the _intrinsicMatrix. This should be called when the screen res/orientation changes.
     */
    void computeIntrinsicMatrix(int width, int height);

    /*
     This version accepts a camera used to track and process
     */
    std::vector<VROARImageTrackerOutput> findTarget(cv::Mat inputImage, float* intrinsics, std::shared_ptr<VROARCamera> camera);

    /*
     Finds the _arImageTarget in the inputImage (assumes RGB format). Uses the given intrinsics matrix. Pass in
     NULL for intrinsics ptr if one doesn't exist.
     */
    std::vector<VROARImageTrackerOutput> findTarget(cv::Mat inputImage, float* intrinsics);

    /*
     Finds all the _arImageTargets in the given inputImage asynchronously. Make sure to set the _listener.
     */
    void findTargetAsync(cv::Mat inputImage, float* intrinsics, std::shared_ptr<VROARCamera> camera);

    /*
     Set the listener
     */
    void setListener(std::shared_ptr<VROARImageTrackerListener> listener) {
        _listener = listener;
    }
private:
    
    void updateType();
    /*
     This function goes through and re-creates/re-runs feature detection on the targets
     */
    VROARImageTargetOpenCV updateTargetInfo(std::shared_ptr<VROARImageTarget> arImageTarget);

    /*
     An internal helper method called by findTarget()
     */
    std::vector<VROARImageTrackerOutput> findTargetInternal(cv::Mat inputImage, bool async);

    /*
     A bloated function that compares all the targets w/ the input image w/ BF (brute force) matching
     and also performs pose estimation
     TODO: probably pull out pose estimation into its own function.
     */
    std::vector<VROARImageTrackerOutput> findMultipleTargetsBF(std::vector<cv::KeyPoint> inputKeypoints,
                                                               cv::Mat inputDescriptors,  cv::Mat inputImage,
                                                               float scaleFactor);

    VROARImageTrackerOutput findSingleTargetBF(VROARImageTargetOpenCV target,
                                               std::vector<cv::KeyPoint> inputKeypoints,
                                               cv::Mat inputDescriptors,  cv::Mat inputImage,
                                               float scaleFactor);

    /*
     This function takes a list of raw outputs and adds them to the _targetOutputsMap before invoking findUpdates on
     each updated outputs vector.
     */
    std::vector<VROARImageTrackerOutput> processOutputs(std::vector<VROARImageTrackerOutput> rawOutputs);

    /*
     This function takes a list of "found" raw Outputs and attempts to determine if the given rawOutput adds
     enough information to update or declare a target "found"
     */
    VROARImageTrackerOutput determineFoundOrUpdate(VROARImageTrackerOutput output);
    
    // Different implementations/versions of determineFoundOrUpdate (look at implementation)
    VROARImageTrackerOutput determineFoundOrUpdateV1(VROARImageTrackerOutput output);
    VROARImageTrackerOutput determineFoundOrUpdateV2(VROARImageTrackerOutput output);
    VROARImageTrackerOutput determineFoundOrUpdateV3(VROARImageTrackerOutput output);
    VROARImageTrackerOutput determineFoundOrUpdateV4(VROARImageTrackerOutput output);

#pragma mark - Utility Functions

    /*
     This function takes the input image and returns the factor that we should scale the input by
     in order to maintain performance
     */
    float getScaleFactor(int rows, int cols);

    /*
     This function takes the given inputCols and inputRows and returns the intrinsic matrix
     for this device.
     */
    cv::Mat getIntrinsicMatrix(int inputCols, int inputRows);

    /*
     This function returns the distortion coefficients for this device.
     */
    cv::Mat getDistortionCoeffs();

    /*
     This function takes two outputs and determines if they are similar
     */
    bool areOutputsSimilar(VROARImageTrackerOutput first, VROARImageTrackerOutput second);

    /*
     This function takes two outputs and determines if they are similar using the given distance as
     well as other heuristics.
     */
    bool areOutputsSimilarWithDistance(VROARImageTrackerOutput first, VROARImageTrackerOutput second, double distance);

    /*
     This function takes a vector of points representing the corners and performs sanity checks
     on the corners to determine if they make sense for a rectangular surface
     */
    bool areCornersValid(std::vector<cv::Point2f> corners);

    /*
     This function takes a Rodrigues rotation vector and converts it to a rotation vector (still in
     OpenCV axis though).
     */
    VROVector3f eulerFromRodriguesVector(cv::Mat rodrigues);

    /*
     This function converts from OpenCV axes to Viro axes.
     
     OpenCV axes: X to the right, Z into the screen, Y goes down
     Viro axes: X to the right, Z out of the screen (back), Y goes up
     */
    void convertFromCVToViroAxes(cv::Mat inputTranslation, cv::Mat inputRotation, VROVector3f &outTranslation, VROVector3f &outRotation);

    /*
     This function takes a camera and returns the given translation/rotation (in camera coordinates) as a transform in world coordinates.
 
     returns the identity matrix if camera is null, otherwise returns a transformation matrix.
     */
    VROMatrix4f convertToWorldCoordinates(std::shared_ptr<VROARCamera> camera, VROVector3f translation, VROVector3f rotation);


#pragma mark - Debug Drawing Functions
    /*
     On Android, the following functions automatically draw to the screen whereas on iOS the debug
     Mats should be returned to the end user!
     */

    /*
     This function takes an inputImage and a vector of 4 points (in OpenCV) coordinate space
     and draws lines between the corners (for debugging).
     */
    cv::Mat drawCorners(cv::Mat inputImage, std::vector<cv::Point2f> inputCorners, float scaleFactor);

    /*
     This function draws the matches between image1 and image2, the images are assumed to be in full
     RGB color (they'll get grayscaled within the functions).
     */
    cv::Mat drawMatches(cv::Mat image1, std::vector<cv::KeyPoint> keypoints1,
                        cv::Mat image2, std::vector<cv::KeyPoint> keypoints2,
                        std::vector<cv::DMatch> matches);

    /*
     This function draws the given keypoints on the given image
     */
    cv::Mat drawKeypoints(cv::Mat image, std::vector<cv::KeyPoint> keypoints);

    /*
     This function attempts to draw the given Mat to the screen (for debugging)
     */
    void drawMatToScreen(cv::Mat image);

#pragma mark - Camera Calibration Functions
    /*
     To enable this, set _needsCalibration = true in the contructor and
     _numCalibrationSamples = (however many samples you want before processing).

     This function calibrates the camera using an input image containing a 7x10 chessboard
     pattern within it (6x9 corners).
     */
    void findChessboardForCalibration(cv::Mat inputImage);
    void calculateIntrinsicProperties();

#pragma mark - Timing Functions

    long getCurrentTimeMs();
    long _startTime;

#pragma mark - Private Fields

    std::shared_ptr<VROARImageTarget> _arImageTarget;
    
    std::shared_ptr<VROARCamera> _currentCamera;

    std::weak_ptr<VROARImageTrackerListener> _listener;
    std::atomic_int _runningThreads;

    // The number of feature points we should extract from the input image
    int _numberFeaturePoints;
    // The minimum number of "good" matches we need before going onto pose estimation
    int _minGoodMatches;
    // The ratio used for BF Knn matching, a higher value is more relaxed.
    double _matchRatio;

    // The type of image tracker to use (not really meant to be set by the user)
    VROARImageTrackerType _type;
    // The feature (detector) to use to process each input image
    cv::Ptr<cv::Feature2D> _feature;
    // The feature (detector) to use to process the targets
    cv::Ptr<cv::Feature2D> _targetFeature;
    // The type of matcher to use for matching
    int _matcherType;
    // The BruteForce matcher to match feature descriptors
    cv::Ptr<cv::BFMatcher> _matcher;

    cv::Ptr<cv::FlannBasedMatcher> _flannMatcher;

    // Whether or not we should use a K-nearest-neighbor (or normal) BruteForce matcher
    bool _useBfKnnMatcher;
    
    // total time and iteration counts for successful runs
    double _totalTime;
    double _totalIteration;

    // total time and iteration counts for failed runs (unable to find target)
    double _totalFailedTime;
    double _totalFailedIteration;

    // List if ARImageTargets to search for
    std::vector<std::shared_ptr<VROARImageTarget>> _arImageTargets;
    // Map between VROARImageTargets and VROARImageTargetOpenCVs (contianing more info).
    std::map<std::shared_ptr<VROARImageTarget>, VROARImageTargetOpenCV> _targetToTargetMap;

    /*
     An array of intrinsic values set by the caller (currently only iOS provides this).
     */
    float *_intrinsics;

    /*
     The computed/set distortion coefficients. These remain static and only have to be computed once.
     */
    cv::Mat _distortionCoeffs;

    /*
     The current intrinsic matrix to use. This needs to be recomputed if screen res or orientation changes.
     */
    cv::Mat _intrinsicMatrix;

    /*
     Calibration props - used to enable/tune calibration
     */
    // Whether or not we should run calibration (runs alongside tracking)
    bool _needsCalibration;
    // Number of successful calibration images to use for calibration.
    int _numCalibrationSamples;
    // Keeps track of how many times calibration has been called with a new frame.
    int _calibrationFrameCount;
    // Keeps track of how many successful calibration images have been captured.
    int _calibrationFoundCount;
    // Vector of each calibration image's found points.
    std::vector<std::vector<cv::Point2f>> _foundPoints;
    // The size of the input image.
    cv::Size _inputSize;
};

#endif /* VROARImageTracker_h */

#endif /* ENABLE_OPENCV */
