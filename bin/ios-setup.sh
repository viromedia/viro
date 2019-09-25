#!/usr/bin/env bash

#
# Copyright © 2017 Viro Media. All rights reserved.
#

# NOTE: this script is executed at the root of the new project

VIRO_PROJECT_NAME=$1 # The name of the user's project. The cwd should be already here.
VIRO_VERBOSE=$2 # True/False whether or not the user ran init w/ --verbose option

echo "==== Running iOS Setup Script ==="

if [ "$VIRO_VERBOSE" = true ]; then
  echo "running with verbose logging"
fi

cp node_modules/react-viro/ios/dist/podFile/* ios/
cp node_modules/react-viro/ios/dist/bin/* ios/
chmod a+x ios/ngrok_launch.sh
chmod a+x ios/ngrok_term_launch.command

#replace Podfile with proper project name
TARGET_FILEPATH=ios/Podfile
LINE_TO_ADD="$VIRO_PROJECT_NAME"
SEARCH_PATTERN="VIRO_PROJECT_NAME"
LINE_TO_REPLACE=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")
/usr/bin/sed -i '' "s/$SEARCH_PATTERN/$LINE_TO_ADD/g" $TARGET_FILEPATH

cd ios
#install the pod for ios
pod install

#run xcodeproj script to modify ios project settings.
ruby proj_add_shell_script.rb "$VIRO_PROJECT_NAME"

#modify info.plist file using PlistBuddy(installed by xcode)
/usr/libexec/PlistBuddy -c "Add:NSCameraUsageDescription string" $VIRO_PROJECT_NAME/Info.plist || true
/usr/libexec/PlistBuddy -c "Set:NSCameraUsageDescription 'Camera is used to scan the QR code of your VR headset.'" $VIRO_PROJECT_NAME/Info.plist

yes | cp -rf ../node_modules/react-viro/ios/dist/AppDelegate.m $VIRO_PROJECT_NAME/AppDelegate.m
TARGET_FILEPATH=$VIRO_PROJECT_NAME/AppDelegate.m
LINE_TO_ADD="$VIRO_PROJECT_NAME"
SEARCH_PATTERN="VIRO_PROJECT_NAME"
LINE_TO_REPLACE=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")
/usr/bin/sed -i '' "s/$SEARCH_PATTERN/$LINE_TO_ADD/g" $TARGET_FILEPATH
