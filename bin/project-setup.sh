#!/usr/bin/env bash

#
# Copyright © 2017 Viro Media. All rights reserved.
#

# NOTE: this script is executed at the root of the new project

# This file moves most of the actual project file setup out of the react-viro-cli
# It takes 2 arguments, the project name and verbose flags
#

VIRO_PROJECT_NAME=$1 # The name of the user's project. The cwd should be already here.
VIRO_VERBOSE=$2 # True/False whether or not the user ran init w/ --verbose option

# run the javascript-setup script
./node_modules/react-viro/bin/javascript-setup.sh $VIRO_PROJECT_NAME $VIRO_VERBOSE

# copy the setup-ide script to the root of the directory and make it executable
cp ./node_modules/react-viro/bin/setup-ide.sh .
chmod +x setup-ide.sh

# copy the ViroFBX tool to their bin directory
mkdir bin
cp ./node_modules/react-viro/bin/ViroFBX bin

# run the post-install script if available
if [ -f ./node_modules/react-viro/bin/post-install.sh ]; then
  ./node_modules/react-viro/bin/post-install.sh
fi

NGROK_ERROR_MESSAGE="
!!!!!!!!!!!!!!!!! ERROR !!!!!!!!!!!!!!!!!!
Installing ngrok@2.2.3 globally failed
Please run the following command manually:

    npm install -g ngrok

Otherwise, you won't be able to use ngrok (for iOS)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"

echo "Attempting to install ngrok@2.2.3 globally..."
npm install -g ngrok@2.2.3 || printf "$NGROK_ERROR_MESSAGE"