/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroImage
 * @flow
 */
'use strict';

import { requireNativeComponent, View, Platform, findNodeHandle } from 'react-native';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
import React from 'react';
var NativeModules = require('react-native').NativeModules;
var PropTypes = React.PropTypes;
var StyleSheet = require('react-native/Libraries/StyleSheet/StyleSheet');

var ViroPropTypes = require('./Styles/ViroPropTypes');
var StyleSheetPropType = require('react-native/Libraries/StyleSheet/StyleSheetPropType');
var stylePropType = StyleSheetPropType(ViroPropTypes);

/**
 * Used to render a ViroImage
 */
var ViroImage = React.createClass({
  propTypes: {
    ...View.propTypes,
    /**
     * The image file, which is required
     */
    source: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./image.jpg')
      PropTypes.number,
    ]).isRequired,

    /**
     * The position of the card
     */
    position: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    scalePivot: PropTypes.arrayOf(PropTypes.number),
    rotationPivot: PropTypes.arrayOf(PropTypes.number),
    opacity: PropTypes.number,
    width: PropTypes.number,
    height: PropTypes.number,
    resizeMode: PropTypes.oneOf(['scaleToFill','scaleToFit','stretchToFill']),
    imageClipMode: PropTypes.oneOf(['none', 'clipToBounds']),
    stereoMode:PropTypes.oneOf(['leftRight', 'rightLeft', 'topBottom', 'bottomTop', 'none']),
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    lightBitMask : PropTypes.number,
    shadowCastingBitMask : PropTypes.number,
    acceptShadows : PropTypes.bool,
    onTransformUpdate: React.PropTypes.func,
    visible: PropTypes.bool,
    style: stylePropType,

    // Required to be local source static image by using require(''./image.jpg').
    // or by specifying local uri.
    // If not set, the image will be transparent until the source is downloaded
    placeholderSource:PropTypes.oneOfType([
      // TODO: Tooling to support documenting these directly and having them display in the docs.
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      PropTypes.number,
    ]),

    // DEPRECATION WARNING: DO NOT USE THE FOLLOWING PROP!
    placeHolderSource:PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      PropTypes.number,
    ]),

    mipmap: PropTypes.bool,
    format: PropTypes.oneOf(['RGBA8', 'RGBA4', 'RGB565']),

    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
    onScroll: React.PropTypes.func,
    onSwipe: React.PropTypes.func,
    onDrag: React.PropTypes.func,
    onFuse: PropTypes.oneOfType([
          React.PropTypes.shape({
            callback: React.PropTypes.func.isRequired,
            timeToFuse: PropTypes.number
          }),
          React.PropTypes.func
        ]),
    /**
     * Callback triggered when we are processing the assets to be
     * displayed in this ViroImage (either downloading / reading from file).
     */
    onLoadStart: React.PropTypes.func,

    /**
     * Callback triggered when we have finished processing assets to be
     * displayed. Wether or not assets were processed successfully and
     * thus displayed will be indicated by the parameter "success".
     * For example:
     *
     *   _onLoadEnd(event:Event){
     *      // Indication of asset loading success
     *      event.nativeEvent.success
     *   }
     *
     */
    onLoadEnd: React.PropTypes.func,

    /**
     * Callback triggered when the image fails to load. Invoked with
     * {nativeEvent: {error}}
     */
    onError: React.PropTypes.func,
    physicsBody: React.PropTypes.shape({
      type: React.PropTypes.oneOf(['dynamic','kinematic','static']).isRequired,
      mass: PropTypes.number,
      restitution: PropTypes.number,
      shape: React.PropTypes.shape({
        type: PropTypes.string.isRequired,
        params: PropTypes.arrayOf(PropTypes.number)
      }),
      friction: PropTypes.number,
      useGravity: PropTypes.bool,
      enabled: PropTypes.bool,
      velocity: PropTypes.arrayOf(PropTypes.number),
      force: PropTypes.oneOfType([
        PropTypes.arrayOf(React.PropTypes.shape({
          power: PropTypes.arrayOf(PropTypes.number),
          position: PropTypes.arrayOf(PropTypes.number)
        })),
        React.PropTypes.shape({
          power: PropTypes.arrayOf(PropTypes.number),
          position: PropTypes.arrayOf(PropTypes.number)
        }),
      ]),
      torque: PropTypes.arrayOf(PropTypes.number)
    }),

    viroTag: PropTypes.string,
    onCollided: React.PropTypes.func,
  },

  getInitialState: function() {
    return {
      propsPositionState:this.props.position,
      nativePositionState:undefined
    }
  },

  _onLoadStart: function(event: Event) {
    this.props.onLoadStart && this.props.onLoadStart(event);
  },

  _onLoadEnd: function(event: Event) {
    this.props.onLoadEnd && this.props.onLoadEnd(event);
  },

  _onError: function(event: Event) {
    this.props.onError && this.props.onError(event);
  },

  _onHover: function(event: Event) {
    this.props.onHover && this.props.onHover(event.nativeEvent.isHovering, event.nativeEvent.position, event.nativeEvent.source);
  },

  _onClick: function(event: Event) {
    this.props.onClick && this.props.onClick(event.nativeEvent.position, event.nativeEvent.source);
  },

  _onClickState: function(event: Event) {
    this.props.onClickState && this.props.onClickState(event.nativeEvent.clickState, event.nativeEvent.position, event.nativeEvent.source);
    let CLICKED = 3; // Value representation of Clicked ClickState within EventDelegateJni.
    if (event.nativeEvent.clickState == CLICKED){
        this._onClick(event)
    }
  },

  _onTouch: function(event: Event) {
    this.props.onTouch && this.props.onTouch(event.nativeEvent.touchState, event.nativeEvent.touchPos, event.nativeEvent.source);
  },

  _onScroll: function(event: Event) {
      this.props.onScroll && this.props.onScroll(event.nativeEvent.scrollPos, event.nativeEvent.source);
  },

  _onSwipe: function(event: Event) {
      this.props.onSwipe && this.props.onSwipe(event.nativeEvent.swipeState, event.nativeEvent.source);
  },

  _onDrag: function(event: Event) {
      this.props.onDrag
        && this.props.onDrag(event.nativeEvent.dragToPos, event.nativeEvent.source);
  },

  _onFuse: function(event: Event){
    if (this.props.onFuse){
      if (typeof this.props.onFuse === 'function'){
        this.props.onFuse(event.nativeEvent.source);
      } else if (this.props.onFuse != undefined && this.props.onFuse.callback != undefined){
        this.props.onFuse.callback(event.nativeEvent.source);
      }
    }
  },

  applyImpulse: function(force, position) {
    NativeModules.VRTNodeModule.applyImpulse(findNodeHandle(this), force, position);
  },

  applyTorqueImpulse: function(torque) {
    NativeModules.VRTNodeModule.applyTorqueImpulse(findNodeHandle(this), torque);
  },

  setInstantaneousVelocity: function(velocity) {
    NativeModules.VRTNodeModule.setInstantaneousVelocity(findNodeHandle(this), velocity);
  },

  _onCollided: function(event: Event){
    if (this.props.onCollided){
      this.props.onCollided(event.nativeEvent.viroTag, event.nativeEvent.collidedPoint,
                                                           event.nativeEvent.collidedNormal);
    }
  },

  // Called from native on the event a positional change has occured
  // for the underlying control within the renderer.
  _onNativeTransformUpdate: function(event: Event){
    var position =  event.nativeEvent.position;
    this.setState({
      nativePositionState:position
    }, () => {
      if (this.props.onTransformUpdate){
        this.props.onTransformUpdate(position);
      }
    });
  },

  // Set the propsPositionState on the native control if the
  // nextProps.position state differs from the nativePositionState that
  // reflects this control's current vroNode position.
  componentWillReceiveProps(nextProps){
    if(nextProps.position != this.state.nativePositionState){
      var newPosition = [nextProps.position[0], nextProps.position[1], nextProps.position[2], Math.random()];
      this.setState({
        propsPositionState:newPosition
      });
    }
  },

  // Ignore all changes in native position state as it is only required to
  // keep track of the latest position prop set on this control.
  shouldComponentUpdate: function(nextProps, nextState) {
    if (nextState.nativePositionState != this.state.nativePositionState){
      return false;
    }

    return true;
  },

  setNativeProps: function(nativeProps) {
    this._component.setNativeProps(nativeProps);
  },

  render: function() {
    var defaultPlaceholder = require('./Resources/viro_blank.png');
    var imgsrc = resolveAssetSource(this.props.source);
    var placeholderSrc;
    if (this.props.src) {
      console.error('The <ViroImage> component takes a `source` property rather than `src`.');
    }

    if (this.props.material) {
      console.error('The <ViroImage> component takes a `materials` property rather than `material`.');
    }

    if (this.props.placeholderSource) {
      placeholderSrc = resolveAssetSource(this.props.placeholderSource);
    } else {
      switch (Platform.OS) {
        case 'ios':
         /*
          On iOS in dev mode, it takes time to download the default placeholder,
          so we use the renderer to set transparency instead.
          */
          break;
        case 'android':
          placeholderSrc = resolveAssetSource(defaultPlaceholder);
          break;
      }
    }

    // Since materials and transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let materials = typeof this.props.materials === 'string' ? new Array(this.props.materials) : this.props.materials;
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;

    let timeToFuse = undefined;
    if (this.props.onFuse != undefined && typeof this.props.onFuse === 'object'){
        timeToFuse = this.props.onFuse.timeToFuse;
    }

    let transformDelegate = this.props.onTransformUpdate != undefined ? this._onNativeTransformUpdate : undefined;

    // Create native props object.
    let nativeProps = Object.assign({}, this.props);
    nativeProps.position = this.state.propsPositionState;
    nativeProps.onNativeTransformDelegateViro = transformDelegate;
    nativeProps.hasTransformDelegate = this.props.onTransformUpdate != undefined;
    nativeProps.materials = materials;
    nativeProps.source = imgsrc;
    nativeProps.placeHolderSource = placeholderSrc;
    nativeProps.placeholderSource = placeholderSrc;
    nativeProps.transformBehaviors = transformBehaviors;
    nativeProps.onLoadStartViro = this._onLoadStart;
    nativeProps.onLoadEndViro = this._onLoadEnd;
    nativeProps.onErrorViro = this._onError;
    nativeProps.style = [this.props.style];
    nativeProps.onHoverViro = this._onHover;
    nativeProps.onClickViro = this._onClickState;
    nativeProps.onTouchViro = this._onTouch;
    nativeProps.onScrollViro = this._onScroll;
    nativeProps.onSwipeViro = this._onSwipe;
    nativeProps.onDragViro = this._onDrag;
    nativeProps.canHover = this.props.onHover != undefined;
    nativeProps.canClick = this.props.onClick != undefined || this.props.onClickState != undefined;
    nativeProps.canTouch = this.props.onTouch != undefined;
    nativeProps.canScroll = this.props.onScroll != undefined;
    nativeProps.canSwipe = this.props.onSwipe != undefined;
    nativeProps.canDrag = this.props.onDrag != undefined;
    nativeProps.canFuse = this.props.onFuse != undefined;
    nativeProps.onFuseViro = this._onFuse;
    nativeProps.timeToFuse = timeToFuse;
    nativeProps.canCollide = this.props.onCollided != undefined;
    nativeProps.onCollidedViro = this._onCollided;
    nativeProps.ref = component => {this._component = component; };
    return (
      <VRTImage {...nativeProps}/>
    );
  }
});

var VRTImage = requireNativeComponent(
  'VRTImage', ViroImage, {
    nativeOnly: {
            onLoadStartViro: true,
            onLoadEndViro: true,
            onErrorViro: true,
            canHover: true,
            canClick: true,
            canTouch: true,
            canScroll: true,
            canSwipe: true,
            canDrag: true,
            onHoverViro:true,
            onClickViro:true,
            onTouchViro:true,
            onScrollViro:true,
            onSwipeViro:true,
            onDragViro:true,
            canFuse: true,
            onFuseViro:true,
            timeToFuse:true,
            canCollide:true,
            onCollidedViro:true,
            onNativeTransformDelegateViro:true,
            hasTransformDelegate:true
          }
  }
);

module.exports = ViroImage;
