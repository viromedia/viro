# Viro React
Viro React is a platform for developers to rapidly build augmented reality (AR) and virtual reality (VR) experiences. Developers write in React Native, and Viro runs their code natively across all mobile VR (including Google Daydream, Samsung Gear VR, and Google Cardboard for iOS and Android) and AR (iOS ARKit and Android ARCore) platforms. [More info here](http://docs.viromedia.com/).

This project contains various sample Viro projects.

[Sign up](https://viromedia.com/signup) for an API key. The platform is free to use with no limits on distribution.

To report bugs/issues with Viro platform, please file new issues on this repository.

## Instructions for running sample projects using Testbed app:

1. Follow directions on our [Quick start guide](https://docs.viromedia.com/docs/quick-start) to setup dependencies for trying these sample projects with the Viro Media App.
2. Clone the repo into your workspace with git: `git clone https://github.com/viromedia/viro.git`.
3. Edit App.js, depending on your test device, and replace "API_KEY_HERE" with the key emailed to you with signup.
4. Run `npm install` from the root of this project.
5. Run `npm start` from the root of this project.
6. Open the Viro Media App, slide out the left panel and select "Enter Testbed".
7. Type the entire ngrok URL output (xxxxxx.ngrok.io) at the top of the terminal into the text field and hit 'GO'
8. You should now be in the application! Enjoy!

## Changing Between Samples

1. Open App.js in a text editor.
2. For AR, set showARScene=true at line 44.
3. For VR, Modify line 61: `scene: scenes['360 Photo Tour'],` to a scene defined in the `scenes` dictionary on line 30.
3. Reload/restart the application.

## More information

Check out our [website](http://www.viromedia.com/).

Look at our [documentation](http://docs.viromedia.com/).

Join our Slack group [here](https://join.slack.com/t/virodevelopers/shared_invite/enQtMzI3MzgwNDM2NDM5LTBiOTRkMDg0Zjc0ODIyYTAxZDczYzFmODJhMDUxZjVhOTk0NGVhMjNlZTY0ZjkxNjMzZGEyNDNlYzc4ZTkzNzQ).


## Sample Code Overview

### [Hello World](https://github.com/viromedia/viro/tree/master/js/HelloWorld)

<a href="https://github.com/viromedia/viro/tree/master/js/HelloWorld">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/js/HelloWorld/helloworld.gif">
</a>

A scene with a 360 photo that displays "Hello World".

### [360 Photo Tour](https://github.com/viromedia/viro/tree/master/js/360PhotoTour)

<a href="https://github.com/viromedia/viro/tree/master/js/HelloWorld">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/js/360PhotoTour/360_photo_tour.gif">
</a>

360 photo tour example that shows you how to display a 360 photo with clickable hot spots.

### [Human Body](https://github.com/viromedia/viro/tree/master/js/HumanBody)

<a href="https://github.com/viromedia/viro/tree/master/js/HumanBody">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/js/HumanBody/heart_demo.gif">
</a>

This example showcases 3d objects. Orbit around a 3d Heart to see it from different angles!

### [VR MediaPlayer](https://github.com/viromedia/viro/tree/master/js/ViroMediaPlayer)

<a href="https://github.com/viromedia/viro/tree/master/js/ViroMediaPlayer">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/js/ViroMediaPlayer/movie_theater.gif">
</a>

Learn how to display and play 2d and 360 video with interactive play controls that can play, pause and stop.

### [Product Showcase](https://github.com/viromedia/viro/tree/master/js/ProductShowcase)

<a href="https://github.com/viromedia/viro/tree/master/js/ProductShowcase">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/js/ProductShowcase/product_showcase.gif">
</a>

Learn how to display and play 2d and 360 video with interactive play controls that can play, pause and stop.
A demonstration on how to do an interactive shopping app for TV's. Uses flexbox for UI and 3d objects with animations.

### [AR Sample](https://github.com/viromedia/viro/tree/master/js/ARSample)

<a href="https://github.com/viromedia/viro/tree/master/js/ARSample/ar_sample.gif">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/js/ARSample/ar_sample.gif">
</a>

A scene with objects, text and animation displayed on ARKit planes detected in the scene.

### [AR Full Sample App](https://github.com/viromedia/ViroARSampleApp)
A complete React Native w/Viro AR Sample App. Demonstrates how to place, drag, and scale objects in the real world while providing 2D UI feedback.

<a href="https://github.com/viromedia/ViroARSampleApp">
<img src="https://raw.githubusercontent.com/viromedia/viro/master/js/ViroARSampleApp/ar_sample_app.gif">
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

