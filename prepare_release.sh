#!/bin/bash

#
# Copyright © 2017 Viro Media. All rights reserved.
#

set -x

# to be safe, clear out any old libraries in the output directory
echo '== Clearing out the old build artifacts =='
rm android/viro_bridge/build/outputs/aar/*.aar

echo '== Building the React-Viro library =='
( cd android && gradle assemble )

echo '== Checking for build artifacts =='
if [ ! -f android/viro_bridge/build/outputs/aar/viro_bridge-debug.aar ]
then
    echo "Unable to find viro-bridge debug output!";
    exit
fi

if [ ! -f android/viro_bridge/build/outputs/aar/viro_bridge-release.aar ]
then
    echo "Unable to find viro-bridge debug output!";
    exit
fi

echo '== Copying build artifacts to the lib directory =='
rm android/react_viro/*.aar
cp android/viro_bridge/build/outputs/aar/viro_bridge-debug.aar android/react_viro/react_viro-debug.aar
cp android/viro_bridge/build/outputs/aar/viro_bridge-release.aar android/react_viro/react_viro-release.aar

