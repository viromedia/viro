/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */
import * as ModelData from  '../../model/ModelItems';
import * as PortalData from  '../../model/PortalItems';

 const initialState = {
   modelItems: ModelData.getInitModelArray(),
   portalItems: PortalData.getInitPortalArray(),
   effectItems: undefined,
 }


 function toggleItemSelected(state = {}, action) {
   switch (action.type) {
     case 'TOGGLE_MODEL_SELECTED':
     case 'TOGGLE_PORTAL_SELECTED':
       return {
         ...state,
         selected: !state.selected,
       };
     default:
       return state;
   }
 }

 function changeLoadState(state = {}, action) {
   switch (action.type) {
     case 'CHANGE_MODEL_LOAD_STATE':
     case 'CHANGE_PORTAL_LOAD_STATE':
      console.log("Changing load state to:" + action.loadState);
       return {
         ...state,
         loading: action.loadState,
       };
     default:
       return state;
   }
 }

 function changePortalPhoto(state = {}, action) {
    switch (action.type) {
        case 'CHANGE_PORTAL_PHOTO':
          return {
            ...state,
            portal360Image: action.photoSource,
          };
        default:
          return state;
    }
 }

function modifyItem(state = [], action) {
    switch (action.type) {
      case 'CHANGE_PORTAL_PHOTO':
        return [
          ...state.slice(0, action.index),
          changePortalPhoto(state[action.index], action),
          ...state.slice(parseInt(action.index) +1),
        ]
      case 'TOGGLE_MODEL_SELECTED':
      case 'TOGGLE_PORTAL_SELECTED':
        return [
          ...state.slice(0, action.index),
          toggleItemSelected(state[action.index], action),
          ...state.slice(parseInt(action.index) +1),
        ]

      case 'CHANGE_MODEL_LOAD_STATE':
      case 'CHANGE_PORTAL_LOAD_STATE':
        return [
          ...state.slice(0, action.index),
          changeLoadState(state[action.index], action),
          ...state.slice(parseInt(action.index) +1),
        ];
      default:
        return state;
    }
}

function arobjects(state = initialState, action) {
  switch (action.type) {
    case 'TOGGLE_MODEL_SELECTED':
    case 'CHANGE_MODEL_LOAD_STATE':
      console.log("Figment: EXECUTING REDUCER ADDAROBJECT: " + action.index);
      var updatedModelInfo = modifyItem(state.modelItems.slice(0), action);
      console.log("Figment: REDUCER NEW STATE:");
      console.log("Figment: " + updatedModelInfo);
      return {
        ...state,
        modelItems: updatedModelInfo.slice(0),
      };
    case 'CHANGE_PORTAL_LOAD_STATE':
    case 'TOGGLE_PORTAL_SELECTED':
    case 'CHANGE_PORTAL_PHOTO':
        var updatedPortalInfo = modifyItem(state.portalItems.slice(0), action);
        return {
          ...state,
          portalItems: updatedPortalInfo.slice(0),
      }
    default:
      return state;
  }
}

module.exports = arobjects;
