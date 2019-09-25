/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 */
 'use strict';

 import React, { Component } from 'react';
 import {
   AppRegistry,
   StyleSheet,
   Text,
   View
 } from 'react-native';

 import {
   ViroSceneNavigator,
   ViroScene,
   ViroARScene,
   ViroBox,
   ViroMaterials,
   ViroNode,
   ViroOrbitCamera,
   ViroCamera,
   ViroAmbientLight,
   ViroOmniLight,
   ViroSpotLight,
   ViroDirectionalLight,
   ViroImage,
   ViroVideo,
   Viro360Image,
   Viro360Video,
   ViroFlexView,
   ViroUtils,
   ViroText,
   ViroAnimations,
   ViroQuad,
   ViroSkyBox,
   ViroPortal,
   ViroPortalScene,
   ViroSphere,
   Viro3DObject,
 } from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;
var portalDistance = 1.25;
var offsetTorwardsCamera =0.25;
var Uri360Video = {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"};

var ViroPortalTest = createReactClass({
   getInitialState() {
     return {

     };
   },

   render: function() {
      return (
        <ViroARScene>
            <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
            <ViroText fontSize={35}  style={styles.baseTextTwo} color="#ffffff"
              position={[0, 0, 3.5]} width={7} height ={3} maxLines={3} transformBehaviors={["billboard"]}
              text={"You should see this text behind the unpassable portal." }
             />

            <ViroPortalScene position={[0, 0, 1]} passable={false} scale={[1, 1, 1]}>
               <ViroPortal >
                 <Viro3DObject source={require('../res/portal_ring.obj')}
                               position={[0, 0, 0]}
                               rotation={[0, 0, 0]}
                               scale={[0.04, 0.18, 0.08]}
                               materials={["ring"]}  type="OBJ" />
               </ViroPortal>

               <ViroSkyBox color="#66b7f9" />
                 <ViroBox width={0.25} height={0.25} position={[0, 0, -2]}
                          animation={{name:"boxSpin", loop:true, run:true}}
                  />
            </ViroPortalScene>

            <ViroText position={[-2, 0, 0]} text={"Release Menu"}
                     style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
                     transformBehaviors={["billboard"]}/>

            {
              // Stacked Portals
            }
            <ViroPortalScene passable={true} position={[0, 0, -portalDistance + offsetTorwardsCamera]} scale={[1, 1, 1]}  onPortalEnter={this._onPortalEnter} onPortalExit={this._onPortalExit}>
               <ViroPortal>
                 <Viro3DObject source={require('../res/portal_ring.obj')}
                               position={[0, 0, 0]}
                               rotation={[0, 0, 0]}
                               scale={[0.04, 0.18, 0.08]}
                               materials={["ring"]}  type="OBJ"/>
               </ViroPortal>

               <Viro360Video
                    source={Uri360Video}
                    loop={true} />

              <Viro3DObject source={require('../../release_test/res/earth_obj.obj')}
                  resources={[require('../../release_test/res/earth_jpg.jpg'),
                              require('../../release_test/res/earth_normal.jpg')]}
                  position={[0.5, 0.0, -0.5]}
                  scale={[0.01, 0.01, 0.01]}
                  materials={["earth"]}
                  type="OBJ" />

              <ViroText position={[-2, 0.5, -2]} text={"Release Menu"}
                      style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
                      transformBehaviors={["billboard"]}/>
                 <ViroBox width={0.25} height={0.25} position={[-1.5, 0, -2]}
                    animation={{name:"boxSpin", loop:true, run:true}} />

              <ViroPortalScene position={[0, 0, -portalDistance*2 + offsetTorwardsCamera]} scale={[1, 1, 1]} passable={true}>
                 <ViroPortal>
                   <Viro3DObject source={require('../res/portal_ring.obj')}
                                scale={[0.04, 0.18, 0.08]}
                                 materials={["ring"]}  type="OBJ" />
                 </ViroPortal>

                 <Viro360Image source={require('../res/360_park.jpg')} />

                 <ViroText position={[-2, 0.5, -2]} text={"Release Menu"}
                          style={styles.instructionText} onClick={()=>{this.props.arSceneNavigator.replace("ARReleaseMenu", {scene: require("./ARReleaseMenu")})}}
                          transformBehaviors={["billboard"]}/>

                  <ViroBox width={0.125} height={0.125} position={[-0.75, 0, -1]} materials={["box1"]}
                      animation={{name:"boxSpin", loop:true, run:true}} />

              </ViroPortalScene>
            </ViroPortalScene>

            {
              // Portal behind the stacked portals (you should not see this.)
            }
            <ViroPortalScene position={[0, 0, -portalDistance*3 + offsetTorwardsCamera]} passable={true} scale={[1, 1, 1]}>
               <ViroPortal>
                 <Viro3DObject source={require('../res/portal_ring.obj')}
                               position={[0, 0, 0]}
                               rotation={[0, 0, 0]}
                               scale={[0.18, 0.18, 0.08]}
                               materials={["ring"]}  type="OBJ" />
               </ViroPortal>

               <ViroSkyBox color="#FF0000" />

                 <ViroBox width={0.25} height={0.25} position={[-1.5, 0, -2]}
                      animation={{name:"boxSpin", loop:true, run:true}}/>
            </ViroPortalScene>

        </ViroARScene>
      );
   },
   /*
   Replace the Earth OBJ with the following once we fix the perf issue:

               <Viro3DObject source={require('./res/xwing.obj')}
                             resources={[require("./res/star-wars-x-wing.mtl")]}
                             position={[0.5, 0, -0.5]}
                             materials={["grey"]}
                             rotation={[0,45,-45]}
                             scale={[0.2, 0.2, 0.2]} type="OBJ"
               />
    */

  _onPortalEnter() {
    console.log("On Portal Enter invoked");
  },

  _onPortalExit() {
    console.log("On Portal Exit invoked");
  },
 });


var styles = StyleSheet.create({
  baseTextTwo: {
      fontFamily: 'Arial',
      fontSize: 30,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
  instructionText: {
      fontFamily: 'Arial',
      fontSize: 30,
      color: '#FFFFFF',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

ViroMaterials.createMaterials({
  ring: {
    lightingModel: "Lambert",
    diffuseTexture: require('../res/portal_ring_d.png'),
  },
  box1: {
    shininess : 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('../res/sun_2302.jpg'),
  },
  grey: {
    shininess : 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('./res/grey.jpg'),
  },
  earth: {
    lightingModel: "Blinn",
    diffuseTexture: require('../../release_test/res/earth_jpg.jpg'),
    normalTexture: require('../../release_test/res/earth_normal.jpg'),
  }
});


ViroAnimations.registerAnimations({
  boxSpin:{properties:{rotateY:"+=180"}, duration:1000},
  forward:{properties:{positionZ:"-="+portalDistance,}, duration:3000, delay:1000},
  backward:{properties:{positionZ:"+="+portalDistance,}, duration:3000},
  loopRotate:{properties:{rotateY:"+=180"}, duration:1000},
  moveRight:{properties:{positionX:"+="+portalDistance,}, duration:3000, delay:0},
  moveLeft:{properties:{positionX:"-="+portalDistance,}, duration:3000, delay:0},
  leftRightMove:[
      ["moveLeft", "moveRight", "moveRight", "moveLeft"]
  ],
  sequentialAnim:[
        ["forward","forward","leftRightMove", "forward","leftRightMove", "forward", "loopRotate", "backward","leftRightMove", "backward","leftRightMove", "backward", "loopRotate"]
  ],
});

module.exports = ViroPortalTest;
