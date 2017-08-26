/**
 * Copyright (c) 2017-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

import React, { Component, PropTypes } from 'react';

import {
  AppRegistry,
  Text,
  View,
  StyleSheet,
  PixelRatio,
  ListView,
  Image,
  TouchableHighlight,
  TouchableOpacity,
  ActivityIndicator,
  ActionSheetIOS,
  CameraRoll,
  Alert,
  ScrollView,
} from 'react-native';

var ROW_PREFIX = "ROW_";
var COLUMN_PREFIX = "COLUMN_";

var TAB_STOCK = "stock";
var TAB_360 = "360";
var TAB_RECENT = "recent";
var REF_PREFIX = "ref";
var REF_DELIM = "_";

var STOCK_360_PHOTOS = [
  {
    source : require('../res/360_diving.jpg'),
    height : 1,
    width : 2,
  },
  {
    source : require('../res/360_guadalupe.jpg'),
    height : 1,
    width : 2,
  },
  {
    source : require('../res/360_space.jpg'),
    height : 1,
    width : 2,
  },
  {
    source : require('../res/360_waikiki.jpg'),
    height : 1,
    width : 2,
  },
  {
    source : require('../res/360_westlake.jpg'),
    height : 1,
    width : 2,
  },
];

export class PhotosSelector extends Component {
  // -- Props/State/Constructors

  static propTypes = {
    /*
     Number of elements in a row.
     */
    columns : PropTypes.number,
    /*
     rows denotes the number of rows visible, on the screen at one time (height of scroll view)
     */
    rows : PropTypes.number,
    searchIncrement: PropTypes.number,
    searchQuota : PropTypes.number, // number of assets we'll search through to find 360 content.
    /*
     This callback function takes 2 arguments:

     index: index of which photo was selected

     source : pass this into the source property of React or Viro image components
     */
    onPhotoSelected : PropTypes.func,
  }

  // default props
  static defaultProps = {
    columns : 3,
    rows : 2,
    searchIncrement: 12,
    searchQuota : 24,
  }

  // default state
  state = {
    scrollViewWidth : 0, // width of the scrollView itself
    scrollViewHeight : 0, // height of the scrollView itself
  }

  componentDidMount() {
    this._updateDataSource();
    this._getCameraRollAssets();
  }

  constructor(props) {
    super(props);

    // these are kinda like instance variables...
    // just using the fact that everything's an object in JS.
    this.user360Photos = [];
    this.userPhotos = [];
    this.fetchCount = 0;
    this.isFetching = false;
    this.fetchedAllPhotos = false;
    this.selectedTab = TAB_STOCK;
    this.selectedRow = -1;
    this.selectedColumn = -1;
    this.refList = {};

    // initialize some more state
    var dataSource = new ListView.DataSource({rowHasChanged: (r1, r2) => r1 !== r2});
    this.state.dataSource = dataSource;

    // binding functions to this
    this._updateDataSource = this._updateDataSource.bind(this)
    this._getPhotos = this._getPhotos.bind(this);
    this._storeDimensions = this._storeDimensions.bind(this);
    this._getRow = this._getRow.bind(this);
    this._getTabBar = this._getTabBar.bind(this);
    this._getRowElements = this._getRowElements.bind(this);
    this._getImageIndex = this._getImageIndex.bind(this);
    this._getElementClick = this._getElementClick.bind(this);
    this._getCameraRollAssets = this._getCameraRollAssets.bind(this);
    this._getTabPress = this._getTabPress.bind(this);
    this._filterPhotos = this._filterPhotos.bind(this);
    this._storeRef = this._storeRef.bind(this);
    this._getRefKey = this._getRefKey.bind(this);
    this._updatePhotoSelection = this._updatePhotoSelection.bind(this);
  }

  _getPhotos() {
    if (this.selectedTab == TAB_STOCK) {
      return STOCK_360_PHOTOS;
    } else if (this.selectedTab == TAB_360) {
      return this.user360Photos;
    } else if (this.selectedTab == TAB_RECENT) {
      return this.userPhotos;
    }
  }

  _updateDataSource(onStateSetFunc) {
    let data = this._getPhotos();
    let newData = [];
    for (var i = 0; i < data.length; i+=this.props.columns) {
      let subArray = data.slice(i, i + this.props.columns);
      newData.push(subArray);
    }
    this.setState({
      dataSource : this.state.dataSource.cloneWithRows(newData)
    }, onStateSetFunc)
  }

  // -- Render functions --

  render() {

    return(
      <View style={[localStyles.outerContainer, this.props.style]}>
        <ListView style={localStyles.scrollView} dataSource={this.state.dataSource}
          renderRow={this._getRow} onLayout={this._storeDimensions}/>
        {this._getTabBar()}
      </View>
    );
  }

  _storeDimensions(event) {
    var {x, y, width, height} = event.nativeEvent.layout;
    this.setState({
      scrollViewWidth : width,
      scrollViewHeight : height
    })
  }

  _getRow(data, sectionId, rowIndex) {
    if (rowIndex == this.state.dataSource.getRowCount() - 1 && this.selectedTab == TAB_RECENT
        && !this.isFetching && !this.fetchedAllPhotos) {
      this._getCameraRollAssets();
    }

    let height = this.state.scrollViewHeight / this.props.rows
    return (
      <View key={ROW_PREFIX + rowIndex} style={[localStyles.rowContainer, {height : height}]} >
        {this._getRowElements(data, rowIndex)}
      </View>)
  }

  // row number, 0th indexed.
  _getRowElements(data, rowNumber) {
    let toReturn = [];
    for (var i = 0; i < this.props.columns; i++) {
      toReturn.push((
        <View key={COLUMN_PREFIX + this.selectedTab + i} style={localStyles.rowElement} >
          {this._getImageIndex(data[i], rowNumber, i)}
        </View>
      ));
    }
    return toReturn;
  }

  _getImageIndex(data, row, column) {
    // data could be undefined... if it is, do nothing!
    if (data) {
      let index = this.props.columns * row + column;
      return (
        <TouchableOpacity style={[localStyles.touchElement]} activeOpacity={.5} focusedOpacity={.5} onPress={this._getElementClick(row, column, data)} >
          <Image style={localStyles.fetchedImageStyle} source={data.source} ref={this._storeRef(row, column)}
              onError={(error)=>{console.log("[PhotoSelector] load image error: " + error.nativeEvent.error)}} />
        </TouchableOpacity>
      )
    }
  }

  _storeRef(row, column) {
    return (ref) => {
      let key = this._getRefKey(row, column);
      this.refList[key] = ref;
    }
  }

  _getRefKey(row, column) {
    return REF_PREFIX + REF_DELIM + row + REF_DELIM + column;
  }

  _getElementClick(row, column, source) {
    let photos = this._getPhotos();
    return () => {
      let index = this.props.columns * row + column;
      if (row == this.selectedRow && column == this.selectedColumn) {
        return;
      }
      console.log("[PhotoSelector] user selected index " + index);
      this._updatePhotoSelection(row, column);
      this.props.onPhotoSelected && this.props.onPhotoSelected(index, source);
    }
  }

  _updatePhotoSelection(row, column) {

    let previousSelection = this.refList[this._getRefKey(this.selectedRow, this.selectedColumn)];
    if (previousSelection) {
      previousSelection.setNativeProps({ style : { opacity : 1 } });
    }

    let newSelection = this.refList[this._getRefKey(row, column)];
    if (newSelection) {
      newSelection.setNativeProps({ style: { opacity : .5 } });
    }

    this.selectedRow = row;
    this.selectedColumn = column;
  }

  _getTabBar() {
    return(
      <View style={localStyles.tabBarContainer}>
        <TouchableOpacity style={localStyles.tabTouch} activeOpacity={.5} onPress={this._getTabPress(TAB_STOCK)} >
          <Text style={localStyles.tabBarText}>Stock</Text>
        </TouchableOpacity>

        <TouchableOpacity style={localStyles.tabTouch} activeOpacity={.5} onPress={this._getTabPress(TAB_360)} >
          <Text style={localStyles.tabBarText}>360°</Text>
        </TouchableOpacity>

        <TouchableOpacity style={localStyles.tabTouch} activeOpacity={.5} onPress={this._getTabPress(TAB_RECENT)} >
          <Text style={localStyles.tabBarText}>Recent</Text>
        </TouchableOpacity>
      </View>
    )
  }

  _getTabPress(type) {
    return () => {

      if (this.selectedTab == type) {
        return; // do nothing!
      }

      this.refList = {};
      this.selectedTab = type;
      this._updatePhotoSelection(-1,-1);
      this._updateDataSource();
    }
  }

  // -- Camera Roll processing --
  // https://facebook.github.io/react-native/docs/cameraroll.html

  _getCameraRollAssets() {
    this.isFetching = true;
    console.log("[PhotoSelector] fetching Camera Roll assets.");
    CameraRoll.getPhotos({
      first: this.props.searchIncrement,
      assetType : 'Photos',
      after : this.endCursor,
      groupTypes : 'All'
    }).then((retValue)=>{
      var numResults = retValue.edges.length;
      console.log("[PhotoSelector] got " + numResults + " Camera Roll assets.");
      var photos360 = [];
      var photos = [];

      // the function takes the two arrays and filters the photos into them and also culls extraneous info.
      this._filterPhotos(retValue.edges, photos360, photos);

      console.log("[PhotoSelector] after filtering, found " + photos360.length + " 360 photos and " + photos.length + " non-360 photos");

      this.user360Photos.push(...photos360);
      this.userPhotos.push(...photos);
      this.endCursor = retValue.page_info.end_cursor;
      this.fetchCount += numResults;

      if (!retValue.page_info.has_next_page) {
        this.fetchedAllPhotos = true;
      }

      // continue getting assets if we've not reached the search quota and we didn't run out of results.
      if ((this.fetchCount < this.props.searchQuota) && (retValue.page_info.has_next_page)) {
        this._updateDataSource(()=>{this._getCameraRollAssets()});
      } else {
        this.isFetching = false;
        this._updateDataSource();
      }
    }).catch((err)=>{
      console.log("[PhotoSelector] error while fetching Camera Roll assets: " + err.message);
      // TODO: handle photo selector assets error
    })
  }

  _filterPhotos(edges, photos360, photos) {
    for (var i = 0; i < edges.length; i++) {
      let edge = edges[i];

      let image = {
        source : { uri : edge.node.image.uri},
        width : edge.node.image.width,
        height : edge.node.image.height
      }
      if (this._is360Photo(edge)) {
        photos360.push(image);
      } else {
        photos.push(image);
      }
    }
  }

  _is360Photo(edge) {
    let ratio = edge.node.image.width / edge.node.image.height;
    return ratio > 1.9 && ratio < 2.2;
  }

}

var localStyles = StyleSheet.create({
  outerContainer : {
    flexDirection : 'column',
    backgroundColor : 'white',
  },
  scrollView: {
    height: '80%',
    width: '100%',
    borderWidth: 1,
    borderColor: 'white',
  },
  rowContainer : {
    flexDirection : 'row',
    width: '100%'
  },
  tabBarContainer: {
    flex : 1,
    flexDirection : 'row',
    justifyContent: 'space-around',
    alignItems: 'center',
  },
  tabTouch : {
    flex : 1,
    flexDirection : 'row',
    height : '100%',
    alignItems: 'center',
    backgroundColor : '#AAAAAA',
  },
  tabBarText:{
    flex : 1,
    fontSize : 20,
    textAlign: 'center',
  },
  rowElement : {
    flex : 1,
    borderWidth: .5,
    borderColor: 'white',
  },
  touchElement : {
    width : '100%',
    height : '100%',
    justifyContent: 'center',
    alignItems: 'center',
  },
  fetchedImageStyle : {
    position : 'absolute',
    top : 0,
    left : 0,
    bottom : 0,
    right : 0,
    width: undefined, // react-native is weird, for images we need to set width/height undefined.
    height: undefined,
    resizeMode : "cover"
  },
  opaque: {
    position : 'absolute',
    top : 0,
    left : 0,
    bottom : 0,
    right : 0,
    backgroundColor : "#ffffff99"
  },
  invisible: {
    position : 'absolute',
    top : 0,
    left : 0,
    bottom : 0,
    right : 0,
    backgroundColor : "#00000000"
  },
  selectedIcon : {
    position : 'absolute',
    bottom : 0,
    right : 0,
    width : 50,
    height : 50,
    resizeMode : "contain"
  }
});

module.exports = PhotosSelector;
