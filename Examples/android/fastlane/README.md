fastlane documentation
================
# Installation

Make sure you have the latest version of the Xcode command line tools installed:

```
xcode-select --install
```

Install _fastlane_ using
```
[sudo] gem install fastlane -NV
```
or alternatively using `brew cask install fastlane`

# Available Actions
## Android
### android renderer_aar
```
fastlane android renderer_aar
```
Build renderer
### android examples_gradle_clean
```
fastlane android examples_gradle_clean
```

### android npm_pack
```
fastlane android npm_pack
```

### android gvr_release_test
```
fastlane android gvr_release_test
```
Build GVR release tests
### android ovr_release_test
```
fastlane android ovr_release_test
```
Build OVR release tests
### android release_test
```
fastlane android release_test
```
Build GVR & OVR release tests

----

This README.md is auto-generated and will be re-generated every time [fastlane](https://fastlane.tools) is run.
More information about fastlane can be found on [fastlane.tools](https://fastlane.tools).
The documentation of fastlane can be found on [docs.fastlane.tools](https://docs.fastlane.tools).
