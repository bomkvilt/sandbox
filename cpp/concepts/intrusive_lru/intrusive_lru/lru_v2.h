#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/unordered_set.hpp>

namespace lru::v2::internal {
    namespace bi = boost::intrusive;

    template <typename U, typename V, typename Hash, typename Equal>
    concept noexcept_find = requires(const U& u, const V& v) {
        requires noexcept(Hash{}(u));
        requires noexcept(Hash{}(v));
        requires noexcept(Equal{}(u, v));
        requires noexcept(Equal{}(v, u));
    };

    template <
        typename Key,
        typename Value,
        typename Hash = std::hash<Key>,
        typename Equal = std::equal_to<Key>,
        typename Allocator = std::allocator<std::byte>
    >
    class LRUCache {
    public:
        using allocator_type = Allocator;

        explicit LRUCache(size_t capacity, Allocator allocator = Allocator{})
            : capacity_{capacity}
            , allocator_{allocator}
            , node_allocator_{allocator}
            , index_buckets_(capacity_ * 2)
            , index_{typename Index::bucket_traits(index_buckets_.data(), index_buckets_.size())} {
            if (capacity_ == 0) {
                throw std::invalid_argument("Capacity must be greater than 0");
            }
        }

        LRUCache(LRUCache&& other) noexcept
            : capacity_{other.capacity_}
            , size_{other.size_}
            , allocator_{std::move(other.allocator_)}
            , node_allocator_{std::move(other.node_allocator_)}
            , index_buckets_{std::move(other.index_buckets_)}
            , index_{std::move(other.index_)}
            , list_{std::move(other.list_)} {
        }

        LRUCache& operator=(this LRUCache& self, LRUCache&& other) noexcept {
            if (std::addressof(self) != std::addressof(other)) {
                self.capacity_ = std::move(other.capacity_);
                self.size_ = std::move(other.size_);
                self.allocator_ = std::move(other.allocator_);
                self.node_allocator_ = std::move(other.node_allocator_);
                self.index_buckets_ = std::move(other.index_buckets_);
                self.index_ = std::move(other.index_);
                self.list_ = std::move(other.list_);
            }
            return self;
        }

        // NOTE: storred data should not be copied
        LRUCache(const LRUCache& other) = delete;

        // NOTE: storred data should not be copied
        LRUCache& operator=(const LRUCache& other) = delete;

        ~LRUCache() noexcept {
            clear();
        }

    public:
        [[nodiscard]]
        bool empty(this const LRUCache& self) noexcept {
            return self.size_ == 0;
        }

        [[nodiscard]]
        size_t capacity(this const LRUCache& self) noexcept {
            return self.capacity_;
        }

        [[nodiscard]]
        size_t size(this const LRUCache& self) noexcept {
            return self.size_;
        }

        void clear(this LRUCache& self) noexcept {
            while (not self.list_.empty()) {
                self.erase(self.list_.back());
            }
        }

        /// @brief Get any value associated with the key.
        /// @param key The key to get the value for.
        /// @return Any value associated with the key, or std::nullopt if the key is not in the cache.
        template <typename K>
        [[nodiscard]]
        std::optional<Value> get(const K& key) noexcept(noexcept_find<Key, K, Hash, Equal>) {
            auto it = index_.find(key);
            if (it == index_.end()) {
                return std::nullopt;
            }

            promote(*it); // noexcept
            return it->value;
        }

        /// @brief Insert or override a cached key-value pair.
        /// @param key The key to insert.
        /// @param value The value to insert.
        void put(Key key, Value value) {
            // Try in-place update
            if (auto it = index_.find(key); it != index_.end()) {
                if constexpr (std::is_move_assignable_v<Value>) {
                    // NOTE: we use equivalence classes, not exact equality
                    it->key = std::move(key);
                    it->value = std::move(value);
                    promote(*it);
                    return;
                } else {
                    erase(*it);
                }
            }

            // Allocate and construct a new node
            Node* node = NodeAllocatorTraits::allocate(node_allocator_, 1);
            try {
                NodeAllocatorTraits::construct(node_allocator_, node, std::move(key), std::move(value));
                list_.push_front(*node);
                index_.insert(*node);
                size_ += 1;
            } catch (...) {
                NodeAllocatorTraits::deallocate(node_allocator_, node, 1); // noexcept
                throw;
            }

            // Evict if necessary
            if (size_ >= capacity_) {
                erase(list_.back());
            }
        }

        template <typename K>
        [[nodiscard]]
        bool erase(const K& key) noexcept(noexcept_find<Key, K, Hash, Equal>) {
            auto it = index_.find(key);
            if (it == index_.end()) {
                return false;
            }

            erase(*it); // noexcept
            return true;
        }

        template <typename K>
        [[nodiscard]]
        bool contains(const K& key) const noexcept(noexcept_find<Key, K, Hash, Equal>) {
            return index_.find(key) != index_.end();
        }

        [[nodiscard]]
        allocator_type get_allocator(this const LRUCache& self) noexcept {
            return self.allocator_;
        }

    private:
        struct Node
            : public bi::list_base_hook<bi::link_mode<bi::normal_link>>
            , public bi::unordered_set_base_hook<bi::link_mode<bi::normal_link>> {
            Key key;
            Value value;

            template <typename K, typename V>
            Node(K&& k, V&& v)
                : key(std::forward<K>(k))
                , value(std::forward<V>(v)) {
            }

            bool operator==(this const Node& self, const Node& other) noexcept(
                noexcept(Equal{}(std::declval<const Key&>(), std::declval<const Key&>()))
            ) {
                return Equal{}(self.key, other.key);
            }
        };

        using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
        using NodeAllocatorTraits = std::allocator_traits<NodeAllocator>;

        struct NodeKey {
            using type = Key;

            static const Key& operator()(const Node& e [[clang::lifetimebound]]) noexcept {
                return e.key;
            }

            static const Key& operator()(const Key& k [[clang::lifetimebound]]) noexcept {
                return k;
            }
        };

        struct NodeHash {
            static size_t operator()(const Node& e) noexcept(noexcept(Hash{}(e.key))) {
                return Hash{}(e.key);
            }

            static size_t operator()(const Key& k) noexcept(noexcept(Hash{}(k))) {
                return Hash{}(k);
            }
        };

        struct NodeEqual {
            template <typename A, typename B>
            requires requires(const A& a, const B& b) {
                { NodeKey{}(a) } -> std::same_as<const Key&>;
                { NodeKey{}(b) } -> std::same_as<const Key&>;
                { Equal{}(NodeKey{}(a), NodeKey{}(b)) } -> std::same_as<bool>;
            }
            bool operator()(const A& a, const B& b) const noexcept(noexcept(Equal{}(NodeKey{}(a), NodeKey{}(b)))) {
                return Equal{}(NodeKey{}(a), NodeKey{}(b));
            }
        };

        using List = bi::list<
            Node,
            bi::constant_time_size<false> // NOTE: it's no need to track the size
        >;

        using Index = bi::unordered_set<
            Node,
            bi::key_of_value<NodeKey>,
            bi::hash<NodeHash>,
            bi::equal<NodeEqual>,
            bi::constant_time_size<false> // NOTE: it's no need to track the size
        >;

    private:
        void promote(this LRUCache& self, Node& node) noexcept {
            self.list_.erase(self.list_.iterator_to(node));
            self.list_.push_front(node);
        }

        void erase(this LRUCache& self, Node& node) noexcept {
            // Unlink from containers
            self.list_.erase(self.list_.iterator_to(node));   // noexcept
            self.index_.erase(self.index_.iterator_to(node)); // noexcept
            self.size_ -= 1;

            // Deallocate the node regardless on destructor exceptions and do not re-throw:
            // - https://wg21.cmeerw.net/cwg/issue353
            //
            // - https://eel.is/c++draft/basic.stc#dynamic.deallocation
            //   If a deallocation function terminates by throwing an exception,
            //   the behavior is undefined.
            //
            // - https://eel.is/c++draft/expr.delete#6
            //   The deallocation function is called regardless of whether the destructor
            //   for the object or some element of the array throws an exception.
            try {
                NodeAllocatorTraits::destroy(self.node_allocator_, &node);
            } catch (...) {
                // pass
            }
            NodeAllocatorTraits::deallocate(self.node_allocator_, &node, 1); // noexcept
        }

    private:
        size_t capacity_ = 0;
        size_t size_ = 0;
        Allocator allocator_;
        NodeAllocator node_allocator_;

        std::vector<typename Index::bucket_type> index_buckets_;
        Index index_;

        List list_;
    };
} // namespace lru::v2::internal

namespace lru::v2 {
    using internal::LRUCache;
}
