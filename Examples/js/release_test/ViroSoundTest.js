/**
 * Copyright (c) 2017-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View,
  Platform
} from 'react-native';

import {
  ViroSceneNavigator,
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroCamera,
  ViroImage,
  ViroVideo,
  Viro360Image,
  Viro360Video,
  ViroFlexView,
  ViroUtils,
  ViroAnimations,
  ViroQuad,
  ViroSkyBox,
  ViroSpatialSound,
  ViroSoundField,
  ViroSound,ViroText,
} from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;

var stateOne = {
  flex: {flex: 1},
  material: "redColor",
  width: 3,
  showSurface: true,
};

var stateTwo = {
  flex: {flex: 2},
  material: "greenColor",
  width: 3,
  showSurface: false,
}

var ReleaseMenu = require("./ReleaseMenu.js");

/*
 This file tests a simple flex view and updating properties within it.
 */
var ViroSoundTest = createReactClass({
  getInitialState: function() {
    return {
      ...stateOne,
      state: 1,
      soundSource: 1,

      volume: 1,
      isPlaying: false,
      looping: false,
      mute: false,
      volume: 1,
      toggleSource:0,

      maxDistance: 10,
      minDistance: 7,
      soundRoom:1,
      soundType:1,
      soundDistance:-7,
      cameraPos:0,
      preloadedSound : false,
    }
  },

    _toggleSoundType(){
      var newSoundType = this.state.soundType + 1;
      if (newSoundType > 3) {
          newSoundType = 1;
      }

      this.setState({
          soundType:newSoundType
      });
    },

  render: function() {
    return (
      <ViroScene reticleEnabled={true} onClick={this._onClick} soundRoom={this._getSoundRoomMaterial()} >
        <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        <ViroCamera position={[0, 0, this.state.cameraPos]} active={true}/>
        <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, 0]} transformBehaviors={["billboard"]} onClick={this._showNext} />

        <ViroSkyBox color="#AA1460" />
        {this._getSoundControls()}
        {this._testNormalSound()}
        {this._testSpatialSound()}
        {this._testSoundField()}
      </ViroScene>
    );
  },
  _getSoundControls(){
    var stringSource = "local";
    if (this.state.toggleSource == 1) {
        stringSource = "remote uri";
    } else if (this.state.toggleSource == 2) {
        stringSource = "preloaded";
    }

    var soundType="NONE";
    if (this.state.soundType == 1) {
        soundType = "Normal Sound";
    } else if (this.state.soundType == 2) {
        soundType = "Spatial Sound";
    } else if (this.state.soundType == 3) {
        soundType = "Sound Field";
    }

    return(
       <ViroNode position={[0,0,-4]}>

       <ViroText style={styles.centeredText} position={[0, 2, 0]} width={3} height ={2} fontSize={30}
                 text={"Toggle SoundType: " + soundType} textLineBreakMode='Justify' onClick={this._toggleSoundType}/>

       <ViroText style={styles.centeredText} position={[-2, 1, 0]} width={2} height ={2}
        text={"isPlaying: " + this.state.isPlaying} textLineBreakMode='Justify' onClick={this._togglePlay}/>

       <ViroText style={styles.centeredText} position={[0, 1, 0]} width={2} height ={2}
        text={"Loop: " + this.state.looping} textLineBreakMode='Justify' onClick={this._toggleLoop}/>

       <ViroText style={styles.centeredText} position={[2, 1, 0]} width={2} height ={2}
        text={"Mute: " + this.state.mute} textLineBreakMode='Justify' onClick={this._toggleMute}/>

       <ViroText style={styles.centeredText} position={[-2, 0, 0]} text={"Change Volume from: " + this.state.volume}
        width={2} height ={2}  onClick={this._toggleVolume} />

       <ViroText style={styles.centeredText} position={[0, 0, 0]} width={2} height ={2}
        text={"Source: " + stringSource} textLineBreakMode='Justify' onClick={this._toggleSource}/>

        <ViroText style={styles.centeredText} position={[2, 0, 0]} width={2} height ={2}
                text={this.state.preloadedSound ? "Sound is Preloaded" : "Sound is NOT Preloaded"}
                textLineBreakMode='Justify' onClick={this._preloadSound}/>

        </ViroNode>

    );

  },

  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./ViroLightTest')});
  },

  _getSoundRoom() {
    if (this.state.soundRoom == 0) {
      return "transparent";
    }
    else if (this.state.soundRoom == 1) {
      return "acoustic_ceiling_tiles";
    }
    else if (this.state.soundRoom == 2) {
      return "brick_bare";
    }
    else if (this.state.soundRoom == 3) {
      return "brick_painted";
    }
    else if (this.state.soundRoom == 4) {
      return "concrete_block_coarse";
    }
    else if (this.state.soundRoom == 5) {
      return "concrete_block_painted";
    }
    else if (this.state.soundRoom == 6) {
      return "curtain_heavy";
    }
    else if (this.state.soundRoom == 7) {
      return "glass_thin";
    }
    else if (this.state.soundRoom == 8) {
      return "glass_thick";
    }
    else if (this.state.soundRoom == 9) {
      return "linoleum_on_concrete";
    }
    else if (this.state.soundRoom == 10) {
      return "marble";
    }
    else if (this.state.soundRoom == 11) {
      return "grass";
    }
    else if (this.state.soundRoom == 12) {
      return "metal";
    }
    else if (this.state.soundRoom == 13) {
      return "parquet_on_concrete";
    }
    else if (this.state.soundRoom == 14) {
      return "plaster_rough";
    }
    else if (this.state.soundRoom == 15) {
      return "plaster_smooth";
    }
    else if (this.state.soundRoom == 16) {
      return "plywood_panel";
    }
    else if (this.state.soundRoom == 17) {
      return "polished_concrete_or_tile";
    }
    else if (this.state.soundRoom == 18) {
      return "sheet_rock";
    }
    else if (this.state.soundRoom == 19) {
      return "water_or_ice_surface";
    }
    else if (this.state.soundRoom == 20) {
      return "wood_ceiling";
    }
    else if (this.state.soundRoom == 21) {
      return "wood_panel";
    }
  },

  _getSoundRoomMaterial() {
    return {
      size: [15, 2, 3],
      wallMaterial: this._getSoundRoom(),
      floorMaterial: this._getSoundRoom(),
      ceilingMaterial: this._getSoundRoom(),
    }
  },

  _getSpatialControls() {
    return(
        <ViroNode position={[0,0,-4]}>
         <ViroText style={styles.centeredText}  position={[-2, -1.25, 0]} width={1.5} height={2}
                text={"Toggle Atenuation distance: " + this.state.minDistance + " - " + this.state.maxDistance}
                onClick={this._toggleAttenuationDistance}/>

         <ViroText style={styles.centeredText}  position={[0, -2, 0]} width={1.5} height={2}
                text={"Toggle Sound Z: " + this.state.soundDistance}
                onClick={this._toggleSoundDistance}/>

        <ViroText style={styles.centeredText} position={[0, -2.75, 0]} width={1.5} height={2}
               text={"Toggle Camera Z: " + this.state.cameraPos}
               onClick={this._toggleCameraPosition}/>
        </ViroNode>
        );
  },

  _getSoundFieldControls() {
    var room = this._getSoundRoom();
    return(
        <ViroNode position={[0,0,-4]}>
         <ViroText style={styles.centeredText} position={[0, -3, 0]} width={1.5} height ={2}
                text={"Toggle Room: " + room}  onClick={this._toggleRoomMaterial}/>
        </ViroNode>
        );
  },

    _toggleSoundDistance() {
        var newsoundDistance = this.state.soundDistance - 0.2;

        if (newsoundDistance < -13){
            newsoundDistance = -7;
        }

        this.setState({
                  soundDistance:newsoundDistance
             });

      },

      _toggleCameraPosition() {
          var newCameraDistance = this.state.cameraPos + 1;
          if (newCameraDistance > 3){
              newCameraDistance = -3;
          }

          this.setState({
              cameraPos:newCameraDistance,
          });
        },

    _toggleAttenuationDistance() {
        var newMax = this.state.maxDistance + 1;
        var newMin = this.state.minDistance + 1;

        if (newMax > 16){
            newMax = 10;
        }

        if (newMin > 13){
            newMin = 7;
        }

        this.setState({
            maxDistance:newMax,
            minDistance:newMin
        });
      },

  _togglePlay() {
    this.setState({
          isPlaying:!this.state.isPlaying
     });
  },
  _toggleLoop() {
    this.setState({
          looping:!this.state.looping
     });
  },
  _toggleMute() {
    this.setState({
          mute:!this.state.mute
     });
  },

  _toggleVolume() {
      this.setState({
        volume: Math.random(),
      });
  },
  _toggleSource() {
    var newToggleSource = this.state.toggleSource + 1;
    if (newToggleSource > 2){
        newToggleSource = 0;
    }

    this.setState({
          toggleSource:newToggleSource
     });
  },
  _preloadSound() {
    if (this.state.preloadedSound) {
      ViroSound.unloadSounds([
        "cube_sound",
      ]);
      this.setState({
        preloadedSound : false,
      })
    } else {
      ViroSound.preloadSounds({
        "cube_sound" : "https://s3-us-west-2.amazonaws.com/viro/cube_sound.wav",
      });
      this.setState({
        preloadedSound : true,
      })
    }
  },
  _unloadSound(){
        ViroSound.unloadSounds([
         "cube_sound",
       ]);
  },

  _toggleRoomMaterial(){
    var newRoomMateralFlag = (this.state.soundRoom + 1) % 22;
    this.setState({
        soundRoom:newRoomMateralFlag
    });
  },

  _testNormalSound() {
    if (this.state.soundType == 1) {
      return(
        <ViroSound
            paused={!this.state.isPlaying}
            muted={this.state.mute}
            source={this._getSource()}
            loop={this.state.looping}
            volume={this.state.volume}
            onFinish={this.onFinishSound}/>
      );
    }
  },

  _testSpatialSound() {
    if (this.state.soundType == 2) {
        return(
               <ViroNode>
                    {this._getSpatialControls()}
                    <ViroSpatialSound
                        rolloffModel="logarithmic"
                        paused={!this.state.isPlaying}
                        muted={this.state.mute}
                        minDistance={this.state.minDistance}
                        maxDistance={this.state.maxDistance}
                        position={[0,0,this.state.soundDistance]}
                        source={this._getSource()}
                        loop={this.state.looping}
                        volume={this.state.volume}
                        onFinish={this.onFinishSpatial}/>
                </ViroNode>
        );
    }
  },

  _testSoundField() {
    if (this.state.soundType == 3) {
        return(
          <ViroNode>
            {this._getSoundFieldControls()}
            <ViroSoundField
                paused={!this.state.isPlaying}
                muted={this.state.mute}
                source={this._getSource()}
                loop={this.state.looping}
                volume={this.state.volume}
                onFinish={this.onFinishSound}/>
          </ViroNode>
        );
    }
  },

  _getSurface(component) {
    if (this.state.showSurface) {
      return (<ViroQuad style={{flex:1}} materials={"redColor"} />)
    }
  },

  _getAdditionalSounds(component) {
    if (this.state.state == 1 || true) {
      return;
    } else {
      return (<ViroSpatialSound source={{uri : "http://www.jetcityorange.com/musical-notes/G4-392.0.mp3"}} loop={true} position={this.state.position} />);
    }
  },

  _getSource(component) {
      // Ambsonic sound field
      if (this.state.soundType == 3) {
        return require("./res/thelin.wav");
      }
      else {
        var stringSource = require("../res/metronome.mp3");
        if (this.state.toggleSource == 1) {
          stringSource = {uri : "https://freesound.org/data/previews/176/176177_670687-lq.mp3"}
        } else if (this.state.toggleSource == 2) {
          stringSource = "cube_sound";
        }
        return stringSource;
      }
  },

  onFinishSound() {
    console.log("ViroSoundTest OnFinished playing normal sound");
  },


  onFinishSpatial() {
    console.log("ViroSoundTest OnFinished playing Spatial sound");
  },
  /*
  <Viro360Image source={{uri: "http://cdn3-www.dogtime.com/assets/uploads/gallery/pembroke-welsh-corgi-dog-breed-pictures/prance-8.jpg"}} rotation={[-30,90,0]} />
  */
});


var styles = StyleSheet.create({
  centeredText: {
      fontFamily: 'Arial',
      fontSize: 25,
      color: '#ffffff',
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

ViroMaterials.createMaterials({
  redColor: {
    diffuseColor: "#ff0000"
  },
  greenColor: {
    diffuseColor: "#00ff00"
  },
});

ViroAnimations.registerAnimations({
  testLoopRotate:{properties:{rotateY:"+45"}, duration:500},
});
/*
ViroSound.preloadSounds({
  "metronome" : "http://www.kozco.com/tech/32.mp3",
  "cube_sound" : "https://s3-us-west-2.amazonaws.com/viro/cube_sound.wav",
  "g_note" : "http://www.jetcityorange.com/musical-notes/G4-392.0.mp3",
});
*/
module.exports = ViroSoundTest;
