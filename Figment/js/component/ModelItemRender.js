/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */
'use strict';

import React, { Component } from 'react';
import * as LoadConstants from '../redux/LoadingStateConstants';
import * as UIConstants from '../redux/UIConstants';
import * as ModelData from  '../model/ModelItems';
import {
  ViroScene,
  ViroARScene,
  ViroARNode,
  ViroARPlaneSelector,
  ViroARPlane,
  ViroBox,
  ViroMaterials,
  ViroNode,
  Viro3DObject,
  ViroText,
  ViroSpotLight,
  ViroSurface,
} from 'react-viro';


var PropTypes = React.PropTypes;

var ModelItemRender = React.createClass({
    propTypes: {
        modelIDProps: PropTypes.any,
        onLoadCallback: PropTypes.func,
        onClickStateCallback: PropTypes.func,
        hitTestMethod: PropTypes.func,
    },

    componentWillMount() {
      this._ref_object = null;
      this._ref_shadow_surface = null;
      this._modelData = ModelData.getModelArray()
    },

    getInitialState() {
      return {
        scale : ModelData.getModelArray()[this.props.modelIDProps.index].scale,
        rotation : [0, 0, 0],
        nodeIsVisible : false,
        position: [0, 0, 0],
        shouldBillboard : false,
      }
    },

    render: function() {
        var modelItem = ModelData.getModelArray()[this.props.modelIDProps.index];
        let transformBehaviors = {}
        if (this.state.shouldBillboard) {
          transformBehaviors.transformBehaviors = this.state.shouldBillboard ? "billboardY" : [];
        }
        let lightPosition = [0, 2, 0]
        let lightScalePivot = modelItem.scalePivot.slice();
        lightScalePivot[1] = lightScalePivot[1] - lightPosition[1]; // we want the scale pivot to be the same as the object

        // below we OR the light bitmask with 1 on the object because the default bitmask for lights
        // is 1 and we want the object to be lit up by all lights, but only have shadows casted by
        // one SpotLight contain within this component
        return (

          <ViroARNode key={this.props.modelIDProps.uuid} visible={this.state.nodeIsVisible} position={this.state.position} onDrag={()=>{}}>
            <ViroSpotLight
              innerAngle={5}
              outerAngle={30}
              direction={[0,-1,0]}
              position={lightPosition}
              scale={this.state.scale}
              scalePivot={modelItem.scalePivot}
              color="#ffffff"
              castsShadow={true}
              influenceBitMask={this.props.bitMask}/>
            <Viro3DObject ref={this._setComponentRef()}
                {...transformBehaviors}
                position={modelItem.position}
                scale={this.state.scale}
                scalePivot={modelItem.scalePivot}
                rotation={this.state.rotation}
                source={modelItem.obj}
                materials={modelItem.materials}
                resources={modelItem.resources}
                animation={modelItem.animation}
                lightBitMask={this.props.bitMask | 1}
                shadowCastingBitMask={this.props.bitMask}
                onClickState={this._onClickState(this.props.modelIDProps.uuid)}
                onError={this._onError(this.props.modelIDProps.uuid)}  onRotate={this._onRotateGesture()} onLoadStart={this._onObjectLoadStart(this.props.modelIDProps.uuid)} onLoadEnd={this._onObjectLoadEnd(this.props.modelIDProps.uuid)}
                onPinch={this._onPinchIndex()} />

            <ViroSurface
              ref={this._setShadowRef()}
              rotation={[-90, 0, 0]}
              position={[0, -.005 * this.props.bitMask, 0]}
              width={2} height={2}
              scale={this.state.scale}
              lightBitMask={this.props.bitMask | 1}
              materials={"transparentFloor"}
              acceptShadows={true} />

          </ViroARNode>
        );
    },

    // not sure why we're returning a lambda vs just returning this function. I remember
    // vik mentioning something about why we should do this. We should confirm and add a
    // comment.
    _setComponentRef() {
      return (component) => {
        console.log("Component ref value:");
        this._ref_object = component;
      }
    },

    _setShadowRef() {
      return (component) => {
        this._ref_shadow_surface = component;
      }
    },

    _onClickState(uuid) {
     return ((clickState, position, source)=> {
      if (clickState == 1) { // clickState == 1 -> "ClickDown"
        // if "ClickDown", then enable billboardY
        this.setState({shouldBillboard : true});
      } else if (clickState == 2) { // clickState == 2 -> "ClickUp"
        // for some reason this method gives us values "opposite" of what they should be
        // which is why we negate the y rotation, but also the y-rotation values are
        // always within -90 -> 90 so the x/z need to be adjusted back to 0 and the y
        // rotation recalculated in order for the rotation gesture to function properly
        this._ref_object.getTransformAsync().then((retDict)=>{
          let rotation = retDict.rotation;
          let absX = Math.abs(rotation[0]);
          let absZ = Math.abs(rotation[2]);
          // negate the y rotation
          let yRotation = - (rotation[1]);

          // if the X and Z aren't 0, then adjust the y rotation.
          if (absX > 1 && absZ > 1) {
            yRotation = 180 - (yRotation);
          }

          this.setState({
            rotation : [0,yRotation,0],
            shouldBillboard : false
          });
        })
      }
      this.props.onClickStateCallback(uuid, clickState, UIConstants.LIST_MODE_MODEL);
    });
  },
    _onRotateGesture() {
      return ((rotateState, rotationFactor, source)=> {
          this._onRotate(rotateState, rotationFactor, source);
      });
    },

    _onPinchIndex() {
      return ((pinchState, scaleFactor, source)=> {
          this._onPinch(pinchState, scaleFactor, source);
      });
    },

    /*
     Rotation should be relative to its current rotation *not* set to the absolute
     value of the given rotationFactor.
     */
    _onRotate(rotateState, rotationFactor, source) {
      if(rotateState == 1) {
        console.log("STARTING ROTATE WITH Rotation factor: " + rotationFactor);
        return;
      } else if(rotateState ==2){
        console.log("MID ROTATE WITH Rotation factor: " + rotationFactor);
      } else if(rotateState == 3) {
        console.log("END ROTATE WITH Rotation factor: " + rotationFactor);
        this.setState({
          rotation : [this.state.rotation[0], this.state.rotation[1] - rotationFactor, this.state.rotation[2]]
        })
        return;
      }

      this._ref_object.setNativeProps({rotation:[this.state.rotation[0], this.state.rotation[1] - rotationFactor, this.state.rotation[2]]});
    },

    /*
     Pinch scaling should be relative to its last value *not* the absolute value of the
     scale factor. So while the pinching is ongoing set scale through setNativeProps
     and multiply the state by that factor. At the end of a pinch event, set the state
     to the final value and store it in state.
     */
    _onPinch(pinchState, scaleFactor, source) {
      if(pinchState == 1) {
        console.log("STARTING PINCH WITH Scale factor: " + scaleFactor);
        return;
      } else if(pinchState == 2){
        console.log("MID PINCH WITH Scale factor: " + scaleFactor);
      } else if(pinchState == 3) {
        console.log("END PINCH WITH Scale factor: " + scaleFactor);
        this.setState({
          scale : this.state.scale.map((x)=>{return x * scaleFactor})
        });
        return;
      }

      var newScale = this.state.scale.map((x)=>{return x * scaleFactor})
      this._ref_object.setNativeProps({scale:newScale});
      this._ref_shadow_surface.setNativeProps({scale:newScale});
    },

    _onError(uuid) {
        return () => {
          console.log("MODEL has error HAS ERROR");
          this.props.loadCallback(uuid, LoadConstants.ERROR);
          //this.props.arSceneNavigator.viroAppProps.loadingObjectCallback(index, LoadingConstants.LOAD_ERROR);
        };

      },

    _onObjectLoadStart(uuid) {
        console.log("_onObjectLoadStart uuid:" + uuid);
        return () => {
          this.props.onLoadCallback(uuid, LoadConstants.LOADING);
        };
    },

    _onObjectLoadEnd(uuid) {
        return () => {
          this.props.onLoadCallback(uuid, LoadConstants.LOADED);
          this.props.hitTestMethod(this._onARHitTestResults);
          //this.props.arSceneNavigator.viroAppProps.loadingObjectCallback(index, LoadingConstants.LOADED);
        };
    },

    _onARHitTestResults(forward, results) {
      let validPosition = undefined;
      if (results.length > 0) {
        for (var i = 0; i < results.length; i++) {
          let result = results[i];
          if (result.type == "ExistingPlaneUsingExtent") {
            this.setState({
              position : result.transform.position,
              nodeIsVisible: true,
            });
            return;
          } else if (result.type == "FeaturePoint") {
            var distance = Math.sqrt((result.transform.position[0] * result.transform.position[0]) + (result.transform.position[1] * result.transform.position[1]) + (result.transform.position[2] * result.transform.position[2]));
            if(distance < 2) {
              console.log("Skipping this result since distance is :" + distance);
              continue;
            }
            validPosition = result.transform.position;
          }
        }
      }
      //no valid point found, just project the forward vector out 3 meters.
      var newPos = validPosition ? validPosition : [forward[0] * 3, forward[1]* 3, forward[2]* 3];
      console.log("DIDN'T FIND HIT TEST, new arnode projected position:");
      console.log(newPos);
      this.setState({
        position : newPos,
        nodeIsVisible: true,
      });
    }
});

ViroMaterials.createMaterials({
  porsche: {
    lightingModel:"Blinn",
    diffuseTexture: require("../res/car_porsche/Porsche911turboS_diff.jpg"),
  },
  bball: {
    lightingModel:"Blinn",
    diffuseTexture: require("../res/bball/bball.jpg"),
  },
  ring: {
    diffuseTexture: require("../res/portal_ring/portal_ring.png"),
  },
  tesla: {
    shininess: 1.0,
    lightingModel:"Blinn",
  },
  transparentFloor: {
    writesToDepthBuffer: false,
  },
});



module.exports = ModelItemRender;
