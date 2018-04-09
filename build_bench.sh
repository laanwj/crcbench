#!/bin/bash
PLATFORM=$1
if [ -z "$PLATFORM" ]; then
    echo "Must specify platform (arm64 or sse42)"
    exit 1
fi
ARGS=""
if [ "$PLATFORM" == "arm64" ]; then
    ARGS="-march=armv8-a+crc -DBENCH_ARM64"
fi
if [ "$PLATFORM" == "sse42" ]; then
    ARGS="-msse4.2 -DBENCH_SSE42"
fi
g++ crcbench.cc crc32c.cc perf.cpp -O2 -o crcbench -std=c++11 ${ARGS}


