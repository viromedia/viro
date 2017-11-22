#!/usr/bin/env node
const { exec } = require('child_process');

function checkAndRunSetup() {
  exec("npm -v", function(error, stdout, stderr) {
    var npm_version = parseInt(stdout.split(".")[0]);
    if (npm_version >= 5) {
      console.log("NPM Version is >= 5, running extra setup for react-viro script. See VIRO-1382 for more info.");
      setupReactViro();
    } else {
      console.log("NPM Version is < 5, not running extra setup.");
    }
    return
  });
}

// Check for npm version!
function setupReactViro() {
  var react_viro_json = require("../package.json");
  var files = react_viro_json.files;

  console.log("Deleting the react-viro symlink");
  exec("rm node_modules/react-viro");

  console.log("Creating new directories...");
  exec("mkdir -p node_modules/react-viro/android");
  exec("mkdir -p node_modules/react-viro/ios");

  for (var i = 0; i < files.length; i++) {
    var file = files[i]
    console.log("copying over react-viro/" + file);
    var result = exec("cp -r ../" + file + " node_modules/react-viro/" + file, function(error, stdout, stderr) {
      if (error !== null) {
        console.log("exec error: " + error);
      }
    });
  }

  console.log("Copying over package.json and index.js")
  exec("cp ../package.json node_modules/react-viro/")
  exec("cp ../index.js node_modules/react-viro/")
}

checkAndRunSetup();
