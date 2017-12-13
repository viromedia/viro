fastlane documentation
================
# Installation

Make sure you have the latest version of the Xcode command line tools installed:

```
xcode-select --install
```

## Choose your installation method:

<table width="100%" >
<tr>
<th width="33%"><a href="http://brew.sh">Homebrew</a></th>
<th width="33%">Installer Script</th>
<th width="33%">RubyGems</th>
</tr>
<tr>
<td width="33%" align="center">macOS</td>
<td width="33%" align="center">macOS</td>
<td width="33%" align="center">macOS or Linux with Ruby 2.0.0 or above</td>
</tr>
<tr>
<td width="33%"><code>brew cask install fastlane</code></td>
<td width="33%"><a href="https://download.fastlane.tools">Download the zip file</a>. Then double click on the <code>install</code> script (or run it in a terminal window).</td>
<td width="33%"><code>sudo gem install fastlane -NV</code></td>
</tr>
</table>

# Available Actions
## Android
### android release_renderer_react_lib
```
fastlane android release_renderer_react_lib
```
Build react library version ViroRenderer - viro_renderer-release.aar

And copy it to ../../../react-viro/android/viro_renderer/
### android release_renderer_rendererTest
```
fastlane android release_renderer_rendererTest
```
Build all artifacts for rendererTest (ovr, gvr, arcore, scene)
### android release_renderer_releaseTest
```
fastlane android release_renderer_releaseTest
```
Build all artifacts for releaseTest (ovr, gvr, arcore, scene)
### android release_renderer_memoryTest
```
fastlane android release_renderer_memoryTest
```
Build all artifacts for memoryLeakTest (ovr, gvr, arcore, scene)
### android release_renderer
```
fastlane android release_renderer
```
Build all artifacts for ViroRenderer (ovr, gvr, arcore, scene)
### android renderer_aar
```
fastlane android renderer_aar
```
Build renderer
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
