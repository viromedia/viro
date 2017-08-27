
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
} from 'react-viro';


var PropTypes = require('react/lib/ReactPropTypes');

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
        position: [0, 0, 0],
        shouldBillboard : false,
      }
    },

    componentWillMount() {
      this._ref_object = null;
    },

    render: function() {
        var j = this.props.index;
        let transformBehaviors = {}
        if (this.state.shouldBillboard) {
          transformBehaviors.transformBehaviors = this.state.shouldBillboard ? "billboardY" : [];
        }
        return (
          <ViroARNode key={j} visible={this.state.nodeIsVisible} position={this.state.position} onDrag={()=>{}}>
            <ViroPortal position={[0, 0, 0]} rotation={this.state.rotation}
                scale={this.state.scale} onRotate={this._onRotateGesture(j)}
                onPinch={this._onPinchIndex(j)} passable={true}
                ref={this._setComponentRef()} {...transformBehaviors}
                onClickState={this._onClickState(j)} >

               <ViroPortalFrame>
                 <Viro3DObject source={this.props.portalItem.obj}
                               position={[0, 0, 0]}
                               materials={this.props.portalItem.materials}
                               resource={this.props.portalItem.resources} onLoadStart={this._onObjectLoadStart(j)} onLoadEnd={this._onObjectLoadEnd(j)}/>
               </ViroPortalFrame>
              {this._renderPortalInside()}
            </ViroPortal>
          </ViroARNode>
        );
    },

    _setComponentRef() {
      return (component) => {
        console.log("SETTING COMPONENT REF!!! index:" + this.props.index);
        console.log("Component ref value:");
        this._ref_object = component;
      }
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
        this.props.onClickStateCallback(index, clickState, UIConstants.LIST_MODE_PORTAL);
      });
    },

    _renderPortalInside() {
        console.log("_renderPortalInside");
        console.log(this.props.portalItem.portal360Image);
        console.log("_renderPortalInside source:");
        console.log(this.props.portalItem.portal360Image.source);
        if(this._is360Photo(this.props.portalItem.portal360Image.width, this.props.portalItem.portal360Image.height)) {
          return (
            <Viro360Image source={this.props.portalItem.portal360Image.source} />
          );
        } else {
          var viewArray = [];
          viewArray.push(<ViroSphere position={[0,0,0]} radius={56} facesOutward={false} key="background_portal" materials="theatre" />);
          if(this.props.portalItem.portal360Image.source.uri.endsWith("mp4")) {
            console.log("movie uri: " + this.props.portalItem.portal360Image.source.uri);
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

    _onRotateGesture(index) {
      return ((rotateState, rotationFactor, source)=> {
          this._onRotate(rotateState, rotationFactor, source, index);
      });
    },

    _onPinchIndex(index) {
      return ((pinchState, scaleFactor, source)=> {
          this._onPinch(pinchState, scaleFactor, source, index);
      });
    },

    /*
     Rotation should be relative to its current rotation *not* set to the absolute
     value of the given rotationFactor.
     */
    _onRotate(rotateState, rotationFactor, source, index) {
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

      console.log("ONROTATE INDEX:" + index);

      this._ref_object.setNativeProps({rotation:[this.state.rotation[0], this.state.rotation[1] - rotationFactor, this.state.rotation[2]]});
    },

    /*
     Pinch scaling should be relative to its last value *not* the absolute value of the
     scale factor. So while the pinching is ongoing set scale through setNativeProps
     and multiply the state by that factor. At the end of a pinch event, set the state
     to the final value and store it in state.
     */
    _onPinch(pinchState, scaleFactor, source, index) {
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

      console.log("ONPINCH INDEX:" + index);

      var newScale = this.state.scale.map((x)=>{return x * scaleFactor})
      this._ref_object.setNativeProps({scale:newScale});
    },

    _onError(index) {
        return () => {
          console.log("MODEL has error HAS ERROR" + index);
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
      if (results.length > 0) {
         for (var i = 0; i < results.length; i++) {
           let result = results[i];
           if (result.type == "ExistingPlaneUsingExtent" || result.type == "FeaturePoint") {
             console.log("FOUND HIT TEST, new arnode projected position:");
             console.log(result.transform.position);
             var distance = Math.sqrt((result.transform.position[0] * result.transform.position[0]) + (result.transform.position[1] * result.transform.position[1]) + (result.transform.position[2] * result.transform.position[2]));
             if(distance < 2) {
              console.log("Skipping this result since distance is :" + distance);
              continue;
             }

             this.setState({
               position : result.transform.position,
               nodeIsVisible: true,
             });
             return;
           }
         }
      }
      //no valid point found, just project the forward vector out 3 meters.
      var newPos = [forward[0] * 3, forward[1]* 3, forward[2]* 3];
      console.log("DIDN'T FIND HIT TEST, new arnode projected position:");
      console.log(newPos);
      this.setState({
        position : newPos,
        nodeIsVisible: true,
      });
    },
});

ViroMaterials.createMaterials({
  portal_ship: {
    diffuseTexture: require("../res/portal_ship/portal_ship_diffuse.png"),
    normalTexture: require("../res/portal_ship/portal_ship_normal.png"),
  },
  theatre: {
    diffuseTexture: require("../res/360_dark_theatre.jpg"),
  },
});

module.exports = PortalItemRender;
