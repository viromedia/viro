//
//  VROStringUtil.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 11/7/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROStringUtil_h
#define VROStringUtil_h

#include <stdio.h>
#include <string>
#include <vector>

class VROStringUtil {
    
public:

    static std::string toString64(uint64_t i);
    static std::string toString(int i);
    static std::string toString(double n, int precision);
    static std::wstring toWString(int i);
    static std::wstring toWString(double n, int precision);

    static int   toInt(std::string s);
    static float toFloat(std::string s);
    
    static std::vector<std::string> split(const std::string &s,
                                          const std::string &delimiters,
                                          bool emptiesOk);
    static std::vector<std::wstring> split(const std::wstring &s,
                                           const std::wstring &delimiters,
                                           bool emptiesOk);

    // Simple string compare, not unicode safe (since there are multiple ways of representing some
    // characters and this function does a character-by-character comparison.
    static bool strcmpinsensitive(const std::string& a, const std::string& b);
    
    static void toLowerCase(std::string &str);
    static bool startsWith(const std::string &candidate, const std::string &prefix);
    static bool endsWith(const std::string& candidate, const std::string& ending);

    // Replace the first instance of the 'from' in 'str' with 'to'. The input is modified.
    static bool replace(std::string &str, const std::string &from, const std::string &to);

    // Replace all instances of 'from' in 'str' with 'to'. The input is modified.
    static void replaceAll(std::string &str, const std::string &from, const std::string &to);

private:
    
};

#endif /* VROStringUtil_h */
