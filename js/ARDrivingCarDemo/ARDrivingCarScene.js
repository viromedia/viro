'use strict';

import React, { Component } from 'react';

import {StyleSheet} from 'react-native';

import {
  ViroARScene,
  ViroText,
  ViroConstants,
  ViroARImageMarker,
  Viro3DObject,
  ViroAmbientLight,
  ViroMaterialVideo,
  ViroMaterials,
  ViroAnimations,
  ViroNode,
  ViroPortal,
  ViroPortalScene,
  Viro360Video,
  Viro360Image,
  ViroVideo,
  ViroDirectionalLight,
  ViroLightingEnvironment,
  ViroImage,
  ViroSound,
  ViroParticleEmitter,
  ViroSpotLight,
  ViroQuad,
  ViroBox,
} from 'react-viro';

import renderIf from './renderIf';

const carScale = .1; // this is the scale of the car

// NOTE: make sure friction != drivingAcceleration!
const maxSpeed = .19 * carScale; // m/s
const drivingAcceleration = .08 * carScale; // m/s/s
const reverseAcceleration = .17 * carScale; // m/s/s
const friction = -.03 * carScale; // m/s/s
const intervalTime = 16; //ms
const distanceToFullTurn = .4 * carScale; // meters - this is how far the car should travel before it completes a circle
const wheelCircumference = 1 * carScale;
const maxLeanRotation = 10;
var currentAcceleration = 0; // m/s/s
var currentVelocity = 0; // m/s
var currentPosition = [0,0,0];
var currentDirection = [0,0,-1];
var currentRotation = 0; // this is a rotation about the Y in radians...
var currentLeanRotation = 0;
var wheelTurnRotation = 0; // the rotation of the wheel due to a turn (Y-ais)
var wheelDrivingRotation = 0; // the rotation of the wheel due to driving (X-axis)

var count = 0;

// keeps track of the prop value!
var shouldResetCarValue = false;

export default class ARDrivingCarScene extends Component {

  constructor() {
    super();

    // Set initial state here
    this.state = {
      text : "Initializing AR...",
      refreshFlag : false,
      modelWorldRotation : [0,0,0],
      displayHitReticle: false,
      foundPlane : false,
      planeReticleLocation: [0,0,0],
      shouldBillboard : true,
      isReady : false,
      lastFoundPlaneLocation : [0,0,0],
      volumeLevel : .3,
      showCone : false,
    };

    // bind 'this' to functions
    this._onInitialized = this._onInitialized.bind(this);
    this._getScanningQuads = this._getScanningQuads.bind(this);
    this._setInitialCarDirection = this._setInitialCarDirection.bind(this);
    this._getCarModel = this._getCarModel.bind(this);
    this._onCameraARHitTest = this._onCameraARHitTest.bind(this);
    this._computeNewLocation = this._computeNewLocation.bind(this);
    this._computeAcceleration = this._computeAcceleration.bind(this);
    this._resetCar = this._resetCar.bind(this);
    this._resetCarValues = this._resetCarValues.bind(this);
  }

  componentDidMount() {

  }

  componentWillUnmount() {
    if (this && this.timer) {
      clearInterval(this.timer);
    }
  }

  render() {

    if (this.props.arSceneNavigator.viroAppProps.isReady && !this.state.isReady) {
      // Setting state in the render() function *could* cause an infinite loop so make
      // sure you aren't. This'll cause a yellow box warning, but you can throw it in 
      // a setTimeout for 100ms before setting isReady to true to prevent that too.
      this.setState({
        isReady : true
      })

      setTimeout(()=>{this._setInitialCarDirection()}, 400);
    }
    
    let resetValue = this.props.arSceneNavigator.viroAppProps.shouldResetCar;
    if (resetValue && shouldResetCarValue != resetValue) {
      setTimeout(()=>{this._resetCar()}, 50);
    }
    shouldResetCarValue = resetValue;

    let onCameraARHitTestCallback = this.state.isReady ? undefined : this._onCameraARHitTest

    let environmentLightSource = require('./res/learner_park_1k.hdr');

    // we should pause the acceleration sound if we're not ready OR we're not either driving forward or backwards (2 & 8 bitmask)
    let shouldPauseAccelSound = !this.state.isReady || !(this.props.arSceneNavigator.viroAppProps.direction & 10)
    let shouldPauseIdleSound = !this.state.isReady || !shouldPauseAccelSound

    return (
      <ViroARScene ref={(scene)=>{this.scene = scene}} onCameraARHitTest={onCameraARHitTestCallback} onTrackingUpdated={this._onInitialized}
        physicsWorld={{gravity : [0, -5, 0]}}>

        <ViroLightingEnvironment source={environmentLightSource} />

        {this._getScanningQuads()}

        {this._getCarModel()}

        <ViroSound source={require('./res/car_ambient.mp3')} paused={!this.state.isReady} loop={true} />
        <ViroSound source={require('./res/car_drive.mp3')} paused={shouldPauseAccelSound} loop={true} />
        <ViroSound source={require('./res/car_idle.mp3')} paused={shouldPauseIdleSound} loop={true} volume={this.state.volumeLevel} />

      </ViroARScene>
    );
  }

  _setInitialCarDirection() {
    if (this.car) {
      this.car.getTransformAsync().then((retDict)=>{
        let rotation = retDict.rotation;
        let absX = Math.abs(rotation[0]);
        let absZ = Math.abs(rotation[2]);

        let yRotation = rotation[1];

        // if the X and Z aren't 0, then adjust the y rotation (the quaternion flipped the X or Z).
        if (absX != 0 && absZ != 0) {
          yRotation = 180 - (yRotation);
        }

        yRotation = yRotation;

        this.setState({
          modelWorldRotation : [0, yRotation, 0],
          shouldBillboard : false,
        }, ()=>{
          this.timer = setInterval(()=>{
            this._computeNewLocation();
          }, intervalTime)
        });
      });
    }
  }

  _getScanningQuads() {
    if (this.state.isReady) {
      return;
    }

    return (
      <ViroNode transformBehaviors={"billboardY"} position={this.state.planeReticleLocation}
        scale={[.5, .5, .5]} >
        <ViroImage rotation={[-90, 0, 0]} visible={this.state.foundPlane} source={require('./res/tracking_diffuse_2.png')}/>
        <ViroImage rotation={[-90, 0, 0]} visible={!this.state.foundPlane} source={require('./res/tracking_diffuse.png')} />
      </ViroNode>
    )
  }

  _getCarModel() {

    let position = this.state.isReady ? this.state.lastFoundPlaneLocation : [0 ,20, 0];

    var transformBehaviors = this.state.shouldBillboard ? "billboardY" : [];

    return (
      <ViroNode position={position} rotation={this.state.modelWorldRotation} transformBehaviors={transformBehaviors}>
        <ViroNode ref={(car)=>{this.car = car}}
          scale={[carScale,carScale,carScale]} >

          <ViroAmbientLight ref={(light)=>{this.ambientLight = light}} color={'#f5f8e0'}
            intensity={200} />

          <ViroQuad width={5.691} height={5.691} materials={["dropShadow"]} rotation={[-90,0,0]}/>

          <Viro3DObject
            ref={(car)=>{this.carRotationNode = car}}
            position={[0,0,0]}
            source={require('./res/car_body.vrx')}
            type='VRX'
            resources={[
              require('./res/bumblebee_Base_Color.png'),
              require('./res/bumblebee_Metallic.jpg'),
              require('./res/bumblebee_Roughness.jpg'),
              require('./res/bumblebee_Normal_OpenGL.jpg'),
            ]} />

          {/* Front left - need 2 containers, 1 for the side-to-side rotation, 1 for spin*/}
          <ViroNode ref={(wheel)=>{this.frontLeftWheelContainer = wheel}}
            position={[-.610, .363, -1.336]} >
            <ViroNode ref={(wheel)=>{this.frontLeftWheel = wheel}} >
              <Viro3DObject
                source={require('./res/car_wheels.vrx')}
                type='VRX'
                rotation={[0, 180, 0]} // the left wheels need to be rotated 180
                resources={[
                  require('./res/wheels_Base_Color.jpg'),
                  require('./res/wheels_Metallic.jpg'),
                  require('./res/wheels_Roughness.jpg'),
                  require('./res/wheels_Normal_OpenGL.jpg'),
                ]}/>
            </ViroNode>
          </ViroNode>

          {/* Front right - need 2 containers, 1 for the side-to-side rotation, 1 for spin*/}
          <ViroNode ref={(wheel)=>{this.frontRightWheelContainer = wheel}}
            position={[.610, .363, -1.336]} >
            <ViroNode ref={(wheel)=>{this.frontRightWheel = wheel}} >
              <Viro3DObject
                source={require('./res/car_wheels.vrx')}
                type='VRX'
                resources={[
                  require('./res/wheels_Base_Color.jpg'),
                  require('./res/wheels_Metallic.jpg'),
                  require('./res/wheels_Roughness.jpg'),
                  require('./res/wheels_Normal_OpenGL.jpg'),
                ]}/>
            </ViroNode>
          </ViroNode>

          {/* Rear left */}
          <ViroNode ref={(wheel)=>{this.rearLeftWheel = wheel}}
            position={[-.610, .363, 1.355]} >
            <Viro3DObject
              source={require('./res/car_wheels.vrx')}
              type='VRX'
              rotation={[0, 180, 0]} // the left wheels need to be rotated 180
              resources={[
                require('./res/wheels_Base_Color.jpg'),
                require('./res/wheels_Metallic.jpg'),
                require('./res/wheels_Roughness.jpg'),
                require('./res/wheels_Normal_OpenGL.jpg'),
              ]}/>
          </ViroNode>

          {/* Rear right */}
          <ViroNode ref={(wheel)=>{this.rearRightWheel = wheel}}
            position={[.610, .363, 1.355]} >
            <Viro3DObject
              source={require('./res/car_wheels.vrx')}
              type='VRX'
              resources={[
                require('./res/wheels_Base_Color.jpg'),
                require('./res/wheels_Metallic.jpg'),
                require('./res/wheels_Roughness.jpg'),
                require('./res/wheels_Normal_OpenGL.jpg'),
              ]}/>
          </ViroNode>


        </ViroNode>
      </ViroNode>
    )
  }

  _onInitialized(state, reason) {
    if (state == ViroConstants.TRACKING_NORMAL) {
      this.setState({
        text : "Hello World!"
      });
    } else if (state == ViroConstants.TRACKING_NONE) {
      // Handle loss of tracking
    }
  }

  _onCameraARHitTest(results) {
    if(results.hitTestResults.length > 0) {
      for (var i = 0; i < results.hitTestResults.length; i++) {
        let result = results.hitTestResults[i];
        if (result.type == "ExistingPlaneUsingExtent") {
          this.setState({
            planeReticleLocation : result.transform.position,
            displayHitReticle: true,
            foundPlane : true,
            lastFoundPlaneLocation : result.transform.position
          });
          this.props.arSceneNavigator.viroAppProps.setIsOverPlane(true);
          return;
        }
      }
    }

    //else we made it here, so just forward vector with unmarked.
    let newPosition = [results.cameraOrientation.forward[0] * 1.5, results.cameraOrientation.forward[1]* 1.5, results.cameraOrientation.forward[2]* 1.5];
    newPosition[0] = results.cameraOrientation.position[0] + newPosition[0];
    newPosition[1] = results.cameraOrientation.position[1] + newPosition[1];
    newPosition[2] = results.cameraOrientation.position[2] + newPosition[2];
    this.setState({
      planeReticleLocation : newPosition,
      displayHitReticle: true,
      foundPlane : false,
    });
    this.props.arSceneNavigator.viroAppProps.setIsOverPlane(false);
  }

  _computeNewLocation() {

    let pressedDirectionButtons = this.props.arSceneNavigator.viroAppProps.direction

    let computedVelocity = currentVelocity + currentAcceleration * (intervalTime / 1000)
    
    if (currentAcceleration == friction) {
      // if we aren't driving (friction) then make sure we never fall below 0 speed
      currentVelocity = Math.max(computedVelocity, 0);
    } else if (currentAcceleration == -friction) {
      // if we aren't driving (negative friction) then make sure we never go above 0 speed
      currentVelocity = Math.min(computedVelocity, 0);
    } else {
      // if we are driving, then make sure we never go above maxSpeed (in either positive or negative)
      currentVelocity = Math.max(Math.min(computedVelocity, maxSpeed), - maxSpeed);
    }

    // immediately compute the next frame's accelerations now (it can be later, but sometimes we return early)
    this._computeAcceleration();

    let desiredLeanRotation = 0
    // compute new directions based on the joystick
    let turnRatio = this.props.arSceneNavigator.viroAppProps.leftRightRatio
    if ( (pressedDirectionButtons & 5) > 0) { // if left or right was pressed...
      let additionalRotation = 0;
      if ( (pressedDirectionButtons & 1) > 0 ) { // Left
        additionalRotation = - (currentVelocity * (intervalTime / 1000) / distanceToFullTurn * turnRatio) * 2 * Math.PI; // radians
        wheelTurnRotation = turnRatio * 60;
        desiredLeanRotation = - maxLeanRotation * Math.abs(currentVelocity / maxSpeed);

      } else if ( (pressedDirectionButtons & 4) > 0 ) { // right
        additionalRotation = (currentVelocity * (intervalTime / 1000) / distanceToFullTurn * turnRatio) * 2 * Math.PI; // radians
        wheelTurnRotation = turnRatio * - 60;
        desiredLeanRotation = maxLeanRotation * Math.abs(currentVelocity / maxSpeed);
      }

      // compute new currentDirection based on the new additional rotation we're adding
      currentDirection = [
        Math.cos(additionalRotation) * currentDirection[0] - Math.sin(additionalRotation) * currentDirection[2], // x
        0, // y
        Math.sin(additionalRotation) * currentDirection[0] + Math.cos(additionalRotation) * currentDirection[2]// z
      ]

      currentRotation -= additionalRotation; // our platform rotation is "opposite" of the equation here

    } else {
      wheelTurnRotation = 0;
    }

    // based on the desiredLeanRotation, move the currentLeanRotation .5 degrees towards the desired one
    if (currentLeanRotation != desiredLeanRotation) {
      if (currentLeanRotation > desiredLeanRotation) {
        currentLeanRotation -= .5;
      } else {
        currentLeanRotation += .5;
      }
    }

    // set the front wheels' turn rotation
    this.frontRightWheelContainer.setNativeProps({
      rotation : [0, wheelTurnRotation, 0]
    })
    this.frontLeftWheelContainer.setNativeProps({
      rotation : [0, wheelTurnRotation, 0]
    })

    if (currentVelocity != 0) {
      // compute the new position & wheel turn.
      let distanceX = currentDirection[0] * currentVelocity;
      let distanceY = currentDirection[1] * currentVelocity;
      let distanceZ = currentDirection[2] * currentVelocity;
      currentPosition = [
        currentPosition[0] + distanceX,
        currentPosition[1] + distanceY,
        currentPosition[2] + distanceZ
      ]

      let totalDistance = Math.sqrt(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);
      // we need to negativize because that's how our axis are set up
      let additionalDriveRotation = - totalDistance / wheelCircumference * 360; 
      if (currentVelocity < 0) {
        additionalDriveRotation = - additionalDriveRotation // rotation should be backwards if we're in reverse!
      }
      wheelDrivingRotation = (wheelDrivingRotation + additionalDriveRotation) % 360;

      // set the car's rotation/position
      this.car.setNativeProps({
        position : currentPosition,
        rotation : [0, currentRotation * 180 / Math.PI, 0], // we're only rotating about the Y & we need to convert to degrees
      })

      this.carRotationNode.setNativeProps({
        rotation : [0, 0, currentLeanRotation]
      })

      // set all the wheel's driving rotation
      this.frontRightWheel.setNativeProps({
        rotation : [wheelDrivingRotation, 0, 0]
      })
      this.frontLeftWheel.setNativeProps({
        rotation : [wheelDrivingRotation, 0, 0]
      })
      this.rearRightWheel.setNativeProps({
        rotation : [wheelDrivingRotation, 0, 0]
      })
      this.rearLeftWheel.setNativeProps({
        rotation : [wheelDrivingRotation, 0, 0]
      })

    }
  }

  _computeAcceleration() {
    let pressedDirectionButtons = this.props.arSceneNavigator.viroAppProps.direction

    if ( (pressedDirectionButtons & 2) > 0 ) {
      if (currentVelocity < 0) {
        currentAcceleration = reverseAcceleration;
      } else {
        currentAcceleration = drivingAcceleration;
      }
    } else if ( (pressedDirectionButtons & 8) > 0) {
      if (currentVelocity > 0) {
        currentAcceleration = - reverseAcceleration;
      } else {
        currentAcceleration = - drivingAcceleration;
      }
    } else {
      // if we were accelerating forward or were already applying friction, then keep applying friction
      if (currentAcceleration == drivingAcceleration || currentAcceleration == friction) {
        currentAcceleration = friction;
      } else {
        currentAcceleration = - friction;
      }
    }
  }

  _resetCar() {
    this.scene.getCameraOrientationAsync().then((orientation)=>{
      let position = orientation.position
      let forward = orientation.forward
      let xzMagnitude = Math.sqrt(forward[0] * forward[0] + forward[2] * forward[2]);

      let distanceFromUser = 1; //meters
      let newPosition = [position[0] + (forward[0] / xzMagnitude * distanceFromUser),
                       this.state.lastFoundPlaneLocation[1], // we want to use the current Y position!
                       position[2] + (forward[2] / xzMagnitude * distanceFromUser)]

      this._resetCarValues()

      this.setState({
        lastFoundPlaneLocation : newPosition,
      })

    });
  }

  _resetCarValues() {
    currentAcceleration = 0; // m/s/s
    currentVelocity = 0; // m/s
    currentPosition = [0,0,0];
    currentDirection = [0,0,-1];
    currentRotation = 0; // this is a rotation about the Y in radians...

    this.car.setNativeProps({
      position : currentPosition,
      rotation : [0,0,0],
    })
  }
}

ViroMaterials.createMaterials({
  dropShadow: {
    diffuseTexture: require('./res/car_shadow.png'),
    lightingModel: "Constant",
    blendMode : 'Subtract',
  },
  invisibleMaterial : {
    diffuseColor : '#ffffff00'
  }
});

var styles = StyleSheet.create({
  helloWorldTextStyle: {
    fontFamily: 'Arial',
    fontSize: 30,
    color: '#ffffff',
    textAlignVertical: 'center',
    textAlign: 'center',  
  },
});

module.exports = ARDrivingCarScene;