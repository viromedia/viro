//
//  VROUniform.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/14/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#include <string>
#include <memory>
#include "VROVector3f.h"
#include "VROVector4f.h"
#include "VROMatrix4f.h"
#include "VROLog.h"
#include "VROOpenGL.h"

class VROShaderModifier;
class VROGeometry;
class VROMaterial;

/*
 The various types of properties that may be set for a shader.
 */
enum class VROShaderProperty {
    Bool,
    Int,
    Float,
    Vec2,
    Vec3,
    Vec4,
    BVec2,
    BVec3,
    BVec4,
    IVec2,
    IVec3,
    IVec4,
    Mat2,
    Mat3,
    Mat4
};

/*
 Wraps around a uniform variable, holding its location in the shader and the glUniform
 function needed to set its value.
 */
class VROUniform {
    
public:
    
    static VROUniform *newUniformForType(const std::string &name, VROShaderProperty type, int arraySize);
    
    VROUniform(const std::string &name) :
        _name(name),
        _location(-1) {
    }
    
    virtual ~VROUniform() {}
    
    const std::string getName() const {
        return _name;
    }
    
    virtual void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) = 0;
    
    inline void setLocation(int location) {
        this->_location = location;
    }
    
    virtual void reset() {
        //sublcass to reset any cached value
    }
    
    void setVec3(VROVector3f value) {
        if (_location == -1) {
            return;
        }
        glUniform3f(_location, value.x, value.y, value.z);
    }
    
    void setVec4(VROVector4f value) {
        if (_location == -1) {
            return;
        }
        glUniform4f(_location, value.x, value.y, value.z, value.w);
    }
    
    void setMat4(VROMatrix4f value) {
        if (_location == -1) {
            return;
        }
        
        glUniformMatrix4fv(_location, 1, GL_FALSE, value.getArray());
    }
    
    void setInt(int value) {
        if (_location == -1) {
            return;
        }
        glUniform1i(_location, value);
    }
    
    void setFloat(float value) {
        if (_location == -1) {
            return;
        }
        glUniform1f(_location, value);
    }
    
protected:
    int _location;

private:
    const std::string _name;
    
};

class VROUniform1i: public VROUniform {
public:
    VROUniform1i(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize), _curValue(0) {
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) {
        if (_location == -1) {
            return;
        }
        GLint *val = (GLint *) value;
        
        if (*val != _curValue) {
            glUniform1iv(_location, _arraySize, val);
            _curValue = *val;
        }
    }
    
    void reset() {
        _curValue = 0;
    }
    
private:
    const int _arraySize;
    int _curValue;
    
};

class VROUniform2i: public VROUniform {
public:
    VROUniform2i(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize) {
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) {
        //passert (_location != -1);
        glUniform2iv(_location, _arraySize, (GLint *) value);
    }
    
private:
    const int _arraySize;
    
};

class VROUniform3i: public VROUniform {
public:
    VROUniform3i(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize) {
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) {
        passert (_location != -1);
        glUniform3iv(_location, _arraySize, (GLint *) value);
    }
    
private:
    const int _arraySize;
    
};

class VROUniform4i: public VROUniform {
public:
    VROUniform4i(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize) {
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) {
        //passert (_location != -1);
        glUniform4iv(_location, _arraySize, (GLint *) value);
    }
    
private:
    const int _arraySize;
    
};

class VROUniform1f: public VROUniform {
public:
    
    VROUniform1f(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize), _curValue(9999) {
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) {
        if (_location == -1) {
            return;
        }
        GLfloat *val = (GLfloat *) value;
        
        if (_arraySize > 1 || *val != _curValue) {
            glUniform1f(_location, *val);
            _curValue = *val;
        }
    }
    
    void reset() {
        _curValue = 9999;
    }
    
private:
    const int _arraySize;
    float _curValue;
    
};

class VROUniform2f: public VROUniform {
public:
    VROUniform2f(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize) {
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) {
        //passert (_location != -1);
        glUniform2fv(_location, _arraySize, (GLfloat *) value);
    }
    
private:
    const int _arraySize;
    
};

class VROUniform3f: public VROUniform {
public:
    VROUniform3f(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize) {
        _curValue[0] = 0;
        _curValue[1] = 0;
        _curValue[2] = 0;
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) {
        if (_location == -1) {
            return;
        }
        
        GLfloat *val = (GLfloat *) value;
        if (_arraySize > 1 || memcmp(val, _curValue, sizeof(GLfloat) * 3) != 0) {
            glUniform3fv(_location, _arraySize, val);
            memcpy(_curValue, val, sizeof(GLfloat) * 3);
        }
    }
    
    void getCurrentValue(void *result) {
        memcpy(result, _curValue, sizeof(float) * 3);
    }
    
    void reset() {
        _curValue[0] = 0;
        _curValue[1] = 0;
        _curValue[2] = 0;
    }
    
private:
    const int _arraySize;
    GLfloat _curValue[3];
    
};

class VROUniform4f: public VROUniform {
public:
    VROUniform4f(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize) {
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) {
        //passert (_location != -1);
        glUniform4fv(_location, _arraySize, (GLfloat *) value);
    }
    
private:
    const int _arraySize;
    
};

class VROUniformMat2: public VROUniform {
public:
    VROUniformMat2(const std::string &name) :
    VROUniform(name) {
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) {
        //passert (_location != -1);
        glUniformMatrix2fv(_location, 1, GL_FALSE, (GLfloat *) value);
    }
    
};

class VROUniformMat3: public VROUniform {
public:
    VROUniformMat3(const std::string &name) :
    VROUniform(name) {
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) {
        //passert (_location != -1);
        glUniformMatrix3fv(_location, 1, GL_FALSE, (GLfloat *) value);
    }
    
};

class VROUniformMat4: public VROUniform {
public:
    VROUniformMat4(const std::string &name) :
    VROUniform(name) {
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material) {
        //passert (_location != -1);
        glUniformMatrix4fv(_location, 1, GL_FALSE, (GLfloat *) value);
    }
    
};

class VROUniformShaderModifier : public VROUniform {
public:
    VROUniformShaderModifier(const std::string &name, std::shared_ptr<VROShaderModifier> modifier) :
        VROUniform(name),
        _modifier(modifier) {
    }
    
    void set(const void *value, const VROGeometry *geometry, const VROMaterial *material);
    
private:
    std::shared_ptr<VROShaderModifier> _modifier;
    
};

