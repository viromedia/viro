
import * as LoadingConstants from '../redux/LoadingStateConstants';
import {getEmptyEffect, getSnow, getFireWorks} from '../effects/effects.js'


var EffectItems = [
  {
    "effect": getEmptyEffect,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/btn_temp_2.png"),
    "selected": false,
  },
  {
    "effect": getSnow,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_effects_snow.png"),
    "selected": false,
  },
  {
    "effect": getFireWorks,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_effects_fireworks.png"),
    "selected": false,
  },
]

module.exports = {
  getInitEffectArray: function() {
    return EffectItems;
  }
};
