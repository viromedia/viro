#
# Copyright © 2017 Viro Media. All rights reserved.
#

# NOTE: this script is executed at the root of the new project

#!/bin/bash

VIRO_PROJECT_NAME=$1 # The name of the user's project. The cwd should be already here.
VIRO_VERBOSE=$2 # True/False whether or not the user ran init w/ --verbose option

if [ "$VIRO_VERBOSE" = true ]; then 
  echo "running with verbose logging"
fi

echo "ios-setup.sh - doing nothing"

