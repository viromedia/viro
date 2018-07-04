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
   ViroPortalScene,
   ViroPortal,
   ViroSphere,
   Viro3DObject,
 } from 'react-viro';

var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;
var portalDistance = 1.25;
var offsetTorwardsCamera =0.25;
var Uri360Video = {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"};
var ReleaseMenu = require("./ReleaseMenu.js");

var ViroPortalTest = createReactClass({
   getInitialState() {
     return {

     };
   },

   /*
   Portals are placed in the following order:
   -------> Dirction in -Z axis
   [P0]  [Camera -> starting facing / going this way]   [P1 [P2]] [P3]
   */

   render: function() {
      return (
        <ViroScene>

                <ViroCamera position={[0,0,0]} active={true} animation={{name:"sequentialAnimPortal",
                            loop:true,
                            run:true }} />


             <ReleaseMenu position={[-0.5 , 0, -0.5]} sceneNavigator={this.props.sceneNavigator}/>

            <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
            <ViroText fontSize={35}  style={styles.baseTextTwo} color="#ffffff"
              position={[0, 0, 3.5]} width={7} height ={3} maxLines={3} transformBehaviors={["billboard"]}
              text={"You should see this text behind the unpassable portal." }
             />


            {
              // Portal P0. You should not be able to enter this portal.
            }
            <ViroPortalScene position={[0, 0, 1]} passable={false} scale={[1, 1, 1]}  onPortalEnter={this._onPortalEnter} onPortalExit={this._onPortalExit}>
               <ViroPortal>
                 <Viro3DObject source={require('./res/portal_ring.obj')}
                               position={[0, 0, 0]}
                               rotation={[0, 0, 0]}
                               scale={[0.04, 0.18, 0.08]}
                               materials={["ring"]} type="OBJ" />
               </ViroPortal>
               <ViroSkyBox color="#ff0000" />

               <ReleaseMenu position={[-1.0 , 0, -0.5]} sceneNavigator={this.props.sceneNavigator}/>
                <ViroBox width={0.25} height={0.25} position={[0, 0, -2]} animation={{name:"boxSpinPortal", loop:true, run:true}}/>

            </ViroPortalScene>


            {
              // Recursively stacked Portals [P1  [P2]]
            }
            <ViroPortalScene passable={true} position={[0, 0, -portalDistance + offsetTorwardsCamera]} scale={[1, 1, 1]}  onPortalEnter={this._onPortalEnter} onPortalExit={this._onPortalExit}>
               <ViroPortal>
                 <Viro3DObject source={require('./res/portal_ring.obj')}
                               position={[0, 0, 0]}
                               rotation={[0, 0, 0]}
                               scale={[0.04, 0.18, 0.08]}
                               materials={["ring"]} type="OBJ" />
               </ViroPortal>
               <Viro360Image source={require('./res/360_waikiki.jpg')} />
                     <Viro3DObject source={require('./res/xwing.obj')}
                                   resources={[require("./res/star-wars-x-wing.mtl")]}
                                   position={[2.5, 0.5, -1.5]}
                                   materials={["grey"]}
                                   rotation={[0,45,-45]}
                                   scale={[0.2, 0.2, 0.2]} type="OBJ"
                     />
                     <ReleaseMenu position={[-1.0 , 0, -2]} sceneNavigator={this.props.sceneNavigator}/>

                <ViroBox width={0.25} height={0.25} position={[-1.5, 0, -2]} animation={{name:"boxSpinPortal", loop:true, run:true}}/>

              <ViroPortalScene position={[0, 0, (-portalDistance*2) + offsetTorwardsCamera]} scale={[1, 1, 1]} passable={true}>
                 <ViroPortal>
                   <Viro3DObject source={require('./res/portal_ring.obj')}
                                scale={[0.04, 0.18, 0.08]}
                                 materials={["ring"]} type="OBJ" />
                 </ViroPortal>
                 <ReleaseMenu position={[-1.0 , 0, -2]} sceneNavigator={this.props.sceneNavigator}/>
                 <Viro360Image source={require('./res/360_park.jpg')} />
                   <ViroBox width={0.125} height={0.125} position={[-0.75, 0, -1]} materials={["box1"]}
                    animation={{name:"boxSpinPortal", loop:true, run:true}}/>

              </ViroPortalScene>
            </ViroPortalScene>

            {
              // Portal behind the recursively stacked portals in the root level (you should not see or enter this.)
              // This should be portal [P3]
            }
            <ViroPortalScene position={[0, 0, (-portalDistance*3) + offsetTorwardsCamera]} passable={true} scale={[1, 1, 1]}>
               <ViroPortal>
                 <Viro3DObject source={require('./res/portal_ring.obj')}
                               position={[0, 0, 0]}
                               rotation={[0, 0, 0]}
                               scale={[0.04, 0.18, 0.08]}
                               materials={["ring"]} type="OBJ" />
               </ViroPortal>

               <ViroSkyBox color="#f96d66" />

                 <ViroBox width={0.25} height={0.25} position={[-1.5, 0, -2]} animation={{name:"boxSpinPortal", loop:true, run:true}}/>
            </ViroPortalScene>

        </ViroScene>
      );
   },


  _onPortalEnter() {
    console.log("Viro On Portal Enter invoked");
  },

  _onPortalExit() {
    console.log("Viro On Portal Exit invoked");
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
});

ViroMaterials.createMaterials({
  ring: {
    lightingModel: "Lambert",
    diffuseTexture: require('./res/portal_ring_d.png'),
  },
  box1: {
    shininess : 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('./res/sun_2302.jpg'),
  },
  grey: {
    shininess : 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('./res/grey.jpg'),
  },
});


ViroAnimations.registerAnimations({
  forwardPortalWithDelay:{properties:{positionZ:"-=1.5"}, duration:3000, delay:3000},
  boxSpinPortal:{properties:{rotateY:"+=180"}, duration:1000},
  forwardPortal:{properties:{positionZ:"-=2.5"}, duration:3000},
  backwardPortal:{properties:{positionZ:"+=2.5"}, duration:3000},
  loopRotatePortal:{properties:{rotateY:"+=180"}, duration:1000},
  moveRightPortal:{properties:{positionX:"+=2",}, duration:3000, delay:0},
  moveLeftPortal:{properties:{positionX:"-=2"}, duration:3000, delay:0},
  forwardOne:{properties:{positionZ:"-=1",}, duration:1000, delay:0},
  leftRightMovePortal:[
      ["moveLeftPortal", "moveRightPortal", "moveRightPortal", "moveLeftPortal"]
  ],
  sequentialAnimPortal:[
        ["forwardPortalWithDelay",  // -1.5 into z
        "leftRightMovePortal",
        "forwardPortal",            // net -4 into z
        "leftRightMovePortal",
        "loopRotatePortal",
        "backwardPortal",           // net -1.5 into z
        "leftRightMovePortal",
         "backwardPortal",          // net +1 into z
        "backwardPortal",           // net +3.5 into z
         "loopRotatePortal",
         "forwardPortal",
         "forwardOne"]              // net 0 into z - back where we started.
  ],
});

module.exports = ViroPortalTest;
