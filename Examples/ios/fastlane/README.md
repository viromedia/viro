fastlane documentation
================
# Installation

Make sure you have the latest version of the Xcode command line tools installed:

```
xcode-select --install
```

## Choose your installation method:

| Method                     | OS support                              | Description                                                                                                                           |
|----------------------------|-----------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------|
| [Homebrew](http://brew.sh) | macOS                                   | `brew cask install fastlane`                                                                                                          |
| Installer Script           | macOS                                   | [Download the zip file](https://download.fastlane.tools). Then double click on the `install` script (or run it in a terminal window). |
| RubyGems                   | macOS or Linux with Ruby 2.0.0 or above | `sudo gem install fastlane -NV`                                                                                                       |

# Available Actions
## iOS
### ios release_react_viro_lib
```
fastlane ios release_react_viro_lib
```
Build react, ViroReact frameworks
### ios release_react_viro_static_lib
```
fastlane ios release_react_viro_static_lib
```

### ios release_react_viro_lib_clean
```
fastlane ios release_react_viro_lib_clean
```

### ios jenkins_release_tests
```
fastlane ios jenkins_release_tests
```

### ios jenkins_upload_release_tests_testflight
```
fastlane ios jenkins_upload_release_tests_testflight
```


----

This README.md is auto-generated and will be re-generated every time [fastlane](https://fastlane.tools) is run.
More information about fastlane can be found on [fastlane.tools](https://fastlane.tools).
The documentation of fastlane can be found on [docs.fastlane.tools](https://docs.fastlane.tools).
