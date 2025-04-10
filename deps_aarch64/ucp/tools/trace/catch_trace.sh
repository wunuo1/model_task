#!/bin/bash
# Copyright (c) 2024 Horizon Robotics.All Rights Reserved.
#
# The material in this file is confidential and contains trade secrets
# of Horizon Robotics Inc. This is proprietary information owned by
# Horizon Robotics Inc. No part of this work may be disclosed,
# reproduced, copied, transmitted, or used in any way for any purpose,
# without the express written permission of Horizon Robotics Inc.


LOG_FILE=""
TRACE_FILE=""
TRACE_MODE="task"

function show_usage() {
cat <<EOF
  Usage: bash $0 <options> -o/--output output_file
  available options:
    -i|--input: set input trace log file, default is empty (use logcat to catch trace log)
    -o|--output: set output trace json file
    -m|--mode: set trace mode ([task|thread]), default is task
    -h|--help
EOF
  exit
}

function catch_log() {
  if [ -z $LOG_FILE ]; then
    logcat -s UCPT:* | awk '/UCPT/' | awk -F "UCPT] " '{print $2, ","}' > $TRACE_FILE
  else
    cat $LOG_FILE | awk '/UCPT/' | awk -F "UCPT] " '{print $2, ","}' > $TRACE_FILE
  fi
}

function postprocess() {
  sed -i '1 s/^{/[{/' $TRACE_FILE
  sed -i '$ s/,$/]/' $TRACE_FILE
  if [[ $TRACE_MODE == "thread" ]]; then
    sed -i 's/"tid":\([0-9]\+\)\(.*"thid":\)\([0-9]\+\)/"tid":\3\2\3/' $TRACE_FILE
  fi
  sed -i 's/thid/thread_id/' $TRACE_FILE
}

function sigint_handler() {
  postprocess
}

trap sigint_handler SIGINT

MODE_TYPE_OPTS=(task thread)
GETOPT_ARGS=`getopt -o i:o:m:h -al input:,output:,mode:,help -- "$@"`
eval set -- "$GETOPT_ARGS"

while [ -n "$1" ]
do
  case "$1" in
    -i|--input)
      LOG_FILE=$2
      shift 2
      ;;
    -o|--output)
      TRACE_FILE=$2
      shift 2
      ;;
    -m|--mode)
      TRACE_MODE=$2
      shift 2
      if [[ ! "${MODE_TYPE_OPTS[*]}" =~ $TRACE_MODE ]]; then
        echo "invalid trace mode type: $TRACE_MODE"
        show_usage
      fi
      ;;
    -h|--help) show_usage; break;;
    --) break ;;
    *) echo $1,$2 show_usage; break;;
  esac
done

if [ -z $TRACE_FILE ]; then
  echo "output trace file is empty"
  show_usage
fi

catch_log
postprocess
