#!/usr/bin/env bash

# Copyright (c) 2017-present, Viro Media, Inc.
# All rights reserved.
#
# Set terminal title
echo -en "\033]0;Ngrok\a"
clear

ngrok http 8081

echo "Process terminated. Press <enter> to close the window"
read
