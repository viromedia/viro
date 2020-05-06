# Disclaimer on using Mendix/Viro
This is a fork for internal testing and compliments the requirements of the Mendix Platform. Mendix offers no support or assurance on the features provided. Use on your own risk.

# Viro React
Viro React is a platform for developers to rapidly build augmented reality (AR) and virtual reality (VR) experiences. Developers write in React Native, and Viro runs their code natively across all mobile VR (including Google Daydream, Samsung Gear VR, and Google Cardboard for iOS and Android) and AR (iOS ARKit and Android ARCore) platforms. [More info here](http://docs.viromedia.com/).

This project contains various sample Viro projects.

The platform is free to use with no limits on distribution.

To report bugs/issues with Viro platform, please file new issues on this repository.

![Viro Bridge CI Pipeline](https://github.com/dthian/viro/workflows/Viro%20Bridge%20CI%20Pipeline/badge.svg)

## Instructions for running sample projects using Testbed app:

1. Follow directions on our [Quick start guide](https://docs.viromedia.com/docs/quick-start) to setup dependencies for trying these sample projects with the Viro Media App.
2. Clone the repo into your workspace with git: `git clone https://github.com/viromedia/viro.git`.
3. Go into the code-samples directory.
4. Run `npm install` from the root of this project.
5. Run `npm start` from the root of this project.
6. Open the Viro Media App, slide out the left panel and select "Enter Testbed".
7. Type the entire ngrok URL output (xxxxxx.ngrok.io) at the top of the terminal into the text field and hit 'GO'
8. You should now be in the application! Enjoy!

To rebuild the testbed app from source, please refere to the testbed repo [here](https://github.com/viromedia/viro-media-app/tree/master).

## Instructions for running sample code as a stand alone project (with no Testbed app):
Tried the samples through our Testbed app and now want to try deploying sample code to your device as standalone apps? These quick steps below should get you started:
1. Follow steps 1 - 4 from above (instructions for using with Testbed app)
2. For Android, make sure you have downloaded and installed Android Studio from [here](https://developer.android.com/studio/install) to get required SDK and platform-tools for building android apps
    Make sure you have the required environment variables set - `$ANDROID_HOME`, and added `platform-tools` to `$PATH` variable. If not,
    ```
    export ANDROID_HOME=/YOUR_PATH_TO/Android/sdk
    export PATH=$ANDROID_HOME/platform-tools:$PATH
    export PATH=$ANDROID_HOME/tools:$PATH
    ```
    Build and launch android app by executing the following from the root of the project
    ```
    react-native run-android --variant=gvrDebug
    ```
3. For iOS, in Xcode open `ViroSample.xcworkspace` in `ios/` directory.
    Select the right "Team" for `ViroSample` and `ViroSampleTest` target under `General -> Signing`
    Hit play to build and launch the app on your iOS device

### Changing Between Samples

1. Open App.js in a text editor.
2. For AR, set showARScene=true at line 44.
3. For VR, Modify line 61: `scene: scenes['360 Photo Tour'],` to a scene defined in the `scenes` dictionary on line 30.
3. Reload/restart the application.

## Instructions for using a CI-built Viro React platform from Mainline:
You can also try the latest mainline build containing bleeding edge features and fixes. Please keep in mind that mainline builds may not be as stable as release builds. To do so, simply:

1. Go to the [Viro Actions Workflows](https://github.com/viromedia/viro/actions) for this project.
2. You should see a list of "Viro Bridge CI Pipeline" workflows.
3. Click on the latest successfully built workflow pipeline (should be a checkmark next to it).
4. Download the latest built ViroReact.tgz artiface.
4. Clone this repo into your workspace with git: `git clone https://github.com/viromedia/viro.git`.
5. Go into the code-samples directory.
6. Run `npm install` from the root of this project. 
7. Remove the ViroReact framework that was pulled down from the npm install (you are going to use the pre-built one).
8. npm install ../path/to/your/downloadedArtifact.tgz

## Instructions for manually building the Viro React platform:

### Building iOS Viro React:

1. Follow directions on our [Quick start guide](https://docs.viromedia.com/docs/quick-start) to setup dependencies.
2. Clone the repo into your workspace with git: `git clone https://github.com/viromedia/viro.git`.
3. Build our iOS renderer using build instructions outlined in our [Virocore](https://github.com/viromedia/virocore/blob/master/README.md) repo.
4. Verify you see new files created in `ios/dist` folder.
5. Install pods in `ios/` folder:
   ```
   cd ios
   pod install
   ```
6. Install node_modules in test folder:
   ```
   cd test
   npm install
   ```
7. Install pods in the `ViroExample` folder:
   ```
   cd test/ios/ViroExample
   pod install
   ```
8. Open `ViroExample.xcworkspace` in Xcode. (Make sure you open the .xcworkpace file, and **not*** the .xcodeproj file!)
9. Select Product->Scheme. If you don't see a `React` scheme, hit `Manage Schemes...`. In the dialog box, add `React` scheme.
10. Go through build configuration (Debug vs Release) for schemes `React`, `ViroReact` and `ViroExample` and ensure they are all either Release or Debug, depending on what you are trying to build.
11. That's it! Now build React scheme for `Generic iOS Device`, followed by ViroReact scheme for the same target.
Note:
    ```
    11.a If you want the ability to run on Simulator, 
         change target to any of the `iOS Simulator` targets instead of `Generic iOS Device`. 
    11.b If in your own app project setup, you prefer to include Viro React as a static library 
         rather than relying on `use_frameworks!` - build scheme `ViroReact_static_lib` 
         instead of `ViroReact` as mentioned above in step #11. 
    ```
12. You should see a new file `libViroReact.a` at `ios/dist/lib/libViroReact.a`.
13. To run Viro React tests, run `ViroExample` scheme on your plugged in iOS device.

### Building Android Viro React:
1. Under the viro directory, run `./prepareRelease.sh`.
2. Your android bridge should now be built under release.
3. You should see a new file created at android/react_viro/react_viro-release.aar
4. To build android release tests:
   ```
   $ cd test/android
   $ ./gradlew assembleGvrRelease
   ```
5. Install app-gvr-release.apk from `test/android/app/build/output/gvr/app-gvr-release.apk` onto your plugged in Android device.


### Bundling and using built iOS and Android into a single npm tar package:
1. The `./prepareRelease.sh` you ran above builds android react bridge and bundles both iOS and Android bridge into a `react-viro-*.tgz` file. * for current version from `package.json` file.

## More information

Check out our [website](http://www.viromedia.com/).

Look at our [documentation](http://docs.viromedia.com/).

Join our Slack group [here](https://join.slack.com/t/virodevelopers/shared_invite/enQtMzI3MzgwNDM2NDM5LTdhMjg5OTJkZGEwYmI0Yzg0N2JkMzJhODVmNmY4YmUyOGY4YjMyZmFmMGFhMTMyMzZiYzU0MGUxMGIzZDFiNjY).

## Sample Code

### [Figment AR (Complete Open Source App)](https://github.com/viromedia/figment-ar)
A repository containing the entire source code, built using ViroReact and React Native, for Viro Media's award winning Figment AR App. Available on [Google Play](https://play.google.com/store/apps/details?id=com.viro.figment) and [App Store](https://itunes.apple.com/us/app/figment-ar/id1270018902?mt=8).

<a href="https://github.com/viromedia/figment-ar"><img src="code-samples/screenshots/figment_1.png" width="200"/> <img src="code-samples/screenshots/figment_2.png" width="200"/> <img src="code-samples/screenshots/figment_3.png" width="200"/> <img src="code-samples/screenshots/figment_4.png" width="200"/></a>

#### Download complete source at https://github.com/viromedia/figment-ar

### Sample Code Overview

A scene with a 360 photo that displays "Hello World".

### [360 Photo Tour](https://github.com/viromedia/viro/tree/master/code-samples/js/360PhotoTour)

<a href="https://github.com/viromedia/viro/tree/master/code-samples/js/HelloWorld">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/code-samples/js/360PhotoTour/360_photo_tour.gif">
</a>

360 photo tour example that shows you how to display a 360 photo with clickable hot spots.

### [Human Body](https://github.com/viromedia/viro/tree/master/code-samples/js/HumanBody)

<a href="https://github.com/viromedia/viro/tree/master/code-samples/js/HumanBody">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/code-samples/js/HumanBody/heart_demo.gif">
</a>

This example showcases 3d objects. Orbit around a 3d Heart to see it from different angles!

### [VR MediaPlayer](https://github.com/viromedia/viro/tree/master/code-samples/js/ViroMediaPlayer)

<a href="https://github.com/viromedia/viro/tree/master/code-samples/js/ViroMediaPlayer">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/code-samples/js/ViroMediaPlayer/movie_theater.gif">
</a>

Learn how to display and play 2d and 360 video with interactive play controls that can play, pause and stop.

### [Product Showcase](https://github.com/viromedia/viro/tree/master/code-samples/js/ProductShowcase)

<a href="https://github.com/viromedia/viro/tree/master/code-samples/js/ProductShowcase">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/code-samples/js/ProductShowcase/product_showcase.gif">
</a>

Learn how to display and play 2d and 360 video with interactive play controls that can play, pause and stop.
A demonstration on how to do an interactive shopping app for TV's. Uses flexbox for UI and 3d objects with animations.

### [AR Sample](https://github.com/viromedia/viro/tree/master/code-samples/js/ARSample)

<a href="https://github.com/viromedia/viro/tree/master/code-samples/js/ARSample">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/code-samples/js/ARSample/ar_sample.gif">
</a>

A scene with objects, text and animation displayed on ARKit planes detected in the scene.

### [AR Car Marker Demo](https://github.com/viromedia/viro/tree/master/code-samples/js/ARCarDemo)

<a href="https://github.com/viromedia/viro/tree/master/code-samples/js/ARCarDemo">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/code-samples/js/ARCarDemo/viro_car_marker_demo.gif">
</a>

An ARScene that looks for this [image](https://github.com/viromedia/viro/tree/master/code-samples/js/ARCarDemo/res/logo.png) and adds a model of a car with the ability to change its colors.

### [AR Driving Car Demo](https://github.com/viromedia/viro/tree/master/js/ARDrivingCarDemo)

<a href="https://github.com/viromedia/viro/tree/master/js/ARDrivingCarDemo">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/code-samples/js/ARDrivingCarDemo/ARDrivingCarDemo.gif">
</a>

An ARScene that lets the user place a car on a surface and drive it around the world. See instructions in the project directory to enable the demo.

### [AR Poster Demo](https://github.com/viromedia/viro/tree/master/code-samples/js/ARPosterDemo)

<a href="https://github.com/viromedia/viro/tree/master/code-samples/js/ARPosterDemo">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/code-samples/js/ARPosterDemo/viro_black_panther_marker_demo.gif">
</a>

An ARScene that searches for this vertical [marker](https://github.com/viromedia/viro/tree/master/code-samples/js/ARPosterDemo/res/blackpanther.jpg) and renders Black Panther jumping out of the marker.

### [AR Tracking Business Card Demo - iOS Only](https://github.com/viromedia/viro/tree/master/code-samples/js/ARBusinessCard)
<a href="https://github.com/viromedia/viro/tree/master/code-samples/js/ARBusinessCard">
<img src="https://github.com/vnovick/armonster-arkit2-businesscard/raw/master/business_card.gif">
</a>

An ARScene that tracks this [business card](https://github.com/viromedia/viro/blob/master/code-samples/js/ARBusinessCard/res/business_card.png) continously using image tracking.

### [AR Full Sample App](https://github.com/viromedia/ViroARSampleApp)
A complete React Native w/Viro AR Sample App. Demonstrates how to place, drag, and scale objects in the real world while providing 2D UI feedback.

<a href="https://github.com/viromedia/ViroARSampleApp">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/code-samples/js/ViroARSampleApp/ar_sample_app.gif">
</a>

## Tutorials

### [How to build an interactive AR app in 5 minutes](https://blog.viromedia.com/how-to-build-an-interactive-ar-app-in-5-mins-w-react-native-viro-ar-e420147e1612)

<a href="https://blog.viromedia.com/how-to-build-an-interactive-ar-app-in-5-mins-w-react-native-viro-ar-e420147e1612">
<img src="https://cdn-images-1.medium.com/max/1600/1*IwW479jvJFOwbZ7OgDJT3A.gif" />
</a>

### [How to build AR Portals in 5 minutes](https://blog.viromedia.com/how-to-build-ar-portals-in-5-mins-w-react-native-viro-ar-b939850def94)

<a href="https://blog.viromedia.com/how-to-build-ar-portals-in-5-mins-w-react-native-viro-ar-b939850def94">
<img src="https://cdn-images-1.medium.com/max/1600/1*YnWurSj2n-AtU26AvbXxVA.gif"/>
</a>

### [Add Snapchat-like AR Lenses to any app](https://blog.viromedia.com/add-snapchat-ar-lenses-to-any-app-w-react-native-viro-ar-9d4053769782)

<a href="https://blog.viromedia.com/add-snapchat-ar-lenses-to-any-app-w-react-native-viro-ar-9d4053769782">
<img src="https://cdn-images-1.medium.com/max/1600/1*iTkW2kiLIwOwJ5e_HHxI6Q.gif" />
</a>

