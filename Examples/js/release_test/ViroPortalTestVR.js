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
   ViroPortal,
   ViroPortalFrame,
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

   render: function() {
      return (
        <ViroScene>
             <ViroAnimatedComponent
                            animation="sequentialAnimPortal"
                            run={true}
                            loop={true} >
                <ViroCamera position={[0,0,0]} active={true} />
             </ViroAnimatedComponent>

            <ViroOmniLight position={[0, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
            <ViroPortal position={[0, 0, 1]} passable={false} scale={[1, 1, 1]}>
               <ViroPortalFrame>
                 <Viro3DObject source={require('./res/portal_ring.obj')}
                               position={[0, 0, 0]}
                               rotation={[0, 0, 0]}
                               scale={[0.04, 0.18, 0.08]}
                               materials={["ring"]} />
               </ViroPortalFrame>

               <ViroSkyBox color="#66b7f9" />

               <ViroAnimatedComponent animation="boxSpinPortal" run={true} loop={true}>
                 <ViroBox width={0.25} height={0.25} position={[0, 0, -2]}/>
               </ViroAnimatedComponent>
            </ViroPortal>


            {
              // Stacked Portals
            }
            <ViroPortal passable={true} position={[0, 0, -portalDistance + offsetTorwardsCamera]} scale={[1, 1, 1]}>
               <ViroPortalFrame>
                 <Viro3DObject source={require('./res/portal_ring.obj')}
                               position={[0, 0, 0]}
                               rotation={[0, 0, 0]}
                               scale={[0.04, 0.18, 0.08]}
                               materials={["ring"]}  />
               </ViroPortalFrame>
               <Viro360Image source={require('./res/360_park.jpg')} />

      {         /*<Viro360Video
                    source={Uri360Video}
                    loop={true} />
                  */}

                    <Viro3DObject source={require('./res/male02_obj.obj')}
                                  resources={[require('./res/male02.mtl'),
                                              require('./res/01_-_Default1noCulling.JPG'),
                                              require('./res/male-02-1noCulling.JPG'),
                                              require('./res/orig_02_-_Defaul1noCulling.JPG')]}
                                  position={[0.5, 0, -0.5]}
                                  scale={[0.01, 0.01, 0.01]}
                     />



               <ViroAnimatedComponent animation="boxSpinPortal" run={true} loop={true}>
                 <ViroBox width={0.25} height={0.25} position={[-1.5, 0, -2]}/>
               </ViroAnimatedComponent>

              <ViroPortal position={[0, 0, (-portalDistance*2) + offsetTorwardsCamera]} scale={[1, 1, 1]} passable={true}>
                 <ViroPortalFrame>
                   <Viro3DObject source={require('./res/portal_ring.obj')}
                                scale={[0.04, 0.18, 0.08]}
                                 materials={["ring"]} />
                 </ViroPortalFrame>

                 <Viro360Image source={require('./res/360_park.jpg')} />

                 <ViroAnimatedComponent animation="boxSpinPortal" run={true} loop={true}>
                   <ViroBox width={0.125} height={0.125} position={[-0.75, 0, -1]} materials={["box1"]}/>
                 </ViroAnimatedComponent>

              </ViroPortal>
            </ViroPortal>

            {
              // Portal behind the stacked portals (you should not see this.)
            }
            <ViroPortal position={[0, 0, (-portalDistance*3) + offsetTorwardsCamera]} passable={true} scale={[1, 1, 1]}>
               <ViroPortalFrame>
                 <Viro3DObject source={require('./res/portal_ring.obj')}
                               position={[0, 0, 0]}
                               rotation={[0, 0, 0]}
                               scale={[0.04, 0.18, 0.08]}
                               materials={["ring"]} />
               </ViroPortalFrame>

               <ViroSkyBox color="#66b7f9" />

               <ViroAnimatedComponent animation="boxSpinPortal" run={true} loop={true}>
                 <ViroBox width={0.25} height={0.25} position={[-1.5, 0, -2]}/>
               </ViroAnimatedComponent>
            </ViroPortal>

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
  forwardPortal:{properties:{positionZ:"-=1.5"}, duration:3000},
  backwardPortal:{properties:{positionZ:"+=1.5"}, duration:3000},
  loopRotatePortal:{properties:{rotateY:"+=180"}, duration:1000},
  moveRightPortal:{properties:{positionX:"+=2",}, duration:3000, delay:0},
  moveLeftPortal:{properties:{positionX:"-=2"}, duration:3000, delay:0},
  leftRightMovePortal:[
      ["moveLeftPortal", "moveRightPortal", "moveRightPortal", "moveLeftPortal"]
  ],
  sequentialAnimPortal:[
        ["forwardPortalWithDelay",
        "leftRightMovePortal",
        "forwardPortal",
        "leftRightMovePortal",
        "loopRotatePortal",
        "backwardPortal",
        "leftRightMovePortal",
         "backwardPortal",
         "leftRightMovePortal",
         "loopRotatePortal"]
  ],
});

module.exports = ViroPortalTest;
