/**
 * @file GVRAudioEngine.h
 * @brief GVRAudioEngine.h File
 */

#import <Foundation/Foundation.h>

/**
 * High-level Google VR Audio Engine. The GVRAudioEngine allows the user
 * to spatialize sound sources in 3D space, including distance and height cues.
 * The GVRAudioEngine is capable of playing back spatial sound in three
 * separate ways:
 *
 * * The first method, known as **Sound Object** rendering, allows the user to
 *   create a virtual sound source in 3D space. These sources, while
 *   spatialized, are fed with mono audio data.
 * * The second method allows the user to play back **Ambisonic soundfield**
 *   recordings. Ambisonic recordings are multi-channel audio files which are
 *   spatialized all around the listener in 360 degrees. These can be thought of
 *   as recorded or prebaked soundfields. They can be of great use for
 *   background effects which sound perfectly spatial. Examples include rain
 *   noise, crowd noise or even the sound of the ocean off to one side.
 * * The third method, referred to here as **Stereo Sounds**, allow the user to
 *   directly play back non-spatialized mono or stereo audio files. This is
 *   useful for music and other such audio.
 *
 * *****************************************************************************
 *
 * ### Construction
 *
 *     - (id)initWithRenderingMode:(renderingMode)rendering_mode;
 *
 * Alternatively, using init without parameters will default to binaural high
 * quality mode.
 *
 * #renderingMode is an enum which specifies a global rendering configuration
 * setting:
 *
 * * `kRenderingModeStereoPanning`:
 *   Stereo panning of all Sound Objects. This disables HRTF-based rendering.
 * * `kRenderingModeBinauralLowQuality`:
 *   This renders Sound Objects over a virtual array of 8 loudspeakers arranged
 *   in a cube about the listener’s head. HRTF-based rendering is enabled.
 * * `kRenderingModeBinauralHighQuality`:
 *   This renders Sound Objects over a virtual array of 16 loudspeakers arranged
 *   in an approximate equidistribution about the listener’s HRTF-based
 *   rendering is enabled.
 *
 * To optimize the rendering performance for headphones *and* speaker playback,
 * the speaker stereo mode can be enabled which automatically switches to stereo
 * panning when headphones are not plugged in. Note that this can lead to
 * varying CPU usage based on the audio output routing.
 *
 * If ARC is not enabled, a call to the dealloc method must be made. See the
 * Example Usage snippet below.
 *
 * Audio playback can be started and stopped by calling the methods #start and
 * #stop:
 *
 *     - (bool)start;
 *     - (void)stop;
 *
 * *****************************************************************************
 *
 * ### Sound files and preloading
 *
 * Both mono sound files for use with Sound Objects and multi-channel Ambisonic
 * sound files can be preloaded into memory before playback or alternatively
 * streamed during playback. Preloading can be useful to reduce CPU usage
 * especially if the same audio clip is likely to be played back many times. In
 * this case playback latency is also reduced.
 *
 * Sound files can be preloaded into memory by calling #preloadSoundFile: :
 *
 *     - (bool)preloadSoundFile:(const NSString*)filename;
 *
 * Unused sound files can be unloaded with a call to #unloadSoundFile: :
 *
 *     - (void)unloadSoundFile:(const NSString*)filename;
 *
 * NOTE: If a sound object, soundfield or stereo sound is created with a file
 * that has not been preloaded, that audio will be streamed.
 *
 *
 * *****************************************************************************
 *
 * ### Sound objects
 *
 * The GVRAudioEngine allows the user to create virtual Sound Objects which can
 * be placed anywhere in space around the listener. These Sound Objects take as
 * input mono audio data which is then spatialized.
 *
 * Sounds can be played back in 3D space with a call to the #createSoundObject:
 * function:
 *
 *     - (int)createSoundObject:(const NSString*)filename;
 *
 * Here the filename serves as a handle on the audio file.
 *
 * This method returns an int handle which can be used to refer to the Sound
 * Object as it is manipulated.
 *
 * Playback of a Sound Object can be initiated with a call to
 * #playSound:loopingEnabled: :
 *
 *     - (void)playSound:(int)sourceId loopingEnabled:(bool)loopingEnabled;
 *
 * and paused and resumed via #pauseSound: and #resumeSound: :
 *
 *     - (void)pauseSound:(int)sourceId;
 *     - (void)resumeSound:(int)sourceId;
 *
 * The `loopingEnabled` boolean allows the user to specify whether the sound
 * source should repeat continuously or should be played as a “single shot”.
 *
 * A Sound Object’s position in space can be altered by calling
 * #setSoundfieldRotation:x:y:z:w: :
 *
 *     - (void)setSoundObjectPosition:(int)soundObjectId
 *                                  x:(float)x
 *                                  y:(float)y
 *                                  z:(float)z;
 *
 * The three variables `x`, `y`, `z` denote the position in Cartesian world
 * space coordinates at which the Sound Object shall appear.
 *
 *
 * The behavior of Sound Objects with respect to their distance from the
 * listener can be controlled via calls to the following method:
 *
 *     - (void)setSoundObjectDistanceRolloffModel:(int)soundObjectId
 *                                   rolloffModel:(distanceRolloffModel)rolloffModel
 *                                    minDistance:(float)minDistance
 *                                    maxDistance:(float)maxDistance;
 *
 *     - (void)setSoundObjectDistanceAttenuation:(int)soundObjectId
 *                           distanceAttenuation:(float)distanceAttenuation;
 *
 * This enables a user to choose between logarithmic and linear distance rolloff
 * methods, or to completely disable distance rolloff effects.
 *
 * A Sound Object’s loudness can be altered by calling #setSoundVolume:volume: :
 *
 *     - (void)setSoundVolume:(int)sourceId volume:(float)volume;
 *
 * The volume variable allows the user to control the loudness of individual
 * sources. This can be useful when some of your mono audio files are
 * intrinsically louder than others. A value of 1.0f ensures that the mono audio
 * amplitude is not modified.
 *
 * Caution is encouraged when using very loud (e.g. 0dB FS normalized) mono
 * audio data, audio data that has been heavily dynamic range compressed or when
 * using multiple sources. In order to avoid clipping, individual sound object
 * volumes can be reduced by calling the #setSoundVolume: method.
 *
 * The user can ensure that the Sound Object is currently playing before calling
 * the above methods with a call to #isSoundPlaying: :
 *
 *     - (bool)isSoundPlaying:(int)sourceId;
 *
 * Once one is finished with a sound object and wish to remove it, simply place
 * a call to #stopSound: :
 *
 *     - (void)stopSound:(int)sourceId;
 *
 * On making a call to this function the Sound Object is destroyed and the
 * corresponding integer handle no longer refers to a valid Sound Object.
 *
 * *****************************************************************************
 *
 * ### Ambisonic soundfields
 *
 * The GVRAudioEngine is also designed to play back Ambisonic
 * soundfields. These are captured or pre rendered 360 degree recordings. It is
 * best to think of them as equivalent to 360 degree video. While they envelop
 * and surround the listener, they only react to rotational movement of the
 * listener. That is, one cannot walk towards features in the soundfield.
 * Soundfields are ideal for accompanying 360 degree video playback, for
 * introducing background and environmental effects such as rain or crowd noise,
 * or even for pre baking 3D audio to reduce rendering costs.
 *
 * A preloaded multi-channel Ambisonic sound file can be used to create a
 * soundfield with a call to #createSoundfield: :
 *
 *     - (int)createSoundfield:(const NSString*)filename;
 *
 * Once again an integer handle is returned allowing the user to begin playback
 * of the soundfield, to alter the soundfield’s volume, or to stop soundfield
 * playback and as such destroy the object.
 *
 *     - (void)playSound:(int)soundObjectId loopingEnabled:(bool)loopingEnabled;
 *     - (void)setSoundVolume:(int)sourceId volume:(float)volume;
 *     - (void)pauseSound:(int)sourceId;
 *     - (void)resumeSound:(int)sourceId;
 *     - (void)stopSound:(int)sourceId;
 *
 * Ambisonic soundfields can also be rotated about the listener's head in order
 * to align the components of the soundfield with the visuals of the game/app
 * using #setSoundfieldRotation:x:y:z:w:.
 *
 *     - (void)setSoundfieldRotation:(int)soundfieldId
 *                               x:(float)x
 *                               y:(float)y
 *                               z:(float)z
 *                               w:(float)w;
 *
 * *****************************************************************************
 *
 * ### Stereo sounds
 *
 * The VrAudioEngine allows the direct non-spatialized playback of both stereo
 * and mono audio. Such audio is often used for music or sound effects that
 * should not be spatialized.
 *
 * A stereo sound can be created with a call to #createStereoSound: :
 *
 *     - (int)createStereoSound:(const NSString*)filename;
 *
 * *****************************************************************************
 *
 * ### Paused Sounds and Stopped Sounds
 *
 * When using sound sources of any of the above types, the user can ensure that
 * the given source is currently playing before calling.
 *
 *   - (bool)isSoundPlaying:(int)sourceId;
 *
 * This method will return false if the source has been either paused or
 * stopped, and true if the source is currently playing.
 *
 * Once one is finished with a Sound Object and wish to remove it, a call can be
 * placed to:
 *
 *   - (void)stopSound(int):sourceId;
 *
 * Once a source has been stopped it is destroyed and the corresponding sourceId
 * will be invalid. Sources which have been played with the |looping_enabled|
 * parameter disabled will also be destroyed once playback of the full audio
 * clip has completed.
 *
 * To check whether a given sourceId corresponds to a valid source which exists
 * and is in a playable state, a call can be made to:
 *
 *   - (bool)isSourceIdValid(int):sourceId;
 *
 * By using this pair of methods a user can differentiate between sources which
 * have been paused and those which have ceased.
 *
 * *****************************************************************************
 *
 * ### Listener position and rotation
 *
 * In order to ensure that the audio in your application reacts to user head
 * movement it is important to update head orientation in the graphics callback
 * using the head orientation matrix.
 *
 * The following two methods control the listener’s head orientation in terms of
 * audio. #setHeadPosition:y:z: :
 *
 *     - (void)setHeadPosition:(float)x y:(float)y z:(float)z;
 *
 * where `x`, `y` and `z` are cartesian world space coordinates
 *
 * and #setHeadRotation:y:z:w: :
 *
 *     - (void)setHeadRotation:(float)x y:(float)y z:(float)z w:(float)w;
 *
 * where `x`, `y`, `z` and `w` are the components of a quaternion.
 *
 * *****************************************************************************
 *
 * ### Room effects
 *
 * The GVRAudioEngine provides a reverb engine enabling the user to
 * create arbitrary room effects by specifying the size of a room and a material
 * for each surface of the room from the `#materialName` enum. Each of these
 * surface materials has unique absorption properties which differ with
 * frequency. The room created will be centred around the listener. Note that in
 * the GVRAudioEngine the unit of distance is meters.
 *
 * The following methods are used to control room effects. #enableRoom: :
 *
 *     - (void)enableRoom:(bool)enable
 *
 * enables or disables room effects with smooth transitions,
 *
 * and
 * #setRoomProperties:size_y:size_z:wall_material:ceiling_material:floor_material:
 * :
 *
 *     - (void)setRoomProperties:(float)size_x
 *                        size_y:(float)size_y
 *                        size_z:(float)size_z
 *                 wall_material:(materialName)wall_material
 *              ceiling_material:(materialName)ceiling_material
 *                floor_material:(materialName)floor_material;
 *
 * allows the user to describe the room based on its dimensions (`size_x`,
 * `size_y`, `size_z`), and its surface properties. For example one can expect
 * very large rooms to be more reverberant than smaller rooms and for a room
 * with brick surfaces to be more reverberant than one with heavy curtains on
 * every surface.
 *
 * NB: Sources located outside of a room will sound quite different from those
 * inside due to an attenuation of reverb and direct sound while sources far
 * outside of a room will not be audible.
 *
 * #setRoomReverbAdjustments:timeAdjust:brightnessAdjust: can be used to subtly
 * adjust the reverb in a room by changing the gain/attenuation on the reverb,
 * setting a multiplier on the reverberation time to control the reverb's
 * length, or adjusting the balance between the low and high frequency
 * components of the reverb.
 *
 *     - (void)setRoomReverbAdjustments:(float)gain
 *                           timeAdjust:(float)timeAdjust
 *                     brightnessAdjust:(float)brightnessAdjust
 *
 * *****************************************************************************
 *
 * ### Example usage
 *
 *     // Initialize a GVRAudioEngine in binaural high quality rendering mode.
 *     GVRAudioEngine *gvrAudio;
 *     gvrAudio = [[GVRAudioEngine alloc]
 *                       initWithRenderingMode:kRenderingModeBinauralHighQuality];
 *
 *     // Load an audio file (compressed or uncompressed) from the main bundle.
 *     NSString filename = @"mono_audio_file.mp3";
 *     bool filePreloaded = [gvrAudio preloadSoundFile:filename];
 *
 *     // Start audio playback.
 *     bool playbackStarted = [gvrAudio start];
 *
 *     // Create a Sound Object with the preloaded audio file.
 *     int sourceId = -1;
 *     if(filePreloaded) {
 *       sourceId = [gvrAudio createSoundObject:filename];
 *     }
 *
 *     // Begin Playback of the Sound Object.
 *     if (sourceId != -1) {
 *       [gvrAudio playSound:sourceId loopingEnabled:true];
 *     }
 *
 *     // Change the location and volume of the Sound Object.
 *     if(sourceId != -1) {
 *       [gvrAudio setSoundObjectPosition:sourceId x:0.5f y:2.0f z:1.2f];
 *       [gvrAudio setSoundVolume:0.75f];
 *     }
 *
 *     // Change the listener position.
 *     [gvrAudio setHeadPosition:0.5f y:0.5f z:0.5f];
 *
 *     // Stop playback of the preloaded audio file.
 *     if([gvrAudio isSoundPlaying:sourceId]) {
 *       [gvrAudio stopSound:sourceId];
 *     }
 *
 *     // Stop audio playback.
 *     [gvrAudio stop];
 *
 *     // If ARC is not enabled.
 *     [gvrAudio dealloc];
 */
@interface GVRAudioEngine : NSObject

/**
 * @enum renderingMode
 * The rendering mode to use for sound objects.
 */
typedef enum renderingMode {
  /**
   * Stereo panning of all Sound Objects. This disables HRTF-based rendering.
   */
  kRenderingModeStereoPanning,
  /**
   * This renders Sound Objects over a virtual array of 8 loudspeakers arranged
   * in a cube about the listener’s head. HRTF-based rendering is enabled.
   */
  kRenderingModeBinauralLowQuality,
  /**
   * This renders Sound Objects over a virtual array of 16 loudspeakers arranged
   * in an approximate equidistribution about the listener’s HRTF-based
   * rendering is enabled.
   */
  kRenderingModeBinauralHighQuality,
} renderingMode;

/**
 * @enum materialName
 * The surface material to use for room effects.
 */
typedef enum materialName {
  /** Acoustically transparent material, reflects no sound. */
  kTransparent,
  /** Acoustic ceiling tiles, absorbs most frequencies. */
  kAcousticCeilingTiles,
  /** Bare brick, relatively reflective. */
  kBrickBare,
  /** Painted brick. */
  kBrickPainted,
  /** Coarse surface concrete block. */
  kConcreteBlockCoarse,
  /** Painted concrete block. */
  kConcreteBlockPainted,
  /** Heavy curtains. */
  kCurtainHeavy,
  /** Fiber glass insulation. */
  kFiberGlassInsulation,
  /** Thin glass. */
  kGlassThin,
  /** Thick glass. */
  kGlassThick,
  /** Grass. */
  kGrass,
  /** Linoleum on concrete. */
  kLinoleumOnConcrete,
  /** Marble. */
  kMarble,
  /** Galvanized sheet metal. */
  kMetal,
  /** Wooden parquet on concrete. */
  kParquetOnConcrete,
  /** Rough plaster surface. */
  kPlasterRough,
  /** Smooth plaster surface. */
  kPlasterSmooth,
  /** Plywood panel. */
  kPlywoodPanel,
  /** Polished concrete or tile surface. */
  kPolishedConcreteOrTile,
  /** Sheet rock. */
  kSheetrock,
  /** Surface of water or ice. */
  kWaterOrIceSurface,
  /** Wooden ceiling. */
  kWoodCeiling,
  /** Wood paneling. */
  kWoodPanel
} materialName;

/**
 * @enum distanceRolloffModel
 * The distance rolloff model to be used for a given sound object.
 */
typedef enum distanceRolloffModel {
  /**
   * Logarithmic distance rolloff model.
   */
  kLogarithmic,
  /**
   * Linear distance rolloff model.
   */
  kLinear,
  /**
   * No distance rolloff will be applied.
   */
  kNone,
} distanceRolloffModel;

/** Initialize with a rendering quality mode. Note, when the default init method
  * is used, the rendering quality is set to kRenderingModeBinauralHighQuality.
  *
  * @param rendering_mode Chooses the rendering quality mode.
  */
- (id)initWithRenderingMode:(renderingMode)rendering_mode;

/** Starts the audio playback.
  *
  *  @return true on success.
  */
- (bool)start;

/** Stops the audio playback. */
- (void)stop;

/** Must be called from the main thread at a regular rate. It is used to execute
 *  background operations outside of the audio thread.
 */
- (void)update;

/**
 * Enables the stereo speaker mode. It enforces stereo-panning when headphones
 * are *not* plugged into the phone. This helps to avoid HRTF-based coloring
 * effects and reduces computational complexity when speaker playback is
 * active. By default the stereo speaker mode optimization is disabled.
 *
 * @param enable True to enable the stereo speaker mode.
 */
- (void)enableStereoSpeakerMode:(bool)enable;

/** Preloads a local sound file. Note that the local file access method depends
  * on the target platform.
  *
  * @param filename Name of the file used as identifier.
  * @return True on success or if file has been already preloaded.
  */
- (bool)preloadSoundFile:(const NSString*)filename;

/** Unloads a previously preloaded sample from memory. Note that if the sample
  * is currently used, the memory is freed at the moment playback stops.
  *
  * @param filename Name of the file used as identifier.
  */
- (void)unloadSoundFile:(const NSString*)filename;

/** Returns a new sound object handle. Note that the sample should only contain
  * a single audio channel (stereo sources are automatically downmixed to mono).
  * The handle automatically destroys itself at the moment the sound playback
  * has stopped.
  *
  * @param filename The path/name of the file to be played.
  * @return Id of new sound object. Returns kInvalidId if the sound file could
  *     not be loaded or if the number of input channels is > 1.
  */
- (int)createSoundObject:(const NSString*)filename;

/** Returns a new ambisonic soundfield handle. Note that the sample needs to
  * be preloaded and must have 4 separate audio channels. The handle
  * automatically destroys itself at the moment the sound playback has stopped.
  *
  * @param filename The path/name of the file to be played.
  * @return Id of new soundfield. Returns kInvalidId if the sound file could
  *     not be loaded or if the number of requires input channels does not
  *     match.
  */
- (int)createSoundfield:(const NSString*)filename;

/** Returns a new non-spatialized stereo sound handle. Note that the sample must
  * have at most two audio channels. Both mono and stereo audio files are
  * supported. The handle automatically destroys itself at the moment the sound
  * playback has stopped.
  *
  * @param filename The path/name of the file to be played.
  * @return Id of new soundfield. Returns kInvalidId if the sound file could
  *     not be loaded or if the number of required input channels does not
  *     match.
  */
- (int)createStereoSound:(const NSString*)filename;

/** Starts the playback of a sound.
  *
  * @param sourceId Id of the audio source to be played.
  * @param loopingEnabled Enables looped audio playback.
  */
- (void)playSound:(int)sourceId loopingEnabled:(bool)loopingEnabled;

/** Pauses the playback of a sound.
  *
  * @param sourceId Id of the audio source to be paused.
  */
- (void)pauseSound:(int)sourceId;

/** Resumes the playback of a sound.
  *
  * @param sourceId Id of the audio source to be resumed.
  */
- (void)resumeSound:(int)sourceId;

/** Stops the playback of a sound and destroys the corresponding Sound Object
  * or Soundfield.
  *
  * @param sourceId Id of the audio source to be stopped.
  */
- (void)stopSound:(int)sourceId;

/** Checks if a sound is playing.
  *
  * @param sourceId Id of the audio source to be checked.
  * @return True if the sound is being played.
  */
- (bool)isSoundPlaying:(int)sourceId;

/** Checks if a |sourceId| is valid, and that the corresponding source is in a
  * playable state. Sources that have been stopped will be reported as invalid.
  *
  * @param sourceId Id of the audio source to be checked.
  * @return True if the source exists and is in a playable state.
  */
- (bool)isSourceIdValid:(int)sourceId;

/** Repositions an existing sound object.
  *
  * @param soundObjectId Id of the sound object to be moved.
  * @param x X coordinate the sound will be placed at.
  * @param y Y coordinate the sound will be placed at.
  * @param z Z coordinate the sound will be placed at.
  */
- (void)setSoundObjectPosition:(int)soundObjectId
                             x:(float)x
                             y:(float)y
                             z:(float)z;

/**
  * Sets the given sound object source's distance attenuation method with
  * minimum and maximum distances. Maximum distance must be greater than the
  * minimum distance for the method to be set.
  *
  * @param soundObjectId Id of sound object source.
  * @param rolloffModel The distanceRolloffModel to be used for the given. Note
  *     setting the rolloff model to distanceRolloffModel::kNone will allow
  *     distance attenuation to be set manually.
  * @param minDistance Minimum distance to apply distance attenuation method.
  * @param maxDistance Maximum distance to apply distance attenuation method.
  */
- (void)setSoundObjectDistanceRolloffModel:(int)soundObjectId
                              rolloffModel:(distanceRolloffModel)rolloffModel
                               minDistance:(float)minDistance
                               maxDistance:(float)maxDistance;

/** Rotates an existing Ambisonic soundfield.
  *
  * @param soundfieldId Id of the Ambisonic soundfield to be rotated.
  * @param x X component of the quaternion describing the rotation.
  * @param y Y component of the quaternion describing the rotation.
  * @param z Z component of the quaternion describing the rotation.
  * @param w W component of the quaternion describing the rotation.
  */
- (void)setSoundfieldRotation:(int)soundfieldId
                             x:(float)x
                             y:(float)y
                             z:(float)z
                             w:(float)w;

/** Changes the volume of an existing sound.
  *
  * @param sourceId Id of the audio source to be modified.
  * @param volume Volume value. Should range from 0 (mute) to 1 (max).
  */
- (void)setSoundVolume:(int)sourceId volume:(float)volume;

/** Sets the head position.
  *
  * @param x X coordinate of head position in world space.
  * @param y Y coordinate of head position in world space.
  * @param z Z coordinate of head position in world space.
  */
- (void)setHeadPosition:(float)x y:(float)y z:(float)z;

/** Sets the head rotation.
  *
  * @param x X component of quaternion.
  * @param y Y component of quaternion.
  * @param z Z component of quaternion.
  * @param w W component of quaternion.
  */
- (void)setHeadRotation:(float)x y:(float)y z:(float)z w:(float)w;

/** Turns on/off the room reverberation effect.
 *
 *  @param True to enable room effect.
 */
- (void)enableRoom:(bool)enable;

/** Sets the room properties describing the dimensions and surface materials of
 *  a given room.
 *
 * @param size_x Dimension along X axis.
 * @param size_y Dimension along Y axis.
 * @param size_z Dimension along Z axis.
 * @param wall_material Surface material for the four walls.
 * @param ceiling_material Surface material for the ceiling.
 * @param floor_material Surface material for the floor.
 */
- (void)setRoomProperties:(float)size_x
                   size_y:(float)size_y
                   size_z:(float)size_z
            wall_material:(materialName)wall_material
         ceiling_material:(materialName)ceiling_material
           floor_material:(materialName)floor_material;

/** Adjusts the properties of the current reverb, allowing changes to the
 * reverb's gain, duration and low/high frequency balance.
 *
 * @param gain Reverb volume (linear) adjustment in range [0, 1] for
 *     attenuation, range [1, inf) for gain boost.
 * @param timeAdjust Reverb time adjustment multiplier to scale the
 *     reverberation tail length. This value should be >= 0.
 * @param brightness_adjust Reverb brightness adjustment that controls the
 *     reverberation ratio across low and high frequency bands.
 */
- (void)setRoomReverbAdjustments:(float)gain
                      timeAdjust:(float)timeAdjust
                brightnessAdjust:(float)brightnessAdjust;

@end
