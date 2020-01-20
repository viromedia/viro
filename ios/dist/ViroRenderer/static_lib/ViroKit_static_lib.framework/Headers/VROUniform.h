//
//  VROUniform.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/14/16.
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

#ifndef VROUNIFORM_H_
#define VROUNIFORM_H_

#include <string>
#include <memory>
#include <functional>
#include "VROVector3f.h"
#include "VROVector4f.h"
#include "VROMatrix4f.h"
#include "VROLog.h"
#include "VROOpenGL.h"

static const float kInitialValue = -9999;

class VROShaderModifier;
class VROGeometry;
class VROMaterial;
class VROUniform;

typedef std::function<void(VROUniform *uniform,
                           const VROGeometry *geometry, const VROMaterial *material)> VROUniformBindingBlock;

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
    
    virtual void set(const void *value) = 0;
    
    void setLocation(int location) {
        this->_location = location;
    }
    int getLocation() const { return _location; }
    
    virtual void reset() {
        //sublcass to reset any cached value
    }
    
    void setVec3(VROVector3f value) {
        if (_location == -1) {
            return;
        }
        float array[3] = { value.x, value.y, value.z };
        set(array);
    }
    
    void setVec4(VROVector4f value) {
        if (_location == -1) {
            return;
        }
        float array[4] = { value.x, value.y, value.z, value.w };
        set(array);
    }
    
    void setMat4(VROMatrix4f value) {
        if (_location == -1) {
            return;
        }
        set(value.getArray());
    }
    
    void setInt(int value) {
        if (_location == -1) {
            return;
        }
        GL( glUniform1i(_location, value) );
    }
    
    void setFloat(float value) {
        if (_location == -1) {
            return;
        }
        set(&value);
    }
    
protected:
    
    /*
     Location of the uniform in the shader.
     */
    int _location;

private:
    const std::string _name;
    
};

class VROUniform1i: public VROUniform {
public:
    VROUniform1i(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize), _curValue(0) {
    }
    virtual ~VROUniform1i() {}
    
    void set(const void *value) {
        if (_location == -1) {
            return;
        }
        GLint *val = (GLint *) value;
        
        if (*val != _curValue) {
            GL( glUniform1iv(_location, _arraySize, val) );
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
    virtual ~VROUniform2i() {}
    
    void set(const void *value) {
        //passert (_location != -1);
        GL( glUniform2iv(_location, _arraySize, (GLint *) value) );
    }
    
private:
    const int _arraySize;
    
};

class VROUniform3i: public VROUniform {
public:
    VROUniform3i(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize) {
    }
    virtual ~VROUniform3i() {}
    
    void set(const void *value) {
        passert (_location != -1);
        GL( glUniform3iv(_location, _arraySize, (GLint *) value) );
    }
    
private:
    const int _arraySize;
    
};

class VROUniform4i: public VROUniform {
public:
    VROUniform4i(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize) {
    }
    virtual ~VROUniform4i() {}
    
    void set(const void *value) {
        //passert (_location != -1);
        GL( glUniform4iv(_location, _arraySize, (GLint *) value) );
    }
    
private:
    const int _arraySize;
    
};

class VROUniform1f: public VROUniform {
public:
    
    VROUniform1f(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize), _curValue(kInitialValue) {
    }
    virtual ~VROUniform1f() {}
    
    void set(const void *value) {
        if (_location == -1) {
            return;
        }
        GLfloat *val = (GLfloat *) value;
        
        if (_arraySize > 1 || *val != _curValue) {
            GL( glUniform1f(_location, *val) );
            _curValue = *val;
        }
    }
    
    void reset() {
        _curValue = kInitialValue;
    }
    
private:
    const int _arraySize;
    float _curValue;
    
};

class VROUniform2f: public VROUniform {
public:
    VROUniform2f(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize) {
        for (int i = 0; i < 2; i++) {
            _curValue[i] = kInitialValue;
        }
    }
    virtual ~VROUniform2f() {}
    
    void set(const void *value) {
        if (_arraySize > 1 || memcmp(value, _curValue, sizeof(GLfloat) * 2) != 0) {
            GL( glUniform2fv(_location, _arraySize, (GLfloat *) value) );
            memcpy(_curValue, value, sizeof(GLfloat) * 2);
        }
    }
    
private:
    const int _arraySize;
    GLfloat _curValue[2];
    
};

class VROUniform3f: public VROUniform {
public:
    VROUniform3f(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize) {
        for (int i = 0; i < 3; i++) {
            _curValue[i] = kInitialValue;
        }
    }
    virtual ~VROUniform3f() {}
    
    void set(const void *value) {
        if (_location == -1) {
            return;
        }
        
        GLfloat *val = (GLfloat *) value;
        if (_arraySize > 1 || memcmp(val, _curValue, sizeof(GLfloat) * 3) != 0) {
            GL( glUniform3fv(_location, _arraySize, val) );
            memcpy(_curValue, val, sizeof(GLfloat) * 3);
        }
    }
    
    void getCurrentValue(void *result) {
        memcpy(result, _curValue, sizeof(float) * 3);
    }
    
    void reset() {
        for (int i = 0; i < 3; i++) {
            _curValue[i] = kInitialValue;
        }
    }
    
private:
    const int _arraySize;
    GLfloat _curValue[3];
    
};

class VROUniform4f: public VROUniform {
public:
    VROUniform4f(const std::string &name, int arraySize) :
    VROUniform(name), _arraySize(arraySize) {
        for (int i = 0; i < 4; i++) {
            _curValue[i] = kInitialValue;
        }
    }
    virtual ~VROUniform4f() {}
    
    void set(const void *value) {
        if (_arraySize > 1 || memcmp(value, _curValue, sizeof(GLfloat) * 4) != 0) {
            GL( glUniform4fv(_location, _arraySize, (GLfloat *) value) );
            memcpy(_curValue, value, sizeof(GLfloat) * 4);
        }
    }
    
private:
    const int _arraySize;
    GLfloat _curValue[4];
    
};

class VROUniformMat2: public VROUniform {
public:
    VROUniformMat2(const std::string &name) :
    VROUniform(name) {
        for (int i = 0; i < 4; i++) {
            _curValue[i] = kInitialValue;
        }
    }
    virtual ~VROUniformMat2() {}
    
    void set(const void *value) {
        if (memcmp(value, _curValue, sizeof(GLfloat) * 4) != 0) {
            GL( glUniformMatrix2fv(_location, 1, GL_FALSE, (GLfloat *) value) );
            memcpy(_curValue, value, sizeof(GLfloat) * 4);
        }
    }
    
private:
    GLfloat _curValue[4];
    
};

class VROUniformMat3: public VROUniform {
public:
    VROUniformMat3(const std::string &name) :
    VROUniform(name) {
        for (int i = 0; i < 9; i++) {
            _curValue[i] = kInitialValue;
        }
    }
    virtual ~VROUniformMat3() {}
    
    void set(const void *value) {
        if (memcmp(value, _curValue, sizeof(GLfloat) * 9) != 0) {
            GL( glUniformMatrix3fv(_location, 1, GL_FALSE, (GLfloat *) value) );
            memcpy(_curValue, value, sizeof(GLfloat) * 9);
        }
    }
    
private:
    GLfloat _curValue[9];
    
};

class VROUniformMat4: public VROUniform {
public:
    VROUniformMat4(const std::string &name) :
    VROUniform(name) {
        for (int i = 0; i < 16; i++) {
            _curValue[i] = kInitialValue;
        }
    }
    virtual ~VROUniformMat4() {}
    
    void set(const void *value) {
        if (memcmp(value, _curValue, sizeof(GLfloat) * 16) != 0) {
            GL( glUniformMatrix4fv(_location, 1, GL_FALSE, (GLfloat *) value) );
            memcpy(_curValue, value, sizeof(GLfloat) * 16);
        }
    }
    
private:
    GLfloat _curValue[16];
    
};

class VROUniformBinder {
public:
    VROUniformBinder(const std::string &name, VROShaderProperty type, VROUniformBindingBlock bindingBlock) :
        _name(name),
        _type(type),
        _bindingBlock(bindingBlock) {
    }
    virtual ~VROUniformBinder() {
    }
    
    std::string getName() const { return _name; }
    VROShaderProperty getType() const { return _type; }
    void setForMaterial(VROUniform *uniform, const VROGeometry *geometry, const VROMaterial *material);
    
private:
    std::string _name;
    VROShaderProperty _type;
    VROUniformBindingBlock _bindingBlock;
    
};

#endif

