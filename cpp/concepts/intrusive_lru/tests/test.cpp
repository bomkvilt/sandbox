#include <gtest/gtest.h>

#include "../intrusive_lru/lru.h"

using lru::LRUCache;

class LRUCacheTest : public ::testing::Test {
protected:
    using Cache = LRUCache<int, std::string>;

    static constexpr size_t DefaultCapacity = 3;

    void SetUp() override {
        cache_ = std::make_unique<Cache>(DefaultCapacity);
    }

    std::unique_ptr<Cache> cache_;
};

TEST_F(LRUCacheTest, EmptyInitially) {
    EXPECT_TRUE(cache_->empty());
    EXPECT_EQ(cache_->size(), 0u);
    EXPECT_EQ(cache_->capacity(), DefaultCapacity);
}

TEST_F(LRUCacheTest, PutAndGet) {
    cache_->put(1, "one");
    cache_->put(2, "two");
    EXPECT_EQ(cache_->size(), 2u);

    const auto v1 = cache_->get(1);
    ASSERT_TRUE(v1.has_value());
    EXPECT_EQ(*v1, "one");

    const auto v2 = cache_->get(2);
    ASSERT_TRUE(v2.has_value());
    EXPECT_EQ(*v2, "two");
}

TEST_F(LRUCacheTest, Eviction) {
    cache_->put(1, "one");
    cache_->put(2, "two");
    cache_->put(3, "three");
    cache_->put(4, "four"); // should evict 1

    EXPECT_FALSE(cache_->get(1).has_value());
    EXPECT_TRUE(cache_->get(2).has_value());
    EXPECT_TRUE(cache_->get(3).has_value());
    EXPECT_TRUE(cache_->get(4).has_value());
}

TEST_F(LRUCacheTest, UpdateValue) {
    cache_->put(1, "one");
    cache_->put(1, "uno");

    const auto v = cache_->get(1);
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, "uno");
}

TEST_F(LRUCacheTest, ContainsAndErase) {
    cache_->put(1, "one");

    EXPECT_TRUE(cache_->get(1).has_value());
    EXPECT_TRUE(cache_->contains(1));

    EXPECT_TRUE(cache_->erase(1));

    EXPECT_FALSE(cache_->get(1).has_value());
    EXPECT_FALSE(cache_->contains(1));
    EXPECT_FALSE(cache_->erase(1));
}

TEST_F(LRUCacheTest, Clear) {
    cache_->put(1, "one");
    cache_->put(2, "two");
    cache_->clear();
    EXPECT_TRUE(cache_->empty());
    EXPECT_EQ(cache_->size(), 0u);
}
