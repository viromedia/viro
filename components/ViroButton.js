/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroButton
 */
 'user strict';

import { requireNativeComponent, View, StyleSheet } from 'react-native';
import React, { Component } from 'react';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
import PropTypes from 'prop-types';
import { checkMisnamedProps } from './Utilities/ViroProps';

var ViroPropTypes = require('./Styles/ViroPropTypes');
var createReactClass = require('create-react-class');
var StyleSheetPropType = require('react-native/Libraries/DeprecatedPropTypes/DeprecatedStyleSheetPropType');
var stylePropType = StyleSheetPropType(ViroPropTypes);
var ViroNode = require('./ViroNode');
var ViroImage = require('./ViroImage');
var ViroAnimations = require('./Animation/ViroAnimations');
var BTN_TYPE_HOVER = 'hovering';
var BTN_TYPE_NORMAL = 'normal';
var BTN_TYPE_CLICKED = 'clicked';

/**
 * Composite controle for 2D button
 */
 var ViroButton = createReactClass({
  propTypes: {
  	...View.propTypes,
    /**
     * The button image file, which is required
     */
    source: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./image.jpg')
      PropTypes.number,
    ]).isRequired,

    /**
     * The image file, to be displayed when the user is hovering over it
     */
    hoverSource: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./image.jpg')
      PropTypes.number,
    ]),

    /**
     * The image file, to be displayed when the user clicks the button
     */
    clickSource: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./image.jpg')
      PropTypes.number,
    ]),

    /**
     * ##### DEPRECATION WARNING - this prop may be removed in future releases #####
     * The image file, to be displayed when the user taps the button
     */
    tapSource: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./image.jpg')
      PropTypes.number,
    ]),

    /**
     * ##### DEPRECATION WARNING - this prop may be removed in future releases #####
     * The image file, to be displayed when the user is gazing over it
     */
    gazeSource: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./image.jpg')
      PropTypes.number,
    ]),

    position: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scalePivot: PropTypes.arrayOf(PropTypes.number),
    rotationPivot: PropTypes.arrayOf(PropTypes.number),
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
  	  PropTypes.string
  	]),
    animation: PropTypes.shape({
      name: PropTypes.string,
      delay: PropTypes.number,
      loop: PropTypes.bool,
      onStart: PropTypes.func,
      onFinish: PropTypes.func,
      run: PropTypes.bool,
      interruptible: PropTypes.bool,
    }),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
  	  PropTypes.string
  	]),
    onTransformUpdate: PropTypes.func,
    renderingOrder: PropTypes.number,
  	visible: PropTypes.bool,
  	opacity: PropTypes.number,
  	height: PropTypes.number,
  	width: PropTypes.number,
    style: stylePropType,
    dragType: PropTypes.oneOf(["FixedDistance", "FixedDistanceOrigin", "FixedToWorld", "FixedToPlane"]),
    dragPlane: PropTypes.shape({
      planePoint : PropTypes.arrayOf(PropTypes.number),
      planeNormal : PropTypes.arrayOf(PropTypes.number),
      maxDistance : PropTypes.number
    }),

    ignoreEventHandling: PropTypes.bool,
    onHover: PropTypes.func,
    onClick: PropTypes.func,
    onClickState: PropTypes.func,
    onTouch: PropTypes.func,
    onScroll: PropTypes.func,
    onSwipe: PropTypes.func,
    onDrag: PropTypes.func,
    onPinch: PropTypes.func,
    onRotate: PropTypes.func,
    onFuse: PropTypes.oneOfType([
      PropTypes.shape({
        callback: PropTypes.func.isRequired,
        timeToFuse: PropTypes.number
      }),
      PropTypes.func
    ]),
    physicsBody: PropTypes.shape({
      type: PropTypes.oneOf(['Dynamic','Kinematic','Static']).isRequired,
      mass: PropTypes.number,
      restitution: PropTypes.number,
      shape: PropTypes.shape({
        type: PropTypes.oneOf(["Box", "Sphere", "Compound"]).isRequired,
        params: PropTypes.arrayOf(PropTypes.number)
      }).isRequired,
      friction: PropTypes.number,
      useGravity: PropTypes.bool,
      enabled: PropTypes.bool,
      velocity: PropTypes.arrayOf(PropTypes.number),
      force: PropTypes.oneOfType([
        PropTypes.arrayOf(PropTypes.shape({
          value: PropTypes.arrayOf(PropTypes.number),
          position: PropTypes.arrayOf(PropTypes.number)
        })),
        PropTypes.shape({
          value: PropTypes.arrayOf(PropTypes.number),
          position: PropTypes.arrayOf(PropTypes.number)
        }),
      ]),
      torque: PropTypes.arrayOf(PropTypes.number)
    }),

    viroTag: PropTypes.string,
    onCollision: PropTypes.func,
  },

  applyImpulse: function(force, atPosition) {
    this._component.applyImpulse(force, atPosition);
  },

  applyTorqueImpulse: function(torque) {
    this._component.applyTorqueImpulse(torque);
  },

  setVelocity: function(velocity) {
    this._component.setVelocity(velocity);
  },

  _onAnimationStart: function(event: Event) {
    this.props.animation && this.props.animation.onStart && this.props.animation.onStart();
  },

  _onAnimationFinish: function(event: Event) {
    this.props.animation && this.props.animation.onFinish && this.props.animation.onFinish();
  },

  async getTransformAsync() {
    return await this._component.getTransformAsync();
  },

  async getBoundingBoxAsync() {
    return await this._component.getBoundingBoxAsync();
  },

  getInitialState: function() {
    return {buttonType: BTN_TYPE_NORMAL};
  },

  render: function() {
    checkMisnamedProps("ViroButton", this.props);

    // We default to showing the button if it's undefined
    if (this.props.visible === undefined) {
      this.props.visible = true;
    }

    var normalSrcVisible;
    var hoverSrcVisible;
    var clickSrcVisible;
    var hoverSource = this.props.hoverSource || this.props.gazeSource;
    var clickSource = this.props.clickSource || this.props.tapSource;

    let buttonScale = this.props.scale || [1,1,1];
    switch(this.state.buttonType) {
      case BTN_TYPE_HOVER:
        hoverSrcVisible = this.props.visible && true;
        clickSrcVisible = false;
        normalSrcVisible = false;
        break;
      case BTN_TYPE_CLICKED:
        // start scale for button animation
        buttonScale = [0.9 * buttonScale[0],
                       0.9 * buttonScale[1],
                       0.9 * buttonScale[2]];
        hoverSrcVisible = false;
        clickSrcVisible = this.props.visible && true;
        normalSrcVisible = false;
        break;
      default:
        normalSrcVisible = this.props.visible && true;
        hoverSrcVisible = false;
        clickSrcVisible = false;
    }

    // TODO: rather than manually expanding/setting props, we should do
    // {...this.props} which will save us time when adding new properties
    return (
        <ViroNode
            ref={component => {this._component = component}}
            physicsBody={this.props.physicsBody}
            position={this.props.position}
            onTransformUpdate={this.props.onTransformUpdate}
            onClickState={this.props.onClickState}
            onTouch={this.props.onTouch}
            onScroll={this.props.onScroll}
            onSwipe={this.props.onSwipe}
            onHover={this._onButtonHover}
            onClick={this._onButtonClicked}
            onDrag={this.props.onDrag}
            onPinch={this.props.onPinch}
            onRotate={this.props.onRotate}
            onCollision={this.props.onCollision}
            viroTag={this.props.viroTag}
            onFuse={this.props.onFuse}
            animation={this.props.animation}
            onAnimationStartViro={this._onAnimationStart}
            onAnimationFinishViro={this._onAnimationFinish}
            ignoreEventHandling={this.props.ignoreEventHandling}
            dragType={this.props.dragType} >

            <ViroImage
                source={this.props.source}
                rotation={this.props.rotation}
                rotationPivot={this.props.rotationPivot}
                scale={buttonScale}
                scalePivot={this.props.scalePivot}
                opacity={this.props.opacity}
                transformBehaviors={this.props.transformBehaviors}
                visible={normalSrcVisible}
                renderingOrder={this.props.renderingOrder}
                height={this.props.height}
                width={this.props.width}
                materials={this.props.materials} />

            <ViroImage
                source={hoverSource ? hoverSource : this.props.source}
                rotation={this.props.rotation}
                rotationPivot={this.props.rotationPivot}
                scale={buttonScale}
                scalePivot={this.props.scalePivot}
                opacity={this.props.opacity}
                transformBehaviors={this.props.transformBehaviors}
                visible={hoverSrcVisible}
                renderingOrder={this.props.renderingOrder}
                height={this.props.height}
                width={this.props.width}
                materials={this.props.materials} />

            <ViroImage
                source={clickSource ? clickSource :
                        (hoverSource ? hoverSource : this.props.source)}
                rotation={this.props.rotation}
                scale={buttonScale}
                opacity={this.props.opacity}
                transformBehaviors={this.props.transformBehaviors}
                visible={clickSrcVisible}
                renderingOrder={this.props.renderingOrder}
                height={this.props.height}
                width={this.props.width}
                materials={this.props.materials}
                animation={{
                  animation:"clickAnimation",
                  run:clickSrcVisible,
                  onFinish:this._onAnimationFinished
                }} />

    	</ViroNode>

    );
  },

  _onButtonHover: function(isHovering, source) {
    if (isHovering) {
        this.setState({
            buttonType: BTN_TYPE_HOVER
        });
        if (this.props.onHover) {
          this.props.onHover(isHovering, source);
        }
    } else {
      this.setState({
        buttonType: BTN_TYPE_NORMAL
      });
    }
  },

  _onButtonClicked: function(source) {
    this.setState({
      buttonType: BTN_TYPE_CLICKED
    });
    if (this.props.onClick) {
      this.props.onClick(source);
    }
  },

  _onAnimationFinished: function() {
    this.setState({
      buttonType: BTN_TYPE_HOVER
    });
  }
 });

ViroAnimations.registerAnimations({
  clickAnimation: {
    properties: {
      scaleX:"/=0.9",
      scaleY:"/=0.9",
      scaleZ:"/=0.9"
    },
    easing: "Bounce",
    duration: 500,
  },
});
 module.exports = ViroButton;
