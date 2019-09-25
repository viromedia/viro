//
//  VRTBundleURLProvider.m
//  ViroReact
//
//  Created by Vik Advani on 2/1/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
//

#import "VRTBundleURLProvider.h"

@implementation VRTBundleURLProvider

- (BOOL)isPackagerRunning:(NSString *)host
{
    NSURL *url = [serverRootWithHost(host) URLByAppendingPathComponent:@"status"];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    NSURLResponse *response;
    NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:NULL];
    NSString *status = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    return [status isEqualToString:@"packager-status:running"];
}

- (NSString *)guessPackagerHost
{
    static NSString *ipGuess = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        NSString *ipPath = [[NSBundle mainBundle] pathForResource:@"ngrokendpoint" ofType:@"txt"];
        ipGuess = [[NSString stringWithContentsOfFile:ipPath encoding:NSUTF8StringEncoding error:nil]
                   stringByTrimmingCharactersInSet:[NSCharacterSet newlineCharacterSet]];
    });
    
    
    if (ipGuess != nil && [self isPackagerRunning:ipGuess]) {
        return ipGuess;
    }
    return nil;
}

static NSURL *serverRootWithHost(NSString *host)
{
    return [NSURL URLWithString:host];
}


- (NSString *)packagerServerHost
{
    NSString *host = [self guessPackagerHost];
    if (host) {
        return host;
    }
    return nil;
}

- (NSURL *)jsBundleURLForBundleRoot:(NSString *)bundleRoot fallbackResource:(NSString *)resourceName
{
    resourceName = resourceName ?: @"main";
    NSString *packagerServerHost = [self packagerServerHost];
    if (!packagerServerHost) {
        return [[NSBundle mainBundle] URLForResource:resourceName withExtension:@"jsbundle"];
    } else {
        NSString *path = [NSString stringWithFormat:@"/%@.bundle", bundleRoot];
        // When we support only iOS 8 and above, use queryItems for a better API.
        NSString *query = [NSString stringWithFormat:@"platform=ios&dev=%@&minify=%@",
                           @"true",
                           @"false"];
        return [[self class] resourceURLForResourcePath:path packagerHost:packagerServerHost query:query];
    }
}

+ (NSURL *)resourceURLForResourcePath:(NSString *)path
                         packagerHost:(NSString *)packagerHost
                                query:(NSString *)query
{
    NSURLComponents *components = [NSURLComponents componentsWithURL:serverRootWithHost(packagerHost) resolvingAgainstBaseURL:NO];
    components.path = path;
    if (query != nil) {
        components.query = query;
    }
    return components.URL;
}

@end
