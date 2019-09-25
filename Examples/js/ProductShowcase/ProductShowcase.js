'use strict';

import React, { Component } from 'react';

import {StyleSheet} from 'react-native';

import {
  ViroScene,
  ViroImage,
  ViroText,
  Viro360Image,
  ViroFlexView,
  ViroVideo,
  Viro3DObject,
  ViroMaterials,
  ViroCamera,
  ViroOrbitCamera,
  ViroNode,
  ViroAnimatedComponent,
  ViroDirectionalLight,
  ViroAnimations,
  ViroBox,
  ViroButton,
  ViroQuad,
} from 'react-viro';

var createReactClass = require('create-react-class');

/*
 * This function returns the correct stock status text depending on the
 * given argument.
 */
function getInStockText(inStock) {
  return inStock ? "In stock." : "Out of stock.";
}

/*
 * This function returns the correct product source text depending on the
 * given argument.
 *
 * online - true/false whether or not the product is available online
 * giftWrappable - true/false whether or not the product is giftwrappable
 */
function getProductSourceText(online, giftWrappable) {
  var toReturn = "";
  if (online) {
    toReturn += "Ships from and sold online.";
    if (giftWrappable) {
      toReturn += " Gift-wrap available.";
    }
  } else {
    toReturn += "Pick-up in store only.";
  }
  return toReturn;
}

/*
 * Register the required materials. These can be dynamically registered based on
 * the product list.
 */
ViroMaterials.createMaterials({
  samsung_tv: {
    shininess: 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('./res/samsung_tv_tex.jpg'),
    specularTexture: require('./res/samsung_tv_specular.jpg')
  },
  sony_camera_body: {
    shininess : 2.0,
    lightingModel : "Blinn",
    cullMode : "None",
    diffuseTexture : require("./res/sony_camera_body_diffuse.png"),
    specularTexture : require("./res/sony_camera_body_specular.png")
  },
  sony_camera_lens: {
    shininess : 2.0,
    lightingModel : "Blinn",
    cullMode : "None",
    diffuseTexture : require("./res/sony_camera_lens_diffuse.png"),
    specularTexture : require("./res/sony_camera_lens_specular.jpg")
  },
  opaque: {
    diffuseColor : "#ffffff80"
  }
});

/*
 * Array of products to display and their various properties.
 */
var products = [
  {
    "productTitleText" : "Samsung UN50KU6300 Television",
    "productDescriptionText" : "Samsung UN50KU6300 Curved 50-Inch 4K Ultra HD Smart LED",
    "productDetails1" : "Refresh Rate: 240CMR(Effective)",
    "productDetails2" : "Backlight: LED",
    "productDetails3" : "Dimensions(W x H x D): TV without stand: 44.4 x 25.8 x 2.5",
    "price" : "$648.00",
    "inStock" : true,
    "online" : true,
    "giftWrappable" : true,
    "productVideo" : require('./res/samsung_tv_vid.mp4'),
    "previewImage" : require('./res/btn_tv.png'),
    // model information
    "objects" : [require('./res/samsung_tv.obj')],
    "objectOffset" : [0, 0, 0],
    "objectScaleMultipler" : [1,1,1],
    "objectRotation" : [0, 38, 0],
    "materials" : ["samsung_tv"],
  },
  {
    "productTitleText" : "Sony a5100 Camera",
    "productDescriptionText" : "16-50mm Mirrorless Digital Camera with 3-Inch Flip Up LCD (Black)",
    "productDetails1" : "Ultra-fast auto focus with 179 AF points and 6 FPS",
    "productDetails2" : "Capture life in high resolution with 24MP APS-C sensor",
    "productDetails3" : "Instant sharing via smartphone with Wi-Fi® and NFC1",
    "price" : "$498.00",
    "inStock" : true,
    "online" : false,
    "giftWrappable" : true,
    "productVideo" : require('./res/sony_camera_vid.mp4'),
    "previewImage" : require('./res/btn_camera.png'),
    // model information
    "objects" : [require('./res/sony_camera_body.obj'), require('./res/sony_camera_lens.obj')],
    "objectOffset" : [-.2, -.7, 0],
    "objectScaleMultipler" : [.8,.8,.8],
    "objectRotation" : [0, 38, 0],
    "materials" : ["sony_camera_body", "sony_camera_lens"],
  }
]

var ProductShowcase = createReactClass({
  getInitialState() {
    return {
        hideInfoCards: false,
        runAnimation: false,
        mainAnimation: "fadeOut",
        videoPaused: false,
        productToShow: 0,
        objectAnimation: "scaleAndRotate",
        runObjectAnimation: false,
        globalAnimation : "fadeOut",
        runGlobalAnimation : false,
        onFinishGlobal : this._switchProduct
    };
  },
  render: function() {
    var product = products[this.state.productToShow];
    return (
      <ViroScene >

        <Viro360Image source={require('./res/interior_viro.jpg')} />

        <ViroDirectionalLight color="#ffffff" direction={[0, 0, -1.0]} />

        {/* Outer animation block used to fade in/out components when switching between products */}
        <ViroAnimatedComponent animation={this.state.globalAnimation} run={this.state.runGlobalAnimation} loop={false} onFinish={this.state.onFinishGlobal} >
          <ViroNode>

            {/* This is the top left text box containing the title + description text */}
            <ViroAnimatedComponent animation={this.state.mainAnimation} run={this.state.runAnimation} loop={false} >
              <ViroFlexView style={styles.titleContainer} position={[-3.8, 1, -7]} rotation={[0, 40, 0]} height={2} width={4} >
                <ViroText style={styles.prodTitleText} text={product.productTitleText} width={4} height={.5} />
                <ViroText style={styles.prodDescriptionText} text={product.productDescriptionText} />
              </ViroFlexView>
            </ViroAnimatedComponent>

            {/* This is the bottom left text box containing the product details text */}
            <ViroAnimatedComponent animation={this.state.mainAnimation} run={this.state.runAnimation} loop={false} >
              <ViroFlexView style={styles.titleContainer} position={[-3.8, -1.1, -7]} rotation={[0, 40, 0]} height={2} width={4} >
                <ViroFlexView style={styles.rowContainer}>
                  <ViroText style={styles.prodDescriptionText} text={product.productDetails1} />
                </ViroFlexView>
                <ViroFlexView style={styles.rowContainer}>
                  <ViroText style={styles.prodDescriptionText} text={product.productDetails2} />
                </ViroFlexView>
                <ViroFlexView style={styles.rowContainer}>
                  <ViroText style={styles.prodDescriptionText} text={product.productDetails3} />
                </ViroFlexView>
              </ViroFlexView>
            </ViroAnimatedComponent>

            {/* This is the top right text box containing the product price and stock status*/}
            <ViroAnimatedComponent animation={this.state.mainAnimation} run={this.state.runAnimation} loop={false} >
              <ViroFlexView style={styles.titleContainer} position={[3.8, 1, -7]} rotation={[0, -40, 0]} height={2} width={4} >
                <ViroFlexView style={styles.rowContainer} >
                  <ViroText style={styles.prodDescriptionText} text="Price:" />
                  <ViroText style={styles.priceText} text={product.price} />
                </ViroFlexView>
                <ViroText style={styles.prodDescriptionText} text={getInStockText(product.inStock)} />
                <ViroText style={styles.prodDescriptionText} text={getProductSourceText(product.online, product.giftWrappable)} />
              </ViroFlexView>
            </ViroAnimatedComponent>

            {this._getProductModel()}

            {this._getVideoIfAvailable()}

            {this._getNavButtons()}

          </ViroNode>
        </ViroAnimatedComponent>
      </ViroScene>
    );
  },
  /*
   * This function returns the Viro3DObjects of the current productToShow.
   */
  _getProductModel() {
    // get the products dictionary
    var product = products[this.state.productToShow];
    var objects = [];

    // a product may have multiple obj's representing it, so create a Viro3DObject component for each 
    for (var i = 0; i < product.objects.length; i++) {
      objects.push((<Viro3DObject key={"model" + i} source={product.objects[i]} type="OBJ" materials={product.materials[i]}/>))
    }

    var position = [0 + product.objectOffset[0], 0 + product.objectOffset[1], -5 + product.objectOffset[2]];
    var scale = [.3 * product.objectScaleMultipler[0], .3 * product.objectScaleMultipler[1], .3 * product.objectScaleMultipler[2]];

    ViroAnimations.registerAnimations({
      resetModel:{properties:{positionX:position[0], positionY:position[1], positionZ:position[2], rotateY: product.objectRotation[1]}, duration:1000},
    })

    return (
      <ViroAnimatedComponent key="obj" animation={this.state.objectAnimation} run={this.state.runObjectAnimation} loop={true} >
        <ViroNode rotation={product.objectRotation} scale={scale} position={position} onClick={this._onClickModel} >
          {objects}
        </ViroNode>
      </ViroAnimatedComponent>
    )
  },
  /*
   * This function is called by the render() function and returns a ViroVideo component if the
   * current product has a product video.
   */
  _getVideoIfAvailable() {
    var product = products[this.state.productToShow];

    // if video not available, then return nothing.
    if (!product.productVideo) {
      return;
    }

    return (
      <ViroAnimatedComponent animation={this.state.mainAnimation} run={this.state.runAnimation} loop={false} >
        <ViroVideo position={[3.8, -1.1, -7]} rotation={[0, -40, 0]} paused={this.state.videoPaused} loop={true} height={2} width={4}
            onClick={this._onClickVideo} source={product.productVideo} />
      </ViroAnimatedComponent>
    );
  },
  /*
   * This function determines if a next and/or previous button is required and returns them to be rendered
   */
  _getNavButtons() {
    var toReturn = [];
    var hasPreviousItem = this.state.productToShow > 0;
    var hasNextItem = this.state.productToShow + 1 < products.length;
    if (hasPreviousItem) {
      toReturn.push(this._getSingleNavButton(false, this.state.productToShow - 1, !hasNextItem));
    }
    if (hasNextItem) {
      toReturn.push(this._getSingleNavButton(true, this.state.productToShow + 1, !hasPreviousItem));
    }
    return toReturn;
  },
  /*
   * This function returns a single "next" or "previous" product button depending on the given parameters
   *
   * forward - true if the button should take the user to the next item, else to the previous button (& determines position)
   * index - the index of the productToShow if the user clicks on this component
   * centered - true/false if the button should be centered.
   */
  _getSingleNavButton(forward, index, centered) {
    var position = [(centered ? 0 : (forward ? 2 : -2)), -3, -7];
    return (
      <ViroAnimatedComponent key={forward ? "forwardBtn" : "backBtn"} animation={this.state.mainAnimation} run={this.state.runAnimation} loop={false} >
        <ViroNode position={position} onClick={this._prepareSwitchProduct(index)} transformBehaviors="billboard" >
          <ViroImage source={products[index].previewImage} width={2.8} height={.7}/>
          <ViroText text={products[index].productTitleText} width={2} height={.7} style={styles.navButtonText}
              textClipMode="clipToBounds" position={[forward ? .1 : -.1, 0, .05]} />
        </ViroNode>
      </ViroAnimatedComponent>
    );
  },
  /*
   * This function returns an anonymous function that serves as the onClick listener of the
   * Nav Buttons at the bottom left/right that allow the user to switch items. The returned
   * function makes all the UI components fade out in preparation for switching the product
   * information. It works in tandem with the _switchProduct() function. 
   *
   * productIndex - the product to switch to after the fade out animation.
   */
  _prepareSwitchProduct(productIndex) {
    return () => {
      this.setState({
        globalAnimation : "fadeOut",
        runGlobalAnimation : true,
        onFinishGlobal : this._switchProduct(productIndex),
      });
    }
  },
  /*
   * This function returns an anonymous function that serves as the onFinish listener of the
   * top-level ViroAnimatedComponent only when it is fading out. The returned function changes
   * the state to the given productIndex and makes the various UI components fade back in. This
   * works in tandem with _prepareSwitchProduct().
   *
   * productIndex - the product to switch to
   */
  _switchProduct(productIndex) {
    return () => {
      this.setState({
        productToShow: productIndex,
        globalAnimation : "fadeIn",
        runGlobalAnimation : true,
        onFinishGlobal : null,
      });
    };
  },
  /*
   * This function is the onClick listener for the product model.
   * 
   * Upon clicking the model, if the info cards are visible, then we hide them and make the model scale up and rotate
   * for the user to get a better view of the model. Else, we stop the rotation, scale down and make the info cards
   * reappear.
   */
  _onClickModel() {
    this.setState({
      hideInfoCards: !this.state.hideInfoCards,
      // if current state is to hide cards, then we want to show them
      mainAnimation: this.state.hideInfoCards ? "fadeIn" : "fadeOut",
      runAnimation: true,
      videoPaused: !this.state.videoPaused,
      // if current state is to hide cards, then we want to scale down/stop rotating the model
      objectAnimation: this.state.hideInfoCards ? "resetModel" : "scaleAndRotate",
      runObjectAnimation: true,
    });
  },
  /*
   * This function is the onClick listener of the ViroVideo component and pauses the video when clicked.
   */
  _onClickVideo() {
    this.setState({
      videoPaused: !this.state.videoPaused,
    });
  },
});

/*
 * Register the various animations we require
 */
ViroAnimations.registerAnimations({
  fadeOut:{properties:{opacity:0}, duration:2000},
  fadeIn:{properties:{opacity: 1}, duration:2000},
  scaleAndRotate:{properties:{rotateY: "+=90", positionZ: "-3"}, duration:1000},
});

/*
 * Register styles for ViroFlexView and ViroText
 */
var styles = StyleSheet.create({
  rowContainer: {
    flex: 1,
    flexDirection: 'row',
  },
  prodTitleText: {
    fontFamily: 'sans-serif-light',
    fontSize: 30,
    color: '#222222',
    textAlignVertical: 'center',
    textAlign: 'left',
  },
  priceText: {
    fontFamily: 'sans-serif-light',
    fontSize: 20,
    color: '#aa3c3c',
    textAlignVertical: 'center',
    textAlign: 'left',
    flex: 4,
  },
  prodDescriptionText: {
    fontFamily: 'sans-serif-light',
    fontSize: 20,
    color: '#222222',
    textAlignVertical: 'center',
    textAlign: 'left',
    flex: 1,
  },
  titleContainer: {
    flexDirection: 'column',
    backgroundColor: "#ffffffdd",
    padding: .2,
  },
  navButtonText: {
    fontFamily: 'sans-serif-light',
    fontSize: 15,
    color: '#ffffff',
    textAlignVertical: 'center',
    textAlign: 'center',
    flex: 1,
  },
});

module.exports = ProductShowcase;
