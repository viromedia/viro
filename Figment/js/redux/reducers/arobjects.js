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
import * as EffectData from  '../../model/EffectItems';
import * as LoadingConstants from '../LoadingStateConstants';

const uuidv1 = require('uuid/v1');

//data model for models is:
//{
//{uuid}: {selected: false, loading:, index:}
//}
 const initialState = {
   //modelItems: ModelData.getInitModelArray(),
  modelItems: {},
   portalItems: PortalData.getInitPortalArray(),
   effectItems: EffectData.getInitEffectArray(),
 }

function newModelItem(indexToCreate) {
  return {uuid: uuidv1(), selected: false, loading: LoadingConstants.NONE, index: indexToCreate};
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
            portal360Image: {...action.photoSource},
          };
        default:
          return state;
    }
 }

 export function toggleEffectSelection(index) {
   return {
     type: 'TOGGLE_EFFECT_SELECTED',
     index: index,
   };
 }


function modifyItem(state = [], action) {
    switch (action.type) {
      case 'CHANGE_PORTAL_PHOTO':
        return [
          ...state.slice(0, action.index),
          changePortalPhoto(state[action.index], action),
          ...state.slice(parseInt(action.index) +1),
        ]
      case 'TOGGLE_PORTAL_SELECTED':
        return [
          ...state.slice(0, action.index),
          toggleItemSelected(state[action.index], action),
          ...state.slice(parseInt(action.index) +1),
        ]

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

function modifyEffectSelection(state = [], action) {
  switch(action.type) {
    case 'TOGGLE_EFFECT_SELECTED':
      var effectToggleArray = [];
      for(var i =0; i<state.length; i++) {
        if(i != action.index) {
          state[i].selected = false;
        } else {
          state[i].selected = !state[i].selected;
        }
        effectToggleArray.push(state[i]);
      }
      return effectToggleArray;
  }
}

function addModelItem(state = {}, action) {
  var model = newModelItem(action.index);
  console.log("addModelItem redux:");
  console.log(model);
  state[model.uuid] = model;
  return state;
}


function removeModelItem(state = {}, action) {
  state[action.uuid] = null;
  return state;
}

function modifyLoadState(state = {}, action) {
  if(state[action.uuid] != null || state[action.uuid] != undefined) {
    state[action.uuid].loading = action.loadState;
  }
  return state;
}

function arobjects(state = initialState, action) {
  switch (action.type) {

    case 'ADD_MODEL':
      return {
        ...state,
        modelItems: {...addModelItem(state.modelItems, action)},
      }
    case 'REMOVE_MODEL':
      return {
        ...state,
        modelItems: {...removeModelItem(state.modelItems, action)},
      }
   case 'CHANGE_MODEL_LOAD_STATE':
      return {
        ...state,
        modelItems: modifyLoadState(state.modelItems, action),
      }
    case 'CHANGE_PORTAL_LOAD_STATE':
    case 'TOGGLE_PORTAL_SELECTED':
    case 'CHANGE_PORTAL_PHOTO':
        var updatedPortalInfo = modifyItem(state.portalItems.slice(0), action);
        return {
          ...state,
          portalItems: updatedPortalInfo.slice(0),
      }
    case 'TOGGLE_EFFECT_SELECTED':
        var updatedEffects = modifyEffectSelection(state.effectItems.slice(0), action);
        return  {
          ...state,
          effectItems: updatedEffects.slice(0),
        }
    default:
      return state;
  }
}

module.exports = arobjects;
