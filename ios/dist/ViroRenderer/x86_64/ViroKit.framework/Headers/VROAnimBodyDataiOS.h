//
//  VROAnimBodyDataiOS.hpp
//  ViroRenderer
//
//  Created by vik.advani on 1/25/19.
//  Copyright © 2019 Viro Media. All rights reserved.
//

#ifndef VROAnimBodyDataiOS_h
#define VROAnimBodyDataiOS_h

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "VROBodyAnimData.h"


class VROBodyAnimDataRecorderiOS: public VROBodyAnimDataRecorder {
public:
    VROBodyAnimDataRecorderiOS();
    void startRecording(VROMatrix4f startWorldTransform, std::map<std::string, float> boneLengths);
    void stopRecording();
    void beginRecordedRow();
    void addJointToRow(std::string jointName, VROVector3f jointPos);
    void endRecordedRow();
    std::string toJSON();
    
private:
    double _startRecordingTime;
    double _endRecordingTime;
    bool _isRecording;
    VROMatrix4f _initRecordWorldTransformOfRootNode;
    // TODO Move recording to abstract class so we can remove #if VRO_PLATFORM_IOS ifdefs.
    NSMutableDictionary * _recordedAnimationData;
    NSMutableArray * _recordedAnimationRows;
    NSMutableDictionary *_animRowDataValues;
    NSMutableDictionary *_currentRow;
    NSMutableDictionary *_jointValues;
    NSMutableDictionary *_mlBoneLengths;

    NSString *_nsBodyAnimTotalTime;
    NSString *_nsBodyAnimAnimRows;
    NSString *_nsBodyAnimTimestamp;
    NSString *_nsBodyAnimJoints;
    NSString *_nsBodyAnimInitModelTransform;
    NSString *_nsBodyVersion;
    NSString *_nsBodyAnimBoneLengths;
};

class VROBodyAnimDataReaderiOS : public VROBodyAnimDataReader {
public:
    VROBodyAnimDataReaderiOS();
    std::shared_ptr<VROBodyAnimData> fromJSON(std::string jsonData);
    
private:
    void loadAnimRows(NSDictionary *dictionary, std::shared_ptr<VROBodyAnimData> animBodyData);
    void loadTotalTime(NSDictionary *dictionary, std::shared_ptr<VROBodyAnimData> animBodyData);
    void loadVersion(NSDictionary *dictionary, std::shared_ptr<VROBodyAnimData> animBodyData);
    void loadWorldStartMatrix(NSDictionary *dictionary, std::shared_ptr<VROBodyAnimData> animBodyData);
    void loadSkeletalBone(NSDictionary *dictionary, std::shared_ptr<VROBodyAnimData> animBodyData);
    std::map<VROBodyJointType, VROVector3f> loadAnimRow(NSDictionary *joints, std::shared_ptr<VROBodyAnimData> bodyANimData);
    
    NSDictionary *_animBody;
    VROMatrix4f _startRootMatrixWorld;
    VROMatrix4f _initPlaybackWorldTransformOfRootNode;
    NSArray *_animRows;
    NSArray *_matrixArray;
    NSString *_nsBodyAnimTotalTime;
    NSString *_nsBodyAnimAnimRows;
    NSString *_nsBodyAnimTimestamp;
    NSString *_nsBodyAnimJoints;
    NSString *_nsBodyAnimInitModelTransform;
    NSString *_nsBodyVersion;
    NSString *_nsBodyAnimBoneLengths;
};

#endif /* VROAnimBodyDataiOS_h */
