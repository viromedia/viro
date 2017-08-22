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

var STOCK_360_PHOTOS = [
  {
    source : require('../res/360_diving.jpg')
  },
  {
    source : require('../res/360_guadalupe.jpg')
  },
  {
    source : require('../res/360_space.jpg')
  },
  {
    source : require('../res/360_waikiki.jpg')
  },
  {
    source : require('../res/360_westlake.jpg')
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

     image: createStrictShapeTypeChecker({
       uri: PropTypes.string.isRequired,
       height: PropTypes.number.isRequired,
       width: PropTypes.number.isRequired,
       isStored: PropTypes.bool,
     })
     */
    onPhotoSelected : PropTypes.func,
  }

  // default props
  static defaultProps = {
    columns : 3,
    rows : 2,
    searchIncrement: 20,
    searchQuota : 60,
  }

  // default state
  state = {
    scrollViewWidth : 0, // width of the scrollView itself
    scrollViewHeight : 0, // height of the scrollView itself
    user360Photos : [],
    userPhotos : [], // non-360 photos
    shouldSearchContinue : true,
    fetchCount : 0,
    selectedIndex : -1,
    selectedTab : TAB_STOCK,
  }

  componentDidMount() {
    this._getCameraRollAssets();
  }

  constructor(props) {
    super(props);

    // binding functions to this
    this._getPhotos = this._getPhotos.bind(this);
    this._storeDimensions = this._storeDimensions.bind(this);
    this._getRows = this._getRows.bind(this);
    this._getTabBar = this._getTabBar.bind(this);
    this._getRowElements = this._getRowElements.bind(this);
    this._getImageIndex = this._getImageIndex.bind(this);
    this._getElementClick = this._getElementClick.bind(this);
    this._getCameraRollAssets = this._getCameraRollAssets.bind(this);
    this._getTabPress = this._getTabPress.bind(this);
    this._filterPhotos = this._filterPhotos.bind(this);
  }

  _getPhotos() {
    if (this.state.selectedTab == TAB_STOCK) {
      return STOCK_360_PHOTOS;
    } else if (this.state.selectedTab == TAB_360) {
      return this.state.user360Photos;
    } else if (this.state.selectedTab == TAB_RECENT) {
      return this.state.userPhotos;
    }
  }

  // -- Render functions --

  render() {
    return(
      <View style={[localStyles.outerContainer, this.props.style]}>
        <ScrollView style={localStyles.scrollView} onLayout={this._storeDimensions} bounces={true}>
          {this._getRows()}
        </ScrollView>
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

  _getRows() {
    let toReturn = [];
    let height = this.state.scrollViewHeight / this.props.rows
    let rows = this._getPhotos().length / this.props.columns + 1

    for (var i = 0; i < parseInt(rows); i++) {
      toReturn.push((
        <View key={ROW_PREFIX + i} style={[localStyles.rowContainer, {height : height}]}>
          {this._getRowElements(i)}
        </View>
      ));
    }
    return toReturn;
  }

  // row number, 0th indexed.
  _getRowElements(rowNumber) {
    let toReturn = [];
    for (var i = 0; i < this.props.columns; i++) {
      var index = this.props.columns * rowNumber + i;
      toReturn.push((
        <View key={COLUMN_PREFIX + this.state.selectedTab + i} style={localStyles.rowElement} >
          {this._getImageIndex(index)}
        </View>
      ));
    }
    return toReturn;
  }

  _getImageIndex(index) {
    let photos = this._getPhotos();
    if (index < photos.length) {
      let opacity = index == this.state.selectedIndex ? {opacity : .5} : {opacity : 1};
      return (
        <TouchableOpacity style={[localStyles.touchElement]} activeOpacity={.5} focusedOpacity={.5} onPress={this._getElementClick(index)} >
          <Image style={[localStyles.fetchedImageStyle, opacity]} source={photos[index].source}
              onError={(error)=>{console.log("[PhotoSelector] load image error: " + error.nativeEvent.error)}} />
        </TouchableOpacity>
      )
    }
  }

  _getElementClick(index) {
    let photos = this._getPhotos();
    if (index < photos.length) {
      return () => {
        if (index == this.state.selectedIndex) {
          return;
        }
        console.log("[PhotoSelector] user selected index " + index);
        this.setState({
          selectedIndex : index
        })
        this.props.onPhotoSelected && this.props.onPhotoSelected(index, photos[index].source);
      }
    }
    return ()=>{}
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

      if (this.state.selectedTab == type) {
        return; // do nothing!
      }

      this.setState({
        selectedIndex : -1,
        selectedTab : type,
      })
    }
  }

  // -- Camera Roll processing --
  // https://facebook.github.io/react-native/docs/cameraroll.html

  _getCameraRollAssets() {
    if (!this.state.shouldSearchContinue) {
      return;
    }

    console.log("[PhotoSelector] fetching Camera Roll assets.");
    CameraRoll.getPhotos({
      first: this.props.searchIncrement,
      assetType : 'Photos',
      after : this.state.endCursor,
      groupTypes : 'All'
    }).then((retValue)=>{
      var numResults = retValue.edges.length;
      console.log("[PhotoSelector] got " + numResults + " Camera Roll assets.");
      var photos360 = [];
      var photos = [];

      // the function takes the two arrays and filters the photos into them and also culls extraneous info.
      this._filterPhotos(retValue.edges, photos360, photos);

      console.log("[PhotoSelector] after filtering, found " + photos360.length + " 360 photos and " + photos.length + " non-360 photos");

      // we should continue search if we haven't met quota AND if there are still more results to be found
      var shouldSearchContinue = (this.state.fetchCount + numResults) < this.props.searchQuota && numResults == this.props.searchIncrement;

      this.setState({
        user360Photos : [...this.state.user360Photos, ...photos360],
        userPhotos : [...this.state.userPhotos, ...photos],
        endCursor : retValue.page_info.end_cursor,
        fetchCount : this.state.fetchCount + numResults,
        shouldSearchContinue,
      }, ()=>{this._getCameraRollAssets()}); // run this function again once the state settles.
    }).catch((err)=>{
      console.log("[PhotoSelector] error while fetching Camera Roll assets: " + err.message);
      // TODO: handle photo selector assets error
    })
  }

  _filterPhotos(edges, photos360, photos) {
    for (var i = 0; i < edges.length; i++) {
      let edge = edges[i];
      if (this._is360Photo(edge)) {
        photos360.push({source : { uri : edge.node.image.uri}});
      } else {
        photos.push({source : { uri : edge.node.image.uri}});
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

