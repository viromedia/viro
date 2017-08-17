//
//  VROParticleEmitter.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROParticleEmitter_h
#define VROParticleEmitter_h

#include "VROMatrix4f.h"
#include "VROVector3f.h"
#include "VROVector4f.h"
#include <memory>
#include <vector>

class VRODriver;
class VROTexture;
class VRORenderContext;
class VRONode;
class VROSurface;
class VROParticleUBO;
class VROParticle;

/*
 Struct containing all properties relating to a single particle, such as
 life cycle spawn, transformations, color and physics states.
 TODO: VIRO-XXXX Update VROParticle as we complete the Modifier Modules
 */
struct VROParticle{
    // Position at which particle had spawned at, local to the particle emitter.
    VROMatrix4f startLocalTransform;

    // Position at which particle had spawned at, in world transform.
    VROMatrix4f spawnedWorldTransform;

    // Current position of the particle, local to the particle emitter.
    VROMatrix4f currentLocalTransform;

    // Current position of the particle, in world transform
    VROMatrix4f currentWorldTransform;

    // True if this particle's scale, rotation and position are continously
    // affected by changes in the emitter's transform after it has been spawned.
    bool fixedToEmitter;

    // Starting and ending values representing scale, rotation and color properties
    VROVector3f startScale;
    VROVector3f startRotation;
    VROVector3f endRotation;
    VROVector3f endScale;
    VROVector4f colorStart;
    VROVector4f colorEnd;
    VROVector4f colorCurrent;

    // Basic Physics properties
    VROVector3f velocity;
    VROVector3f acceleration;

    // True if the particle has died but is not yet due to be de-allocated.
    // This is mainly used for recycling particles quickly and to reduce the
    // numerous number of allocations / de-allocations of particle objects.
    bool isZombie;

    // Time at which this particle was spawned at in milliseconds.
    double spawnTimeMs;

    // Time at which this particle is killed and becomes a zombie, in milliseconds.
    double killedTimeMs;

    // Duration of time a particle remains alive, in milliseconds.
    double lifePeriodMs;

    // Duration of time a particle remains a zombie before being deallocated, in milliseconds.
    double zombiePeriodMs;
};

/*
 VROParticleEmitter handles the behavior of quad particles emitted in the scene in terms of
 its life cycle, physical motion, and visual characteristics like color and alpha.
 */
class VROParticleEmitter {
public:
    VROParticleEmitter(std::shared_ptr<VRODriver> driver,
                       std::shared_ptr<VRONode> emitterNode,
                       std::shared_ptr<VROTexture> texture,
                       float width, float height);
    ~VROParticleEmitter();

    /*
     Called per frame to update particle states, lifetime and behavior.
     */
    void update(const VRORenderContext &context);

private:
    /*
     A weak reference to the VRONode that positions this VROParticleEmitter.
     */
    std::weak_ptr<VRONode> _particleEmitterNodeWeak;

    /*
     List of all particles in the scene for this emitter. This includes active particles (particles
     that have just been spawned and are animating in the scene) and also zombie particles
     (particles that are just killed and become a zombie temporarily before being de-allocated
     and removed from this list to encourage the recycling of particle objects).
     */
    std::vector<VROParticle> _particles;

    /*
     List of index references that points/refers to all zombie particles in _particles.
     */
    std::vector<int> _zombieParticles;

    /*
     The maximum number of active particles (not including zombie ones) that this emitter
     can have at any given moment.
     */
    int _maxParticles = 100;

    /*
     The rate at which particles can spawn at, in milliseconds.
     */
    double _particlesSpawnIntervalMs = 100;

    /*
     The number of particles spawned at each spawn interval.
     */
    int _particlesPerSpawn = 3;

    /*
     The last time at which we had last spawned particles.
     */
    double _lastParticleSpawnTime = 0;

    /*
     Below are specific update functions that define the behavior and appearance
     of the particle.
     TODO: VIRO-XXXX Update these functions as we complete the Modifier Modules
     */
    void updateParticlePhysics(double currentTime);
    void updateParticleAppearance(double currentTime);
    void updateParticlesToBeKilled(double currentTime);
    void updateParticleSpawn(double currentTime);
    void updateZombieParticles(double currentTime);

    /*
     Resets the particle to a set of known defaults, so that it can be re-used / re-emitted.
     */
    void resetParticle(VROParticle &particle, double currentTime);
};

#endif /* VROParticleEmitter_h */
