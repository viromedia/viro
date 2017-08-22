//
//  VROParticleEmitter.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROParticleEmitter_h
#define VROParticleEmitter_h

#include "VRODriver.h"
#include "VROParticleModifier.h"

class VROSurface;
class VROParticleUBO;
class VROParticle;
class VRONode;
class VROTexture;

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

    void setRun(bool emit) {
        _requestRun = emit;
    }

    void setDuration(double duration) {
        _duration = duration;
    }

    void setLoop(bool loop) {
        _loop = loop;
    }

    void setFixedToEmitter(bool isFixed) {
        _fixToEmitter = isFixed;
    }

    void setMaxParticles (int maxParticles) {
        _maxParticles = maxParticles;
    }

    void setParticleLifeTime(std::pair <int, int> lifeTime) {
        _particleLifeTime = lifeTime;
    }

    void setEmissionRatePerSecond(std::pair <int, int> rate) {
        _particlesEmittedPerSecond = rate;
    }

    void setEmissionRatePerDistance(std::pair <int, int> rate) {
        _particlesEmittedPerMeter = rate;
    }

    /*
     True if we are no longer emitting particles and have completed the emission cycle.
     */
    bool finishedEmissionCycle();

    /*
     Reset back to the beginning of the emission cycle for this emitter.
     */
    void resetEmissionCycle(bool resetParticles, VROVector3f currentPos);

    /*
     Used for constructing the behavior of how bursts of particles spawn.
     */
    struct VROParticleBurst{
        // Reference factor against which to compare when deciding how to burst-spawn particles.
        VROParticleModifier::VROModifierFactor referenceFactor;

        // Min, Max range of number of particles to spawn.
        std::pair<int, int> numberOfParticles;

        // Starting Reference Value at which to begin the burst.
        double referenceValueStart;

        // Cool down period in milliseconds or distance in meters after each burst.
        double referenceValueInterval;

        // Number of times to repeat this burst.
        int cycles;
    };

    /*
     Sets on this emitter a list of VROParticleBurst to emit.
     */
    void setParticleBursts(std::vector<VROParticleBurst> bursts) {
        _bursts = bursts;
        _scheduledBurst = bursts;
    }

    /*
     Below are specific particle modifiers that can be set by the bridge, that defines the
     behavior and appearance of emitted particles.
     */
    void setAlphaModifier(std::shared_ptr<VROParticleModifier> mod) {
        _alphaModifier = mod;
    }
    void setColorModifier(std::shared_ptr<VROParticleModifier> mod) {
        _colorModifier = mod;
    }
    void setScaleModifier(std::shared_ptr<VROParticleModifier> mod) {
        _scaleModifier = mod;
    }
    void setRotationModifier(std::shared_ptr<VROParticleModifier> mod) {
        _rotationModifier = mod;
    }
    void setVelocityModifier(std::shared_ptr<VROParticleModifier> mod) {
        _velocityModifier = mod;
    }
    void setAccelerationmodifier(std::shared_ptr<VROParticleModifier> mod) {
        _accelerationModifier = mod;
    }

private:
    /*
     A weak referenceFactor to the VRONode that positions this VROParticleEmitter.
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
     Vector containing all particles that have died so that they can be reused. Particles
     in a zombie start after a certain time will be de-allocated.
     */
    std::vector<VROParticle> _zombieParticles;

#pragma mark - Particle Emission Behaviors
    /*
     Flag for setting the _run state on a render pass. This is required because emitter states that
     are updated as a result of changes to the _run flag are dependent on certain scene->compute
     render passes to occur first before we compute this Particle emitter (like transforms).
     */
    bool _requestRun;

    /*
     True to continue emitting particles from when the emitter had last stopped.
     */
    bool _run;

    /*
     The length of time in milliseconds this emitter is emitting particles.
     */
    double _duration;

    /*
     If true, the emission cycle will repeat after the duration.
     */
    bool _loop;

    /*
     If false, already emitted particles will not be affected by transformation changes made
     to this emitter's node, else, they would be "locked" to the emitter.
     */
    bool _fixToEmitter;

    /*
     Below are specific particle modifiers that can be set by the bridge, that defines the
     behavior and appearance of emitted particles.
     */
    std::shared_ptr<VROParticleModifier> _alphaModifier;
    std::shared_ptr<VROParticleModifier> _colorModifier;
    std::shared_ptr<VROParticleModifier> _scaleModifier;
    std::shared_ptr<VROParticleModifier> _rotationModifier;
    std::shared_ptr<VROParticleModifier> _velocityModifier;
    std::shared_ptr<VROParticleModifier> _accelerationModifier;

    /*
     Emission of extra particles at specific times or distances during the entire _duration
     this emitter is emitting.
     */
    std::vector<VROParticleBurst> _bursts;

    /*
     Copied from _burst at the beginning of every emitter emission cycle to schedule and
     maintain the state of all incoming burst events.
     */
    std::vector<VROParticleBurst> _scheduledBurst;

    /*
     The maximum number of active particles (not including zombie ones) that this emitter
     can have at any given moment.
     */
    int _maxParticles;

    /*
     The min max lifetime of emitted particles.
     */
    std::pair <int, int> _particleLifeTime;

#pragma mark - Emitter Attributes
    /*
     Total time that has passed since the beginning of this emitter's emission cycle.
     */
    double _emitterTotalPassedTime = 0;

    /*
     Time that has passed since this emitter has last started. It is redefined as the emitter
     is paused / resumed.
     */
    double _emitterPassedTimeSoFar = 0;

    /*
     Time at which this emitter has been started at in milliseconds.
     */
    double _emitterStartTimeMs = 0;

    /*
     Total travelled distance of the emitter since the beginning of this emitter's emission cycle.
     */
    double _emitterTotalPassedDistance = 0;

    /*
     Distance travelled since this emitter has last started. It is redefined as the emitter
     is paused / resumed.
     */
    double _emitterPassedDistanceSoFar = 0;

    /*
     Location at which this emitter has started at. It is redefined as the emitter
     is paused / resumed.
     */
    VROVector3f _emitterStartLocation;

    /*
     Set for determining how many particles should be emitted per distance travelled of this
     emitter, if any.
     */
    std::pair <int, int> _particlesEmittedPerMeter;

    /*
     Position at which we had last emitted particles that were distance spawned.
     */
    VROVector3f _distanceSpawnedLastEmitPosition;

    /*
     Position where we first started distance spawning particles from.
     */
    VROVector3f _distanceSpawnedInitPosition;

    /*
     Rate at which particles are distance spawned, per meter.
     */
    double _distanceSpawnedEmissionRate = 0;

    /*
     Set for determining how many particles should be emitted per second of _duration of this
     emitter, if any.
     */
    std::pair <int, int> _particlesEmittedPerSecond;

    /*
     Time at which we had last emitted particles that were time spawned.
     */
    double _intervalSpawnedLastEmitTime = 0;

    /*
     Time when we had first started time-spawning particles from.
     */
    double _intervalSpawnedInitTime = 0;
    double _particlesSpawnIntervalMs = 100;
    double _intervalSpawnedEmissionRate = 0;

    /*
     Initialize the emitter with default configurations and states.
     */
    void initEmitter();

    /*
     Updates any time / distance state that the emitter needs to determine particle behavior.
     */
    void updateEmitter(double currentTime, std::shared_ptr<VRONode> emitterNode);

#pragma mark - Particle Attributes

    /*
     Below are specific update functions that define the behavior and appearance
     of the particle.
     */
    void updateParticles(double currentTime, const VRORenderContext &context,
                         std::shared_ptr<VRONode> emitterNode);
    void updateParticlePhysics(double currentTime);
    void updateParticleAppearance(double currentTime);
    void updateParticlesToBeKilled(double currentTime);
    void updateParticleSpawn(double currentTime, VROVector3f currentPos);
    void updateZombieParticles(double currentTime);

    /*
     Called when we wish to spawn new particles, given the numberOfParticles. To do so,
     we firstly attempt to recycle zombie particles and create new ones if we do ever run out.
     */
    void spawnParticle(int numberOfParticles, double currentTime);

    /*
     Returns the number of particles to spawn at the given currentTime with the set
     _particlesEmittedPerSecond on this emitter.
     */
    int getSpawnParticlesPerSecond(double currentTime);

    /*
     Returns the number of particles to spawn at the given position with the set
     _particlesEmittedPerMeter on this emitter.
     */
    int getSpawnParticlesPerMeter(VROVector3f currentPos);

    /*
     Returns the number of particles to spawn based on _scheduledBurst.
     */
    int getSpawnParticleBursts();

    /*
     Resets the particle to a set of known defaults, so that it can be re-used / re-emitted.
     */
    void resetParticle(VROParticle &particle, double currentTime);
};

#endif /* VROParticleEmitter_h */
