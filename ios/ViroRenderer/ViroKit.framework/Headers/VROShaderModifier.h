//
//  VROShaderModifier.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/13/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#include <stdlib.h>
#include <vector>
#include <string>
#include <functional>
#include <map>

#import <GLKit/GLKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES3/glext.h>

class VROUniform;

typedef std::function<void(VROUniform *uniform, GLuint location)> VROUniformBindingBlock;

enum class VROShaderEntryPoint {
    Geometry
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
    
    static uint32_t hashShaderModifiers(const std::vector<std::shared_ptr<VROShaderModifier>> &modifiers);
    
    VROShaderModifier(VROShaderEntryPoint entryPoint, std::vector<std::string> input);
    virtual ~VROShaderModifier();
    
    int getShaderModifierId() const {
        return _shaderModifierId;
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
    
    std::string _uniforms;
    std::string _body;
    
    VROShaderEntryPoint _entryPoint;
    std::map<std::string, VROUniformBindingBlock> _uniformBinders;
    
    /*
     Extract the uniforms from the given source string and return them in a new
     string. Mutate the given string, removing the uniforms from the input source.
     */
    std::string extractUniforms(std::string *source);
    void extractNextUniform(std::string *uniforms, std::string *body);
    
};
