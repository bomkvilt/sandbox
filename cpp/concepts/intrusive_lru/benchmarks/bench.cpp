#include <atomic>
#include <chrono>
#include <cstddef>
#include <memory>
#include <memory_resource>
#include <random>
#include <ranges>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

#include <benchmark/benchmark.h>

#include "../intrusive_lru/lru_v1.h"
#include "../intrusive_lru/lru_v2.h"

// =================================================================================================

template <typename Alloc = std::allocator<std::byte>>
class MemoryPressure final {
public:
    MemoryPressure() noexcept
        : stop_flag_{false} {
    }

    ~MemoryPressure() noexcept {
        stop();
    }

    void start(this MemoryPressure& self) {
        self.thread_ = std::thread([&self]() {
            std::mt19937 rng{std::random_device{}()};
            std::uniform_int_distribution<size_t> dist(1, 128 * (2 << 10));

            std::vector<std::byte*> pointers;
            std::vector<size_t> sizes;

            static constexpr size_t kOperations = 100;

            Alloc alloc;
            while (!self.stop_flag_.load()) {
                // Allocate
                for (const size_t _ : std::views::iota(0u, kOperations)) {
                    const size_t size = dist(rng);
                    pointers.push_back(alloc.allocate(size));
                    sizes.push_back(size);
                }
                // Deallocate
                for (const size_t _ : std::views::iota(0u, kOperations)) {
                    if (pointers.empty()) {
                        break;
                    }
                    alloc.deallocate(pointers.back(), sizes.back());
                    pointers.pop_back();
                    sizes.pop_back();
                }
            }
        });
    }

    void stop(this MemoryPressure& self) noexcept {
        self.stop_flag_.store(true);
        if (self.thread_.joinable()) {
            self.thread_.join();
        }
    }

private:
    std::atomic<bool> stop_flag_;
    std::thread thread_;
};

// =================================================================================================

enum class AllocatorKind {
    kStd,
    kPmr,
};

namespace std {
    std::string_view to_string(const AllocatorKind kind) noexcept {
        switch (kind) {
            case AllocatorKind::kStd: {
                return "std";
            }
            case AllocatorKind::kPmr: {
                return "pmr";
            }
        }
    }
} // namespace std

enum class CacheVersion {
    kV1,
    kV2,
};

namespace std {
    std::string_view to_string(const CacheVersion version) noexcept {
        switch (version) {
            case CacheVersion::kV1: {
                return "v1";
            }
            case CacheVersion::kV2: {
                return "v2";
            }
        }
    }
} // namespace std

template <CacheVersion Version, AllocatorKind Kind, typename Key, typename Value, typename Hash, typename Equal>
struct CacheContext;

template <typename Key, typename Value, typename Hash, typename Equal>
struct CacheContext<CacheVersion::kV1, AllocatorKind::kStd, Key, Value, Hash, Equal> {
    using Alloc = std::allocator<std::byte>;
    using Cache = lru::v1::LRUCache<Key, Value, Hash, Equal, Alloc>;

    Alloc allocator;
    Cache cache;

    CacheContext(size_t capacity)
        : allocator{}
        , cache{capacity, allocator} {
    }
};

template <typename Key, typename Value, typename Hash, typename Equal>
struct CacheContext<CacheVersion::kV1, AllocatorKind::kPmr, Key, Value, Hash, Equal> {
    using Alloc = std::pmr::polymorphic_allocator<std::byte>;
    using Cache = lru::v1::LRUCache<Key, Value, Hash, Equal, Alloc>;

    std::pmr::synchronized_pool_resource pool;
    Alloc allocator;
    Cache cache;

    CacheContext(size_t capacity)
        : pool{
            std::pmr::pool_options{
                .max_blocks_per_chunk = 1024,
                .largest_required_pool_block = 128 * (2 << 10),
            },
            std::pmr::get_default_resource()
        }
        , allocator{&pool}
        , cache{capacity, allocator} {
    }
};

template <typename Key, typename Value, typename Hash, typename Equal>
struct CacheContext<CacheVersion::kV2, AllocatorKind::kStd, Key, Value, Hash, Equal> {
    using Alloc = std::allocator<std::byte>;
    using Cache = lru::v2::LRUCache<Key, Value, Hash, Equal, Alloc>;

    Alloc allocator;
    Cache cache;

    CacheContext(size_t capacity)
        : allocator{}
        , cache{capacity, allocator} {
    }
};

template <typename Key, typename Value, typename Hash, typename Equal>
struct CacheContext<CacheVersion::kV2, AllocatorKind::kPmr, Key, Value, Hash, Equal> {
    using Alloc = std::pmr::polymorphic_allocator<std::byte>;
    using Cache = lru::v2::LRUCache<Key, Value, Hash, Equal, Alloc>;

    std::pmr::synchronized_pool_resource pool;
    Alloc allocator;
    Cache cache;

    CacheContext(size_t capacity)
        : pool{
            std::pmr::pool_options{
                .max_blocks_per_chunk = 1024,
                .largest_required_pool_block = 128 * (2 << 10),
            },
            std::pmr::get_default_resource()
        }
        , allocator{&pool}
        , cache{capacity, allocator} {
    }
};

// =================================================================================================

enum class CacheHitType {
    kHit,
    kMiss,
};

namespace std {
    std::string_view to_string(const CacheHitType hit) noexcept {
        switch (hit) {
            case CacheHitType::kHit: {
                return "hit";
            }
            case CacheHitType::kMiss: {
                return "miss";
            }
        }
    }
} // namespace std

namespace lru_cache_benchmarks {
    using Key = int;
    using Value = std::string;
    using Hash = std::hash<Key>;
    using Equal = std::equal_to<Key>;

    using Batch = std::span<std::tuple<Key, Value>>;

    static constexpr size_t kBatchSize = 1000;
    static constexpr size_t kCacheCapacity = 512;
    static constexpr size_t kKeyRange = 1000000;
    static_assert(kBatchSize < kKeyRange);
    static_assert(kBatchSize > 0);

    template <CacheVersion kVersion, AllocatorKind kKind, CacheHitType kHitType>
    struct LRUCacheFixture final {
        std::mt19937 rng;
        std::optional<MemoryPressure<>> mem_pressure;

        benchmark::State& state;
        CacheContext<kVersion, kKind, Key, Value, Hash, Equal> ctx;

        LRUCacheFixture(benchmark::State& state)
            : rng{std::random_device{}()}
            , mem_pressure{}
            , state{state}
            , ctx{kCacheCapacity} {
            mem_pressure.emplace();
            mem_pressure->start();
        }

        ~LRUCacheFixture() {
            mem_pressure->stop();
            mem_pressure.reset();
        }

        Key make_key(const size_t idx) {
            return idx;
        }

        Value make_value(const size_t idx) {
            return std::string{100, 'a'};
        }

        std::vector<std::tuple<Key, Value>> generate_batch(const size_t b, const size_t e) {
            std::vector<std::tuple<Key, Value>> batch(kBatchSize);
            std::binomial_distribution<size_t> dist(e - b, 0.5);
            for (auto& [k, v] : batch) {
                const size_t idx = b + dist(rng);
                k = make_key(idx);
                v = make_value(idx);
            }
            return batch;
        }

        std::vector<std::tuple<Key, Value>> generate_batch() {
            switch (kHitType) {
                case CacheHitType::kHit: {
                    return generate_batch(0u, kCacheCapacity);
                }
                case CacheHitType::kMiss: {
                    return generate_batch(kCacheCapacity, kKeyRange);
                }
            }
        }
    };

    template <CacheVersion kVersion, AllocatorKind kKind, CacheHitType kHitType, typename Op>
    void run(LRUCacheFixture<kVersion, kKind, kHitType>& f, Op&& op) {
        // Initialize cache
        for (const size_t i : std::views::iota(0u, kCacheCapacity)) {
            f.ctx.cache.put(f.make_key(i), f.make_value(i));
        }

        for (auto _ : f.state) {
            // Prepare
            auto batch = f.generate_batch();

            // Measure
            const auto start = std::chrono::high_resolution_clock::now();
            op(f, batch);
            const auto end = std::chrono::high_resolution_clock::now();

            // Register
            const auto time = (end - start).count() / 1.0e9 / batch.size();
            f.state.SetIterationTime(time);
        }
    }

    template <CacheVersion kVersion, AllocatorKind kKind, CacheHitType kHitType>
    void register_benchmarks() {
        auto add_fixture = [&](std::string_view name, auto op) -> void {
            auto* bench = benchmark::RegisterBenchmark(
                std::format(
                    "LRUCacheBench<{}, {}, {}>::{}",
                    std::to_string(kVersion),
                    std::to_string(kKind),
                    std::to_string(kHitType),
                    name
                ),
                [op](benchmark::State& state) -> void {
                    LRUCacheFixture<kVersion, kKind, kHitType> fixture{state};
                    run(fixture, op);
                }
            );
            bench->Unit(benchmark::kNanosecond);
            bench->UseManualTime();
            bench->Iterations(100);
        };

        add_fixture("put", [](LRUCacheFixture<kVersion, kKind, kHitType>& f, Batch batch) -> void {
            for (const auto& [k, v] : batch) {
                f.ctx.cache.put(k, v);
            }
        });

        add_fixture("get", [](LRUCacheFixture<kVersion, kKind, kHitType>& f, Batch batch) -> void {
            for (const auto& [k, v] : batch) {
                benchmark::DoNotOptimize(f.ctx.cache.get(k));
            }
        });
    };

    template <CacheVersion kVersion, AllocatorKind kKind>
    void register_benchmarks() {
        register_benchmarks<kVersion, kKind, CacheHitType::kHit>();
        register_benchmarks<kVersion, kKind, CacheHitType::kMiss>();
    }

    template <CacheVersion kVersion>
    void register_benchmarks() {
        register_benchmarks<kVersion, AllocatorKind::kStd>();
        register_benchmarks<kVersion, AllocatorKind::kPmr>();
    }

    void register_benchmarks() {
        register_benchmarks<CacheVersion::kV1>();
        register_benchmarks<CacheVersion::kV2>();
    }
} // namespace lru_cache_benchmarks

// =================================================================================================

int main(int argc, char** argv) {
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1;
    }

    // Add custom benchmarks
    lru_cache_benchmarks::register_benchmarks();

    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    return 0;
}
