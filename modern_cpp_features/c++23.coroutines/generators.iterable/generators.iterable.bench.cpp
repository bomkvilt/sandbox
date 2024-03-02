module;

#include <benchmark/benchmark.h>

#include <coroutine>
#include <ranges>
#include <string>


// NOTE: clangd has incompleat support of modules
// SA: https://github.com/clangd/clangd/issues/1293
// SA: https://github.com/clangd/clangd/issues/1892
export module generators.iterable.bench;

import generator_v2;


namespace {
    using TInt = int;

    void clangd_clutch(...) {
        // NOTE: clangd issues with unused ```#include <coroutine>```
        auto _ = std::suspend_never{};
    }

    NGeneratorV2::TGeneratorV2<TInt> NaturalNums(TInt end) {
        for (int num = 0; num < end; num += 1) {
            co_yield num;
        }
    }

    auto NaturalNumsLambda(TInt end) {
        auto generator = [num = 0, end]() mutable {
            if (num < end) {
                return num++;
            }
            else {
                return -1;
            }
        };
        return generator;
    }

    TInt Identity(TInt value) {
        return value;
    }
}

namespace {
    TInt CaclNumsCount() {
        char* NUMS_COUNT = std::getenv("NUMS_COUNT");
        if (!NUMS_COUNT) {
            return 10;
        }
        return std::stoi(NUMS_COUNT);
    }
}

// ---------------------------------------------------------
// using c++ generator
// ---------------------------------------------------------

static void BMNormalGenerator(benchmark::State& state) {
    const auto NUMS_COUNT = CaclNumsCount();
    for (auto _ : state) {
        auto nums = ::NaturalNums(NUMS_COUNT);
        for (auto value : nums) {
            benchmark::DoNotOptimize(value);
        }
    }
}
BENCHMARK(BMNormalGenerator);

static void BMNormalGeneratorWithTake(benchmark::State& state) {
    const auto NUMS_COUNT = CaclNumsCount();
    for (auto _ : state) {
        auto nums = ::NaturalNums(NUMS_COUNT) | std::views::take(NUMS_COUNT);
        for (auto value : nums) {
            benchmark::DoNotOptimize(value);
        }
    }
}
BENCHMARK(BMNormalGeneratorWithTake);

static void BMNormalGeneratorWithSubrangeWithOneRound(benchmark::State& state) {
    const auto NUMS_COUNT = CaclNumsCount();
    for (auto _ : state) {
        auto nums = ::NaturalNums(NUMS_COUNT);
        for (auto value : std::ranges::subrange(nums) | std::views::take(NUMS_COUNT)) {
            benchmark::DoNotOptimize(value);
        }
    }
}
BENCHMARK(BMNormalGeneratorWithSubrangeWithOneRound);

static void BMNormalGeneratorWithSubrangeWithTwoRounds(benchmark::State& state) {
    const auto NUMS_COUNT = CaclNumsCount();
    const auto roundA = NUMS_COUNT >> 1;
    const auto roundB = NUMS_COUNT - roundA;
    for (auto _ : state) {
        const auto NUMS_COUNT = CaclNumsCount();
        auto nums = ::NaturalNums(NUMS_COUNT);
        for (auto value : std::ranges::subrange(nums) | std::views::take(roundA)) {
            benchmark::DoNotOptimize(value);
        }
        for (auto value : std::ranges::subrange(nums) | std::views::take(roundB)) {
            benchmark::DoNotOptimize(value);
        }
    }
}
BENCHMARK(BMNormalGeneratorWithSubrangeWithTwoRounds);

// ---------------------------------------------------------
// using views::iota
// ---------------------------------------------------------

static void BMNIota(benchmark::State& state) {
    const auto NUMS_COUNT = CaclNumsCount();
    for (auto _ : state) {
        auto nums = std::views::iota(TInt{0}, NUMS_COUNT);
        for (auto value : nums) {
            benchmark::DoNotOptimize(value);
        }
    }
}
BENCHMARK(BMNIota);

static void BMNIotaWithTake(benchmark::State& state) {
    const auto NUMS_COUNT = CaclNumsCount();
    for (auto _ : state) {
        auto nums = std::views::iota(TInt{0}) | std::views::take(NUMS_COUNT);
        for (auto value : nums) {
            benchmark::DoNotOptimize(value);
        }
    }
}
BENCHMARK(BMNIotaWithTake);

static void BMNIotaWithTakeWithFuncPointer(benchmark::State& state) {
    typedef TInt (*TCallback)(TInt);

    TCallback callback = Identity;
    benchmark::DoNotOptimize(callback);

    const auto NUMS_COUNT = CaclNumsCount();
    for (auto _ : state) {
        auto nums = std::views::iota(TInt{0}) | std::views::take(NUMS_COUNT);
        for (auto value : nums) {
            value = (*callback)(value);
            benchmark::DoNotOptimize(value);
        }
    }
}
BENCHMARK(BMNIotaWithTakeWithFuncPointer);

// ---------------------------------------------------------
// using raw for (;;) cycle
// ---------------------------------------------------------

static void BMRawForCycle(benchmark::State& state) {
    const auto NUMS_COUNT = CaclNumsCount();
    for (auto _ : state) {
        for (TInt value = 0; value < NUMS_COUNT; ++value) {
            benchmark::DoNotOptimize(value);
        }
    }
}
BENCHMARK(BMRawForCycle);

// ---------------------------------------------------------
// using raw for (;;) cycle
// ---------------------------------------------------------

static void BMLabdaGenerator(benchmark::State& state) {
    const auto NUMS_COUNT = CaclNumsCount();
    for (auto _ : state) {
        auto generator = NaturalNumsLambda(NUMS_COUNT);
        for (auto value = generator(); value >= 0; value = generator()) {
            benchmark::DoNotOptimize(value);
        }
    }
}
BENCHMARK(BMLabdaGenerator);
