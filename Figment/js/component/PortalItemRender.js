
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
import * as PortalData from  '../model/PortalItems';

import {
  ViroScene,
  ViroARScene,
  ViroNode,
  Viro3DObject,
  Viro360Image,
  ViroPortalScene,
  ViroPortal,
  ViroMaterials,
  ViroImage,
  ViroSphere,
  ViroVideo,
  ViroSpotLight,
  ViroSurface,
} from 'react-viro';


var PropTypes = React.PropTypes;

// Whether or not objects should billboard when clicked.
var shouldBillboardOnTap = false;

var PortalItemRender = React.createClass({
    propTypes: {
        portalIDProps: PropTypes.any,
        onLoadCallback: PropTypes.func,
        onClickStateCallback: PropTypes.func,
        hitTestMethod: PropTypes.func,
    },

    getInitialState() {
      return {
        scale : PortalData.getPortalArray()[this.props.portalIDProps.index].scale,
        rotation : [0, 0, 0],
        nodeIsVisible : false,
        position: [0, 2, 1], // make it appear initially high in the sky
        shouldBillboard : false,
      }
    },

    componentWillMount() {
        this._portalData = PortalData.getPortalArray()
    },

    render: function() {
      var portalItem = PortalData.getPortalArray()[this.props.portalIDProps.index];
      let transformBehaviors = {}
      if (this.state.shouldBillboard) {
        transformBehaviors.transformBehaviors = this.state.shouldBillboard ? "billboardY" : [];
      }
      return (
        <ViroNode
          {...transformBehaviors}
          key={this.props.portalIDProps.uuid}
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

            <ViroPortalScene
              position={portalItem.position}
              onRotate={this._onRotate}
              onPinch={this._onPinch}
              passable={true}
              scale={portalItem.portalScale}
              onClickState={this._onClickState(this.props.portalIDProps.uuid)} >

              <ViroPortal>
                <Viro3DObject
                  source={portalItem.obj}
                  materials={portalItem.materials}
                  resource={portalItem.resources}
                  type={portalItem.frameType}
                  onLoadStart={this._onObjectLoadStart(this.props.portalIDProps.uuid)}
                  onLoadEnd={this._onObjectLoadEnd(this.props.portalIDProps.uuid)}
                  lightReceivingBitMask={this.props.bitMask | 1}
                  shadowCastingBitMask={this.props.bitMask} />
              </ViroPortal>

              {this._renderPortalInside(portalItem)}

            </ViroPortalScene>
        </ViroNode>
      );
    },

    _setARNodeRef(component) {
      this.arNodeRef = component;
    },

    _onClickState(uuid) {
      return ((clickState, position, source) => {
        if (shouldBillboardOnTap) {
          if (clickState == 1) {
            // if "ClickDown", then enable billboardY
            this.setState({shouldBillboard : true});
          } else if (clickState == 2) {
            // the y-rotation values are always within -90 -> 90 so the x/z need to be
            // adjusted back to 0 and the y rotation recalculated in order for the
            // rotation gesture to function properly
            this.arNodeRef.getTransformAsync().then((retDict)=>{
              let rotation = retDict.rotation;
              let absX = Math.abs(rotation[0]);
              let absZ = Math.abs(rotation[2]);
              
              let yRotation = (rotation[1]);

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
        }
        if (clickState == 2) {
          this.props.onClickStateCallback(uuid, clickState, UIConstants.LIST_MODE_PORTAL);
        }
      });
    },

    _renderPortalInside(portalItem) {
        var portalSource = (this.props.portalIDProps.portal360Image != undefined && this.props.portalIDProps.portal360Image != null) ? this.props.portalIDProps.portal360Image: portalItem.portal360Image;
        if(this._is360Photo(portalSource.width, portalSource.height)) {
          return (
            <Viro360Image key="background_portal" source={portalSource.source} />
          );
        } else {
          var viewArray = [];
          if(portalSource.source.uri.endsWith("mp4")) {

            viewArray.push(<ViroSphere  position={[0,0,0]} radius={56} facesOutward={false} key="background_portal" materials="theatre" />);
            viewArray.push(<ViroVideo key="image_portal" width={1} height={1}  source={portalSource.source}
                         position={[0, 3.9, -39]} scale={[42, 21, 1]} />);
          } else {
            viewArray.push(  <ViroSpotLight key="obj_spotlight"
                innerAngle={5}
                outerAngle={20}
                direction={[0,-1,0]}
                position={[0, 6, 0]}
                color="#ffffff"
                castShadows={true}
                shadowNearZ={.1}
                shadowFarZ={5}
                shadowOpacity={.9} />);
            viewArray.push(<Viro3DObject
                  key="obj_3d"
                  position={[0,-2,-6]}
                  scale={[0.5,0.5,0.5]}
                  source={require('../res/art_gallery/artgallery3.vrx')}
                  resources={[require('../res/art_gallery/art_gallery_projector_diffuse.png'),
                       require('../res/art_gallery/art_gallery_projector_specular.png'),
                       require('../res/art_gallery/art_gallery_walls_diffuse.png'),
                       require('../res/art_gallery/art_gallery_walls_specular.png')]}
                  key="background_portal"
                  type="VRX" />
              );
            {/*viewArray.push(<Viro3DObject
                key="obj_3d_frame"
                opacity={.4}
                position={[0, -2,-6]}
                source={require('../res/art_gallery/artgallery_picture_frame.vrx')}
                resources={[require('../res/art_gallery/art_gallery_projector_picture.png')]}
                type="VRX" />);
            viewArray.push(<Viro3DObject
                key="obj_3d_frame_beam"
                opacity={.4}
                position={[0, -2,-6]}
                source={require('../res/art_gallery/artgallery_projector.vrx')}
                resources={[require('../res/art_gallery/art_gallery_projector_beams_diffuse.png')]}
                type="VRX" />);*/}
            viewArray.push(<ViroImage key="image_portal" width={2} height={4}  resizeMode='scaleToFill' imageClipMode='none' source={portalSource.source}
                        position={[0, 0.8,-5.8]} scale={[1, 1, 1]} />);
            viewArray.push(<Viro360Image key="background_portal_image" source={require('../res/360_space.jpg')} />);

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
    _onRotate(rotateState, rotationFactor, source) {
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
      var newScale = this.state.scale.map((x)=>{return x * scaleFactor})

      if(pinchState == 3) {
        this.setState({
          scale : newScale
        });
        return;
      }

      this.arNodeRef.setNativeProps({scale:newScale});
    },

    _onError(uuid) {
        return () => {
          this.props.loadCallback(uuid, LoadConstants.ERROR);
        };
      },

    _onObjectLoadStart(uuid) {
        return () => {
          this.props.onLoadCallback(uuid, LoadConstants.LOADING);
        };
    },

    _onObjectLoadEnd(uuid) {
        return () => {
          this.props.onLoadCallback(uuid, LoadConstants.LOADED);
          this.props.hitTestMethod(this._onARHitTestResults);
        };
    },

    _onARHitTestResults(position, forward, results) {
      // default position is just 3 forward of the user
      let scaledForwardVector = [forward[0] * 1.2, forward[1]* 1.2, forward[2]* 1.2];
      let newPosition = [position[0] + scaledForwardVector[0], position[1] + scaledForwardVector[1], position[2] + scaledForwardVector[2]];

      // we need to set the position before making the node visible because of a race condition
      // in the case of portals, this could cause the portal to appear where the user is before
      // moving to it's location causing the user to accidentally "pass" through the portal.
      console.log("distance: Setting new position of PortalItem");
      console.log(newPosition);
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
