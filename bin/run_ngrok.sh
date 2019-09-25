#!/usr/bin/env bash

#
# Copyright (c) 2017-present, Viro Media, Inc.
# All rights reserved.

function box_out()
{
  local s=("$@") b w
  for l in "${s[@]}"; do
    ((w<${#l})) && { b="$l"; w="${#l}"; }
  done
  tput setaf 2
  echo " -${b//?/-}-
| ${b//?/ } |"
  for l in "${s[@]}"; do
    printf '| %s%*s%s |\n' "$(tput setaf 7)" "-$w" "$l" "$(tput setaf 2)"
  done
  echo "| ${b//?/ } |
 -${b//?/-}-"
  tput sgr 0
}

ngrokPID=$(pgrep ngrok)
if [ -z "$ngrokPID" ] ; then
  ngrok http 8081 > /dev/null &
else
  echo "Ngrok is already running"
fi

#loop a few times until we get a ngrok response
numtries=0

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
      box_out "NGrok Packager Server endpoint: ${ngrokendpoint}"    # echo tunnel endpoint
      endPointFound=1
fi
