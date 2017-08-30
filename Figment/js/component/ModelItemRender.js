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
        let transformBehaviors = {};
        if (this.state.shouldBillboard) {
          transformBehaviors.transformBehaviors = this.state.shouldBillboard ? "billboardY" : [];
        }

        // below we OR the light bitmask with 1 on the object because the default bitmask for lights
        // is 1 and we want the object to be lit up by all lights, but only have shadows casted by
        // one SpotLight contain within this component
        return (

          <ViroARNode
            {...transformBehaviors}
            key={this.props.modelIDProps.uuid}
            ref={this._setARNodeRef}
            visible={this.state.nodeIsVisible}
            position={this.state.position}
            scale={this.state.scale}
            rotation={this.state.rotation}
            onDrag={()=>{}}>
            
            <ViroSpotLight
              innerAngle={5}
              outerAngle={20}
              direction={[0,-1,0]}
              position={[0, 5, 0]}
              color="#ffffff"
              castsShadow={true}
              influenceBitMask={this.props.bitMask}
              shadowNearZ={.1}
              shadowFarZ={5}
              shadowOpacity={.9} />
            
            <Viro3DObject
              position={modelItem.position}
              source={modelItem.obj}
              materials={modelItem.materials}
              resources={modelItem.resources}
              animation={modelItem.animation}
              lightBitMask={this.props.bitMask | 1}
              shadowCastingBitMask={this.props.bitMask}
              onClickState={this._onClickState(this.props.modelIDProps.uuid)}
              onError={this._onError(this.props.modelIDProps.uuid)}
              onRotate={this._onRotate}
              onPinch={this._onPinch}
              onLoadStart={this._onObjectLoadStart(this.props.modelIDProps.uuid)}
              onLoadEnd={this._onObjectLoadEnd(this.props.modelIDProps.uuid)} />

            <ViroSurface
              rotation={[-90, 0, 0]}
              position={[0, -.001 * this.props.bitMask, 0]}
              width={2} height={2}
              lightBitMask={this.props.bitMask | 1}
              materials={"transparentFloor"}
              acceptShadows={true} />

          </ViroARNode>
        );
    },

    _setARNodeRef(component) {
      this.arNodeRef = component;
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
          this.arNodeRef.getTransformAsync().then((retDict)=>{
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
          });
        }
        this.props.onClickStateCallback(uuid, clickState, UIConstants.LIST_MODE_MODEL);
      });
    },

    /*
     Rotation should be relative to its current rotation *not* set to the absolute
     value of the given rotationFactor.
     */
    _onRotate(rotateState, rotationFactor, source) {
      // ignore the first factor, there's a bug with it. VIRO-1651
      if (rotateState == 1) {
        return;
      }

      if (rotateState == 3) {
        this.setState({
          rotation : [this.state.rotation[0], this.state.rotation[1] - rotationFactor, this.state.rotation[2]]
        })
        return;
      }

      this.arNodeRef.setNativeProps({rotation:[this.state.rotation[0], this.state.rotation[1] - rotationFactor, this.state.rotation[2]]});
    },

    /*
     Pinch scaling should be relative to its last value *not* the absolute value of the
     scale factor. So while the pinching is ongoing set scale through setNativeProps
     and multiply the state by that factor. At the end of a pinch event, set the state
     to the final value and store it in state.
     */
    _onPinch(pinchState, scaleFactor, source) {
      // ignore the first factor, there's a bug with it. VIRO-1651
      if (pinchState == 1) {
        return;
      }

      if (pinchState == 3) {
        this.setState({
          scale : this.state.scale.map((x)=>{return x * scaleFactor})
        });
        return;
      }

      var newScale = this.state.scale.map((x)=>{return x * scaleFactor})
      this.arNodeRef.setNativeProps({scale:newScale});
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
