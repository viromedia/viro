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
import * as EffectsConstants from '../EffectsConstants';

console.log(EffectData);
const uuidv1 = require('uuid/v1');

//data model for models is:
//{
//{uuid}: {selected: false, loading:[LoadingConstant] index:[index into appr Items array]}
//}
 const initialState = {
   //modelItems: ModelData.getInitModelArray(),
  modelItems: {},
   portalItems: {},
   effectItems: EffectData.getInitEffectArray(),
   postProcessEffect: EffectsConstants.EFFECT_NONE,
 }

function newModelItem(indexToCreate) {
  return {uuid: uuidv1(), selected: false, loading: LoadingConstants.NONE, index: indexToCreate};
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
          if(state[action.uuid] != null || state[action.uuid] != undefined) {
            var model = state[action.uuid];
            var newModel = {...model};
            newModel.portal360Image = {...action.photoSource};
            state[action.uuid] = newModel;
          }
          return state;
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

function modifyEffectSelection(state = [], action) {
  switch(action.type) {
    case 'TOGGLE_EFFECT_SELECTED':
      var effectToggleArray = [];
      for(var i =0; i<state.length; i++) {
        if(i != action.index) {
          state[i].selected = false;
        } else {
          if (!state[i].selected) {
            state[i].selected = true;
          } // else if this effect was already selected; do nothing
        }
        effectToggleArray.push(state[i]);
      }
      return effectToggleArray;
    case 'REMOVE_ALL':
      var effectToggleArray = [];
      for(var i =0; i<state.length; i++) {
          state[i].selected = false;
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
    var model = state[action.uuid];
    var newModel = {...model};
    newModel.loading = action.loadState;
    state[action.uuid] = newModel;
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
    case 'ADD_PORTAL':
      return {
        ...state,
        portalItems: {...addModelItem(state.portalItems, action)},
      }
    case 'REMOVE_PORTAL':
      return {
        ...state,
        portalItems: {...removeModelItem(state.portalItems, action)},
      }
    case 'REMOVE_ALL':
      //clear efffects
      var updatedEffects = modifyEffectSelection(state.effectItems.slice(0), action);
      return {
        ...state,
        portalItems:{},
        modelItems:{},
        effectItems: updatedEffects.slice(0),
        postProcessEffect: EffectsConstants.EFFECT_NONE,
      }
    case 'CHANGE_MODEL_LOAD_STATE':
      return {
        ...state,
        modelItems: {...modifyLoadState(state.modelItems, action)},
      }
    case 'CHANGE_PORTAL_LOAD_STATE':
      return {
        ...state,
        portalItems: {...modifyLoadState(state.portalItems, action)},
      }
    case 'CHANGE_PORTAL_PHOTO':
        return {
          ...state,
          portalItems: {...changePortalPhoto(state.portalItems, action)},
      }
    case 'TOGGLE_EFFECT_SELECTED':
        var updatedEffects = modifyEffectSelection(state.effectItems.slice(0), action);
        console.log("postProcessEffect: " + updatedEffects[action.index].postProcessEffect);
        return  {
          ...state,
          effectItems: updatedEffects.slice(0),
          postProcessEffect: updatedEffects[action.index].postProcessEffect,
        }
    default:
      return state;
  }
}

module.exports = arobjects;
