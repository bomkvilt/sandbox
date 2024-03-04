// https://leetcode.com/problems/implement-trie-prefix-tree/description/

#include <array>
#include <stdexcept>
#include <string_view>
#include <vector>


// NOTE: use radix trie for unicode chars
class Trie final {
private:
    /**
     * The class must be mostly noexcept since it's
     * not possible to use RAII pointers here.
     */
    struct TNode final {
    public:
        using TChildren = std::array<TNode*, 'z' - 'a' + 1>;

        TChildren Children;

        bool HasValue = false;

    public:
        constexpr TNode() noexcept {
            Children.fill(nullptr);
        };

        constexpr bool Contains(const char c) const {
            CheckChar(c); return Children[c - 'a'];
        }

        constexpr TNode*& operator[](const char c) {
            CheckChar(c); return Children[c - 'a'];
        }

        constexpr const TNode* operator[](const char c) const {
            CheckChar(c); return Children[c - 'a'];
        }

    private:
        static constexpr void CheckChar(const char c) {
            if (c < 'a' || c > 'z') {
                throw std::out_of_range("only ascii lower case chars are supported");
            }
        }
    };

    struct TDeletor final {
    public:
        TNode& Root;

        // NOTE: noexcept since the class will be called in the dtor
        // TODO: constexpr since c++23
        void operator()() noexcept {
            std::vector<TNode*> stack;
            stack.push_back(&Root);

            while (stack.size()) {
                auto* node = stack.back();
                stack.pop_back();

                for (auto* next : node->Children) {
                    if (next) {
                        stack.push_back(next);
                    }
                }

                delete node;
            }
        }
    };

    struct TSearcher final {
    public:
        TNode& Root;

        constexpr TNode* operator()(const std::string_view s) {
            auto* node = &Root;
            for (const auto c : s) {
                if (node = (*node)[c]; !node) {
                    return nullptr;
                }
            }
            return node;
        }
    };

public:
    constexpr Trie() = default;

    constexpr Trie(Trie&& rhs) noexcept = default;
    constexpr Trie(const Trie& rhs) = delete;

    // TODO: constexpr since c++23
    Trie& operator=(Trie&& rhs) noexcept = default;
    Trie& operator=(const Trie& rhs) = delete;

    // TODO: constexpr since c++23
    ~Trie() noexcept {
        // NOTE: comment for leetcode
        TDeletor{Root}();
    }

public:
    void insert(std::string_view word) {
        auto* node = &Root;
        for (const auto c : word) {
            if (auto*& next = (*node)[c]; !next) {
                node = next = new TNode{};
            }
            else {
                node = next;
            }
        }

        if (word.size() && node) {
            node->HasValue = true;
        }
    }

    bool search(std::string_view word) {
        if (auto* node = TSearcher{Root}(word)) {
            return node->HasValue;
        }
        return false;
    }

    bool startsWith(std::string_view prefix) {
        return TSearcher{Root}(prefix);
    }

private:
    TNode Root;
};
