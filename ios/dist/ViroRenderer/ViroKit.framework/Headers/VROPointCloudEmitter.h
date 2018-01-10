//
//  VROPointCloudEmitter.h
//  ViroKit
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROPointCloudEmitter_h
#define VROPointCloudEmitter_h

#include <stdio.h>
#include "VROParticleEmitter.h"

class VRODriver;
class VRONode;
class VROSurface;
class VROARSession;
class VROBillboardConstraint;

/*
 Class that inheirits from VROParticleEmitter that uses the particle system to draw
 PointCloud points that we get from the given VROARSession.

 Only things that can really be modified are the surface, scale and # of particles
 */
class VROPointCloudEmitter : public VROParticleEmitter {
public:
    VROPointCloudEmitter(std::shared_ptr<VRODriver> driver,
                         std::shared_ptr<VROARSession> session);
    virtual ~VROPointCloudEmitter() {};

    void setParticleScale(VROVector3f scale) {
        _particleScale = scale;
    }

    void clearParticles();
    void update(const VRORenderContext &context, const VROMatrix4f &computedTransform);

    void resetParticleSurface();
    void setParticleSurface(std::shared_ptr<VROSurface> particleSurface);

private:
    std::weak_ptr<VROARSession> _arSession;
    VROVector3f _particleScale;

    /*
     This function updates the _particles array with the given pointCloudPoints by reusing particles,
     resurrecting zombie particles and/or creating new ones to meet the demand. It also computes
     the boundingbox that contains all the points.
     */
    VROBoundingBox updateParticles(std::vector<VROVector4f> pointCloudPoints,
                                   const VRORenderContext &context);

    /*
     This function updates the UBO w/ the particles in _particles.
     */
    void updateUBO(VROBoundingBox boundingBox);

    /*
     Computes the transform for the given particle with the given position, constraint
     and also updates the given boundingBox.
     */
    void computeParticleTransform(VROParticle *particle,
                                  VROVector4f position,
                                  std::shared_ptr<VROBillboardConstraint> constraint,
                                  VROBoundingBox *boundingBox,
                                  const VRORenderContext &context);
    
    /*
     This function adds all _particles from startIndex and on to _zombieParticles
     */
    void zombifyParticles(int startIndex);
};

#endif /* VROPointCloudEmitter_h */
