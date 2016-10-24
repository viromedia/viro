#!/bin/bash

npm install
sed -i '' 's/react-native/react-viro/g' node_modules/react/lib/*
cd ios
pod repo update
pod install

