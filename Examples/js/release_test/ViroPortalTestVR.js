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
   ViroAnimatedComponent,
   ViroSurface,
   ViroSkyBox,
   ViroPortalScene,
   ViroPortal,
   ViroSphere,
   Viro3DObject,
 } from 'react-viro';

let polarToCartesian = ViroUtils.polarToCartesian;
var portalDistance = 1.25;
var offsetTorwardsCamera =0.25;
var Uri360Video = {uri:"https://s3-us-west-2.amazonaws.com/viro/360_surf.mp4"};

var ViroPortalTest = React.createClass({
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
             <ViroAnimatedComponent
                            animation="sequentialAnim"
                            run={true}
                            loop={true} >
                <ViroCamera position={[0,0,0]} active={true} />
             </ViroAnimatedComponent>

            <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
            {
              // Portal P0. You should not be able to enter this portal.
            }
            <ViroPortalScene position={[0, 0, 1]} passable={false} scale={[1, 1, 1]}>
               <ViroPortal>
                 <Viro3DObject source={require('./res/portal_ring.obj')}
                              type="OBJ"
                               position={[0, 0, 0]}
                               rotation={[0, 0, 0]}
                               scale={[0.04, 0.18, 0.08]}
                               materials={["ring"]} type="OBJ" />
               </ViroPortal>

               <ViroSkyBox color="#66b7f9" />

               <ViroAnimatedComponent animation="boxSpin" run={true} loop={true}>
                 <ViroBox width={0.25} height={0.25} position={[0, 0, -2]}/>
               </ViroAnimatedComponent>
            </ViroPortalScene>


            {
              // Recursively stacked Portals [P1  [P2]]
            }
            <ViroPortalScene passable={true} position={[0, 0, -portalDistance + offsetTorwardsCamera]} scale={[1, 1, 1]}>
               <ViroPortal>
                 <Viro3DObject source={require('./res/portal_ring.obj')}
                               type="OBJ"
                               position={[0, 0, 0]}
                               rotation={[0, 0, 0]}
                               scale={[0.04, 0.18, 0.08]}
                               materials={["ring"]} type="OBJ" />
               </ViroPortal>
               <Viro360Image source={require('./res/360_park.jpg')} />

                    {
                      /*<Viro360Video
                        source={Uri360Video}
                        loop={true} />
                        */
                    }

                    <Viro3DObject source={require('./res/male02_obj.obj')}
                                   type="OBJ"
                                  resources={[require('./res/male02.mtl'),
                                              require('./res/01_-_Default1noCulling.JPG'),
                                              require('./res/male-02-1noCulling.JPG'),
                                              require('./res/orig_02_-_Defaul1noCulling.JPG')]}
                                  position={[0.5, 0, -0.5]}
                                  scale={[0.01, 0.01, 0.01]} type="OBJ"
                     />



              <ViroAnimatedComponent animation="boxSpinPortal" run={true} loop={true}>
                 <ViroBox width={0.25} height={0.25} position={[-1.5, 0, -2]}/>
              </ViroAnimatedComponent>

              <ViroPortalScene position={[0, 0, (-portalDistance*2) + offsetTorwardsCamera]} scale={[1, 1, 1]} passable={true}>
                 <ViroPortal>
                   <Viro3DObject source={require('./res/portal_ring.obj')}
                                scale={[0.04, 0.18, 0.08]}
                                 materials={["ring"]} type="OBJ" />
                 </ViroPortal>

                 <Viro360Image source={require('./res/360_park.jpg')} />

                 <ViroAnimatedComponent animation="boxSpin" run={true} loop={true}>
                   <ViroBox width={0.125} height={0.125} position={[-0.75, 0, -1]} materials={["box1"]}/>
                 </ViroAnimatedComponent>

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
                               type="OBJ"
                               scale={[0.04, 0.18, 0.08]}
                               materials={["ring"]} type="OBJ" />
               </ViroPortal>

               <ViroSkyBox color="#f96d66" />

               <ViroAnimatedComponent animation="boxSpin" run={true} loop={true}>
                 <ViroBox width={0.25} height={0.25} position={[-1.5, 0, -2]}/>
               </ViroAnimatedComponent>
            </ViroPortalScene>

        </ViroScene>
      );
   },
 });

var styles = StyleSheet.create({
  baseTextTwo: {
      fontFamily: 'Arial',
      fontSize: 10,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
});

ViroMaterials.createMaterials({
  ring: {
    lightingModel: "Lambert",
    diffuseTexture: require('./res/portal_ring.png'),
  },
  box1: {
    shininess : 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('./res/sun_2302.jpg'),
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
         "leftRightMovePortal",
         "loopRotatePortal",
         "forwardOne"]              // net 0 into z - back where we started.
  ],
});

module.exports = ViroPortalTest;
