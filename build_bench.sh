#!/bin/bash
PLATFORM=$1
if [ -z "$PLATFORM" ]; then
    echo "Must specify platform (arm)"
    exit 1
fi
ARGS=""
if [ "$PLATFORM" == "arm" ]; then
    ARGS="-march=armv8-a+crc -DBENCH_ARM"
fi
g++ crcbench.cc crc32c.cc perf.cpp -O2 -o crcbench -std=c++11 ${ARGS}


