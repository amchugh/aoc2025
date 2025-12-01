#!/bin/bash
mkdir -p .build
cd .build
cmake .. && cmake --build . && ./bin/aoc2025 "$@"
