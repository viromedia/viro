/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *//**
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
   ViroQuad,
   ViroSkyBox,
   ViroSphere,
   Viro3DObject,
   ViroSpinner,
 } from 'react-viro';

var ReleaseMenu = require("./ReleaseMenu.js");

let polarToCartesian = ViroUtils.polarToCartesian;

var createReactClass = require('create-react-class');

 // **This test has not been done.  This is placeholder for scene and navigation arrows**

 var ViroFlexViewLayoutTest = createReactClass({

   getInitialState() {
     return {
        flexViewLayout:1,
     };
   },

   render: function() {
     return (
      <ViroScene>
        <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
        {this._chooseFlexLayout()}
        <ViroText style={styles.baseTextTwo}  position={[0,-3, -3]} width={2} height={2}
                 text={"Change flexbox Layout"}
                 onClick={this._toggleFlexboxLayout}/>
      </ViroScene>

     );
   },

  _toggleFlexboxLayout() {
    var newflexboxLayout = this.state.flexViewLayout + 1;
    if (newflexboxLayout > 4){
        newflexboxLayout = 1;
    }
    console.log("New flexlayout: " + newflexboxLayout);
    this.setState({
                  flexViewLayout:newflexboxLayout
            });
  },
  _chooseFlexLayout() {
      if(this.state.flexViewLayout==1) {
          return this._flexLayoutFour();
      } else if(this.state.flexViewLayout==2) {
          return this._flexLayoutOne();
      } else if(this.state.flexViewLayout==3) {
          return this._flexLayoutThree();
      } else if(this.state.flexViewLayout==4) {
          return this._flexLayoutTwo();
      }
},
  _flexLayoutOne() {
    return (
      <ViroFlexView style={styles.titleContainer} position={[0, 0, -4]} rotation={[0, 0, 0]} height={2} width={4}>
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
    );
  },

  _flexLayoutTwo() {
    return (
      <ViroFlexView
        style={styles.galleryContainer} position={[0, 0, -4]}
        width={5} height={5}  rotation={[0, 0, 0]}>
        <ViroFlexView style={styles.horizontalFlex}>
          <ViroFlexView style={styles.centerText}>
            <ViroText style={styles.elementText} text="Real Estate"/>
            <ViroImage style={{flex:.8}} source={require('./res/sun_2302.jpg')} />
          </ViroFlexView>
          <ViroFlexView style={styles.centerText}>
            <ViroText style={styles.elementText} text="Adventure"/>
            <ViroImage style={{flex:.8}} source={require('./res/seattle1.jpg')} />
          </ViroFlexView>
        </ViroFlexView>
        <ViroFlexView style={styles.horizontalFlex} >
          <ViroFlexView style={styles.centerText}>
            <ViroText style={styles.elementText} text="Travel"/>
            <ViroImage style={{flex:.8}} source={require('./res/heart_d.jpg')} />
          </ViroFlexView>
          <ViroFlexView style={styles.centerText}>
            <ViroText style={styles.elementText} text="Sports"/>
            <ViroImage style={{flex:.8}} source={require('./res/grid_bg.jpg')} />
          </ViroFlexView>
        </ViroFlexView>
      </ViroFlexView>
    );
  },

  //Taken from: https://github.com/browniefed/react-native-screens/blob/master/screens/login/index.js
  _flexLayoutThree() {
    return (
      <ViroFlexView position={[0, 0, -4]} height={5} width={5}>
        <ViroFlexView style={{flex:1}} >
          <ViroFlexView style={styles.layout3markWrap} >
            <ViroImage style={styles.layout3mark} source={require('./res/card_main.png')} />
          </ViroFlexView>
          <ViroFlexView style={styles.layout3wrapper}>
            <ViroFlexView style={styles.layout3inputWrap}>
              <ViroFlexView style={styles.layout3iconWrap}>
                <ViroImage source={require('./res/grid_bg.jpg')} style={styles.layout3icon} />
              </ViroFlexView>
              <ViroText text="Username" style={styles.layout3input}  />
            </ViroFlexView>
            <ViroFlexView style={styles.layout3inputWrap}>
              <ViroFlexView style={styles.layout3iconWrap}>
                <ViroImage source={require('./res/grid_bg.jpg')} style={styles.layout3icon} />
              </ViroFlexView>
              <ViroText style={styles.layout3input} text="Password" />
            </ViroFlexView>
            <ViroFlexView style={{flex:.3}}>
                <ViroText style={styles.layout3forgotPasswordText} text="Forgot Password?" />
              </ViroFlexView>
              <ViroFlexView style={styles.layout3button}>
                <ViroText style={styles.layout3buttonText} text="First In" />
              </ViroFlexView>
          </ViroFlexView>
          <ViroFlexView style={styles.layout3container}>
            <ViroFlexView style={styles.layout3signupWrap}>
                <ViroFlexView style={{flex:1, backgroundColor: "#ff0000"}}>
                  <ViroText style={styles.layout3signupLinkText} text="Second Up"/>
                </ViroFlexView>
            </ViroFlexView>
          </ViroFlexView>
        </ViroFlexView>
      </ViroFlexView>
    );
  },

  _flexLayoutFour() {
    return (
      <ViroFlexView position={[0, 0, -4]} width={2} height={2} style={{flexDirection: 'column'}}>
        <ViroVideo style={{flex:.5}} source={require('./res/Climber.mp4')} />
        <ViroImage style={{flex: 1}} source={require('./res/seattle1.jpg')} />
      </ViroFlexView>
    );
  },

 });
var styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  rowContainer: {
    flex: 1,
    flexDirection: 'row',
  },
  elementText: {
    fontSize: 30,
    color: '#ffffff',
    textAlign: 'center',
    flex: 1,
  },
  baseTextTwo: {
      fontFamily: 'Arial',
      fontSize: 20,
      color: '#ffffff',
      flex: 1,
      textAlignVertical: 'center',
      textAlign: 'center',
  },
  titleContainer: {
      flexDirection: 'column',
      padding: .2,
      color: '#ffffff',
  },
  prodDescriptionText: {
    fontFamily: 'sans-serif-light',
    fontSize: 20,
    color: '#ffffff',
    textAlignVertical: 'center',
    textAlign: 'left',
    flex: 1,
  },
  galleryContainer: {
    flexDirection: 'column',
    padding: .1,
    margin: .1,
    backgroundColor: "#00ff00"
  },
  horizontalFlex: {
    backgroundColor: "#ff0000",
    flex: .5,
    flexDirection: 'row',
    margin: .1,
  },
  centerText: {
    flex: .5,
    padding: .2,
    margin: .1,
    flexDirection: 'column',
  },
  layout3container: {
    flex: 1,
  },
  layout3markWrap: {
    flex: 1,
    paddingVertical: .10,
  },
  layout3mark: {
    flex: 1,
  },
  layout3wrapper: {
    paddingVertical: .10,
  },
  layout3inputWrap: {
    flexDirection: "row",
    marginVertical: .10,
    height: .40,
    backgroundColor: "#CCC"
  },
  layout3iconWrap: {
    paddingHorizontal: .7,
    alignItems: "center",
    justifyContent: "center",
  },
  layout3icon: {
    height: .5,
    width: .5,
  },
  layout3input: {
    flex: 1,
    paddingHorizontal: .10,
    backgroundColor: "#fff",
    fontSize: 20,
  },
  layout3button: {
    backgroundColor: "#FF3366",
    paddingVertical: .05,
    alignItems: "center",
    justifyContent: "center",
    marginTop: .05,
    flex: 2,
  },
  layout3buttonText: {
    color: "#FFF",
    fontSize: 18,
    textAlign: 'center',
  },
  layout3forgotPasswordText: {
    color: "#D8D8D8",
    textAlign: "right",
    paddingRight: .15,
    fontSize: 20,
  },
  layout3signupWrap: {
    flexDirection: "row",
    alignItems: "center",
    justifyContent: "center",
  },
  layout3accountText: {
    color: "#D8D8D8",
    fontSize: 18,
  },
  layout3signupLinkText: {
    color: "#FFF",
    marginLeft: .2,
    fontSize: 20,
  }
});

ViroMaterials.createMaterials({
  redColor: {
  fresnelExponent: .5,
   shininess: 2.0,
    diffuseColor: "#ff0000"
  },
  blue: {
      shininess: 2.0,
      lightingModel: "Lambert",
      diffuseColor: "#0000ff"
    },
  layout3background: {
    diffuseTexture: require('./res/grid_bg.jpg')
  },
  heart: {
      lightingModel: "Phong",
      diffuseTexture: require('./res/earth.png'),
    },
 });
 module.exports = ViroFlexViewLayoutTest;
