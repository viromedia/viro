/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

import { requireNativeComponent, View } from 'react-native';
import React, { Component } from 'react';
var PropTypes = require('react/lib/ReactPropTypes');

var ViroBox = React.createClass({
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    visible: PropTypes.bool,
    opacity: PropTypes.number,
    width: PropTypes.number,
    height: PropTypes.number,
    length: PropTypes.number,
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),

    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
    onScroll: React.PropTypes.func,
    onSwipe: React.PropTypes.func,

    /**
     * Enables high accuracy gaze collision checks for this object.
     * This can be useful for complex 3D objects where the default
     * checking method of bounding boxes do not provide adequate
     * collision detection coverage.
     *
     * NOTE: Enabling high accuracy gaze collision checks has a high
     * performance cost and should be used sparingly / only when
     * necessary.
     *
     * Flag is set to false by default.
     */
    highAccuracyGaze:PropTypes.bool,
    onDrag: React.PropTypes.func,
  },

  _onHover: function(event: Event) {
    this.props.onHover && this.props.onHover(event.nativeEvent.source, event.nativeEvent.isHovering);
  },

  _onClick: function(event: Event) {
    this.props.onClick && this.props.onClick(event.nativeEvent.source);
  },

  _onClickState: function(event: Event) {
    this.props.onClickState && this.props.onClickState(event.nativeEvent.source, event.nativeEvent.clickState);
    let CLICKED = 3; // Value representation of Clicked ClickState within EventDelegateJni.
    if (event.nativeEvent.clickState == CLICKED){
        this._onClick(event)
    }
  },

  _onTouch: function(event: Event) {
    this.props.onTouch && this.props.onTouch(event.nativeEvent.source, event.nativeEvent.touchState, event.nativeEvent.touchPos);
  },

  _onScroll: function(event: Event) {
    this.props.onScroll && this.props.onScroll(event.nativeEvent.source, event.nativeEvent.scrollPos);
  },

  _onSwipe: function(event: Event) {
    this.props.onSwipe && this.props.onSwipe(event.nativeEvent.source, event.nativeEvent.swipeState);
  },

  _onDrag: function(event: Event) {
      this.props.onDrag
        && this.props.onDrag(event.nativeEvent.source, event.nativeEvent.dragToPos);
  },
  render: function() {
    // Since materials and transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let materials = typeof this.props.materials === 'string' ? new Array(this.props.materials) : this.props.materials;
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;
    if (this.props.material) {
      console.error('The <ViroBox> component takes a `materials` property rather than `material`.');
    }

    if(materials) {
      if(materials.length!=1 && materials.length!=6) {
        console.error("<ViroBox> can only take 1 material for all sides or 6, one for each side.");
      }
    }


    return (
        <VRTBox
            {...this.props}
            materials={materials}
            canHover={this.props.onHover != undefined}
            canClick={this.props.onClick != undefined || this.props.onClickState != undefined}
            canTouch={this.props.onTouch != undefined}
            canScroll={this.props.onScroll != undefined}
            canSwipe={this.props.onSwipe != undefined}
            canDrag={this.props.onDrag != undefined}
            onHoverViro={this._onHover}
            onClickViro={this._onClickState}
            onTouchViro={this._onTouch}
            onScrollViro={this._onScroll}
            onSwipeViro={this._onSwipe}
            onDragViro={this._onDrag}/>
    );
  }
});

var VRTBox = requireNativeComponent(
    'VRTBox', ViroBox, {
        nativeOnly: {
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
            onDragViro:true}
    }
);

module.exports = ViroBox;
