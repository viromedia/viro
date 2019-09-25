'use strict';

import React, { Component } from 'react';

import {StyleSheet} from 'react-native';

import {
  ViroScene,
  ViroText,
  Viro360Image,
  ViroOrbitCamera,
  ViroCamera,
  ViroFlexView,
  ViroMaterials,
  ViroUtils,
  ViroQuad,
  ViroSound,
  ViroSpatialSound,
  ViroAnimations, 
  ViroAnimatedComponent,
  ViroSphere,
  ViroNode,
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var createReactClass = require('create-react-class');

var polarToCartesian = ViroUtils.polarToCartesian;

function getTimeSeconds() {
  return Math.floor(new Date().getTime() / 1000);
}

var numSurfaces = 40;
var selectedViewRadius = 4.9;
var changeThreshold = 7; // how many seconds to give to user to find the selected view 5 seconds + 1 sec delay
var changeThresholdAmount = .4;
var minThreshold = 4;
var lastUpdatedTime = getTimeSeconds();
var shouldMakeImpossible = false; // if this is on then user cant score... except when the system lags ><

function getViewToFind() {
  return Math.floor((Math.random() * numSurfaces));
}

var positions = []
for (var i = 0; i < numSurfaces; i++) {
  var temp = {};
  temp.radius = (Math.random() * 2) + 5; // radius is between 5 and 7
  temp.theta = (Math.random() * 360);
  temp.phi = (Math.random() * 360);
  // Make sure the bottom 90 degrees is free of any boxes
  while ((temp.phi > 225 && temp.phi < 315) || (temp.phi > 60 && temp.phi < 120)) {
    temp.phi = (Math.random() * 360);
  }
  positions.push(temp);
}

/*
Ideas:
- after a successful hit, place sphere around user that fades away to hide reshuffle - done
- after a successful hit, reshuffle/reposition everything?
- after an unsuccessful hit, make the wrong box go away and minus score
- add total timer - at the end, black sphere w/ score (send score to server?)
- add in-game timer, after 5 seconds, move the active panel? - done
- after each successful one, show score around user - done!
- move the active surface to the front (r=4.5?) - done!
- move radius back to 5-7? - done!
- add welcome screen - done
- add highscores to welcome screen?
- after every 5 successful hits - make timer faster
*/

var WhackAMole = createReactClass({
  mixins: [TimerMixin],
  componentDidMount: function() {
    this.setInterval(
      () => {
        if (getTimeSeconds() - lastUpdatedTime >= changeThreshold) {
          console.log("kirby too late! we're changing the viewToFind!");
          this.setState({
            viewToFind : getViewToFind()
          });
          lastUpdatedTime = getTimeSeconds();
        }
      },
      1000
    );
  },
  getInitialState() {
    return {
      viewToFind : getViewToFind(),
      score : 0,
      successPaused : true,
      wrongPaused : true,
      seekingPaused : false,
      readyToFind: true, // this determines whether or not we should allow the user to "click" the viewToFind yet,
                         // it's disabled right now to make the app feel more responsive because there's a delay in the sound onfinish.
      runAnimations: false, // TODO: VIRO-815 this on when we fix the crashing from the local reference table overflow error
      muteSounds: false,
      showPauseScreen: false,
    };
  },
  render: function() {
    return (
      <ViroScene>
        <ViroSound source={require("./res/success_chime_short.mp3")} paused={this.state.successPaused} loop={false} onFinish={this._successOnFinish} muted={this.state.muteSounds} />
        <ViroSound source={require("./res/wrong_buzzer_shorter.mp3")} paused={this.state.wrongPaused} loop={false} onFinish={this._wrongOnFinish} muted={this.state.muteSounds} />
        <ViroNode visible={this.state.showPauseScreen} >
          <ViroSphere materials={"blackColor"} radius={4} facesOutward={false} />
          <ViroText text={("Score: \n" + this.state.score)} height={2}
            position={[0, 0, -3]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
          <ViroText text={("Score: \n" + this.state.score)} height={2}
            position={[0, 0, 3]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
          <ViroText text={("Score: \n" + this.state.score)} height={2}
            position={[-3, 0, 0]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
          <ViroText text={("Score: \n" + this.state.score)} height={2}
            position={[3, 0, 0]} transformBehaviors={"billboard"} style={styles.helloWorldTextStyle} />
        </ViroNode>
        {this._getAllStuff()}
      </ViroScene>
    );
  },
  _getAllStuff() {
    var views = [];
    for (var i = 0; i < numSurfaces; i++) {
      views.push(this._createView(i));
    }
    var soundPosition = polarToCartesian([positions[this.state.viewToFind].radius,
                                          positions[this.state.viewToFind].theta,
                                          positions[this.state.viewToFind].phi]);
    views.push((<ViroSpatialSound key="seekingSound" source={require("./res/seeking_music_shorter.mp3")} position={soundPosition}
                 paused={false} loop={true} muted={this.state.muteSounds} />));
    return views;
  },
  _createView(index) {
    var position = polarToCartesian([index == this.state.viewToFind ? selectedViewRadius : positions[index].radius,
                                     positions[index].theta,
                                     positions[index].phi]);
    var material = index == this.state.viewToFind ? "blueColor" : "redColor";
    var onClick = index == this.state.viewToFind ? this._onClickCorrect : this._onClickWrong;
    var onHover = index == this.state.viewToFind && shouldMakeImpossible ? this._onHoverCorrect : undefined;
    var name = "";
    if (index % 2 == 0) {
      name = ("rotateZ" + String(index));
    } else {
      name = ("rotateNegZ" + String(index));
    }
    return (
      <ViroAnimatedComponent key={index} animation={name} run={this.state.runAnimations} loop={true} >
        <ViroQuad position={position} materials={material}
            transformBehaviors={"billboard"} width={1} height={1} onClick={onClick} onHover={onHover} />
      </ViroAnimatedComponent>
    );
  },
  _onHoverCorrect() {
    console.log("kirby onHoverCorrect")
    lastUpdatedTime = getTimeSeconds();
    this.setState({
      viewToFind : getViewToFind()
    })
  },
  _onClickCorrect() {
    console.log("kirby correct view clicked! readyToFind? " + this.state.readyToFind);

    // make the viewToFind change faster.
    if (changeThreshold > minThreshold) {
      changeThreshold = changeThreshold - changeThresholdAmount;
    }

    if (this.state.readyToFind) {
      lastUpdatedTime = getTimeSeconds();
      this.setState({
        successPaused : false,
        seekingPaused : true,
        readyToFind : true,
        score : this.state.score + 1,
        viewToFind : getViewToFind(),
        showPauseScreen : true,
      });
    }
  },
  _onClickWrong() {
    console.log("kirby onClickWrong!");
    this.setState({
      wrongPaused : false,
      seekingPaused : true,
    });
  },
  _successOnFinish() {
    console.log("kirby Success on finish!");
    this.setState({
      successPaused : true,
      seekingPaused : false,
      readyToFind : true,
      showPauseScreen : false,
    })
  },
  _wrongOnFinish() {
    this.setState({
      wrongPaused : true,
      seekingPaused : false,
    })
  }
});

/*
        <ViroCamera position={[0, 0, 2]} active={true} />
        <ViroOrbitCamera position={[0,0,2]} active={false} focalPoint={[0,0,0]} />
        <Viro360Image source={require('./res/guadalupe_360.jpg')} />

        <ViroFlexView style={{flexDirection: 'column'}}
            materials={["label_flex_view"]} position={[0,0,-2]} width={5} height={5}>
          <ViroText text="Hello World!" style={styles.helloWorldTextStyle} />
        </ViroFlexView>
*/

var styles = StyleSheet.create({
  helloWorldTextStyle: {
    fontFamily: 'Arial',
    fontSize: 50,
    color: '#ffffff',
    flex: 1,
    textAlignVertical: 'center',
    textAlign: 'center',  
  },
});

ViroMaterials.createMaterials({
  label_flex_view: {
    lightingModel: "Constant",
    diffuseTexture: require('./res/textures/text_box.png'),
  },
  redColor: {
    diffuseColor: "#ff0000"
  },
  blueColor: {
    diffuseColor: "#0000ff"
  },
  blackColor: {
    diffuseColor: "#000000",
    writesToDepthBuffer: true,
  },
});

let animations = {}
for (var i = 0; i < numSurfaces; i+=2) {
  var name = ("rotateZ" + String(i))
  animations[name] = {properties:{rotateZ:"+135"}, duration:3000};
}

for (var i = 1; i < numSurfaces; i+=2) {
  var name = ("rotateNegZ" + String(i))
  animations[name] = {properties:{rotateZ:"+-135"}, duration:3000};
}
ViroAnimations.registerAnimations(animations);


module.exports = WhackAMole;

