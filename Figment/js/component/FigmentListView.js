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

import * as LoadingConstants from '../redux/LoadingStateConstants';
import React, { Component } from 'react';
import {StyleSheet, TouchableHighlight, ActivityIndicator, View, ListView, Image} from 'react-native';

var PropTypes = require('react/lib/ReactPropTypes');

var FigmentListView = React.createClass({
    propTypes: {
        items: PropTypes.array,
        onPress: PropTypes.func,
    },

    getInitialState: function() {
      var ds = new ListView.DataSource({rowHasChanged: (r1, r2) => r1 !== r2});
      return {
        rowChanged: 0,
        dataRows: this.props.items,
        dataSource: ds.cloneWithRows(this.props.items),
      };
    },

    componentWillReceiveProps(nextProps) {
      var newRows = nextProps.items.slice(0);
      newRows[this.state.rowChanged] = {
          ...nextProps.items[this.state.rowChanged],
      };

      this.setState({
        dataSource: this.state.dataSource.cloneWithRows(newRows),
        dataRows: newRows
      });
    },

    render: function() {
      if(this.state.dataSource == undefined) {
          return (<View/>);
      }
      console.log("RETURNING LISTVIEW!!");
      return (
      <ListView
        horizontal={true}
        contentContainerStyle={styles.listViewContainer}
        dataSource={this.state.dataSource}
        renderRow={this._renderListItem} />);
    },

    _renderListItem(data, sectionid, rowId) {
        console.log("renderListItem rowID: " + rowId);
        console.log("Data: " + data.selected + ", data.loading:" + data.loading);
        if(data.loading == LoadingConstants.LOADED) {
          return (
            <View style={{marginLeft: 10}}>
              <TouchableHighlight onPress={this._onListItemPressed(rowId)} underlayColor="#00000000">
                <Image source={data.icon_img} style={styles.photo} >
                  <Image source={require("../res/btn_cancel.png")} style={styles.photo} />
                </Image>
              </TouchableHighlight>
            </View>
          );
        } else if(data.loading == LoadingConstants.NONE) {
            return (
              <View style={{marginLeft: 10}}>
                <TouchableHighlight onPress={this._onListItemPressed(rowId)} underlayColor="#00000000">
                  <Image source={data.icon_img} style={styles.photo} />
                </TouchableHighlight>
              </View>
            );
        } else if( data.loading == LoadingConstants.LOADING) {
            return (
            <View style={{marginLeft: 10}}>
                <Image source={data.icon_img} style={styles.photo} >
                  <ActivityIndicator animating={true} size='large'/>
                </Image>
            </View>
            );
        }
    },

    _onListItemPressed(rowId) {
        return () => {
          // if we tapped a list icon but we are still loading just return.
          this.setState({
             rowChanged: parseInt(rowId),
          });
          this.props.onPress(rowId);
        }
    },
});

var styles = StyleSheet.create({
  flex : {
    flex : 1,
  },
  aRContainer: {
    flex:8,
  },
  listViewContainer: {
    height:100,
    padding: 12,
  },
  photo: {
    height: 80,
    width: 80,
    borderRadius: 20,
    justifyContent: 'center',
    alignItems: 'center',
    paddingLeft:10,
    paddingRight:10,
  },
  submitText: {
    color:'#fff',
    textAlign:'center',
    fontSize : 20
  },
  overlayView : {
    position: 'absolute',
    bottom: 10,
    left: 10,
    height: 80,
    width: 80,
    paddingTop:20,
    paddingBottom:20,
    backgroundColor:'#68a0cf',
    borderRadius: 10,
    borderWidth: 1,
    borderColor: '#fff',
  },
  recordingTimeText: {
    textAlign: 'center',
    color: '#ffffff',
    marginBottom: 5,
    marginTop:20,
    borderWidth: 1,
  },
});

module.exports = FigmentListView;
