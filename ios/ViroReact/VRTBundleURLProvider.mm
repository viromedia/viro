//
//  VRTBundleURLProvider.m
//  ViroReact
//
//  Created by Vik Advani on 2/1/17.
//  Copyright © 2017 Viro Media. All rights reserved.
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
