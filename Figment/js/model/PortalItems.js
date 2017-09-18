
import * as LoadingConstants from '../redux/LoadingStateConstants';

var POSITION_OFFSET = .05 // 5 cm

var PortalItems = [
  {
    "name": "portal_archway",
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_archway.png"),
    "obj": require("../res/portal_archway/portal_archway.vrx"),
    "materials": null,
    "portal360Image": {source:require("../res/360_diving.jpg"), width:2, height:1},
    "animation":undefined,
    "scale": [1, 1, 1],
    "portalScale": [.2, .2, .2], // scale on the portal, normalizes its height to ~1
    "position": [0, 0, 0],
    "frameType": "VRX",
    "resources": [require('../res/portal_archway/portal_archway_normal.png'),
                  require('../res/portal_archway/portal_archway_diffuse.png'),
                  require('../res/portal_archway/portal_archway_specular.png'),
                  require('../res/portal_archway/portal_entry.png')],
  },
  {
    "name": "portal_picture_frame",
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_picture_frame.png"),
    "obj": require("../res/portal_picture_frame/portal_picture_frame.vrx"),
    "materials": null,
    "portal360Image": {source:require("../res/360_guadalupe.jpg"), width:2, height:1},
    "animation":null,
    "scale": [1, 1, 1],
    "portalScale": [.45, .45, .45],
    "position": [0, 0, 0],
    "frameType": "VRX",
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [require('../res/portal_picture_frame/portal_picture_frame_specular.png'), require('../res/portal_picture_frame/portal_picture_frame_diffuse.png'), require('../res/portal_picture_frame/portal_picture_frame_normal.png')],
  },
  {
    "name": "portal_window_frame",
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_windowframe.png"),
    "obj": require("../res/portal_window_frame/portal_window_frame.vrx"),
    "materials": null,
    "portal360Image": {source:require("../res/360_guadalupe.jpg"), width:2, height:1},
    "animation":null,
    "scale": [1, 1, 1],
    "portalScale": [.275, .275, .275],
    "position": [0, 0, 0],
    "frameType": "VRX",
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [require('../res/portal_window_frame/portal_window_frame_specular.png'), require('../res/portal_window_frame/portal_window_frame_diffuse.png'), require('../res/portal_window_frame/portal_window_frame_normal.png')],
  },
    {
    "name": "portal_wood_frame",
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_wood_doorframe.png"),
    "obj": require("../res/portal_wood_frame/portal_wood_frame.vrx"),
    "materials": null,
    "portal360Image": {source:require("../res/360_westlake.jpg"), width:2, height:1},
    "animation":null,
    "scale": [1, 1, 1],
    "portalScale": [.2, .2, .2],
    "position": [0, 0, 0],
    "frameType": "VRX",
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [require('../res/portal_wood_frame/portal_wood_frame_specular.png'), require('../res/portal_wood_frame/portal_wood_frame_diffuse.png'), require('../res/portal_wood_frame/portal_wood_frame_normal.png')],
  },
  {
    "name": "portal_ship",
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_shipdoor.png"),
    "obj": require("../res/portal_ship/portal_ship.vrx"),
    "materials": ["portal_ship"],
    "portal360Image": {source:require("../res/360_waikiki.jpg"), width:2, height:1},
    "animation":null,
    "scale": [1, 1, 1],
    "portalScale": [.07, .07, .07],
    "position": [0, .5, 0],
    "frameType": "VRX",
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
