//
//  VROStringUtil.hpp
//  ViroRenderer
//
//  Created by Raj Advani on 11/7/16.
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
    
    static std::vector<std::string>  split(const std::string &s,
                                           const std::string &delimiters,
                                           bool emptiesOk);
    static std::vector<std::wstring> split(const std::wstring &s,
                                           const std::wstring &delimiters,
                                           bool emptiesOk);
    
    // Trim the whitespace at the front and back of the string, and return the result.
    static std::string trim(std::string s);

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
    
    /*
     Escape reserved and unsafe characters from the given text so it will work as a URL
     query string.
     */
    static std::string escapeReservedURLCharacters(std::string text);
    
    /*
     Escape whitespace from the given text.
     */
    static std::string escapeSpaces(std::string text);
    
    /*
     Separate the given url into its scheme + host, path, query, and fragment. The components
     are returned in a list, whereby:
     
     index 0: scheme + host
     index 1: path
     index 2: query
     index 3: fragment
     
     For example, if the input string is 'http://localhost:8080/path/to/file?query#fragment', then
     the return value will be:
     
     index 0: http://localhost:8080
     index 1: /path/to/file
     index 2: ?query
     index 3: #fragment
     
     Returns an empty list if the URL could not be parsed.
     */
    static std::vector<std::string> parseURL(std::string url);
    
    /*
     Encode the given URL by parsing it into its components, escaping the path and query
     parts, then aggregating the URL back together.
     */
    static std::string encodeURL(std::string url);
    
    /*
     Print the given code, with line numbers.
     */
    static void printCode(const std::string &code);

private:
    
};

#endif /* VROStringUtil_h */
