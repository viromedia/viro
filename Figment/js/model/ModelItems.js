
import * as LoadingConstants from '../redux/LoadingStateConstants';

var ModelItems = [
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/btn_temp.png"),
    "obj": require("../res/pug_animated/pug_animated.vrx"),
    "materials": null,
    "animation":{name:"Take 001", delay:0, loop:true, run:true},
    "scale": [.3, .3, .3],
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [require('../res/pug_animated/pug_body_diffuse.jpg'), require('../res/pug_animated/pug_body_normal.png')],
  },
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/btn_temp_2.png"),
    "obj": require("../res/emoji_sad/emoji_sad.vrx"),
    "materials": null,
    "animation":null,
    "scale": [.2, .2, .2],
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [require('../res/emoji_sad/emoji_sad_diffuse.png'), require('../res/emoji_sad/emoji_sad_normal.png')],
  },
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/btn_temp.png"),
    "obj": require("../res/emoji_smile/emoji_smile.vrx"),
    "materials": null,
    "animation":null,
    "scale": [0.2, 0.2, 0.2],
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [require('../res/emoji_smile/emoji_smile_diffuse.png'), require('../res/emoji_smile/emoji_smile_normal.png')],
  },
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/btn_temp_2.png"),
    "obj": require("../res/bball/bball.obj"),
    "materials": ["bball"],
    "animation":null,
    "scale": [0.05, 0.05, 0.05],
    "physics": {shape:{type:'sphere', params:[0.2]}, type:'dynamic', restitution:1.0, mass:.623, enabled:true},
    "ref_pointer": undefined,
    "resources": [require('../res/bball/bball_NRM.png'), require('../res/bball/bball_SPEC.png')],
  },

  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/btn_temp.png"),
    "obj": require("../res/car_porsche/car_porsche_911.obj"),
    "animation":null,
    "materials": ["porsche"],
    "scale":[0.2, 0.2, 0.2],
    "physics": undefined,
    "ref_pointer": undefined,
    "resources":null,
  }
]

module.exports = {
  getInitModelArray: function() {
    return ModelItems;
  }
};
