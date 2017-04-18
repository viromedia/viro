#!/usr/bin/env bash

#
# Copyright © 2017 Viro Media. All rights reserved.
#

SETUP_ANDROID_STUDIO=false
SETUP_XCODE=false

printHelp() {
  printf "\n  ================== setup-ide.sh =================
  This script sets up IDEs based on the the following arguments:
    --all        - sets up all IDEs we support
    --android    - sets up Android Studio
    --ios        - sets up XCode\n"
}

case $1 in
  all|--all)
    SETUP_XCODE=true
    SETUP_ANDROID_STUDIO=true
  ;;
  android|--android)
    SETUP_ANDROID_STUDIO=true
  ;;
  ios|--ios)
    SETUP_XCODE=true
  ;;
  help|-h|--help)
    printHelp
    exit 0
  ;;
  *)
    printf "\nUnknown arguments provided! See help\n"
    printHelp
    exit 1
  ;;
esac

if [ ! -f package.json ]; then
  printf "== ERROR ==\n"
  printf "Unable to find the file [package.json]. Please run this within your project's root.\n"
  exit 1
fi

# Find the project's name, it should appear like this: "name" : "ProjectName".
# TODO: use a JSON parser and/or prompt the user if it's correct, also maybe accept a name args.
PROJECT_NAME=$(grep name package.json | cut -d '"' -f 4 | head -1)

if [ ! -d ./node_modules/react-viro ]; then
  printf "== ERROR ==\n"
  printf "Unable to find the react-viro module (under node_modules). Have you run npm install?\n"
  exit 1
fi

if $SETUP_ANDROID_STUDIO; then
  ./node_modules/react-viro/bin/android-setup.sh $PROJECT_NAME true
fi

if $SETUP_XCODE; then
  ./node_modules/react-viro/bin/ios-setup.sh $PROJECT_NAME true
fi

