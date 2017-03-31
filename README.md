# Viro Platform

This project contains various sample Viro projects.

To report bugs/issues with Viro platform, please file new issues on this repository.

## Instructions for running sample projects using Testbed app:

1. Follow directions on our [Quick start guide](http://docs.viromedia.com/v1.0.0/docs/quick-start) to setup dependencies for trying these sample projects with the Viro Media App.
2. Clone the repo into your workspace with git: `git clone https://github.com/viromedia/viro.git`.
3. Edit index.ios.js or index.android.js, depending on your test device, and replace "API_KEY_HERE" with the key emailed to you with signup.
4. Run `npm install` from the root of this project.
5. Run `npm start` from the root of this project.
6. Open the Viro Media App, slide out the left panel and select "Enter Testbed".
7. Type the entire ngrok URL output (xxxxxx.ngrok.io) at the top of the terminal into the text field and hit 'GO'
8. You should now be in the application! Enjoy!

## Changing Between Samples

1. Open the index.ios.js or index.android in a text editor
2. Modify line 37: `scene: scenes['360 Photo Tour'],` to a scene defined in the `scenes` dictionary on line 23.
3. Reload/restart the application.

## More information

Check out our [website](http://www.viromedia.com/).

Look at our [documentation](http://docs.viromedia.com/).

