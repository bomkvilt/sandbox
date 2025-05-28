# benchmark

Use [bench.ipynb](<./bench.ipynb>) for result visualisation.

```sh
bazelisk run -c opt :bench -- \
  --benchmark_enable_random_interleaving=true \
  --benchmark_report_aggregates_only=true \
  --benchmark_repetitions=10
```

![results](<./benchmarks/bench.png>)
