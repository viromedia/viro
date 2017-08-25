
import * as LoadingConstants from '../redux/LoadingStateConstants';

var PortalItems = [
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_archway.png"),
    "obj": require("../res/portal_archway/portal_archway.vrx"),
    "materials": null,
    "portal360Image": {source:require("../res/360_diving.jpg"), width:2, height:1},
    "animation":undefined,
    "scale": [.2, .2, .2],
    "resources": [require('../res/portal_archway/portal_archway_normal.png'), require('../res/portal_archway/portal_archway_diffuse.png'),require('../res/portal_archway/portal_archway_specular.png')],
  },
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_picture_frame.png"),
    "obj": require("../res/portal_picture_frame/portal_picture_frame.vrx"),
    "materials": null,
    "portal360Image": {source:require("../res/360_guadalupe.jpg"), width:2, height:1},
    "animation":null,
    "scale": [.2, .2, .2],
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [require('../res/portal_picture_frame/portal_picture_frame_specular.png'), require('../res/portal_picture_frame/portal_picture_frame_diffuse.png'), require('../res/portal_picture_frame/portal_picture_frame_normal.png')],
  },
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_windowframe.png"),
    "obj": require("../res/portal_window_frame/portal_window_frame.vrx"),
    "materials": null,
    "portal360Image": {source:require("../res/360_guadalupe.jpg"), width:2, height:1},
    "animation":null,
    "scale": [.2, .2, .2],
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [require('../res/portal_window_frame/portal_window_frame_specular.png'), require('../res/portal_window_frame/portal_window_frame_diffuse.png'), require('../res/portal_window_frame/portal_window_frame_normal.png')],
  },
    {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_wood_doorframe.png"),
    "obj": require("../res/portal_wood_frame/portal_wood_frame.vrx"),
    "materials": null,
    "portal360Image": {source:require("../res/360_westlake.jpg"), width:2, height:1},
    "animation":null,
    "scale": [.2, .2, .2],
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [require('../res/portal_wood_frame/portal_wood_frame_specular.png'), require('../res/portal_wood_frame/portal_wood_frame_diffuse.png'), require('../res/portal_wood_frame/portal_wood_frame_normal.png')],
  },
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_shipdoor.png"),
    "obj": require("../res/portal_ship/portal_ship.vrx"),
    "materials": ["portal_ship"],
    "portal360Image": {source:require("../res/360_waikiki.jpg"), width:2, height:1},
    "animation":null,
    "scale": [.2, .2, .2],
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [ require('../res/portal_ship/portal_ship_normal.png'), require('../res/portal_ship/portal_ship_diffuse.png')],
  },
]


module.exports = {
  getInitPortalArray: function() {
    return PortalItems;
  }
};
