
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
var PropTypes = require('react/lib/ReactPropTypes');

var ViroPropTypes = require('./Styles/ViroPropTypes');
var StyleSheetPropType = require('react-native/Libraries/StyleSheet/StyleSheetPropType');
var stylePropType = StyleSheetPropType(ViroPropTypes);
var ViroNode = require('./ViroNode');
var ViroImage = require('./ViroImage');
var ViroAnimatedComponent = require('./ViroAnimatedComponent');
var ViroAnimations = require('./Animation/ViroAnimations');
var BTN_TYPE_HOVER = 'hovering';
var BTN_TYPE_NORMAL = 'normal';
var BTN_TYPE_CLICKED = 'clicked';
var RCT_BUTTON_REF = 'virobuttoncomponent';

/**
 * Composite controle for 2D button
 */
 var ViroButton = React.createClass({
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
      transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
  	  PropTypes.string
  	]),
  	visible: PropTypes.bool,
  	opacity: PropTypes.number,
  	height: PropTypes.number,
  	width: PropTypes.number,
    style: stylePropType,

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
    physicsBody: React.PropTypes.shape({
      type: React.PropTypes.oneOf(['dynamic','kinematic','static']).isRequired,
      mass: PropTypes.number,
      restitution: PropTypes.number,
      shape: React.PropTypes.shape({
        type: PropTypes.string.isRequired,
        params: PropTypes.arrayOf(PropTypes.number)
      }).isRequired,
      friction: PropTypes.number,
      useGravity: PropTypes.bool,
      enabled: PropTypes.bool,
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

  applyImpulse: function(force, atPosition) {
    this.refs[RCT_BUTTON_REF].applyImpulse(force, atPosition);
  },

  applyTorqueImpulse: function(torque) {
    this.refs[RCT_BUTTON_REF].applyTorqueImpulse(torque);
  },

  getInitialState: function() {
    return {buttonType: BTN_TYPE_NORMAL};
  },
  render: function() {

    if (this.props.material) {
      console.error('The <ViroButton> component takes a `materials` property rather than `material`.');
    }
    if (this.props.gazeSource) {
      console.warn("[Viro] ViroButton.gazeSource has been DEPRECATED. Please use hoverSource instead.");
    }
    if (this.props.tapSource) {
      console.warn("[Viro] ViroButton.tapSource has been DEPRECATED. Please use clickSource instead.");
    }

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

    return (
        <ViroNode
            ref={RCT_BUTTON_REF}
            physicsBody={this.props.physicsBody}
            position={this.props.position}
            onClickState={this.props.onClickState}
            onTouch={this.props.onTouch}
            onScroll={this.props.onScroll}
            onSwipe={this.props.onSwipe}
            onHover={this._onButtonHover}
            onClick={this._onButtonClicked}
            onDrag={this.props.onDrag}
            onCollided={this.props.onCollided}
            viroTag={this.props.viroTag}
            onFuse={this.props.onFuse}>

            <ViroImage
                source={this.props.source}
                rotation={this.props.rotation}
                rotationPivot={this.props.rotationPivot}
                scale={buttonScale}
                scalePivot={this.props.scalePivot}
                opacity={this.props.opacity}
                transformBehaviors={this.props.transformBehaviors}
                visible={normalSrcVisible}
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
                height={this.props.height}
                width={this.props.width}
                materials={this.props.materials} />

            <ViroAnimatedComponent
                animation="clickAnimation"
                run={clickSrcVisible}
                onFinish={this._onAnimationFinished}>

                    <ViroImage
                        source={clickSource ? clickSource :
                                (hoverSource ? hoverSource : this.props.source)}
                        rotation={this.props.rotation}
                        scale={buttonScale}
                        opacity={this.props.opacity}
                        transformBehaviors={this.props.transformBehaviors}
                        visible={clickSrcVisible}
                        height={this.props.height}
                        width={this.props.width}
                        materials={this.props.materials} />

            </ViroAnimatedComponent>
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
