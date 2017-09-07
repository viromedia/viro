/**
 * Copyright (c) 2017-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

import React, { Component } from 'react';


import { connect } from 'react-redux';
import { BlurView } from 'react-native-blur';
import {addPortalWithIndex, removePortalWithUUID, addModelWithIndex, removeAll, removeModelWithUUID,toggleEffectSelection, changePortalLoadState, changePortalPhoto, changeModelLoadState, changeItemClickState, switchListMode, removeARObject, displayUIScreen } from './redux/actions';
import TimerMixin from 'react-timer-mixin';

import * as LoadingConstants from './redux/LoadingStateConstants';
import * as UIConstants from './redux/UIConstants';
import renderIf from './helpers/renderIf';
import ButtonComponent from './component/ButtonComponent';
import RecordButton from './component/RecordButton';
import ContextMenuButton from './component/ContextMenuButton';
import FigmentListView from './component/FigmentListView';
import PhotosSelector from './component/PhotosSelector';
import * as ModelData from  './model/ModelItems';
import * as PortalData from  './model/PortalItems';

const kObjSelectMode = 1;
const kPortalSelectMode = 2;
const kEffectSelectMode = 3;

const kPreviewTypePhoto = 1;
const kPreviewTypeVideo = 2;


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
  Button,
  StatusBar,
} from 'react-native';

import {
  ViroARSceneNavigator,
  ViroConstants,
} from 'react-viro';

import Video from 'react-native-video';

var InitialScene = require('./figment');

export class App extends Component {

  constructor(props) {
    super(props);

    this._renderShareScreen = this._renderShareScreen.bind(this);
    this._renderButtonLeftMenu = this._renderButtonLeftMenu.bind(this);
    this._renderRecord = this._renderRecord.bind(this);
    this._startRecording = this._startRecording.bind(this);
    this._stopRecording = this._stopRecording.bind(this);
    this._setARNavigatorRef = this._setARNavigatorRef.bind(this);
    this._onListItemLoaded = this._onListItemLoaded.bind(this);
    this._onListPressed = this._onListPressed.bind(this);
    this._getListItems = this._getListItems.bind(this);
    this._saveToCameraRoll = this._saveToCameraRoll.bind(this);
    this._renderPhotosSelector = this._renderPhotosSelector.bind(this);
    this._takeScreenshot = this._takeScreenshot.bind(this);
    this._onPhotoSelected = this._onPhotoSelected.bind(this);
    this._onItemClickedInScene = this._onItemClickedInScene.bind(this);
    this._onContextMenuRemoveButtonPressed = this._onContextMenuRemoveButtonPressed.bind(this);
    this._startStopWatch = this._startStopWatch.bind(this);
    this._getLoadingforModelIndex = this._getLoadingforModelIndex.bind(this);
    this._constructListArrayModel = this._constructListArrayModel.bind(this);
    this._onContextClearAll = this._onContextClearAll.bind(this);

    this.state = {
      currentModeSelected:kObjSelectMode,
      videoUrl: null,
      haveSavedMedia: false,
      playPreview : false,
      viroAppProps: {loadingObjectCallback: this._onListItemLoaded, clickStateCallback: this._onItemClickedInScene},
      showPhotosSelector : false,
      previewType: kPreviewTypeVideo,
      lastSelectedPortalUUID: -1,
      timer:null,
      hours: '00',
      minutes: '00',
      seconds: '00',
      miliseconds: '00',

    };
  }

  render() {
      console.log("RERENDER App OCCURRED");
      return (
        <View style={localStyles.flex}>
          <StatusBar hidden={true} />
          <ViroARSceneNavigator style={localStyles.arView} apiKey="7EEDCB99-2C3B-4681-AE17-17BC165BF792"
            initialScene={{scene: InitialScene}}  ref={this._setARNavigatorRef} viroAppProps={this.state.viroAppProps}
            />


          {renderIf(this.props.currentScreen != UIConstants.SHOW_SHARE_SCREEN,
            <View style={localStyles.listView}>
              <BlurView style={localStyles.absolute} blurType="dark" blurAmount={10} />
              <FigmentListView items={this._getListItems()} onPress={this._onListPressed} />
            </View>
            )}
          

          {console.log("CurrentScreen = " + this.props.currentScreen)}
          {this._renderContextMenu()}
          {this._renderRecord()}        
          {this._renderButtonLeftMenu()}
          {this._renderShareScreen()}
          {this._renderPhotosSelector()}

        </View>
      );
  }

  _renderContextMenu() {
    console.log("_renderContextMenu + index: " + this.props.currentItemSelectionIndex + ", clickState: " + this.props.currentItemClickState);
    var selectedItemIndex = this.props.currentItemSelectionIndex;
    var clickState = this.props.currentItemClickState;
    var totalHeight = 120;
    if(this.props.currentSelectedItemType != UIConstants.LIST_MODE_PORTAL) {
        totalHeight = 80;
    }
    // If clickState == 2, start timer for 2 seconds, then dispatch state change to reset item selection
    if (selectedItemIndex != -1 && clickState == 2) {
      TimerMixin.setTimeout(
        () => {
          this.props.dispatchChangeItemClickState(-1, '', '');
        },
        3500
      );
    }
      return (
        <View style={{position:'absolute', flex:1, flexDirection:'column', justifyContent: 'space-between',alignItems: 'flex-end', right:10, top:20, width:80, height:totalHeight}}>
          {console.log("this.props.currentItemSelectionIndex in render - " + this.props.currentItemSelectionIndex)}
          {renderIf(this.props.currentItemSelectionIndex != -1 && (this.state.showPhotosSelector==false),
            <ContextMenuButton onPress={this._onContextMenuRemoveButtonPressed} 
                    stateImageArray={[require("./res/btn_trash.png")]}
                    style={localStyles.previewScreenButtons} />
          )}

          {renderIf(this.props.currentItemSelectionIndex != -1 && (this.state.showPhotosSelector==false),
            <ContextMenuButton onPress={this._onContextClearAll}
                    stateImageArray={[require("./res/btn_clear_all.png")]}
                    style={localStyles.previewScreenButtons} />
          )}

          {renderIf(this.props.currentItemSelectionIndex != -1 && (this.props.currentSelectedItemType == UIConstants.LIST_MODE_PORTAL) && (this.state.showPhotosSelector==false),
            <ContextMenuButton onPress={()=>{this.setState({showPhotosSelector:true, lastSelectedPortalUUID:this.props.currentItemSelectionIndex})}} 
                    stateImageArray={[require("./res/btn_add_pic.png")]}
                    style={localStyles.previewScreenButtons} />
          )}
        </View>

    );
  }

  _onContextMenuRemoveButtonPressed() {
    var index = this.props.currentItemSelectionIndex;
    console.log("_onContextMenuRemoveButtonPressed - index: " + this.props.currentItemSelectionIndex + ", clickState: " + this.props.currentItemClickState + ", type: " + this.props.currentSelectedItemType);
    if (this.props.currentItemSelectionIndex != -1 && this.props.currentItemClickState != '') {
      if (this.props.currentSelectedItemType == UIConstants.LIST_MODE_MODEL) {

        this.props.dispatchRemoveModelWithUUID(index);
      }

      if(this.props.currentSelectedItemType == UIConstants.LIST_MODE_PORTAL) {
        if(this.props.portalItems[index].selected == true) {
            this.props.dispatchChangePortalLoadState(index, LoadingConstants.NONE);
            this.setState({
              lastSelectedPortalUUID:-1,
            });
        }

        this.props.dispatchRemovePortalWithUUID(index);
      }
      this.props.dispatchChangeItemClickState(-1, '', '');

    }
  }

  _onContextClearAll() {
      Alert.alert(
        "Remove All Objects",
        "Are you sure you want to clear the entire scene?",
        [
          {text: 'Cancel', onPress: () => {}},
          {text: 'OK', onPress: () => this.props.dispatchRemoveAll()},
        ],
      );
  }

  _renderPhotosSelector() {
    // TODO: remove the return to render the selector when portal is tapped
    if (this.state.showPhotosSelector == true) {
      var photoSelectorViews = [];
        photoSelectorViews.push(<StatusBar key="statusBarKey" hidden={true} />);
        photoSelectorViews.push(<View key="topPhotoBar" style={localStyles.topPhotoBar}>
          <BlurView style={localStyles.absolute} blurType="dark" blurAmount={10} />
          <View style={{flex:1, backgroundColor:"#00000000"}}/>
            <Text style={localStyles.photosText}>My Photos</Text>
            <Text onPress={()=>{this.setState({showPhotosSelector:false})}}
                  style={localStyles.doneText}>Done</Text>
          </View>);
          photoSelectorViews.push(<PhotosSelector key="photosSelector" style={localStyles.photosSelectorStyle} rows={2.3} columns={4}
          onPhotoSelected={this._onPhotoSelected}/>);
          return photoSelectorViews;
    }
    return null;
  }

  _onPhotoSelected(index, source) {
    console.log("onPhotoSelected index: " + index + "source:");
    console.log(source);
    this.props.dispatchChangePortalPhoto(this.state.lastSelectedPortalUUID, source);
  }

  _setARNavigatorRef(ARNavigator){
    console.log("SETTING ARNAVIGATOR REF!!");
    this._arNavigator = ARNavigator;
  }

  _renderShareScreen() {
    if(this.props.currentScreen == UIConstants.SHOW_SHARE_SCREEN) {
      return (
        <View style={localStyles.shareScreenContainer} >

          {/* So with react-native-video, if you turn repeat to true and then onEnd pause
              the video, you'll end up with black screen. So we should simply not repeat
              and seek to 0 when we want to play the video again (seeking will auto start
              the video player too, but we set the state to true to dismiss the play btn)*/}

          {renderIf(this.state.previewType == kPreviewTypePhoto, <Image source={{uri:this.state.videoUrl}} style={localStyles.backgroundVideo} />)}

          {renderIf(this.state.previewType == kPreviewTypeVideo, <Video ref={(ref) => {this.player = ref}}
            source={{uri : this.state.videoUrl}} paused={!this.state.playPreview}
            repeat={false} style={localStyles.backgroundVideo}
            onEnd={()=>{this.setState({playPreview : false})}} />
          )}


          {renderIf(!this.state.playPreview && (this.state.previewType == kPreviewTypeVideo),
          <TouchableOpacity onPress={()=>{this.player.seek(0); this.setState({ playPreview : true })}} underlayColor="#00000000">
            <Image source={require("./res/btn_play.png")} style={localStyles.previewPlayButton} />
          </TouchableOpacity>
          )}

          <View style={{position:'absolute', left:25, bottom:15, width:30, height:30}}>
            <TouchableHighlight onPress={()=>{this._openShareActionSheet()}} underlayColor="#00000000">
              <Image source={require("./res/btn_share.png")} style={localStyles.previewScreenButtonShare} />
            </TouchableHighlight>
          </View>

          <View style={{position:'absolute', left:25, top:20, width:30, height:30}}>
            <TouchableHighlight onPress={()=>{this.props.dispatchDisplayUIScreen(UIConstants.SHOW_MAIN_SCREEN)}} underlayColor="#00000000">
              <Image source={require("./res/btn_close.png")} style={localStyles.previewScreenButtonClose} />
            </TouchableHighlight>
          </View>
        </View>
      )
    }
  }

  _renderButtonLeftMenu() {
    var buttons = [];
    // render the object mode button.
    buttons.push(
        <ButtonComponent key="button_portals"
          onPress={()=>{this.props.dispatchSwitchListMode(UIConstants.LIST_MODE_PORTAL, UIConstants.LIST_TITLE_PORTALS)}}
          buttonState={(this.props.listMode==UIConstants.LIST_MODE_PORTAL) ? 'on':'off'}
          stateImageArray={[require("./res/btn_mode_portals_on.png"), require("./res/btn_mode_portals.png")]}
          style={localStyles.screenIcon} selected={(this.props.listMode == UIConstants.LIST_MODE_PORTAL)}
          />);

    buttons.push(
        <ButtonComponent key="button_effects"
          onPress={()=>{this.props.dispatchSwitchListMode(UIConstants.LIST_MODE_EFFECT, UIConstants.LIST_TITLE_EFFECTS)}}
          buttonState={(this.props.listMode==UIConstants.LIST_MODE_EFFECT) ? 'on':'off'}
          stateImageArray={[require("./res/btn_mode_effects_on.png"), require("./res/btn_mode_effects.png")]}
          style={localStyles.screenIcon} selected={(this.props.listMode == UIConstants.LIST_MODE_EFFECT)}
          />);

    buttons.push(
        <ButtonComponent key="button_models"
            onPress={()=>{this.props.dispatchSwitchListMode(UIConstants.LIST_MODE_MODEL, UIConstants.LIST_TITLE_MODELS)}}
            buttonState={(this.props.listMode==UIConstants.LIST_MODE_MODEL) ? 'on':'off'}
            stateImageArray={[require("./res/btn_mode_objects_on.png"), require("./res/btn_mode_objects.png")]}
            style={localStyles.screenIcon} selected={(this.props.listMode == UIConstants.LIST_MODE_MODEL)}
            />);
    console.log("UIConstants." + this.props.currentScreen);
    if(this.props.currentScreen == UIConstants.SHOW_MAIN_SCREEN || this.props.currentScreen == UIConstants.SHOW_RECORDING_SCREEN) {
      if (this.state.showPhotosSelector==false) {
      return (
          /*Don't know why space-around works, but it works! Don't touch this. Magic.*/
           <View style={{position:'absolute', flexDirection:'column', justifyContent: 'space-around',left:10, bottom:107, width:70, height:160, flex:1}}>
              {buttons}
           </View>
        );
      }
    }
    return null;
  }

  _renderRecord() {
    var recordViews = [];

    if(this.props.currentScreen == UIConstants.SHOW_RECORDING_SCREEN) {
      recordViews.push(
        <View key="record_timeline" style={{position: 'absolute', backgroundColor: '#00000066', left: 0, right: 0, top: 0, height:34,  alignSelf: 'stretch', alignItems: 'center', justifyContent: 'center'}}>
          <Text style={localStyles.recordingTimeText}>{this.state.hours}:{this.state.minutes}:{this.state.seconds}</Text>
        </View>
      );
    }

    recordViews.push(
      <View key="record_button_container" style={{position: 'absolute', left: 0, right: 0, bottom: 77,  alignItems: 'center', height: 58}}>
        <View key="record_container" style={{left: 0, right: 0, bottom: 0, flexDirection: 'row', justifyContent: 'flex-end', alignItems:'center', height: 58, width: 120}}>
            <View key="container" style={{flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center', height: 58, width: 89}}>
          <RecordButton
            key="record_button" onPress={()=>{(this.props.currentScreen==UIConstants.SHOW_MAIN_SCREEN) ? this._startRecording(): this._stopRecording()}}
            buttonState={(this.props.currentScreen==UIConstants.SHOW_MAIN_SCREEN) ? 'off':'on'}
            stateImageArray={[require("./res/btn_stop.png"), require("./res/btn_record.png")]}
            style={localStyles.recordIcon}
            />
      {/* </View>);

      // recordViews.push(
      //   <View key="camera_button_container" style={{position: 'absolute',  right: 70, bottom: 85,  alignItems: 'center'}}>*/}
           <ButtonComponent
            key="camera_button" onPress={()=>{this._takeScreenshot()}}
            buttonState={(this.props.currentScreen==UIConstants.SHOW_MAIN_SCREEN) ? 'off':'on'}
            stateImageArray={[require("./res/btn_camera.png"), require("./res/btn_camera.png")]}
            style={localStyles.cameraIcon}
            />
            </View>
          </View>
        </View>);
    return recordViews;
  }

  _takeScreenshot() {
    this._arNavigator._takeScreenshot("figment_still", false).then((retDict)=>{
      if (!retDict.success) {
        if (retDict.errorCode == ViroConstants.RECORD_ERROR_NO_PERMISSION) {
          this._displayVideoRecordAlert("Screenshot Error", "Please allow camera permissions!" + errorCode);
        }
      }

      this.setState({
        videoUrl: "file://" + retDict.url,
        haveSavedMedia : false,
        playPreview : false,
        previewType: kPreviewTypePhoto,
      });
      this.props.dispatchDisplayUIScreen(UIConstants.SHOW_SHARE_SCREEN);
    });
  }

  _startRecording() {
    console.log("[JS] begin recording!");
    this._arNavigator._startVideoRecording("figment_video", false,
       (errorCode)=>{
        this._displayVideoRecordAlert("Recording Error", "[JS] onError callback errorCode: " + errorCode);
        this.props.dispatchDisplayUIScreen(UIConstants.SHOW_MAIN_SCREEN);
        });
    this.props.dispatchDisplayUIScreen(UIConstants.SHOW_RECORDING_SCREEN);

    this._startStopWatch();
  }

  _startStopWatch() {
    // Stopwatch at the top while recording
    let timer = TimerMixin.setInterval(() => {

      var seconds = (Number(this.state.seconds) + 1).toString(),
          minutes = this.state.minutes,
          hours = this.state.hours;

      if ( Number(this.state.seconds) == 59) {
        minutes = (Number(this.state.minutes) + 1).toString();
        seconds = '00';
      }

      if ( Number(this.state.minutes) == 59) {
        hours = (Number(this.state.hours) + 1).toString();
        minutes = '00';
        seconds = '00';
      }

      this.setState({
        hours: hours.length == 1 ? '0' + hours: hours,
        minutes : minutes.length == 1 ? '0' + minutes: minutes,
        seconds : seconds.length == 1 ? '0' + seconds: seconds,
      });
    }, 1000);
    this.setState({timer});
  }

  _stopRecording() {

    this._arNavigator._stopVideoRecording().then((retDict)=>{
      console.log("[JS] success? " + retDict.success);
      console.log("[JS] the url was: " + retDict.url);
      if (!retDict.success) {
        if (retDict.errorCode == ViroConstants.RECORD_ERROR_NO_PERMISSION) {
          this._displayVideoRecordAlert("Recording Error", "Please allow camera record permissions!" + errorCode);
        }
      }
      this.setState({
        videoUrl: "file://" + retDict.url,
        haveSavedMedia : false,
        playPreview : true,
        previewType: kPreviewTypeVideo,
      });
      this.props.dispatchDisplayUIScreen(UIConstants.SHOW_SHARE_SCREEN);

      // Stop stop watch at the top
      clearInterval(this.state.timer);
      this.setState({
          hours: '00',
          minutes: '00',
          seconds: '00',
          miliseconds: '00',
        });

    });
  }

  _saveToCameraRoll() {
    if (this.state.videoUrl != undefined && !this.state.haveSavedMedia) {
      this.setState({
        haveSavedMedia : true
      })
      CameraRoll.saveToCameraRoll(this.state.videoUrl)
        .then(Alert.alert('Success!', 'Added to Camera Roll.', [{text: 'OK'}]))
        .catch((err) => {console.log('Error saving to Camera Roll:', err)})
    }
  }

  _displayVideoRecordAlert(title, message) {
    Alert.alert(
      title,
      message,
      [
        {text: 'OK', onPress: () => this.props.dispatchDisplayUIScreen(UIConstants.SHOW_MAIN_SCREEN)},
      ],
      { cancelable: false }
    )
  }

  _onListPressed(index) {
    if(this.props.listMode == UIConstants.LIST_MODE_MODEL) {
      this.props.dispatchAddModel(index);
    }

    if(this.props.listMode == UIConstants.LIST_MODE_PORTAL) {
        this.props.dispatchAddPortal(index);
    }

    if(this.props.listMode == UIConstants.LIST_MODE_EFFECT) {
      this.props.dispatchToggleEffectSelection(index);
    }
  }

  _onListItemLoaded(index, loadState) {
    console.log("Dispatching change load state:" + index + ", loadState:" + loadState);
    if(this.props.listMode == UIConstants.LIST_MODE_MODEL) {
      this.props.dispatchChangeModelLoadState(index, loadState);
    }

    if(this.props.listMode == UIConstants.LIST_MODE_PORTAL) {
      this.props.dispatchChangePortalLoadState(index, loadState);
    }
  }

  _onItemClickedInScene(index, clickState, itemType) {
    console.log("Dispatching item clicked state: " + index + ", clickState:" + clickState);
    this.props.dispatchChangeItemClickState(index, clickState, itemType);
  }

  _getListItems() {
    if(this.props.listMode == UIConstants.LIST_MODE_MODEL) {
      return this._constructListArrayModel(ModelData.getModelArray(), this.props.modelItems);
    }else if(this.props.listMode == UIConstants.LIST_MODE_PORTAL) {
      return this._constructListArrayModel(PortalData.getPortalArray(), this.props.portalItems);
    } else if(this.props.listMode == UIConstants.LIST_MODE_EFFECT) {
      return this.props.effectItems;
    }
  }

  _constructListArrayModel(sourceArray, items) {
      var listArrayModel = [];
      // This is really slow, optimize by indexing index of model to loading variable.
      for(var i =0; i<sourceArray.length; i++) {
          listArrayModel.push({icon_img:sourceArray[i].icon_img, loading:this._getLoadingforModelIndex(i, items)})
      }
     return listArrayModel;
  }

  _getLoadingforModelIndex(index, items) {
    console.log("_getLoadingforModelIndex invoked!");
    if(items == null || items == undefined) {
      return LoadingConstants.NONE;
    }
    var loadingConstant = LoadingConstants.NONE;

    Object.keys(items).forEach(function(currentKey) {
      if(items[currentKey] != null && items[currentKey] != undefined) {
        if(items[currentKey].loading != LoadingConstants.NONE && items[currentKey].index == index){
          console.log("_getLoadingforModelIndex returning " + items[currentKey].loading);
          loadingConstant = items[currentKey].loading;
        }
      }
    });

    console.log("_getLoadingforModelIndex returning LoadingConstants.NONE");
    return loadingConstant;
  }

  _openShareActionSheet() {
      ActionSheetIOS.showShareActionSheetWithOptions({url:this.state.videoUrl, message: "Shared by Figment AR", subject: "Made by Figment AR"},  (error) => alert(error),
      (success, method) => {
      // For now nothing; This is called when the "Share screen returns"
    });
  }
}

App.propTypes =  {
  objIndex: React.PropTypes.number.isRequired,
}

App.defaultProps =  {
  objIndex: -1,
}

var localStyles = StyleSheet.create({
  flex : {
    flex : 1,
  },
  arView: {
    flex:1,
  },
  listView: {
    flex:1,
    height : 72,
    width : '100%',
    position : 'absolute',
    justifyContent: 'center',
    alignItems: 'center',
    bottom : 0,
  },
  topPhotoBar: {
    height : 50,
    width : '100%',
    position : 'absolute',
    top : 0,
    flexDirection: 'row',
    justifyContent: 'space-between',
  },
  listViewText: {
    textAlign: 'left',
    color: '#d6d6d6',
    fontFamily: 'Helvetica Neue',
    marginLeft:25,
    marginTop:5,
    marginBottom: 5,
    backgroundColor: '#00000000',
  },
  doneText: {
    textAlign: 'right',
    color: '#d6d6d6',
    fontWeight: 'bold',
    fontFamily: 'Helvetica Neue',
    fontSize: 16,
    marginTop:20,
    marginRight:10,
    marginBottom: 5,
    backgroundColor: '#00000000',
    flex:1,
  },
  photosText: {
    textAlign: 'center',
    color: '#d6d6d6',
    fontFamily: 'Helvetica Neue',
    fontSize: 16,
    marginTop:20,
    marginBottom: 5,
    backgroundColor: '#00000000',
    flex:1,
  },
  previewScreenButtons: {
    height: 30,
    width: 30,
    justifyContent: 'center',
    alignItems: 'center',
  },
  previewScreenButtonClose: {
    height: 20,
    width: 20,
    justifyContent: 'center',
    alignItems: 'center',
  },

  previewScreenButtonShare: {
    height: 30,
    width: 30,
    borderRadius: 20,
    justifyContent: 'center',
    alignItems: 'center',
  },
  screenIcon: {
    position : 'absolute',
    height: 58,
    width: 58,
  },
    recordIcon: {
    position : 'absolute',
    height: 58,
    width: 58,
    top: -29,
  },
  cameraIcon: {
    position : 'absolute',
    height: 30,
    width: 30,
    top: -15,
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
    color: '#d6d6d6',
    fontFamily: 'Helvetica Neue',
    fontSize:16,
  },
  previewPlayButton : {
    height : 60,
    width : 60,
  },
  shareScreenContainer: {
    position : 'absolute',
    top: 0,
    left: 0,
    bottom: 0,
    right: 0,
    backgroundColor : '#000000',
    flexDirection: 'column',
    justifyContent: 'center',
    alignItems: 'center',
  },
  shareScreenContainerTransparent: {
    position : 'absolute',
    top: 0,
    left: 0,
    bottom: 0,
    right: 0,
    backgroundColor : '#00000000',
    flexDirection: 'column',
    justifyContent: 'center',
    alignItems: 'center',
  },
  backgroundVideo: {
    position: 'absolute',
    top: 0,
    left: 0,
    bottom: 0,
    right: 0,
  },
  photosSelectorStyle : {
    position: 'absolute',
    width: '100%',
    height : '40%',
    bottom : 0
  },
  absolute: {
    position: "absolute",
    top: 0, left: 0, bottom: 0, right: 0,
  },

});

function selectProps(store) {
  console.log("STORE:");
  console.log(store.arobjects.modelItems);
  return {
    modelItems: store.arobjects.modelItems,
    portalItems: store.arobjects.portalItems,
    effectItems: store.arobjects.effectItems,
    currentScreen: store.ui.currentScreen,
    listMode: store.ui.listMode,
    listTitle: store.ui.listTitle,
    currentItemSelectionIndex: store.ui.currentItemSelectionIndex,
    currentItemClickState: store.ui.currentItemClickState,
    currentSelectedItemType: store.ui.currentSelectedItemType,
  };
}


//const mapStateToProps = (state) => {
//  return {
//  objIndex: state.objIndex,
//  }
//}

const mapDispatchToProps = (dispatch) => {
  return {
    dispatchAddPortal: (index) => dispatch(addPortalWithIndex(index)),
    dispatchRemovePortalWithUUID: (uuid) => dispatch(removePortalWithUUID(uuid)),
    dispatchAddModel: (index) => dispatch(addModelWithIndex(index)),
    dispatchRemoveModelWithUUID: (uuid) => dispatch(removeModelWithUUID(uuid)),
    dispatchRemoveAll:() => dispatch(removeAll()),
    dispatchToggleEffectSelection: (index) => dispatch(toggleEffectSelection(index)),
    dispatchChangeModelLoadState:(index, loadState) =>dispatch(changeModelLoadState(index, loadState)),
    dispatchChangePortalLoadState:(index, loadState) =>dispatch(changePortalLoadState(index, loadState)),
    dispatchDisplayUIScreen: (uiScreenState) => dispatch(displayUIScreen(uiScreenState)),
    dispatchSwitchListMode: (listMode, listTitle) =>dispatch(switchListMode(listMode, listTitle)),
    dispatchChangePortalPhoto:(index, source)=>dispatch(changePortalPhoto(index, source)),
    dispatchChangeItemClickState:(index, clickState, itemType) =>dispatch(changeItemClickState(index, clickState, itemType)),
  }
}

export default connect(selectProps, mapDispatchToProps)(App)
