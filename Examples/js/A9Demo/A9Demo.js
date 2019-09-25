'use strict';

import React, { Component } from 'react';

import {StyleSheet} from 'react-native';
import TimerMixin from 'react-timer-mixin';

import {
  ViroScene,
  ViroImage,
  ViroText,
  Viro360Image,
  ViroFlexView,
  ViroVideo,
  Viro3DObject,
  ViroMaterials,
  ViroCamera,
  ViroOrbitCamera,
  ViroNode,
  ViroAnimatedComponent,
  ViroDirectionalLight,
  ViroAnimations,
  ViroBox,
} from 'react-viro';

var createReactClass = require('create-react-class');

var A9Demo = createReactClass({
  mixins: [TimerMixin],
  getInitialState() {
    return {
        inStandMode: false,
        runAnimation: false,
        runTVAnimation: false,
        mainAnimation: "fadeOut",
        tvAnimation: "animateToStand",
        videoPaused: false,
        show3dModel: false,
        loopTV: false,
    };
  },
  componentDidMount: function() {
    this.setTimeout(
      () => { this.setState({
          show3dModel: true,
        });
      },
      2000
    );
  },
  render: function() {
    return (
     <ViroScene>

       <Viro360Image source={require('./res/a9_3d_assets/a9_interior_b.jpg')} />
      <ViroAnimatedComponent animation={this.state.mainAnimation} run={this.state.runAnimation} loop={false} >
       <ViroFlexView style={styles.titleContainer} position={[-3.8, 1, -7]} rotation={[0, 40, 0]} height={2} width={4}>
          <ViroText style={styles.prodTitleText} text="Samsung UN88JS9500" width={4} height={.5}/>
          <ViroText style={styles.prodDescriptionText} text="Samsung UN88JS9500 Curved 50-Inch 4K Ultra HD Smart LED" />
        </ViroFlexView>
      </ViroAnimatedComponent>

      <ViroAnimatedComponent animation={this.state.mainAnimation} run={this.state.runAnimation} loop={false} >
        <ViroFlexView style={styles.titleContainer} position={[-3.8, -1.1, -7]} rotation={[0, 40, 0]} height={2} width={4}>
          <ViroFlexView style={styles.rowContainer}>
            <ViroText style={styles.prodDescriptionText} text="Refresh Rate: 240CMR(Effective)" />
          </ViroFlexView>
          <ViroFlexView style={styles.rowContainer}>
            <ViroText style={styles.prodDescriptionText} text="Backlight: LED" />
          </ViroFlexView>
          <ViroFlexView style={styles.rowContainer}>
            <ViroText style={styles.prodDescriptionText} text="Dimensions(W x H x D): TV without stand: 44.9 x 24.5 x 2.4" />
          </ViroFlexView>
        </ViroFlexView>
      </ViroAnimatedComponent>
       <ViroDirectionalLight color="#ffffff" direction={[0, 0, -1.0]} />
        <ViroImage source={require('./res/a9_3d_assets/measure_horiz.png')} width={1.0} height={.1} opacity={this.state.inStandMode ? 1.0: 0.0} position={[-.78, -.2, -8]}  scale={[3, 3, 3]} rotation={[0, 38, 0]}  />
        <ViroImage source={require('./res/a9_3d_assets/measure_vert.png')} width={.2} height={0.5} rotatation={[0, 0, 0]} opacity={this.state.inStandMode ? 1.0: 0.0} position={[.3, -.95, -8]}  scale={[3, 3, 3]}  />

        {this._get3dObject()}

      <ViroAnimatedComponent animation={this.state.mainAnimation} run={this.state.runAnimation} loop={false} >
        <ViroFlexView style={styles.titleContainer} position={[3.8, 1, -7]} rotation={[0, -40, 0]} height={2} width={4}>
          <ViroFlexView style={styles.rowContainer}>
            <ViroText style={styles.prodDescriptionText} text="Price:" />
            <ViroText style={styles.priceText} text="$598.00" />
          </ViroFlexView>
          <ViroText style={styles.prodDescriptionText} text="In stock." />
          <ViroText style={styles.prodDescriptionText} text="Ships from and sold online. Gift-wrap available." />
      </ViroFlexView>
      </ViroAnimatedComponent>
      <ViroAnimatedComponent animation={this.state.mainAnimation} run={this.state.runAnimation} loop={false} >
        <ViroVideo position={[3.8, -1.1, -7]} rotation={[0, -40, 0]} paused={this.state.videoPaused} loop={true} height={2} width={4} onClick={this._pauseVideo} source={require('./res/a9_3d_assets/amazon_vid.mp4')} />
      </ViroAnimatedComponent>
     </ViroScene>
    );
  },
  _get3dObject() {
      if(this.state.show3dModel) {
        return(<ViroAnimatedComponent animation={this.state.tvAnimation} run={this.state.runTVAnimation} loop={false}>
            <Viro3DObject onClick={this._objectClick} source={require('./res/a9_3d_assets/samsung_tv.obj')} type="OBJ" materials="tv" rotation={[0, 38, 0]} scale={[0.3, 0.3, 0.3]} position={[0, 0, -5]}/>
          </ViroAnimatedComponent>);
      }else {
        <ViroBox position={[0, -50, 0]} />
      }
  },
  _objectClick() {
    if (this.state.inStandMode) {
       this.setState({
         inStandMode: false,
         mainAnimation: "fadeIn",
         runAnimation: true,
         runTVAnimation: true,
         tvAnimation: "animateToFloat",
         videoPaused: false,
         loopTV: false,
       });
   } else {
     this.setState({
       runTVAnimation: true,
       tvAnimation: "animateToStand",
       inStandMode: true,
       mainAnimation: "fadeOut",
       runAnimation: true,
       videoPaused: true,
       loopTV: false,
     });
   }
  },
  _onTVFinish() {
    if(!this.state.inStandMode) {
      this.setState({
        tvAnimation: "animateToFloat",
      });
    }
},

  _pauseVideo() {
    this.setState({
      videoPaused: !this.state.videoPaused,
    });
  },
});

var materials = ViroMaterials.createMaterials({
  tv: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('./res/a9_3d_assets/samsung_tv_tex.jpg'),
    specularTexture: require('./res/a9_3d_assets/samsung_tv_specular.jpg')
  },
});

var animations = ViroAnimations.registerAnimations({
  fadeOut:{properties:{opacity:0}, duration:2000},
  fadeIn:{properties:{opacity: 1}, duration:2000},
  animateToStand:{properties:{positionX:-.84, positionY: -0.99, positionZ:-8}, duration:1000},
  animateToFloat:{properties:{positionX:0, positionY: 0, positionZ:-5}, duration:1000},
});
//animateToFloat used to be rotateY: -30, animateToStand: 38
var styles = StyleSheet.create({
  rowContainer: {
    flex: 1,
    flexDirection: 'row',
  },
  prodTitleText: {
    fontFamily: 'sans-serif-light',
    fontSize: 30,
    color: '#222222',
    textAlignVertical: 'center',
    textAlign: 'left',
  },
  priceText: {
    fontFamily: 'sans-serif-light',
    fontSize: 20,
    color: '#aa3c3c',
    textAlignVertical: 'center',
    textAlign: 'left',
    flex: 4,
  },
  prodDescriptionText: {
    fontFamily: 'sans-serif-light',
    fontSize: 20,
    color: '#222222',
    textAlignVertical: 'center',
    textAlign: 'left',
    flex: 1,
  },
  titleContainer: {
      flexDirection: 'column',
      backgroundColor: "#ffffffdd",
      padding: .2,
  },
});

module.exports = A9Demo;
