//
//  VROShaderModifier.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/13/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef VROShaderModifier_h
#define VROShaderModifier_h

#include <stdlib.h>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <map>
#include "VROOpenGL.h"
#include "VROUniform.h"

class VROUniform;
class VROGeometry;
class VROMaterial;
class VROUniformBinder;
enum class VROShaderProperty;

/*
 The entry point, which signals where in the shader program this modifier will
 act.
 
 ----------------
 
 Geometry entry point. The code may declare uniforms and read/write
 to the following structures:
 
 struct VROShaderGeometry {
   vec3 position;
   vec3 normal;
   vec2 texcoord;
   vec4 tangent;
   vec4 bone_weights;
   ivec4 bone_indices;
 } _geometry;
 
 struct VROTransforms {
   mat4 model_matrix;
   mat4 view_matrix;
   mat4 projection_matrix;
 } _transforms;
 
 The Geometry entry point enables modifiers to change vertex parameters
 in the vertex shader. This includes geometry parameters like position,
 normals, and texcoords, and transform parameters like the model, view,
 and projection matrices.
 
 ----------------
 
 Vertex entry point. The code may declare uniforms and read/write
 to the following structure:
 
 struct VROShaderVertex {
   vec3 position;
 } _vertex;
 
 The Vertex entry point enables modifiers to change the position of
 vertices *after* their transformation into normalized device coordinates.
 The input and output (_vertex.position) is in normalized device coordinates.

 ----------------
 Surface entry point. The code may declare uniforms and read/write
 to the following structure:
 
 struct VROSurface {
   lowp  vec4 diffuse_color;
   highp vec2 diffuse_texcoord;
   lowp float diffuse_intensity;
 
   lowp float shininess;
   lowp  vec3 specular_color;
   highp vec2 specular_texcoord;
 
   highp float roughness;
   highp float metalness;
   highp float ao;
 
   lowp float alpha;
   lowp  vec3 normal;
   highp vec3 position;
 } _surface;
 
 The Surface entry point enables modifiers to change surface parameters, in
 the fragment shader, prior to the lighting computation.
 
 ----------------
 
 Lighting Model entry point. The code runs once per light. It reads from the
 _surface structure and the _light structure below, and sets the results in
 the _lightingContribution structure:
 
 struct VROLightingContribution {
     highp vec3 ambient;
     highp vec3 diffuse;
     highp vec3 specular;
     highp float visibility;
 } _lightingContribution;
 
 struct VROLightUniforms {
    int type;
    highp float attenuation_start_distance;
    highp float attenuation_end_distance;
    highp float attenuation_falloff_exp;
 
    highp vec4 position;
    highp vec4 direction;
 
    highp vec3 color;
    highp float intensity;
 
    highp float spot_inner_angle;
    highp float spot_outer_angle;
 } _light;

 The Lighting Model entry point enables modifiers to define the impact of each
 light on a given material. After being invoked on each light, the lighting
 contributions from each light are accumulated and combined with material surface
 properties to generate the final color. The visibility value (which defaults to
 1.0) is multiplied by the diffuse and specular components; it can be used to
 simulate the impact of shadow.
 
 Note, as an optimization, total ambient light is initialized to the sum
 of all ambient lights. Therefore, in general lighting models will not need to
 add anything to _lightingContribution.ambient.
 
 ----------------
 
 Fragment entry point. The code may declare uniforms and read/write
 to the variable:
 
 highp vec4 _output_color;
 
 The Fragment entry point enables modifiers to alter the final color of each
 fragment, after the lighting computation.
 
 ----------------
 
 Image entry point. The image entry point is *only* available for 2D post-processing
 shaders. The code may declare uniforms for the fragment shader (the vertex
 shader is a fixed quad in normalized device coordinates), and must simply
 set the frag_color.

 frag_color = ... (vec4) ...
 
 The Image entry point enables modifiers to quickly create new post-processing
 functions for VROImageShaderPrograms.
 
 ----------------
 */
enum class VROShaderEntryPoint {
    Geometry,
    Vertex,
    Surface,
    LightingModel,
    Fragment,
    Image,
};

enum class VROShaderSection {
    Uniforms,
    Body
};

/*
 Modifies the source of a VROShaderProgram, and enables the binding of new uniforms
 to said program. A shader modifier can be attached to multiple shaders. Note in such
 cases, the uniform binders will be consistent across shaders (they cannot be different 
 for each shader).
 */
class VROShaderModifier {
    
public:
    
    static std::string getShaderModifierKey(const std::vector<std::shared_ptr<VROShaderModifier>> &modifiers);
    
    /*
     Create a new shader modifier that operates at the given entry point. The input
     should be valid GLSL code, with each line as a separate string. Uniform declarations
     are automatically separated out from the body of the input code.
     */
    VROShaderModifier(VROShaderEntryPoint entryPoint, std::vector<std::string> input);
    virtual ~VROShaderModifier();
    
    VROShaderEntryPoint getEntryPoint() const {
        return _entryPoint;
    }
    int getShaderModifierId() const {
        return _shaderModifierId;
    }
    
    /*
     Names can be added to shader modifiers, for debugging only. Will be appended
     to the parent shader name.
     */
    void setName(std::string name) {
        _name = name;
    }
    std::string getName() const {
        return _name;
    }
    
    /*
     Vertex attributes required by the modifier must be set. This is a bitmask of
     VROShaderMask values.
     */
    void setAttributes(int attributes) {
        _attributes = attributes;
    }
    int getAttributes() const {
        return _attributes;
    }
    
    /*
     Add a string of text and what it should be replaced with. This will perform
     a find and replace on the modified shader.
     */
    void addReplacement(std::string stringMatching, std::string replacementString) {
        _replacements[stringMatching] = replacementString;
    }
    
    /*
     Set a block that will bind the uniform of the given name. This will be 
     invoked each time a shader containining this modifier is bound. The block
     should set the uniform in the shader via glUniform* methods.
     */
    void setUniformBinder(std::string uniform, VROShaderProperty type, VROUniformBindingBlock bindingBlock);
    VROUniformBinder *getUniformBinder(std::string uniform) { return _uniformBinders[uniform]; }
    
    /*
     Get the pragma directive that corresponds to this modifier's entry point and
     the given section within a shader. This is the point in the shader where the
     modifier source will be inserted.
     */
    std::string getDirective(VROShaderSection section) const;
    
    /*
     Get the uniforms for which we have an attached binder.
     */
    std::vector<std::string> getUniforms() const;
    
    /*
     Get the replacement map, used to replace matching lines of code in the
     modified shader with new lines of code.
     */
    const std::map<std::string, std::string> &getReplacements() const {
        return _replacements;
    }
    
    /*
     Get the uniform declaration code. This gets placed at the top of the file.
     */
    std::string getUniformsSource() const {
        return _uniforms;
    }
    
    /*
     Get the body of the source. This gets placed in the main vertex or fragment
     function.
     */
    std::string getBodySource() const {
        return _body;
    }
    
private:
    
    int _shaderModifierId;
    std::string _name;
    
    /*
     VROShaderMask indicating any additional vertex attributes this modifier requires. See
     VROShaderProgram.h for values.
     */
    int _attributes;
    
    /*
     The new uniforms this shader modifier will add. Single string containing
     the uniform declarations, with newlines between them.
     */
    std::string _uniforms;
    
    /*
     The code this modifier is adding to the body of the shader.
     */
    std::string _body;
    
    /*
     Map of lines this modifier will replace in the shader it modifies.
     */
    std::map<std::string, std::string> _replacements;
    
    /*
     Indicates where in the shader this uniform's code will be injected.
     */
    VROShaderEntryPoint _entryPoint;
    
    /*
     Map of uniform names to the underlying type of the uniform, and the binder
     that will set said value on the uniform.
     */
    std::map<std::string, VROUniformBinder *> _uniformBinders;
    
    /*
     Return true if the given line is a variable declaration, and false
     if not. Variable declarations are lines that declare a uniform, in,
     our out variable.
     */
    bool isVariableDeclaration(std::string &line);
    
};

#endif
