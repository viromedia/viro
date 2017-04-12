//
//  VROShaderModifier.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/13/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROShaderModifier_h
#define VROShaderModifier_h

#include <stdlib.h>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <map>
#include "VROOpenGL.h"

class VROUniform;

typedef std::function<void(VROUniform *uniform, GLuint location)> VROUniformBindingBlock;

/*
 The entry point, which signals where in the shader program this modifier will
 act.
 
 ----------------
 
 Geometry entry point. The code may declare uniforms and read/write
 to the following structure:
 
 struct VROShaderGeometry {
   vec3 position;
   vec3 normal;
   vec2 texcoord;
   vec4 tangent;
 } _geometry;
 
 The Geometry entry point enables modifiers to change vertex parameters in 
 the vertex shader.
 
 ----------------
 
 Surface entry point. The code may declare uniforms and read/write
 to the following structure:
 
 struct VROSurface {
   lowp  vec4 diffuse_color;
   highp vec2 diffuse_texcoord;
   lowp float diffuse_intensity;
 
   lowp float shininess;
   highp vec2 specular_texcoord;
 
   lowp float alpha;
   lowp  vec3 normal;
   highp vec3 position;
 } _surface;
 
 The Surface entry point enables modifiers to change surface parameters, in
 the fragment shader, prior to the lighting computation.
 
 ----------------
 
 Fragment entry point. The code may declare uniforms and read/write
 to the variable:
 
 lowp vec4 _output_color;
 
 The Fragment entry point enables modifiers to alter the final color of each
 fragment, after the lighting computation.
 
 ----------------
 */
enum class VROShaderEntryPoint {
    Geometry,
    Surface,
    Fragment,
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
    void setUniformBinder(std::string uniform, VROUniformBindingBlock bindingBlock);
    
    /*
     Invoke the uniform binder for the given uniform.
     */
    void bindUniform(VROUniform *uniform, GLuint location);
    
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
    
    VROShaderEntryPoint _entryPoint;
    std::map<std::string, VROUniformBindingBlock> _uniformBinders;
    
    /*
     Extract the uniforms from the given source string and return them in a new
     string. Mutate the given string, removing the uniforms from the input source.
     For example, if the input is:
     
     uniform float testA;
     uniform float testB;
     _geometry.position.x = _geometry.position.x + testA;
     
     then this function will return:
     
     uniform float testA;
     uniform float testB;
     
     and the input string will be mutated to:
     
     _geometry.position.x = _geometry.position.x + testA;
     */
    std::string extractUniforms(std::string *source) const;
    void extractNextUniform(std::string *uniforms, std::string *body) const;
    
};

#endif
