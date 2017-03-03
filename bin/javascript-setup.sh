#
# Copyright © 2017 Viro Media. All rights reserved.
#

# NOTE: this script is executed at the root of the new project

#!/bin/bash

VIRO_PROJECT_NAME=$1 # The name of the user's project. The cwd should be already here.
VIRO_VERBOSE=$2 # True/False whether or not the user ran init w/ --verbose option

isosx() {
  if [[ $OSTYPE == darwin* ]]; then
    return 0 # in bash 0 is true!
  else
    return 1
  fi
}

vsed() {
  if isosx; then
    sed -i '' "$@"
  else
    sed -i "$@"
  fi
}

echo "==== Running Javascript Setup Script ==="

echo "Copying over HelloWorld files"

cp -r ./node_modules/react-viro/bin/files/javascript/* .

# replace APP_NAME_HERE with actual app names
vsed "s/APP_NAME_HERE/$VIRO_PROJECT_NAME/" index.*

