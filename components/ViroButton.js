
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
     * The image file, to be displayed when the user is gazing over it
     */
    gazeSource: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./image.jpg')
      PropTypes.number,
    ]),

    /**
     * The image file, to be displayed when the user taps the button
     */
    tapSource: PropTypes.oneOfType([
      PropTypes.shape({
        uri: PropTypes.string,
      }),
      // Opaque type returned by require('./image.jpg')
      PropTypes.number,
    ]),

    position: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
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
  },
  getInitialState: function() {
    return {buttonType: BTN_TYPE_NORMAL};
  },
  render: function() {

    if (this.props.material) {
      console.error('The <ViroButton> component takes a `materials` property rather than `material`.');
    }

    // We default to showing the button if it's undefined
    if (!this.props.visible) {

      this.props.visible = true;
    }

    var normalSrcVisible;
    var gazeSrcVisible;
    var tapSrcVisible;

    let buttonScale = this.props.scale;
    switch(this.state.buttonType) {
      case BTN_TYPE_HOVER:
        if (this.props.gazeSource) {
          gazeSrcVisible = this.props.visible && true;
          tapSrcVisible = false;
          normalSrcVisible = false;
        }
        break;
      case BTN_TYPE_CLICKED:
        if (this.props.tapSource) {
          // start scale for button animation
          buttonScale = [0.9 * this.props.scale[0],
                         0.9 * this.props.scale[1],
                         0.9 * this.props.scale[2]];
          gazeSrcVisible = false;
          tapSrcVisible = this.props.visible && true;
          normalSrcVisible = false;

        }
        break;
      default:
          normalSrcVisible = this.props.visible && true;
          gazeSrcVisible = false;
          tapSrcVisible = false;
    }

    return (
        <ViroNode
            position={this.props.position}
            onClickState={this.props.onClickState}
            onTouch={this.props.onTouch}
            onScroll={this.props.onScroll}
            onSwipe={this.props.onSwipe}
            onHover={this._onButtonHover}
            onClick={this._onButtonClicked}
            onDrag={this.props.onDrag}>

            <ViroImage
                source={this.props.source}
                rotation={this.props.rotation}
                scale={buttonScale}
                opacity={this.props.opacity}
                transformBehaviors={this.props.transformBehaviors}
                visible={normalSrcVisible}
                height={this.props.height}
                width={this.props.width}
                materials={this.props.materials} />

            <ViroImage
                source={this.props.gazeSource}
                rotation={this.props.rotation}
                scale={buttonScale}
                opacity={this.props.opacity}
                transformBehaviors={this.props.transformBehaviors}
                visible={gazeSrcVisible}
                height={this.props.height}
                width={this.props.width}
                materials={this.props.materials} />

            <ViroAnimatedComponent
                animation="tapAnimation"
                run={tapSrcVisible}
                onFinish={this._onAnimationFinished}>

                    <ViroImage
                        source={this.props.tapSource}
                        rotation={this.props.rotation}
                        scale={buttonScale}
                        opacity={this.props.opacity}
                        transformBehaviors={this.props.transformBehaviors}
                        visible={tapSrcVisible}
                        height={this.props.height}
                        width={this.props.width}
                        materials={this.props.materials} />

            </ViroAnimatedComponent>
    	</ViroNode>

    );
  },

  _onButtonHover: function(source, isHovering) {
    if (this.props.onHover && isHovering) {
        this.setState({
            buttonType: BTN_TYPE_HOVER
        });
      this.props.onHover(source, isHovering);
    } else if (!isHovering) {
      this.setState({
        buttonType: BTN_TYPE_NORMAL
      });
    }
  },

  _onButtonClicked: function(source) {
    if (this.props.onClick) {
      this.setState({
        buttonType: BTN_TYPE_CLICKED
      });
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
  tapAnimation: {
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
