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

import React, { Component } from 'react';
import {
  StyleSheet,
  ViroScene,
  ViroOmniLight,
  ViroImage,
  Viro360Photo,
  ViroMaterials,
  ViroAnimations,
  ViroAnimatedComponent,

} from 'react-viro';

var MAINCARD_REF = 'maincard';
var MAINCARDTWO_REF = 'maincardtwo';
var PETITECARD_REF = 'petitecardonecard';
var PETITECARDTWO_REF = 'petitecardtwocard';

var AnimationTest = React.createClass({
  getInitialState() {
    return {
       showMainCard: true,
    };
  },
  render: function() {
    var mainAnimation = 'card' + (this.state.showMainCard ? 'In' : 'Out');
    return (
     <ViroScene >

        <Viro360Photo source={require('./res/360_diving.jpg')}  />
        <ViroAnimatedComponent animation={mainAnimation} run={false} ref={MAINCARD_REF} >
            <ViroImage materials={["cardmain"]} position={[0, -.5, -2]} scale={[.1, .1, .1]} onTap={this._onTapShowMain} />
        </ViroAnimatedComponent>

        <ViroAnimatedComponent animation={mainAnimation} run={false} ref={MAINCARDTWO_REF}  >
            <ViroImage materials={["cardpetite"]} position={[0, -.2, -2]} scale={[.1, .1, .1]} onTap={this._onTapShowMain}  />
        </ViroAnimatedComponent>

        <ViroAnimatedComponent animation={mainAnimation} run={false} ref={PETITECARD_REF}  >
            <ViroImage materials={["cardmain"]} position={[0, 0, -2]} scale={[.1, .1, .1]} onTap={this._onTapShowMain} />
        </ViroAnimatedComponent>

        <ViroAnimatedComponent animation={mainAnimation} run={false} ref={PETITECARDTWO_REF}  >
            <ViroImage materials={["cardpetite"]} position={[0, .2, -2]} scale={[.1, .1, .1]} onTap={this._onTapShowMain} />
        </ViroAnimatedComponent>

        <ViroAnimatedComponent animation="testLoopRotate" run={true} loop={true} >
            <ViroImage materials={["poidot"]} position={[1, .4, -2]} />
        </ViroAnimatedComponent>

         <ViroAnimatedComponent animation="testPositionMove" run={true} loop={false} >
             <ViroImage materials={["poidot"]} position={[-1, 0, -8]}  opacity={0.0} transformBehaviors={["billboard"]}/>
         </ViroAnimatedComponent>

        <ViroAnimatedComponent animation="testLoopColor" run={true}  >
            <ViroImage  position={[1, 0, -4]}  transformBehaviors={["billboard"]}  />
        </ViroAnimatedComponent>

         <ViroAnimatedComponent animation="rotateAndMovePicture" run={true} loop={true} onFinish={this._onAnimationFinished}>
             <ViroImage materials={["cardpetite"]} position={[1, -1, -4]} scale={[.5, .5, .5]} />
         </ViroAnimatedComponent>
     </ViroScene>
    );
  },
  _onTapShowMain() {
    this.setState({
      showMainCard: !this.state.showMainCard,
    }, function startCardAnimations() {
    this.refs[MAINCARDTWO_REF].startAnimation();
    this.refs[MAINCARD_REF].startAnimation();
    this.refs[PETITECARD_REF].startAnimation();
    this.refs[PETITECARDTWO_REF].startAnimation();
  });
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
    diffuseTexture: require('./res/card_main.png'),
  },
});

ViroAnimations.registerAnimations({
    cardIn:{properties:{scaleX:1, scaleY:.6, scaleZ:1, opacity: 1}, easing:"Bounce", duration: 5000},

    testLoopRotate:{properties:{rotateY:"+45"}, duration:1000},
    testLoopColor:{properties:{color:"#ffff00"}, duration:10000},

    cardOutScale:{properties:{opacity: 0.0}, easing:"EaseOut", delay:2000, duration: 5000},
    cardOutOpacity:{properties:{scaleX:.1, scaleY:.1, scaleZ:.1}, easing:"EaseOut", delay:1000, duration: 2500},
    cardout:[
        ["cardOutScale"],
        ["cardOutOpacity"]
    ],

    testPositionMoveEaseInPosition:{properties:{positionZ:"+6", opacity: 1.0}, easing:"EaseIn", duration: 6000},
    testPositionMoveBounceScale:{properties:{scaleX:2, scaleY:2}, easing:"Bounce", delay:6000, duration: 4000},
    testPositionMove:[
        ["testPositionMoveEaseInPosition"],
        ["testPositionMoveBounceScale"]
    ],

    moveRight:{properties:{positionX:"+0.3"}, duration: 10000},
    moveLeft:{properties:{positionX:"-0.3", rotateZ:"+45"}, duration: 10000},
    rotate:{properties:{rotateZ:"+45"}, duration:1000},
    rotateAndMovePicture:[
        ["moveRight", "moveLeft"],
        ["rotate"]
    ],
});

module.exports = AnimationTest;
