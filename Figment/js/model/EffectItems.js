
import * as LoadingConstants from '../redux/LoadingStateConstants';
import {getSnow, getFireWorks} from '../effects/effects.js'


var EffectItems = [
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
