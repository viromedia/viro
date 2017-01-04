# Viro Code Samples

This project contains various sample Viro projects.

## Instructions

### Using Testbed App *(Recommended)*:

1. Download the [ViroMedia App](https://itunes.apple.com/us/app/viro-media/id1163100576?mt=8) from the iOS AppStore
2. Clone the repo into your workspace with git: `git clone https://github.com/viromedia/viro-code-samples.git`.
3. Run the setup-testbed.sh script: './setup-testbed.sh'
4. Run `npm start` from the root of the project
5. Open the Viro Media App, slide out the left panel and select "Enter Testbed"
6. Type the entire ngrok URL output at the top of the terminal into the text field and hit 'GO'
7. You should now be in the application! Enjoy!

### Using XCode *(Requires paid iOS Developer Account)*
1. Clone the repo into your workspace with git: `git clone https://github.com/viromedia/viro-code-samples.git`.
2. Make sure you have our [dependencies](http://docs.viromedia.com/docs/starting-a-new-viro-project#section-install-dependencies) installed.
3. Run the setup.sh script: `./setup-xcode.sh`.
4. Open the Xcode workspace at `/ios/ViroCodeSamples.xcworkspace` (you may need to add your Apple ID associated with your developer account if you haven't done so before)
5. Run `npm start` from the root of the project.
6. Run the application. 

### Changing Between Samples

1. Open the index.ios.js in a text editor
2. Modify line 34: `scene: scenes['360 Photo Tour'],` to a scene defined in the `scenes` dictionary on line 20.
3. Reload/restart the application.

## More information

Check out our [website](http://www.viromedia.com/).

Look at our [documentation](http://docs.viromedia.com/).

