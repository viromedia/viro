/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import React, { Component } from 'react';
import { connect } from 'react-redux';
import * as LoadingConstants from './redux/LoadingStateConstants';
import * as UIConstants from './redux/UIConstants';
import ModelItemRender from './component/ModelItemRender';
import PortalItemRender from './component/PortalItemRender';
import EffectItemRender from './component/EffectItemRender';

import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

import {
  ViroScene,
  ViroARScene,
  ViroARNode,
  ViroARPlaneSelector,
  ViroARPlane,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOmniLight,
  ViroOrbitCamera,
  ViroImage,
  ViroVideo,
  ViroSkyBox,
  Viro360Video,
  ViroText,
  ViroSurface,
  Viro3DObject,
  ViroAmbientLight,
  ViroDirectionalLight,
  ViroSpotLight
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var figment = React.createClass({
  mixins: [TimerMixin],

  getInitialState: function() {
    return {
      text : "not tapped",
      currentObj: 0,
      isLoading: false,
      scaleSurface: [1,1,1],
    }
  },

  render: function() {
    
    // the starting bitmask is 1 because the default is 0 (2^0 = 1)
    let startingBitMask = 2;
    // fetch models
    let models = this.renderModels(this.props.modelItems, startingBitMask);
    // increment startingBitMask by the number of models
    startingBitMask += this.props.modelItems.length;
    let portals = this.renderPortals(this.props.portalItems, startingBitMask);

    // TODO: because of how shadows work, techncially we only have 31 bits to
    // work with so after the 32nd shadow object is added, we either crash...
    // or no shadows appear...

    return (
        <ViroARScene ref="arscene" physicsWorld={{gravity:[0, -9.81, 0]}}
            onTrackingInitialized={()=>{console.log("ARScene Initialized!")}}>
          <ViroDirectionalLight color="#ffffff" direction={[0,-1,-.2]}/>
          <ViroAmbientLight color="#ffffff" intensity={200}/>

          <ViroSpotLight
            innerAngle={5}
            outerAngle={90}
            direction={[0,1,0]}
            position={[0, -5, 0]}
            color="#ffffff"
            intensity={500}/>

          {models}
          {portals}
          {this.renderEffects(this.props.effectItems)}
        </ViroARScene>
    );
  },

  renderModels(modelItems, startingBitMask) {
    var renderedObjects = [];
    if(modelItems) {
      var root = this;
      let objBitMask = startingBitMask;
      Object.keys(modelItems).forEach(function(currentKey) {
        if(modelItems[currentKey] != null && modelItems[currentKey] != undefined) {
          renderedObjects.push(
            <ModelItemRender key={modelItems[currentKey].uuid}
              modelIDProps={modelItems[currentKey]}
              hitTestMethod={root._performARHitTest}
              onLoadCallback={root._onLoadCallback}
              onClickStateCallback={root._onModelsClickStateCallback}
              bitMask={Math.pow(2,objBitMask)} />
          );
        }
        objBitMask++;
      });

    }
    return renderedObjects;
  },

  renderPortals(portalItems, startingBitMask) {
    var renderedObjects = [];
    let portalBitMask = startingBitMask;
    if(portalItems) {
      for(var i = 0; i<portalItems.length; i++) {
        if(portalItems[i].selected) {
          renderedObjects.push(
            <PortalItemRender
            key={i}
            portalItem={portalItems[i]}
            index={i}
            hitTestMethod={this._performARHitTest}
            onLoadCallback={this._onLoadCallback}
            onClickStateCallback={this._onPortalsClickStateCallback}
            bitMask={portalBitMask}/>
          );
        }
        portalBitMask++;
      }
    }
    return renderedObjects;
  },

  renderEffects(effectItems) {
    if(effectItems){
      for(var i =0; i<effectItems.length; i++) {
          if(effectItems[i].selected) {
            return (<EffectItemRender key={i} effectItem={effectItems[i]} />);
          }
      }
    }
  },

  _performARHitTest(callback) {
    this.refs["arscene"].getCameraOrientationAsync().then((orientation) => {
      this.refs["arscene"].performARHitTestWithRay(orientation.forward).then((results)=>{
        callback(orientation.forward, results);
      })
    });
  },

  _onLoadCallback(uuid, loadState) {
    console.log("_onLoadCallback" + uuid + ", loadCallback:" + loadState);
    this.props.arSceneNavigator.viroAppProps.loadingObjectCallback(uuid, loadState);
  },
  _onModelsClickStateCallback(uuid, clickState, itemType) {
    console.log("_onClickStateCallback - index: " + uuid + ", clickState: " + clickState);
    this.props.arSceneNavigator.viroAppProps.clickStateCallback(uuid, clickState, itemType);
  },
  _onPortalsClickStateCallback(index, clickState, itemType) {
    console.log("_onClickStateCallback - index: " + index + ", clickState: " + clickState);
    this.props.arSceneNavigator.viroAppProps.clickStateCallback(index, clickState, itemType);
  },
});

function selectProps(store) {
  return {
    modelItems: store.arobjects.modelItems,
    portalItems: store.arobjects.portalItems,
    effectItems: store.arobjects.effectItems,
  };
}

module.exports = connect(selectProps)(figment);
