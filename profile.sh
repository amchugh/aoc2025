#!/bin/bash
sample aoc2025 65 1 -wait -f sample.output
cat sample.output | FlameGraph/stackcollapse-sample.awk | FlameGraph/flamegraph.pl > ./flamegraph.svg
echo "Wrote flamegraph.svg"
