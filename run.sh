#!/bin/bash
BUILD_TYPE=${BUILD_TYPE:-Debug}
# This is needed for asan on macos
export MallocNanoZone='0'
mkdir -p .build
cd .build
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE" && cmake --build . && cd .. && .build/bin/aoc2025 "$@"
