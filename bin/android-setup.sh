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

echo 'Editing MainApplication.java'

LINE_TO_ADD="          new ReactViroPackage(ReactViroPackage.ViroPlatform.GVR)"
TARGET_FILEPATH=$(find android -name MainApplication.java)
SEARCH_PATTERN='new MainReactPackage'
LINE_TO_EDIT=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

# sed -i '' <- make copy while appending nothing (so we override)
# "s/$LINE_TO_EDIT/&," <- & means the same thing w/ an added comma
# $'\\\n' <- evaluates to a newline character
# "$LINE_TO_ADD/" <- substitute a variable & finish sed pattern format
sed -i '' "s/$LINE_TO_EDIT/&,"$'\\\n'"$LINE_TO_ADD/" $TARGET_FILEPATH

LINE_TO_ADD="import com.viromedia.bridge.ReactViroPackage"
SEARCH_PATTERN='import com.facebook.react.shell.MainReactPackage;'
LINE_TO_EDIT=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

sed -i '' "s/$LINE_TO_EDIT/&"$'\\\n'$'\\\n'"$LINE_TO_ADD/" $TARGET_FILEPATH

echo "Updating settings.gradle"

TARGET_FILEPATH=$(find android -name settings.gradle)
cat << EOF >> $TARGET_FILEPATH
include ':react_viro', ':gvr_common', ':viro_renderer'
project(':gvr_common').projectDir = new File('../node_modules/react-viro/android/gvr_common')
project(':viro_renderer').projectDir = new File('../node_modules/react-viro/android/viro_renderer')
project(':react_viro').projectDir = new File('../node_modules/react-viro/android/react_viro')"
EOF

echo "Updating Project's build.gradle"

LINE_TO_ADD="        classpath 'com.android.tools.build:gradle:2.2.2'"
TARGET_FILEPATH=android/build.gradle
SEARCH_PATTERN=classpath
LINE_TO_REPLACE=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

sed -i '' "s/$LINE_TO_REPLACE/$LINE_TO_ADD/g" $TARGET_FILEPATH

echo "Updating App's build.gradle"

TARGET_FILEPATH=android/app/build.gradle
LINE_TO_ADD="        minSdkVersion 23"
SEARCH_PATTERN="minSdkVersion"
LINE_TO_REPLACE=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

sed -i '' "s/$LINE_TO_REPLACE/$LINE_TO_ADD/g" $TARGET_FILEPATH

LINE_TO_ADD="        targetSdkVersion 25"
SEARCH_PATTERN="targetSdkVersion"
LINE_TO_REPLACE=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

sed -i '' "s/$LINE_TO_REPLACE/$LINE_TO_ADD/g" $TARGET_FILEPATH

SEARCH_PATTERN="dependencies {"
LINE_NUMBER=$(grep -n "$SEARCH_PATTERN" "$TARGET_FILEPATH" | cut -d ':' -f 1)

sed -i '' -e "$(($LINE_NUMBER+1)),$(($LINE_NUMBER+3))d" $TARGET_FILEPATH

LINES_TO_ADD=("    compile fileTree(dir: 'libs', include: ['*.jar'])"
"    compile 'com.android.support:appcompat-v7:25.0.0'"
"    compile 'com.facebook.react:react-native:+'"
"    compile project(':gvr_common')"
"    releaseCompile project(path: ':react_viro', configuration: 'release')"
"    debugCompile project(path: ':react_viro', configuration: 'debug')"
"    releaseCompile project(path: ':viro_renderer', configuration: 'release')"
"    debugCompile project(path: ':viro_renderer', configuration: 'debug')"
"    compile 'com.google.protobuf.nano:protobuf-javanano:3.0.0-alpha-7'")
LINE_TO_APPEND_AFTER=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

INDEX=$((${#LINES_TO_ADD[@]}-1))
while [ $INDEX -ge 0 ];
do
  sed -i '' "s/$LINE_TO_APPEND_AFTER/&"$'\\\n'"${LINES_TO_ADD[$INDEX]}/" $TARGET_FILEPATH
  INDEX=$(($INDEX-1))
done

echo "Copying over javascript files..."
# TODO: copy over script
