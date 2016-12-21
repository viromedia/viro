//
//  VROAnimationManager.m
//  React
//
//  Created by Vik Advani on 2/17/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VROAnimationManager.h"
#import <ViroKit/ViroKit.h>



@interface NullableValue:NSObject

@property (nonatomic, retain) NSNumber *number;
@property (nonatomic, assign) BOOL isAdditive;


- (instancetype)initWithNumber:(NSNumber *) number isAdditive:(BOOL)additive;
- (instancetype)init;
- (BOOL)isNull;
- (BOOL)isAdditive;
- (float)getFloatValue;


@end

@implementation NullableValue

- (instancetype)init {
    self = [super init];
    self.number = nil;
    self.isAdditive = false;
    return self;
}

- (instancetype)initWithNumber:(NSNumber *)number isAdditive:(BOOL)additive{
    self = [super init];
    self.number = number;
    self.isAdditive = additive;
    return self;
}


-(BOOL)isNull {
    if(self.number == nil) {
        return true;
    }
    return false;
}

-(float)getFloatValue {
    if(self.number != nil) {
        return [self.number floatValue];
    }
    //default value..
    return 0.0;
}

-(int)getIntValue {
    if(self.number != nil) {
        return [self.number intValue];
    }
    return 0;
}

@end

@implementation AnimationProperty {
}
@end

@implementation VROAnimationManager {
    NSMutableDictionary *_animationPropertyDictionary;
    NSMutableDictionary *_animationChainDictionary;
}



@synthesize bridge = _bridge;

RCT_EXPORT_MODULE()


RCT_EXPORT_METHOD(setJSAnimations:(NSDictionary *)animationsDict)
{
    self.animations = animationsDict;
    [self loadAnimations];
}

//Load material images on the main thread, blocking main thread for now.
- (void)loadAnimations {
    for(id key in self.animations) {
        NSString *animationName = (NSString *)key;
        NSObject *animationObject = [self.animations objectForKey:key];
        // Create an Animation Block for each animation.
        if([animationObject isKindOfClass:[NSDictionary class]]){
            AnimationProperty *animationProperty = [self createAnimationProperty:animationObject];
            [_animationPropertyDictionary setObject:animationProperty forKey:animationName];
        }
            // Pass in the 2 demensional array for containing chained animations.
        else if ([animationObject isKindOfClass:[NSArray class]]) {
            [_animationChainDictionary setObject:animationObject forKey:animationName];
        } else {
            NSLog(@"ERROR! Attempted to create an animation block without the appropriate tags!");
        }
    }
}


- (instancetype)init
{
    self = [super init];
    _animationPropertyDictionary = [[NSMutableDictionary alloc] init];
    _animationChainDictionary = [[NSMutableDictionary alloc] init];
    return self;
}

- (AnimationProperty *) getAnimationPropertyForName:(NSString *)name {
    return _animationPropertyDictionary[name];
}

- (NSArray *)getAnimationChainForName:(NSString *)name {
    return _animationChainDictionary[name];
}

//we have to make sure animations are fully validated on the Javascript side before we get to this point. Add error checking here to to make sure.
-(AnimationProperty *)createAnimationProperty:(NSDictionary *)animationDictionary{
    NSDictionary *propertyDictionary = animationDictionary[@"properties"];
    NSNumber *durationMilliseconds = animationDictionary[@"duration"];
    NSNumber *delayMilliseconds = animationDictionary[@"delay"];
    NSString *functionType = animationDictionary[@"easing"];
    float durationInSeconds = [durationMilliseconds floatValue] / 1000.0;
    float delayInSeconds = [delayMilliseconds floatValue] / 1000.0;

    VROTimingFunctionType type = VROTimingFunctionType::Linear;
    if (!functionType){
        type = [self convertTimingFunction:functionType];
    }

    AnimationProperty *property = [[AnimationProperty alloc] init];
    [property setDelayMilliseconds:delayInSeconds];
    [property setDurationMilliseconds:durationInSeconds];
    [property setFunctionType:type];
    [property setPropertyDictionary:propertyDictionary];
    return property;
}

- (void)mapAnimatedPropertiesToNode:(VRONode *const)node properties:(NSDictionary *)propertyDict propCheckPoints:(NSMutableDictionary *)propCheckPoints{

    NullableValue *positionX = [self getValue:@"positionX" properties:propertyDict];
    NullableValue *positionY = [self getValue:@"positionY" properties:propertyDict];
    NullableValue *positionZ = [self getValue:@"positionZ" properties:propertyDict];

    NullableValue *scaleX = [self getValue:@"scaleX" properties:propertyDict];
    NullableValue *scaleY = [self getValue:@"scaleY" properties:propertyDict];
    NullableValue *scaleZ = [self getValue:@"scaleZ" properties:propertyDict];

    NullableValue *rotateX = [self getValue:@"rotateX" properties:propertyDict];
    NullableValue *rotateY = [self getValue:@"rotateY" properties:propertyDict];
    NullableValue *rotateZ = [self getValue:@"rotateZ" properties:propertyDict];

    NullableValue *opacity = [self getValue:@"opacity" properties:propertyDict];

    NullableValue *color = [self getValue:@"color" properties:propertyDict];

    [self animatePosition:node posX:positionX posY:positionY posZ:positionZ];
    [self animateColor:node color:color];
    [self animateOpacity:node opacity:opacity];
    [self animateScale:node scaleX:scaleX scaleY:scaleY scaleZ:scaleZ];
    [self animateRotation:node rotateX:rotateX rotateY:rotateY rotateZ:rotateZ propCheckPoints:propCheckPoints];
}

- (float)getStoredProperty:(NSMutableDictionary *)propCheckPoints propName:(NSString *)propName {
    NSNumber *number = propCheckPoints[propName];
    if(number == nil){
        propCheckPoints[propName] = [[NSNumber alloc] initWithFloat:0.0];
        return 0.0;
    }

    return [number floatValue];
}


- (NullableValue *)getValue:(NSString *)propertyName properties:(NSDictionary *)propertyDictionary {

    NullableValue *nullableValue = [[NullableValue alloc] init];

    id value = propertyDictionary[propertyName];
    if(value == nil) {
        return nullableValue;
    }

    if ([value isKindOfClass:[NSString class]]) {
        NSString *stringVal = (NSString *)value;
        unichar firstChar = [stringVal characterAtIndex:0];
        int numberIndex = 0;
        BOOL isAdditive = false;
        if (firstChar == '+' ) {
            // First char is a '+'.
            numberIndex = 1;
            isAdditive = true;
        }
      
        if([stringVal length] > 2) {
          unichar secondChar = [stringVal characterAtIndex:1];
          if (secondChar == '=' ) {
            if(numberIndex == 1) {
              numberIndex = 2;
              isAdditive = true;
            }
          }
        }

        NSString *numberString = [stringVal substringFromIndex:numberIndex];
        NSNumberFormatter *f = [[NSNumberFormatter alloc] init];
        f.numberStyle = NSNumberFormatterDecimalStyle;
        NSNumber *number = [f numberFromString:numberString];
        if(number != nil){
            nullableValue = [[NullableValue alloc] initWithNumber:number isAdditive:isAdditive];
            return nullableValue;
        }else{
            //unable to properly parse this value..
            return nullableValue;
        }

    }else if([value isKindOfClass:[NSNumber class]]) {
        NSNumber *number = (NSNumber *)value;
        nullableValue = [[NullableValue alloc] initWithNumber:number isAdditive:false];
        return nullableValue;
    }

    return nullableValue;
}

//Convert string to propert VROTimingFunction enum,
- (VROTimingFunctionType)convertTimingFunction:(NSString *)name {

    if([@"Linear" caseInsensitiveCompare:name] == NSOrderedSame){
        return VROTimingFunctionType::Linear;
    }else if([@"EaseIn" caseInsensitiveCompare:name] == NSOrderedSame){
        return VROTimingFunctionType::EaseIn;
    }else if([@"EaseOut" caseInsensitiveCompare:name] == NSOrderedSame){
        return VROTimingFunctionType::EaseOut;
    }else if([@"EaseInEaseOut" caseInsensitiveCompare:name] == NSOrderedSame){
        return VROTimingFunctionType::EaseInEaseOut;
    }else if([@"Bounce" caseInsensitiveCompare:name] == NSOrderedSame){
        return VROTimingFunctionType::Bounce;
    }else if([@"PowerDecel" caseInsensitiveCompare:name] == NSOrderedSame){
        return VROTimingFunctionType::PowerDecel;
    }
    //return default if nothing else matches
    return VROTimingFunctionType::Linear;
}

//helper methods to animate various properties
- (void)animatePosition:(VRONode *const)node posX:(NullableValue *)positionX posY:(NullableValue *)positionY posZ:(NullableValue *)positionZ {

    if([positionX isNull] && [positionY isNull] && [positionZ isNull]) {
        return;
    }

    float posx = ([positionX isNull]) ? node->getPosition().x : [positionX getFloatValue];
    float posy = ([positionY isNull]) ? node->getPosition().y : [positionY getFloatValue];
    float posz = ([positionZ isNull]) ? node->getPosition().z : [positionZ getFloatValue];

    if([positionX isAdditive]){
        posx += node->getPosition().x;
    }

    if([positionY isAdditive]){
        posy += node->getPosition().y;
    }

    if([positionZ isAdditive]){
        posz += node->getPosition().z;
    }

    node->setPosition({posx, posy, posz});
}

- (void)animateScale:(VRONode *const)node scaleX:(NullableValue *)scaleX scaleY:(NullableValue *)scaleY scaleZ:(NullableValue *)scaleZ {

    if(![scaleX isNull] || ![scaleY isNull] || ![scaleZ isNull]) {
        float scalex = ([scaleX isNull]) ? node->getScale().x : [scaleX getFloatValue];
        float scaley = ([scaleY isNull]) ? node->getScale().y : [scaleY getFloatValue];
        float scalez = ([scaleZ isNull]) ? node->getScale().z : [scaleZ getFloatValue];

        if([scaleX isAdditive]){
            scalex += node->getScale().x;
        }

        if([scaleY isAdditive]){
            scaley += node->getScale().y;
        }

        if([scaleZ isAdditive]){
            scalez += node->getScale().z;
        }
        node->setScale({scalex, scaley, scalez});
    }
}

- (void)animateColor:(VRONode *const)node color:(NullableValue *)color {

    if([color isNull]) {
        return;
    }
    NSUInteger colorVal = (NSUInteger)[color getIntValue];

    //parse the int into seperate color components.
    CGFloat a = ((colorVal >> 24) & 0xFF) / 255.0;
    CGFloat r = ((colorVal >> 16) & 0xFF) / 255.0;
    CGFloat g = ((colorVal >> 8) & 0xFF) / 255.0;
    CGFloat b = (colorVal & 0xFF) / 255.0;

    VROVector4f vecColor(r, g, b, a);
    if (node->getGeometry()) {
        std::shared_ptr<VROGeometry> geometry = node->getGeometry();
        geometry->getMaterials()[0]->getDiffuse().setColor(vecColor);
    }
}

- (void)animateOpacity:(VRONode *const)node opacity:(NullableValue *)opacity {
    if(![opacity isNull]) {
        float opacityVal = ([opacity isNull]) ? node->getOpacity() : [opacity getFloatValue];

        if([opacity isAdditive]) {
            opacityVal += node->getOpacity();
        }
        node->setOpacity(opacityVal);
    }
}

- (void)animateRotation:(VRONode *const)node rotateX:(NullableValue *)rotateX rotateY:(NullableValue *)rotateY rotateZ:(NullableValue *)rotateZ propCheckPoints:(NSMutableDictionary *)propCheckPoints {

    if(![rotateX isNull] || ![rotateY isNull] || ![rotateZ isNull]) {
        float rotatex = ([rotateX isNull]) ? node->getRotation().X : toRadians([rotateX getFloatValue]);
        float rotatey = ([rotateY isNull]) ? node->getRotation().Y : toRadians([rotateY getFloatValue]);
        float rotatez = ([rotateZ isNull]) ? node->getRotation().Z : toRadians([rotateZ getFloatValue]);

        if([rotateX isAdditive]) {
            float storedRotateX = [self getStoredProperty:propCheckPoints propName:@"rotateX"];
            rotatex += storedRotateX;
            propCheckPoints[@"rotateX"] = [[NSNumber alloc] initWithFloat:rotatex];
        }

        if([rotateY isAdditive]) {
            float storedRotateY = [self getStoredProperty:propCheckPoints propName:@"rotateY"];
            rotatey += storedRotateY;
            propCheckPoints[@"rotateY"] = [[NSNumber alloc] initWithFloat:rotatey];
        }

        if([rotateZ isAdditive]) {
            float storedRotateZ = [self getStoredProperty:propCheckPoints propName:@"rotateZ"];
            rotatez += storedRotateZ;
            propCheckPoints[@"rotateZ"] = [[NSNumber alloc] initWithFloat:rotatez];;
        }

        node->setRotation({rotatex, rotatey, rotatez});
    }
}

@end
