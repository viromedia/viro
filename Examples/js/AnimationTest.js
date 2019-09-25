/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

 /**
This class tests the following:
 - Looping animation using poidot Material
 - Animation color, animate color to yellow for ViroImage
 - Opacity animation, animate opacity to 0 when onTap event invoked
 - run flag, animate position on scene mount

 Scale, position and rotation are tested in the above animations. This test only tests AnimatedComponent
 with ViroImage elements. Another test will contain more animation tests with different components.
 */
'use strict';

import React, { Platform } from 'react';
import {
  StyleSheet,
  ViroScene,
  ViroOmniLight,
  ViroImage,
  Viro360Image,
  ViroMaterials,
  ViroAnimations,
  ViroAnimatedComponent,

} from 'react-viro';

var createReactClass = require('create-react-class');

var MAINCARD_REF = 'maincard';
var MAINCARDTWO_REF = 'maincardtwo';
var PETITECARD_REF = 'petitecardonecard';
var PETITECARDTWO_REF = 'petitecardtwocard';

var AnimationTest = createReactClass({
  getInitialState() {
    return {
       mainAnimationIn: false,
       mainAnimation: 'cardOut',
       runAnimation: false,
    };
  },
  render: function() {
    return (
     <ViroScene >

        <Viro360Image source={require('./res/360_diving.jpg')}  />
        <ViroAnimatedComponent animation={this.state.mainAnimation} run={false} ref={MAINCARD_REF} >
            <ViroImage materials={["cardmain"]} position={[0, -.5, -2]} scale={[.1, .1, .1]} onTap={this._onTapShowMain} />
        </ViroAnimatedComponent>

        <ViroAnimatedComponent animation={this.state.mainAnimation} run={this.state.runAnimation} ref={MAINCARDTWO_REF} loop={false} >
            <ViroImage source={require('./res/card_petite_ansu.png')} position={[0, -.2, -2]} scale={[.1, .1, .1]} onTap={this._onTapShowMain}  />
        </ViroAnimatedComponent>

        <ViroAnimatedComponent animation={this.state.mainAnimation} run={this.state.runAnimation} ref={PETITECARD_REF} loop={false}  >
            <ViroImage source={require('./res/card_main.png')} position={[0, 0, -2]} scale={[.1, .1, .1]} onTap={this._onTapShowMain} />
        </ViroAnimatedComponent>

        <ViroAnimatedComponent animation={this.state.mainAnimation} run={this.state.runAnimation} ref={PETITECARDTWO_REF} loop={false} >
            <ViroImage source={require('./res/card_petite_ansu.png')} position={[0, .2, -2]} scale={[.1, .1, .1]} onTap={this._onTapShowMain} />
        </ViroAnimatedComponent>

        <ViroAnimatedComponent animation="testLoopRotate" run={true} loop={true} onFinish={this._onAnimationFinished} onStart={this._onAnimationStarted} >
            <ViroImage source={require('./res/poi_dot.png')} position={[1, .4, -2]} />
        </ViroAnimatedComponent>

         <ViroAnimatedComponent animation="testPositionMove" run={true} loop={false} >
             <ViroImage source={require('./res/poi_dot.png')} position={[-1, 0, -8]}  opacity={0.0} transformBehaviors={["billboard"]}/>
         </ViroAnimatedComponent>

        <ViroAnimatedComponent animation="testLoopColor" run={true}  >
            <ViroImage source={require('./res/card_main.png')} position={[1, 0, -4]}  transformBehaviors={["billboard"]} onTap={this._onTapShowMain} />
        </ViroAnimatedComponent>

         <ViroAnimatedComponent animation="rotateAndMovePicture" run={true} loop={true} >
             <ViroImage source={require('./res/card_petite_ansu.png')} position={[1, -1, -4]} scale={[.5, .5, .5]} />
         </ViroAnimatedComponent>
     </ViroScene>
    );
  },
  _onTapShowMain() {
    this.setState({
      mainAnimation: 'card' + (this.state.mainAnimationIn ? 'In' : 'Out'),
      mainAnimationIn: !this.state.mainAnimationIn,
      runAnimation: true,
    });
  },
  _onAnimationStarted() {
      console.log("AnimationTest on Animation Started");
  },
  _onAnimationFinished(){
      console.log("AnimationTest on Animation Finished!");
  },
});


var materials = ViroMaterials.createMaterials({
  poidot: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseTexture: require('./res/poi_dot.png'),
  },
  cardpetite: {
    shininess : 1.0,
    lightingModel: "Constant",
    diffuseTexture: require('./res/card_petite_ansu.png'),
  },
  cardmain: {
    shininess : 1.0,
    lightingModel: "Constant",
    diffuseTexture: {source:require('./res/card_main.png', mipmap:true},
  },
});

ViroAnimations.registerAnimations({
    cardIn:{properties:{scaleX:1, scaleY:.6, scaleZ:1, opacity: 1}, easing:"Bounce", duration: 5000},

    testLoopRotate:{properties:{rotateY:"+=45"}, duration:1000},
    testLoopColorYellow:{properties:{color:"#ffff00"}, duration:10000},
    testLoopColorBlue:{properties:{color:"#ffff00"}, duration:10000},
    testLoopColor:[
      ["testLoopColorYellow", "testLoopColorBlue"]
    ],


    cardOutScale:{properties:{opacity: 0.0}, easing:"EaseOut", delay:2000, duration: 5000},
    cardOutOpacity:{properties:{scaleX:.1, scaleY:.1, scaleZ:.1}, easing:"EaseOut", delay:1000, duration: 2500},
    cardOut:[
        ["cardOutScale"],
        ["cardOutOpacity"]
    ],

    testPositionMoveEaseInPosition:{properties:{positionZ:"+=6", opacity: 1.0}, easing:"EaseIn", duration: 6000},
    testPositionMoveBounceScale:{properties:{scaleX:2, scaleY:2}, easing:"Bounce", delay:6000, duration: 4000},
    testPositionMove:[
        ["testPositionMoveEaseInPosition"],
        ["testPositionMoveBounceScale"]
    ],

    moveRight:{properties:{positionX:"+=0.3"}, duration: 10000},
    moveLeft:{properties:{positionX:"-=0.3", rotateZ:"+45"}, duration: 10000},
    rotate:{properties:{rotateZ:"+=45"}, duration:1000},
    rotateAndMovePicture:[
        ["moveRight", "moveLeft"],
        ["rotate"]
    ],
});

module.exports = AnimationTest;
