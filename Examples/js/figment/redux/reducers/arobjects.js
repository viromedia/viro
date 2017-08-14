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

 const initialState = {
   modelItems: ModelData.getInitModelArray(),
   portalItems: undefined,
   effectItems: undefined,
 }


 function toggleModelSelected(state = {}, action) {
   switch (action.type) {
     case 'TOGGLE_MODEL_SELECTED':
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
     case 'CHANGE_LOAD_STATE':
      console.log("Changing load state to:" + action.loadState);
       return {
         ...state,
         loading: action.loadState,
       };
     default:
       return state;
   }
 }

function modifyModelItem(state = [], action) {
    switch (action.type) {
      case 'TOGGLE_MODEL_SELECTED':
      return [
        ...state.slice(0, action.index),
        toggleModelSelected(state[action.index], action),
        ...state.slice(parseInt(action.index) +1),
      ]

      case 'CHANGE_LOAD_STATE':
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
      console.log("EXECUTING REDUCER ADDAROBJECT: " + action.index);
      var updatedModelInfo = modifyModelItem(state.modelItems.slice(0), action);
      console.log("REDUCER NEW STATE:");
      console.log(updatedModelInfo);
      return {
        ...state,
        modelItems: updatedModelInfo.slice(0),
      };
    case 'CHANGE_LOAD_STATE':
      var updatedModelInfo = modifyModelItem(state.modelItems.slice(0), action);
      return {
        ...state,
        modelItems: updatedModelInfo.slice(0),
      }
    default:
      console.log("RETURNING DEFAULT STATE!");
      console.log(state);
      return state;
  }
}

module.exports = arobjects;
