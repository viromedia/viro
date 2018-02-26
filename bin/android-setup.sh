#!/usr/bin/env bash

#
# Copyright © 2017 Viro Media. All rights reserved.
#

# NOTE: this script is executed at the root of the new project

VIRO_PROJECT_NAME=$1 # The name of the user's project. The cwd should be already here.
VIRO_VERBOSE=$2 # True/False whether or not the user ran init w/ --verbose option

echo "==== Running Android Setup Script ==="

if [ "$VIRO_VERBOSE" = true ]; then
  echo "Running with verbose logging"
fi

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


echo 'Editing MainApplication.java'

LINE_TO_ADD="          new ReactViroPackage(ReactViroPackage.ViroPlatform.valueOf(BuildConfig.VR_PLATFORM))"
TARGET_FILEPATH=$(find android -name MainApplication.java)
SEARCH_PATTERN='new MainReactPackage'
LINE_TO_EDIT=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

# Adding a line
# sed -i '' <- make copy while appending nothing (so we override)
# "s/$LINE_TO_EDIT/&," <- & means the same thing w/ an added comma
# $'\\\n' <- evaluates to a newline character
# "$LINE_TO_ADD/" <- substitute a variable & finish sed pattern format
vsed "s/$LINE_TO_EDIT/&,"$'\\\n'"$LINE_TO_ADD/" $TARGET_FILEPATH

LINE_TO_ADD="import com.viromedia.bridge.ReactViroPackage;"
SEARCH_PATTERN='import com.facebook.react.shell.MainReactPackage;'
LINE_TO_EDIT=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

# Adding another line
vsed "s/$LINE_TO_EDIT/&"$'\\\n'$'\\\n'"$LINE_TO_ADD/" $TARGET_FILEPATH



echo "Updating settings.gradle"

# Adding some lines to the end of the file
TARGET_FILEPATH=$(find android -name settings.gradle)
cat << EOF >> $TARGET_FILEPATH
include ':react_viro', ':arcore_client', ':gvr_common', ':viro_renderer'
project(':arcore_client').projectDir = new File('../node_modules/react-viro/android/arcore_client')
project(':gvr_common').projectDir = new File('../node_modules/react-viro/android/gvr_common')
project(':viro_renderer').projectDir = new File('../node_modules/react-viro/android/viro_renderer')
project(':react_viro').projectDir = new File('../node_modules/react-viro/android/react_viro')
EOF



echo "Updating Project's build.gradle"

# Replacing the classpath line
LINE_TO_ADD="        classpath 'com.android.tools.build:gradle:2.2.2'"
TARGET_FILEPATH=android/build.gradle
SEARCH_PATTERN=classpath
LINE_TO_REPLACE=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

vsed "s/$LINE_TO_REPLACE/$LINE_TO_ADD/g" $TARGET_FILEPATH



echo "Updating App's build.gradle"

# Updating SDK versions
TARGET_FILEPATH=android/app/build.gradle
LINE_TO_ADD="        minSdkVersion 23"
SEARCH_PATTERN="minSdkVersion"
LINE_TO_REPLACE=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

vsed "s/$LINE_TO_REPLACE/$LINE_TO_ADD/g" $TARGET_FILEPATH

LINE_TO_ADD="        targetSdkVersion 25"
SEARCH_PATTERN="targetSdkVersion"
LINE_TO_REPLACE=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

vsed "s/$LINE_TO_REPLACE/$LINE_TO_ADD/g" $TARGET_FILEPATH

# Enable multidexing
LINE_TO_ADD="        multiDexEnabled true"
SEARCH_PATTERN="targetSdkVersion"
LINE_TO_APPEND_AFTER=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

vsed "s/$LINE_TO_APPEND_AFTER/&"$'\\\n'"$LINE_TO_ADD/" $TARGET_FILEPATH

# Replacing dependencies by first deleting 2 lines and then inserting a few more
SEARCH_PATTERN="dependencies {"
LINE_NUMBER=$(grep -n "$SEARCH_PATTERN" "$TARGET_FILEPATH" | cut -d ':' -f 1)

# delete 2 lines
vsed -e "$(($LINE_NUMBER+1)),$(($LINE_NUMBER+3))d" $TARGET_FILEPATH

LINES_TO_ADD=("    compile fileTree(dir: 'libs', include: ['*.jar'])"
"    compile 'com.android.support:appcompat-v7:25.0.0'"
"    compile 'com.facebook.react:react-native:+'"
"    compile project(':arcore_client') \/\/ remove this if AR not required"
"    compile project(':gvr_common')"
"    compile project(path: ':viro_renderer')"
"    compile project(path: ':react_viro')"
"    compile 'com.google.android.exoplayer:exoplayer:r2.2.0'"
"    compile 'com.google.protobuf.nano:protobuf-javanano:3.0.0-alpha-7'"
"    compile 'com.amazonaws:aws-android-sdk-core:2.2.+'"
"    compile 'com.amazonaws:aws-android-sdk-ddb:2.2.+'"
"    compile 'com.amazonaws:aws-android-sdk-ddb-mapper:2.2.+'"
"    compile 'com.amazonaws:aws-android-sdk-cognito:2.2.+'"
"    compile 'com.amazonaws:aws-android-sdk-cognitoidentityprovider:2.2.+'")
LINE_TO_APPEND_AFTER=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

INDEX=$((${#LINES_TO_ADD[@]}-1))
while [ $INDEX -ge 0 ];
do
  vsed "s/$LINE_TO_APPEND_AFTER/&"$'\\\n'"${LINES_TO_ADD[$INDEX]}/" $TARGET_FILEPATH
  INDEX=$(($INDEX-1))
done

# Adding buildTypes by inserting it before a line
SEARCH_PATTERN="buildTypes {"
LINES_TO_PREPEND=("    productFlavors {"
"        gvr {"
"            resValue 'string', 'app_name', '$VIRO_PROJECT_NAME-gvr'"
"            buildConfigField 'String', 'VR_PLATFORM', '\"GVR\"'"
"        }"
"        ovr {"
"            resValue 'string', 'app_name', '$VIRO_PROJECT_NAME-ovr'"
"            applicationIdSuffix '.ovr'"
"            buildConfigField 'String', 'VR_PLATFORM', '\"OVR_MOBILE\"'"
"        }"
"    }")
LINE_TO_PREPEND_TO=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")

INDEX=0
while [ $INDEX -lt $((${#LINES_TO_PREPEND[@]})) ];
do
  vsed "s/$LINE_TO_PREPEND_TO/${LINES_TO_PREPEND[$INDEX]}"$'\\\n'"&/" $TARGET_FILEPATH
  INDEX=$(($INDEX+1))
done



echo "Updating AndroidManifest.xml"

TARGET_FILEPATH=android/app/src/main/AndroidManifest.xml
SEARCH_PATTERN="category.LAUNCHER"
LINE_TO_ADD='            <category android:name="com.google.intent.category.CARDBOARD" \/>'
LINE_TO_APPEND_TO=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")
# escape the append to line
LINE_TO_APPEND_TO=$(echo $LINE_TO_APPEND_TO | sed -e 's/[]\/$*.^|[]/\\&/g')

vsed "s/$LINE_TO_APPEND_TO/&"$'\\\n'"$LINE_TO_ADD/" $TARGET_FILEPATH

# add camera permissions for AR
SEARCH_PATTERN="permission.SYSTEM_ALERT_WINDOW"
LINE_TO_ADD='    <uses-permission android:name="android.permission.CAMERA" \/>'
LINE_TO_APPEND_TO=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")
# escape the append to line
LINE_TO_APPEND_TO=$(echo $LINE_TO_APPEND_TO | sed -e 's/[]\/$*.^|[]/\\&/g')

vsed "s/$LINE_TO_APPEND_TO/&"$'\\\n'"$LINE_TO_ADD/" $TARGET_FILEPATH

# add optional ARCore meta-data tag
SEARCH_PATTERN="devsupport.DevSettingsActivity"
LINE_TO_ADD='      <meta-data android:name="com.google.ar.core" android:value="optional" \/>'
LINE_TO_APPEND_TO=$(grep "$SEARCH_PATTERN" "$TARGET_FILEPATH")
LINE_TO_APPEND_TO=$(echo $LINE_TO_APPEND_TO | sed -e 's/[]\/$*.^|[]/\\&/g')
vsed "s/$LINE_TO_APPEND_TO/&"$'\\\n'"$LINE_TO_ADD/" $TARGET_FILEPATH



echo "Copying over OVR's additional manifest"

cp -r node_modules/react-viro/bin/files/android .



echo "Updating gradle-wrapper.properties"

TARGET_FILEPATH=$(find android -name gradle-wrapper.properties)

vsed "s/gradle-2.4-all/gradle-2.14.1-all/" $TARGET_FILEPATH



echo "Updating strings.xml"

TARGET_FILEPATH=$(find android -name strings.xml)

# deleting 2nd line in file
vsed '2d' $TARGET_FILEPATH

