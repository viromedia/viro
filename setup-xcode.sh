#!/bin/bash

./setup-testbed.sh
cd ios
pod repo update
pod install

