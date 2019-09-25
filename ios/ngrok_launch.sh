#!/usr/bin/env bash

# Copyright (c) 2017-present, Viro Media, Inc.
# All rights reserved.
#
# This launches a ngrok process to tunnel from
# the packager which enables better test support.
(
# Launch ngrok inside this subshell.
#check if ngrok is running
ngrokPID=$(pgrep ngrok)
if [ -z "$ngrokPID" ] ; then
  open ngrok_term_launch.command
else
  echo "Ngrok is already running"
fi
) &
(
#loop a few times until we get a ngrok response
numtries=0
endpointfile="ngrokendpoint.txt"
THIS_DIR=$(dirname "$0")
# Write the ngrok endpoint to a file, the xcode shell script in ViroSample will read from this file.

# First remove existing ngrok endpoint file if it exists.
if [ -f $THIS_DIR/$endpointfile ] ; then
    rm $THIS_DIR/$endpointfile
fi

# Wait for a bit till until ngrok is responding.
while true; do
  STATUSCODE=$(curl --silent --output /dev/null --write-out "%{http_code}" http://localhost:4040/api/tunnels)

  if test $STATUSCODE -ne 200; then
    sleep 5;
    numtries=$(($numtries + 1))
  else
    break;
  fi

  if (( numtries > 5 )); then
    echo "Unable to get correct ngrok response after $numtries attempts."
    break;
  fi
done

api_output=$(curl --fail --silent GET http://localhost:4040/api/tunnels)

endPointFound=0
if [[ $api_output =~ .*(http:.*\.io)\",\"proto\":\"http\" ]]
  then
      ngrokendpoint="${BASH_REMATCH[1]}"
      endPointFound=1
fi

if [[ $endPointFound -eq 1 ]]; then
  echo "$ngrokendpoint/" > "$THIS_DIR/$endpointfile"
  echo "Wrote ngrok endpoint to: $THIS_DIR/$endpointfile"
fi
)
