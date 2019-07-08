/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 */
'use strict';

import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View,
  Platform
} from 'react-native';

import {
  ViroSceneNavigator,
  ViroScene,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroCamera,
  ViroAmbientLight,
  ViroOmniLight,
  ViroSpotLight,
  ViroDirectionalLight,
  ViroImage,
  ViroVideo,
  Viro360Image,
  Viro360Video,
  ViroFlexView,
  ViroUtils,
  ViroText,
  ViroAnimations,
  ViroQuad,
  ViroSkyBox,
  ViroSphere,
  Viro3DObject,
  ViroLightingEnvironment,
} from 'react-viro';


var createReactClass = require('create-react-class');

let polarToCartesian = ViroUtils.polarToCartesian;
var ReleaseMenu = require("./ReleaseMenu.js");


var Viro3DObjectTest = createReactClass({

  getInitialState() {
    return {
      modelSet:-1,
      title:"Click to start view Model Set!",
      output:" ",
    };
  },

  getLights(hasPBR){
    var views = [];
    if (hasPBR){
      views.push((
        <ViroNode>
        <ViroAmbientLight intensity={500} color="#ffffff"/>
        <ViroOmniLight intensity={300} position={[-0.25, 0.5, -3]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
        <ViroOmniLight intensity={300} position={[0.25, 0, -3]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
        <ViroOmniLight intensity={300} position={[-0.25, 0, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
        <ViroOmniLight intensity={300} position={[0.25, 0.5, 0]} color="#ffffff" attenuationStartDistance={40} attenuationEndDistance={50}/>
        </ViroNode>
      ));
    } else {
      views.push((<ViroLightingEnvironment source={require('./res/ibl_newport_loft.hdr')}/>));
    }
    return views;

  },

  render: function() {
    return (
     <ViroScene onClick={this._onSceneClick}>
      <ReleaseMenu sceneNavigator={this.props.sceneNavigator}/>
      <Viro360Image source={require('./res/ibl_newport_loft.hdr')} isHdr={true} />

      {this.getLights(Platform.OS === 'android')}
      <ViroCamera
          fieldOfView={60}
          position={[0,0,0.5]}
          active={this.state.modelSet ==-1}/>

      <ViroOrbitCamera
          fieldOfView={70}
          position={[0,0,0.5]}
          active={this.state.modelSet != -1}
          focalPoint={[0, 0, -1.75]}/>

      <ViroText position={[0, 0.25, -1.75]} text={this.state.title} style={{textAlign: 'center'}} width={5} fontSize={20} fontFamily={'Arial'}/>
      <ViroText position={[0, -1.5, -1.75]} text={this.state.output}style={{textAlign: 'center'}} fontSize={10} width={10} height={1}/>
      {this._loadModel()}
     </ViroScene>

    );
  },

  _loadModel(){
    var modelSet = this.state.modelSet;
    var views = [];

    if (modelSet == 0){
      views.push((
        <ViroNode  position={[0, -0.5, -1.75]} >
          <Viro3DObject source={require('./res/gltf/BoxInterleaved.glb')}
                        position={[-0.75, 0.0 , 0.0]}
                        scale={[0.35,0.35,0.35]}
                        type="GLB"
                        onError={this._loaded("Interleaved Box", false)}
                        onLoadEnd={this._loaded("Interleaved Box", true)}
                        />
          <Viro3DObject source={require('./res/gltf/SimpleMeshes.gltf')}
                        position={[-0.3, -0.2, 0.0]}
                        scale={[0.35,0.35,0.35]}
                        type="GLTF"
                        onError={this._loaded("TriangleMesh", false)}
                        onLoadEnd={this._loaded("TriangleMesh", true)}
                        />
          <Viro3DObject source={require('./res/gltf/BoxTextured.glb')}
                        position={[0.75, 0, 0.0]}
                        scale={[0.35,0.35,0.35]}
                        type="GLB"
                        onError={this._loaded("TextureBox", false)}
                        onLoadEnd={this._loaded("TextureBox", true)}
                         />
        </ViroNode>
      ));
    } else if (modelSet == 1){
      views.push((
        <ViroNode  position={[0, -0.5, -1.75]} >
          <Viro3DObject source={require('./res/gltf/DuckGlb.glb')}
                        position={[-0.75, 0.0 , 0.0]}
                        scale={[0.35,0.35,0.35]}
                        type="GLB"
                        onError={this._loaded("Duck.glb", false)}
                        onLoadEnd={this._loaded("Duck.glb", true)}
                        />
          <Viro3DObject source={require('./res/gltf/Duck.gltf')}
                        resources={[require('./res/gltf/Duck0.bin'),
                                    require('./res/gltf/DuckCM.png')]}
                        position={[0, 0, 0.0]}
                        scale={[0.35,0.35,0.35]}
                        type="GLTF"
                        onError={this._loaded("Duck.gltf", false)}
                        onLoadEnd={this._loaded("Duck.gltf", true)}
                        />
          <Viro3DObject source={require('./res/gltf/Duck64Encoded.gltf')}
                        position={[0.75, 0, 0.0]}
                        scale={[0.35,0.35,0.35]}
                        type="GLTF"
                        onError={this._loaded("DuckBase64", false)}
                        onLoadEnd={this._loaded("DuckBase64", true)}
                         />
        </ViroNode>
      ));
    } else if (modelSet == 2){
      views.push((
        <ViroNode  position={[0, -0.5, -1.75]} >
          <Viro3DObject source={require('./res/gltf/2CylinderEngine.glb')}
                        position={[0, 0.0 , 0.0]}
                        rotation={[45,45,0]}
                        scale={[0.001,0.001,0.001]}
                        type="GLB"
                        onError={this._loaded("Engine", false)}
                        onLoadEnd={this._loaded("Engine", true)}
                        />
        </ViroNode>
      ));
    } else if (modelSet == 3){
      views.push((
        <ViroNode  position={[0, -0.5, -1.75]} >
          <Viro3DObject source={require('./res/gltf/CesiumMan.glb')}
                        position={[0, 0.0 , 0.0]}
                        scale={[0.55,0.55,0.55]}
                        type="GLB"
                        onError={this._loaded("CesiumMan.glb", false)}
                        onLoadEnd={this._loaded("CesiumMan.glb", true)}
                        animation={{name:"animation_0",
                                    delay:0,
                                    duration:30000,
                                    loop:true,
                                    run:true,
                                    interruptible:true }}
                        />
        </ViroNode>
      ));
    } else if (modelSet == 4){
      views.push((
        <ViroNode  position={[0, -0.5, -1.75]} >
          <Viro3DObject source={require('./res/gltf/CesiumMilkTruck.glb')}
                        position={[0, 0, 0.0]}
                        scale={[0.15,0.15,0.15]}
                        type="GLB"
                        onError={this._loaded("Truck", false)}
                        onLoadEnd={this._loaded("Truck", true)}
                         />
        </ViroNode>
      ));
    } else if (modelSet == 5){
      views.push((
      <ViroNode  position={[0, -0.5, -1.75]} >
        <Viro3DObject source={require('./res/gltf/Avocado.glb')}
                      position={[0, -0.20 , 0.0]}
                      scale={[10,10,10]}
                      type="GLB"
                      onError={this._loaded("Avocado", false)}
                      onLoadEnd={this._loaded("Avocado", true)}
                      />
      </ViroNode>
      ));
    } else if (modelSet == 6){
      views.push((
      <ViroNode  position={[0, -0.5, -1.75]} >
        <Viro3DObject source={require('./res/gltf/FlightHelmet_gltf.gltf')}
                      resources={[require('./res/gltf/FlightHelmet.bin'),
                                  require('./res/gltf/FlightHelmet_baseColor.png'),
                                  require('./res/gltf/FlightHelmet_occlusionRoughnessMetallic.png'),
                                  require('./res/gltf/FlightHelmet_normal.png'),
                                  require('./res/gltf/FlightHelmet_baseColor1.png'),
                                  require('./res/gltf/FlightHelmet_occlusionRoughnessMetallic1.png'),
                                  require('./res/gltf/FlightHelmet_normal1.png'),
                                  require('./res/gltf/FlightHelmet_baseColor2.png'),
                                  require('./res/gltf/FlightHelmet_occlusionRoughnessMetallic2.png'),
                                  require('./res/gltf/FlightHelmet_normal2.png'),
                                  require('./res/gltf/FlightHelmet_baseColor3.png'),
                                  require('./res/gltf/FlightHelmet_occlusionRoughnessMetallic3.png'),
                                  require('./res/gltf/FlightHelmet_normal3.png'),
                                  require('./res/gltf/FlightHelmet_baseColor4.png'),
                                  require('./res/gltf/FlightHelmet_occlusionRoughnessMetallic4.png'),
                                  require('./res/gltf/FlightHelmet_normal4.png')]}
                      position={[0, 0.5, 0.0]}
                      scale={[1.7, 1.7, 1.7]}
                      type="GLTF"
                      onError={this._loaded("FlightHelmet", false)}
                      onLoadEnd={this._loaded("FlightHelmet", true)}
                       />
      </ViroNode>
      ));
    } else if (modelSet == 7){
      views.push((
      <ViroNode  position={[0, -0.5, -1.75]} >
       <Viro3DObject source={require('./res/gltf/Corset.glb')}
                    position={[0, -.2, 0.0]}
                    scale={[18,18,18]}
                    type="GLB"
                    onError={this._loaded("Corset", false)}
                    onLoadEnd={this._loaded("Corset", true)}
                       />
      </ViroNode>
      ));
    } else if (modelSet == 8) {
      views.push((
      <ViroNode  position={[0, -0.75, -1.75]} >
       <Viro3DObject
                    ref={(obj)=>{this.gLTFCube = obj}}
                    source={require('./res/gltf/AnimatedMorphCube.glb')}
                    position={[-2.5, -0.3, -3.5]}
                    scale={[1, 1, 1]}
                    rotation={[0, 45, 0]}
                    morphTargets={[
                      {target:"thin", weight:1.0}
                    ]}
                    animation={{name:"Square",
                                delay:8000,
                                duration:6000,
                                loop:true,
                                run:true,
                                interruptible:true }}
                    type="GLB"
                    onError={this._loaded("AnimatedMorphCube", false)}
                    onLoadEnd={ ()=>{
                      this.setState({output:this.state.output + " AnimatedMorphCube"});
                      this.gLTFCube.getMorphTargets().then((ret)=>{
                        var targets = "Found Box Target:  ";
                        var i = 0;
                        for (i = 0; i < ret.targets.length; i ++){
                          targets = targets + ret.targets[i]+ (i != ret.targets.length -1 ? ", " : "");
                        }

                       this.gLTFCubeText.setNativeProps({text:targets});
                    });
                  }}
          />
        <ViroText ref={(obj)=>{this.gLTFCubeText = obj}} position={[-2.5, 0.8, -3.5]} text={"Box Targets: <Target 1>, <Target 1>, <Target 1>"} style={{textAlign: 'center'}} width={5} fontSize={20} fontFamily={'Arial'}/>

          <Viro3DObject
                       ref={(obj)=>{this.gLTFSphere = obj}}
                       source={require('./res/gltf/AnimatedMorphSphere.glb')}
                       position={[2.5, -0.3, -3.5]}
                       scale={[1, 1, 1]}
                       rotation={[0, 45, 0]}
                       animation={{name:"Globe",
                                   delay:8000,
                                   duration:6000,
                                   loop:true,
                                   run:true,
                                   interruptible:true }}
                       type="GLB"
                       onError={this._loaded("AnimatedMorphSphere", false)}
                       onLoadEnd={ ()=>{
                         this.setState({output:this.state.output + " AnimatedMorphSphere"});
                         this.gLTFSphere.getMorphTargets().then((ret)=>{
                           var targets = "Found Sphere Targets:  ";
                           var i = 0;
                           for (i = 0; i < ret.targets.length; i ++){
                             targets = targets + ret.targets[i]+ (i != ret.targets.length -1 ? ", " : "");
                           }

                          this.gLTFSphereText.setNativeProps({text:targets});
                       });
                     }}
             />
           <ViroText ref={(obj)=>{this.gLTFSphereText = obj}} position={[2.5, 0.8, -3.5]} text={"Sphere Targets: <Target 1>, <Target 1>, <Target 1>"} style={{textAlign: 'center'}} width={5} fontSize={20} fontFamily={'Arial'}/>


     </ViroNode>
      ));
    } else if (modelSet == 9){
      views.push((
      <ViroNode  position={[0, -0.5, -1.75]} >
        <Viro3DObject source={require('./res/gltf/BoomBox.glb')}
                      position={[-0.85, 0.0 , 0.0]}
                      scale={[27.5,27.5,27.5]}
                      type="GLB"
                      onError={this._loaded("BoomBox.glb", false)}
                      onLoadEnd={this._loaded("BoomBox.glb", true)}
                      />
        <Viro3DObject source={require('./res/gltf/DamagedHelmet.glb')}
                      position={[-0.1, 0, 0.0]}
                      scale={[0.35,0.35,0.35]}
                      type="GLB"
                      onError={this._loaded("DamagedHelmet.glb", false)}
                      onLoadEnd={this._loaded("DamagedHelmet.glb", true)}
                      />
        <Viro3DObject source={require('./res/gltf/Suzanne_gltf.gltf')}
                      resources={[require('./res/gltf/Suzanne.bin'),
                                  require('./res/gltf/Suzanne_BaseColor.png'),
                                  require('./res/gltf/Suzanne_MetallicRoughness.png')]}
                      position={[0.85, 0, 0.0]}
                      scale={[0.45,0.45,0.45]}
                      type="GLTF"
                      onError={this._loaded("Suzanne.glb", false)}
                      onLoadEnd={this._loaded("Suzanne.glb", true)}
                       />
      </ViroNode>
        ));
    } else if (modelSet == 10){
      views.push((
      <ViroNode  position={[0, -0.5, -1.75]} >
        <Viro3DObject source={require('./res/gltf/BadVersion.gltf')}
                      position={[-0.85, 0.0 , 0.0]}
                      scale={[27.5,27.5,27.5]}
                      type="GLTF"
                      onError={this._loaded("BadVersion", false)}
                      />
        <Viro3DObject source={require('./res/gltf/MalformedJson.gltf')}
                      position={[-0.1, 0, 0.0]}
                      scale={[0.35,0.35,0.35]}
                      type="GLTF"
                      onError={this._loaded("MalformedJson", false)}
                      />
        <Viro3DObject source={require('./res/gltf/MissingTexture.gltf')}
                      position={[0.85, 0, 0.0]}
                      scale={[0.45,0.45,0.45]}
                      type="GLTF"
                      onError={this._loaded("MissingTexture", false)}
                       />
      </ViroNode>
        ));
    }

    return views;
  },

  _onSceneClick(){
    var modelSet = this.state.modelSet + 1;
    if (modelSet > 10){
      modelSet = -1;
    }

    var title = "Click to start view Model Set!";
    if (modelSet == 0){
      title = "Basic GLTF Geometry:";
    } else if (modelSet == 1){
      title = "GLTF Model File Types:";
    } else if (modelSet == 2 || modelSet == 3 || modelSet == 4){
      title = "Complex Geometry Models:";
    } else if (modelSet == 5 || modelSet == 6 || modelSet == 7){
      title = "GTLF with PBR textures:";
    } else if (modelSet == 8){
      title = "Morph Target Model:";
    } else if (modelSet == 9){
      title = "Very reflective pbr textures:";
    } else if (modelSet == 10){
      title = "Test Invalid Models \n(Should fail gracefully)";
    }
    this.setState({modelSet:modelSet, title:title, output:"->"});
  },

  _loaded(objectTag, success){
    return (soursuccessce) => {
        var result =  objectTag + " load success:" + success + "\n";
       console.log(result);
       this.setState({output:this.state.output + " " + result});
    }
  },
  _showNext() {
    this.props.sceneNavigator.replace({scene:require('./ViroCameraTest')});
  },

  _showPrevious() {
    this.props.sceneNavigator.pop();
  },

});

ViroAnimations.registerAnimations({
  loopRotate:{properties:{rotateY:"+=2"}, duration:250},
  scaleSphere:{properties:{scaleX:1.1, scaleY:1.1, scaleZ:1.1}, duration:300},
  spinSphere:[
      ["loopRotate"],
      ["scaleSphere"],
  ],
});

var styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  elementText: {
    fontFamily: 'HelveticaNeue-Medium',
    fontSize: 10,
    color: '#ffffff',
    textAlign: 'center',
  },
});

ViroMaterials.createMaterials({
  heart: {
    lightingModel: "Constant",
    diffuseTexture: { "uri" : "https://s3-us-west-2.amazonaws.com/viro/heart_d.jpg" },
  },
  grey: {
    shininess : 2.0,
    lightingModel: "Blinn",
    diffuseTexture: require('./res/grey.jpg'),
  },
  earth: {
    lightingModel: "Blinn",
    diffuseTexture: require('./res/earth_jpg.jpg'),
    normalTexture: require('./res/earth_normal.jpg'),
  }
});

module.exports = Viro3DObjectTest;
