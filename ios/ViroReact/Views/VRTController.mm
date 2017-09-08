//
//  VRTController.m
//  React
//
#import <ViroKit/ViroKit.h>
#import <React/RCTLog.h>
#import "VRTController.h"
#import "VRTMaterialManager.h"


@implementation VRTController {
    std::shared_ptr<VROInputPresenter> _presenter;
    std::shared_ptr<VROEventDelegateiOS> _eventDelegate;
    bool _reticleEnabled;
    bool _controllerEnabled;
}

-(instancetype)initWithBridge:(RCTBridge *)bridge {
    self = [super initWithBridge:bridge];
    if (self) {
        _presenter = nil;
        _reticleEnabled = true;
        _controllerEnabled = true;
        _eventDelegate = std::make_shared<VROEventDelegateiOS>(self);
    }
    return self;
}

- (void)sceneWillAppear{
    [super sceneWillAppear];

    _presenter = self.scene->getControllerPresenter();
    _presenter->setEventDelegate(_eventDelegate);
    [self updateVisibility];
}

- (void)setReticleVisibility:(bool)visible {
    _reticleEnabled = visible;
}

- (void)setControllerVisibility:(bool)visible {
    _controllerEnabled = visible;
}

- (NSArray<NSNumber *> *)getControllerForwardVector {
    if (self.context) {
        VROVector3f vec = _presenter->getLastKnownForward();
        return @[@(vec.x), @(vec.y), @(vec.z)];
    } else {
        return @[@0, @0, @0];
    }
}

-(void) updateVisibility {
    if (!_presenter){
        return;
    }

    _presenter->getReticle()->setEnabled(_reticleEnabled);
    _presenter->getRootNode()->setHidden(!_controllerEnabled);
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
    [self updateVisibility];
}

#pragma mark default implementations for VRTEventDelegateProtocol
-(void)setCanClick:(BOOL)canClick {
    _eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnClick, canClick);
}

-(void)setCanFuse:(BOOL)canFuse {
    _canFuse = canFuse;
    _eventDelegate->setEnabledEvent(VROEventDelegate::EventAction::OnFuse, canFuse);
}

-(void)onClick:(int)source clickState:(VROEventDelegate::ClickState)clickState clickLocation:(std::vector<float>)location {
    if (self.onClickViro != nil) {
        if (location.size() > 0) {
            self.onClickViro(@{@"source": @(source),
                               @"clickState":@(clickState),
                               @"position":@[@(location[0]), @(location[1]), @(location[2])]});
        }
    }
}

-(void)onFuse:(int)source{
    if (self.onFuseViro != nil) {
        self.onFuseViro(@{@"source": @(source)});
    }
}
@end


