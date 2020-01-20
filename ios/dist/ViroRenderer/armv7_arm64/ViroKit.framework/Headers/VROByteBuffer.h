//
//  VROByteBuffer.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/21/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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

#ifndef VROBYTEBUFFER_H_
#define VROBYTEBUFFER_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <string>

class VROByteBuffer final {
public:
    
    /*
     Default writing constructor, creating a new byte array of default 
     length.
     */
    VROByteBuffer();

    /*
     Writing constructor, creating a new byte array of the given length.
     */
    VROByteBuffer(size_t capacity);

    /*
     Read constructor. Wrap the given array of bytes of the given length. Set copy to true
     to copy the bytes into this VROByteBuffer. If false, then the bytes will continue to be
     owned by the caller and will not be deleted when this VROByteBuffer is destroyed.
     */
    VROByteBuffer(const void *bytes, size_t length, bool copy = true);

    /*
     Wrap the given string with a VROByteBuffer.

     !!DANGER!!

     The caller must ensure that the passed-in string stays in scope throughout the
     lifetime of this buffer. The string is NOT copied into this object (for perf
     reasons).
     */
    VROByteBuffer(const std::string &byteString);

    /*
     Copy semantics
     */
    VROByteBuffer(VROByteBuffer *toCopy);
    VROByteBuffer(const VROByteBuffer& toCopy) = delete;
    VROByteBuffer& operator=(const VROByteBuffer& rhs) = delete;

    /*
     Move semantics
     */
    VROByteBuffer(VROByteBuffer&& moveFrom);
    VROByteBuffer& operator=(VROByteBuffer&& moveFrom);

    ~VROByteBuffer();
    
    size_t capacity() const {
        return _capacity;
    }

    /*
     Read methods.
     */
    bool           readBool();
    float          readHalf();
    float          readFloat();
    double         readDouble();
    int            readInt();
    uint64_t       readUInt64();
    short          readShort();
    unsigned short readUnsignedShort();
    signed char    readByte();
    unsigned char  readUnsignedByte();
    void *         readPointer();

    /*
     Read strings, allocating new space.
     */
    std::string readSTLStringUTF8NullTerm();
    std::string readSTLString();
    std::string readSTLStringUTF8();
    std::string readSTLText();
    std::string readSTLTextUTF8();

    /*
     Read the next null-terminated string.
     */
    void readStringNullTerm(char *result);
    std::string readStringNullTerm();

    size_t getPosition() const {
        return _pos;
    }
    void setPosition(size_t position);
    void skip(size_t bytes);
    
    /*
     Underlying data manipulation and retrieval.
     */
    char* getData();
    char* getDataFromPosition(size_t position);

    /*
     Create a byte-buffer from a subrange of this byte-buffer, using absolute position and length.
     The underlying bytes will be copied, and the resultant buffer will own said bytes.
     */
    VROByteBuffer *split(size_t offset, size_t length);

    /*
     Peek methods.
     */
    signed char peekByte();
    int peekInt();

    /*
     Batch read methods.
     */
    void copyBytes(void *dest, int numBytes);
    void copyChars(char *dest, int numChars);
    void copyShorts(short *dest, int numShorts);
    void copyInts(int *dest, int numInts);
    void copyFloats(float *dest, int numFloats);
    void copyLongs(uint64_t *dest, int numLongs);
    signed char *readNumChars(int numChars);
    short *readNumShorts(int numShorts);

    /*
     Grow the byte-buffer to fit the given number of additional bytes PAST the
     current position of the buffer. If the buffer has space remaining, do not
     necesarily grow the buffer.
     */
    void grow(size_t additionalBytesRequired);

    /*
     Shrink the buffer down to the given size.
     */
    void shrink(size_t size);

    /*
     Fill the buffer with zeros.
     */
    void clear();

    /*
     Set position to zero.
     */
    void rewind();
    
    /*
     Invoke to make the ByteBuffer *not* delete its underlying bytes when
     deallocated. This is useful when we want to move the data to another
     container like VROData without performing a copy.
     */
    void releaseBytes() {
        _freeOnDealloc = false;
    }

    /*
     Write methods.
     */
    void writeBool(bool value);
    void writeByte(char value);
    void writeShort(short value);
    void writeInt(int value);
    void writeHalf(float value);
    void writeFloat(float value);
    void writeFloats(float *pValues, const int numFloats);
    void writeDouble(double value);
    void writeLong(uint64_t value);
    void writeStringNullTerm(const char *value);
    void writeBytes(const void *bytes, size_t length);
    void writeBuffer(VROByteBuffer *src, size_t length);
    void writeChars(const char *value);
    void writePointer(void *pointer);

    /*
     Fill the buffer with the given value, repeated across the given number of bytes. Equivalent
     to a memset. The buffer must be large enough to fit numBytes; it will not be automatically grown.
     */
    void fill(unsigned char value, size_t numBytes);

    /*
     Write the given number of bytes from this buffer to the destination, incrementing
     the position of both buffers.
     */
    void writeToBuffer(VROByteBuffer *dest, size_t length);

    /*
     Write the given number of bytes from this buffer to the destination, incrementing
     the position of the destination buffer and rewinding this buffer to it's position
     before the call to this function.
     */
    void writeToBufferAndRewind(VROByteBuffer *dest, size_t length) const;

    /*
     Write the entirety of the buffer (from position 0 to capacity) to the given file.
     Create the file if it does not exist.  The second function writes length bytes starting
     at the given offset.
     */
    void writeToFile(const char *path);
    void writeToFile(const std::string &path);
    void writeToFile(const char *path, size_t offset, size_t length);
    void writeToFile(const std::string &path, size_t offset, size_t length);

private:
    
    /*
     The current position (in bytes) in the data. Incremented as we read.
     */
    size_t _pos;
    
    /*
     The total capacity of the buffer (in bytes).
     */
    size_t _capacity;
    
    /*
     The inner buffer containing the data.
     */
    char *_buffer;

    /*
     True if the underlying bytes should be freed when this buffer is deallocated.
     */
    bool _freeOnDealloc;
    
};

#endif /* VROBYTEBUFFER_H_ */
