#pragma once

#include <cstddef>
#include <list>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace lru::v1::internal {
    template <
        typename Key,
        typename Value,
        typename Hash = std::hash<Key>,
        typename Equal = std::equal_to<Key>,
        typename Allocator = std::allocator<std::byte>
    >
    class LRUCache {
    public:
        explicit LRUCache(size_t capacity, Allocator allocator = Allocator{})
            : capacity_{capacity}
            , list_{allocator}
            , index_{allocator} {
            if (capacity_ == 0) {
                throw std::invalid_argument("Capacity must be greater than 0");
            }
        }

        LRUCache(LRUCache&& other) noexcept = default;
        LRUCache& operator=(LRUCache&& other) noexcept = default;

        LRUCache(const LRUCache&) = delete;
        LRUCache& operator=(const LRUCache&) = delete;

        ~LRUCache() noexcept {
            clear();
        }

        [[nodiscard]]
        bool empty() const noexcept {
            return index_.empty();
        }

        [[nodiscard]]
        size_t capacity() const noexcept {
            return capacity_;
        }

        [[nodiscard]]
        size_t size() const noexcept {
            return index_.size();
        }

        void clear() noexcept {
            index_.clear();
            list_.clear();
        }

        template <typename K>
        [[nodiscard]]
        std::optional<Value> get(const K& key) noexcept(noexcept(Hash{}(key)) && noexcept(Equal{}(key, key))) {
            if (auto it = index_.find(key); it != index_.end()) {
                list_.splice(list_.begin(), list_, it->second);
                return it->second->value;
            }
            return std::nullopt;
        }

        void put(Key key, Value value) {
            if (auto it = index_.find(key); it != index_.end()) {
                if constexpr (std::is_move_assignable_v<Value>) {
                    it->second->value = std::move(value);
                    list_.splice(list_.begin(), list_, it->second);
                    return;
                } else {
                    list_.erase(it->second);
                    index_.erase(it);
                }
            }

            // Add a new node
            auto& node = list_.emplace_front(std::move(key), std::move(value));
            index_[node.key] = list_.begin();

            // Evict if necessary
            if (list_.size() >= capacity_) {
                index_.erase(list_.back().key);
                list_.pop_back();
            }
        }

        template <typename K>
        [[nodiscard]]
        bool erase(const K& key) noexcept(noexcept(Hash{}(key)) && noexcept(Equal{}(key, key))) {
            if (auto it = index_.find(key); it != index_.end()) {
                list_.erase(it->second);
                index_.erase(it);
                return true;
            }
            return false;
        }

        template <typename K>
        [[nodiscard]]
        bool contains(const K& key) const noexcept(noexcept(Hash{}(key)) && noexcept(Equal{}(key, key))) {
            return index_.find(key) != index_.end();
        }

    private:
        struct ListNode {
            Key key;
            Value value;
        };

        using ListAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<ListNode>;
        using List = std::list<ListNode, ListAllocator>;

        using IndexValue = List::iterator;
        using IndexNode = std::pair<const Key, IndexValue>;
        using IndexAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<IndexNode>;
        using Index = std::unordered_map<Key, IndexValue, Hash, Equal, IndexAllocator>;

        size_t capacity_;
        List list_;
        Index index_;
    };
} // namespace lru::v1::internal

namespace lru::v1 {
    using internal::LRUCache;
}
