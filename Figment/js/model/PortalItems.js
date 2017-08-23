
import * as LoadingConstants from '../redux/LoadingStateConstants';

var PortalItems = [
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/btn_temp_2.png"),
    "obj": require("../res/portal_ring/portal_ring.obj"),
    "materials": ["ring"],
    "animation":null,
    "scale": [.2, .2, .2],
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [require('../res/portal_ring/portal_ring.png')],
  },
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_archway.png"),
    "obj": require("../res/portal_archway/portal_archway.vrx"),
    "materials": null,
    "animation":undefined,
    "scale": [.2, .2, .2],
    "resources": [require('../res/portal_archway/portal_archway_normal.png'), require('../res/portal_archway/portal_archway_specular.png')],
  },
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_picture_frame.png"),
    "obj": require("../res/portal_picture_frame/portal_picture_frame.vrx"),
    "materials": null,
    "animation":null,
    "scale": [.2, .2, .2],
    "physics": undefined,
    "ref_pointer": undefined,
    "resources": [require('../res/portal_picture_frame/portal_picture_frame_specular.png'), require('../res/portal_picture_frame/portal_picture_frame_diffuse.png'), require('../res/portal_picture_frame/portal_picture_frame_normal.png')],
  },
  {
    "selected": false,
    "loading": LoadingConstants.NONE,
    "icon_img":require("../res/icon_portal_shipdoor.png"),
    "obj": require("../res/portal_ship/portal_ship.obj"),
    "materials": ["portal_ship"],
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
