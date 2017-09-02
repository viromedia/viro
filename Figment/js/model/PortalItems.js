
import * as LoadingConstants from '../redux/LoadingStateConstants';

var POSITION_OFFSET = .05 // 5 cm

var PortalItems = [
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_archway.png"),
    "obj": require("../res/portal_archway/portal_archway.vrx"),
    "materials": null,
    "portal360Image": {source:require("../res/360_diving.jpg"), width:2, height:1},
    "animation":undefined,
    "scale": [1, 1, 1],
    "portalScale": [.4, .4, .4], // scale on the portal, normalizes its height to ~1
    "position": [0, .5, 0],
    "resources": [require('../res/portal_archway/portal_archway_normal.png'),
                  require('../res/portal_archway/portal_archway_diffuse.png'),
                  require('../res/portal_archway/portal_archway_specular.png'),
                  require('../res/portal_archway/portal_entry.png')],
  },
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_picture_frame.png"),
    "obj": require("../res/portal_picture_frame/portal_picture_frame.vrx"),
    "materials": null,
    "portal360Image": {source:require("../res/360_guadalupe.jpg"), width:2, height:1},
    "animation":null,
    "scale": [1, 1, 1],
    "portalScale": [.9, .9, .9],
    "position": [0, .5, 0],
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
    "scale": [1, 1, 1],
    "portalScale": [.55, .55, .55],
    "position": [0, .5, 0],
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
    "scale": [1, 1, 1],
    "portalScale": [.4, .4, .4],
    "position": [0, .5, 0],
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
    "scale": [1, 1, 1],
    "portalScale": [.14, .14, .14],
    "position": [0, .5, 0],
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [ require('../res/portal_ship/portal_ship_normal.png'), require('../res/portal_ship/portal_ship_diffuse.png')],
  },
]


module.exports = {
  getPortalArray: function() {
    return PortalItems;
  }
};
