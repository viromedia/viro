
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
import {
  ViroScene,
  ViroARScene,
  ViroARNode,
  ViroNode,
  Viro3DObject,
  Viro360Image,
  ViroPortal,
  ViroPortalFrame,
  ViroMaterials,
  ViroImage,
  ViroSphere,
  ViroVideo,
  ViroSpotLight,
  ViroSurface,
} from 'react-viro';


var PropTypes = React.PropTypes;

var PortalItemRender = React.createClass({
    propTypes: {
        portalItem: PropTypes.any,
        onLoadCallback: PropTypes.func,
        onClickStateCallback: PropTypes.func,
        index: PropTypes.number,
        hitTestMethod: PropTypes.func,
    },

    getInitialState() {
      return {
        scale : this.props.portalItem.scale,
        rotation : [0, 0, 0],
        nodeIsVisible : false,
        position: [0, 10, 1], // make it appear initially high in the sky
        shouldBillboard : false,
      }
    },

    componentWillMount() {
    },

    render: function() {
      var index = this.props.index;
      let transformBehaviors = {}
      if (this.state.shouldBillboard) {
        transformBehaviors.transformBehaviors = this.state.shouldBillboard ? "billboardY" : [];
      }
      return (
        <ViroARNode
          {...transformBehaviors}
          key={index}
          ref={this._setARNodeRef}
          visible={this.state.nodeIsVisible}
          position={this.state.position}
          scale={this.state.scale}
          rotation={this.state.rotation}
          onDrag={()=>{}} >

          <ViroSpotLight
            innerAngle={5}
            outerAngle={20}
            direction={[0,-1,-.2]}
            position={[0, 5, 1]}
            color="#ffffff"
            castsShadow={true}
            influenceBitMask={this.props.bitMask}
            shadowNearZ={.1}
            shadowFarZ={5}
            shadowOpacity={.9} />
            
            <ViroPortal
              position={this.props.portalItem.position}
              onRotate={this._onRotate}
              onPinch={this._onPinch}
              passable={true}
              scale={this.props.portalItem.portalScale}
              onClickState={this._onClickState(index)} >

              <ViroPortalFrame>
                <Viro3DObject
                  source={this.props.portalItem.obj}
                  materials={this.props.portalItem.materials}
                  resource={this.props.portalItem.resources}
                  onLoadStart={this._onObjectLoadStart(index)}
                  onLoadEnd={this._onObjectLoadEnd(index)}
                  lightBitMask={this.props.bitMask | 1}
                  shadowCastingBitMask={this.props.bitMask} />
              </ViroPortalFrame>

              {this._renderPortalInside()}

            </ViroPortal>

          <ViroSurface
            rotation={[-90, 0, 0]}
            position={[0, -.001 * this.props.bitMask, 0]}
            width={2.5} height={2.5}
            lightBitMask={this.props.bitMask | 1}
            materials={"shadowCatcher"}
            acceptShadows={true} />

        </ViroARNode>
      );
    },

    _setARNodeRef(component) {
      this.arNodeRef = component;
    },

    _onClickState(index) {
      return ((clickState, position, source) => {
        if (clickState == 1) {
          // if "ClickDown", then enable billboardY
          this.setState({shouldBillboard : true});
        } else if (clickState == 2) {
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
          })
        }
        this.props.onClickStateCallback(index, clickState, UIConstants.LIST_MODE_PORTAL);
      });
    },

    _renderPortalInside() {
        if(this._is360Photo(this.props.portalItem.portal360Image.width, this.props.portalItem.portal360Image.height)) {
          return (
            <Viro360Image source={this.props.portalItem.portal360Image.source} />
          );
        } else {
          var viewArray = [];
          viewArray.push(<ViroSphere position={[0,0,0]} radius={56} facesOutward={false} key="background_portal" materials="theatre" />);
          if(this.props.portalItem.portal360Image.source.uri.endsWith("mp4")) {
            viewArray.push(<ViroVideo key="image_portal" width={1} height={1}  source={this.props.portalItem.portal360Image.source}
                         position={[0, 3.9, -39]} scale={[42, 21, 1]} />);
          } else {
            viewArray.push(<ViroImage key="image_portal" width={1} height={1}  resizeMode='scaleToFit' source={this.props.portalItem.portal360Image.source}
                         position={[0, 3.9, -39]} scale={[42, 21, 1]} />);
          }
          return viewArray;
        }
    },

    _is360Photo(width, height) {
      let ratio = width / height;
      return (ratio > 1.9 && ratio < 2.2);
    },

    /*
     Rotation should be relative to its current rotation *not* set to the absolute
     value of the given rotationFactor.
     */
    _onRotate(rotateState, rotationFactor, source, index) {
      // ignore the first factor, there's a bug with it. VIRO-1651
      if(rotateState == 1) {
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
    _onPinch(pinchState, scaleFactor, source, index) {
      // ignore the first factor, there's a bug with it. VIRO-1651
      if(pinchState == 1) {
        return;
      } 

      var newScale = this.state.scale.map((x)=>{return x * scaleFactor})

      if(pinchState == 3) {
        this.setState({
          scale : newScale
        });
        return;
      }

      this.arNodeRef.setNativeProps({scale:newScale});
    },

    _onError(index) {
        return () => {
          this.props.loadCallback(index, LoadConstants.ERROR);
        };
      },

    _onObjectLoadStart(index) {
        return () => {
          this.props.onLoadCallback(index, LoadConstants.LOADING);
        };
    },

    _onObjectLoadEnd(index) {
        return () => {
          this.props.onLoadCallback(index, LoadConstants.LOADED);
          this.props.hitTestMethod(this._onARHitTestResults);
        };
    },

    _onARHitTestResults(forward, results) {
      // default position is just 3 forward of the user
      let newPosition = [forward[0] * 3, forward[1]* 3, forward[2]* 3];

      // try to find a more informed position via the hit test results
      if (results.length > 0) {
        let hitResultPosition = undefined;
        for (var i = 0; i < results.length; i++) {
          let result = results[i];
          if (result.type == "ExistingPlaneUsingExtent") {
            hitResultPosition = result.transform.position;
            break;
          } else if (result.type == "FeaturePoint" && !hitResultPosition) {
            var distance = Math.sqrt((result.transform.position[0] * result.transform.position[0]) + (result.transform.position[1] * result.transform.position[1]) + (result.transform.position[2] * result.transform.position[2]));
            if (distance < 2) {
              hitResultPosition = result.transform.position;
            }
          }
        }

        if (hitResultPosition) {
          newPosition = hitResultPosition;
        }
      }

      // we need to set the position before making the node visible because of a race condition
      // in the case of portals, this could cause the portal to appear where the user is before
      // moving to it's location causing the user to accidentally "pass" through the portal.
      this.setState({
        position : newPosition,
      }, ()=>{
        this.setState({
          nodeIsVisible: true,
        })
      });
    },
});

module.exports = PortalItemRender;
