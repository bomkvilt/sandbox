#!/bin/bash

ROUNDS=20

BASEDIR="$(dirname "${BASH_SOURCE[0]}")"
RESULTS="${BASEDIR}/bench_results/results.json"

PROGRAM="c++23.coro.generators.iterable.bench.built.link"
PROGRAM="$(readlink -f $PROGRAM)"
echo "... actual binary path: '$PROGRAM'"


echo "... output file: '$RESULTS'"
echo "[" > "$RESULTS"

for (( i=1; i <= $ROUNDS; ++i )) do
    if [ $i -gt 1 ]; then
        echo "," >> "$RESULTS";
    fi

    echo "... round $i of $ROUNDS ...";
    "${PROGRAM}" --benchmark_format=json >> "$RESULTS";
done

echo "]" >> "$RESULTS"
echo "... done"
