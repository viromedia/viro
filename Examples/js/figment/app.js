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
import {toggleModelSelection, changeLoadState, removeARObject, displayUIScreen } from './redux/actions';
import * as LoadingConstants from './redux/LoadingStateConstants';
import * as UIConstants from './redux/UIConstants';
import renderIf from './helpers/renderIf';
import ButtonComponent from './component/ButtonComponent';
import FigmentListView from './component/FigmentListView';

const kObjSelectMode = 1;
const kPortalSelectMode = 2;
const kEffectSelectMode = 3;

import {
  AppRegistry,
  Text,
  View,
  StyleSheet,
  PixelRatio,
  ListView,
  Image,
  TouchableHighlight,
  ActivityIndicator,
  ActionSheetIOS,
} from 'react-native';

import {
  ViroARSceneNavigator,
} from 'react-viro';

var InitialScene = require('./figment');

export class App extends Component {

  constructor(props) {
    super(props);
    this.state = {
      text : "Submit",
      objSelection: -1,
    }

    this._renderButtonLeftMenu = this._renderButtonLeftMenu.bind(this);
    this._renderRecord = this._renderRecord.bind(this);
    this._startRecording = this._startRecording.bind(this);
    this._stopRecording = this._stopRecording.bind(this);
    this._setARNavigatorRef = this._setARNavigatorRef.bind(this);
    this._onListItemLoaded = this._onListItemLoaded.bind(this);
    this._onListPressed = this._onListPressed.bind(this),

    this.state = {
      currentModeSelected:kObjSelectMode,
      videoUrl: null,
      viroAppProps: {loadingObjectCallback: this._onListItemLoaded}
    };
  }

  render() {
      console.log("RERENDER App OCCURRED");
      return (
        <View style={localStyles.flex}>
          <ViroARSceneNavigator style={localStyles.aRContainer} apiKey="7EEDCB99-2C3B-4681-AE17-17BC165BF792"
            initialScene={{scene: InitialScene}}  ref={this._setARNavigatorRef} viroAppProps={this.state.viroAppProps}
            />
            {renderIf(this.props.currentScreen != UIConstants.SHOW_SHARE_SCREEN,
            <View style={{height: 100}}>
              <FigmentListView items={this.props.modelItems} onPress={this._onListPressed} />
            </View>)}

          {this._renderButtonLeftMenu()}
          {this._renderRecord()}
          {this._renderShareScreenButtons()}
        </View>
      );
    }

  _setARNavigatorRef(ARNavigator){
    console.log("SETTING ARNAVIGATOR REF!!");
    this._arNavigator = ARNavigator;
  }

  _renderShareScreenButtons() {
    var shareScreenButtons = [];
    if(this.props.currentScreen == UIConstants.SHOW_SHARE_SCREEN) {
        shareScreenButtons.push(<View key="btn_save" style={{position:'absolute', left:10, bottom:10, width:100, height:100}}>
            <TouchableHighlight onPress={()=>{}}>
              <Image source={require("./res/btn_save.png")} style={localStyles.photo} />
            </TouchableHighlight>
            </View>);

        shareScreenButtons.push(<View key="btn_share" style={{position:'absolute', left:120, bottom:10, width:100, height:100}}>
                <TouchableHighlight onPress={()=>{this._openShareActionSheet()}}>
                  <Image source={require("./res/btn_share.png")} style={localStyles.photo} />
                </TouchableHighlight>
                </View>);

        shareScreenButtons.push(<View key="btn_close" style={{position:'absolute', left:10, top:10, width:100, height:100}}>
                        <TouchableHighlight onPress={()=>{this.props.dispatchDisplayUIScreen(UIConstants.SHOW_MAIN_SCREEN)}}>
                          <Image source={require("./res/btn_close.png")} style={localStyles.photo} />
                        </TouchableHighlight>
                        </View>);
    }
    return shareScreenButtons;
  }

  _renderButtonLeftMenu() {
    var buttons = [];
    // render the object mode button.
    if(this.props.currentScreen != UIConstants.SHOW_SHARE_SCREEN) {
      buttons.push(<View key="modebuttonobj" style={{position:'absolute', flex: 1, left:10, bottom:110, width:100, height:100}}>
          <TouchableHighlight onPress={()=>{}}>
            <Image source={require("./res/btn_mode_objects_on.png")} style={localStyles.photo} />
          </TouchableHighlight>
          </View>);

      buttons.push(<View key="modebuttoneffects" style={{position:'absolute', flex: 1, left:10, bottom:210, width:100, height:100}}>
          <TouchableHighlight onPress={()=>{}}>
              <Image source={require("./res/btn_mode_effects.png")} style={localStyles.photo} />
          </TouchableHighlight>
          </View>);

      buttons.push(<View key="modebuttonportals" style={{position:'absolute', flex: 1, left:10, bottom:310, width:100, height:100}}>
            <TouchableHighlight onPress={()=>{}}>
                <Image source={require("./res/btn_mode_portals.png")} style={localStyles.photo} />
            </TouchableHighlight>
            </View>);
    }
    return buttons;
  }

  _renderRecord() {
    var recordViews = [];

    if(this.props.currentScreen == UIConstants.SHOW_RECORDING_SCREEN) {
      recordViews.push(
          <View  key="record_timeline" style={{position: 'absolute', backgroundColor: '#22222244', left: 0, right: 0, top: 0, height:100,  alignSelf: 'stretch', }}>
            <Text style={localStyles.recordingTimeText}>00:01:00</Text>
          </View>
      );
    }

    if(this.props.currentScreen != UIConstants.SHOW_SHARE_SCREEN) {
      console.log("Current screen:" + this.props.currentScreen);
      recordViews.push(
        <View key="record_button_container" style={{position: 'absolute',  left: 0, right: 0, bottom: 110,  alignItems: 'center'}}>
          <ButtonComponent
              key="record_button" onPress={()=>{(this.props.currentScreen==UIConstants.SHOW_MAIN_SCREEN) ? this._startRecording(): this._stopRecording()}}
            buttonState={(this.props.currentScreen==UIConstants.SHOW_MAIN_SCREEN) ? 'off':'on'}
            stateImageArray={[require("./res/btn_stop.png"), require("./res/btn_record.png")]}
            style={localStyles.photo} />
        </View>);
    }
    return recordViews;
  }

  _startRecording() {
    console.log("[JS] begin recording!");
    this._arNavigator._startVideoRecording("testVid11", true,
       (errorCode)=>{
        this._displayVideoRecordAlert("Recording Error", "[JS] onError callback errorCode: " + errorCode);
        this.props.dispatchDisplayUIScreen(UIConstants.SHOW_MAIN_SCREEN);
        });
    this.props.dispatchDisplayUIScreen(UIConstants.SHOW_RECORDING_SCREEN);
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
      });
      this.props.dispatchDisplayUIScreen(UIConstants.SHOW_SHARE_SCREEN);
    });
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
    if(this.props.modelItems[index].selected == true) {
          this.props.dispatchChangeLoadState(index, LoadingConstants.NONE);
    }
    this.props.dispatchToggleModelSelection(index);
  }

  _onListItemLoaded(index, loadState) {
    console.log("Dispatching change load state:" + index + ", loadState:" + loadState);
    this.props.dispatchChangeLoadState(index, loadState);
  }

  _openShareActionSheet() {
      ActionSheetIOS.showShareActionSheetWithOptions({url:this.state.videoUrl},  (error) => alert(error),
      (success, method) => {
      var text;
      if (success) {
        text = `Shared via ${method}`;
      } else {
        text = 'You didn\'t share';
      }
      alert(text);
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

function selectProps(store) {
  console.log("SELECT PROPS INVOKED!!");
  console.log(store);
  return {
    modelItems: store.arobjects.modelItems,
    currentScreen: store.ui.currentScreen,
  };
}


//const mapStateToProps = (state) => {
//  return {
//  objIndex: state.objIndex,
//  }
//}

const mapDispatchToProps = (dispatch) => {
  return {
    dispatchToggleModelSelection: (index) => dispatch(toggleModelSelection(index)),
    dispatchChangeLoadState:(index, loadState) =>dispatch(changeLoadState(index, loadState)),
    dispatchremoveARObject: (index) => dispatch(removeARObject(index)),
    dispatchDisplayUIScreen: (uiScreenState) => dispatch(displayUIScreen(uiScreenState)),
  }
}

export default connect(selectProps, mapDispatchToProps)(App)
