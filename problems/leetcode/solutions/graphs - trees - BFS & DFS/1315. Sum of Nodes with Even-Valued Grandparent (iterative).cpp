// https://leetcode.com/problems/sum-of-nodes-with-even-valued-grandparent/description/

#include <vector>
#include <type_traits>


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};


class Solution {
private:
    struct pre_order_tag {};
    struct post_order_tag {};

public:
    /**
     * The variant is slower (36ms vs. 28ms == +30%) that the recursive one
     * but allows to operate with wery deep trees (more than 1e5) elements.
     */
    int sumEvenGrandparent(const TreeNode* const root) {
        static constexpr size_t OFFSET = 2;

        int summ = 0;

        DFS(root, [&summ]<typename TStage>(
            const TreeNode* const node, const std::vector<const TreeNode*>& path, TStage
        ) -> void {
            if constexpr (std::is_same_v<TStage, pre_order_tag>) {
                if (path.size() > OFFSET && path[path.size() - (OFFSET + 1)]->val % 2 == 0) {
                    summ += node->val;
                }
            }
        });

        return summ;
    }

private:
    template<typename TVisitor>
    static TVisitor DFS(const TreeNode* const root, TVisitor visitor) {
        if (!root) {
            return visitor;
        }

        std::vector<const TreeNode*> delayedStack;
        const TreeNode* prev = nullptr;
        const TreeNode* node = root;

        while (delayedStack.size() || node) {
            if (node) {
                delayedStack.push_back(node);
                visitor(node, delayedStack, pre_order_tag{});
                node = node->left;
                continue;
            }

            node = delayedStack.back();
            if (node->right && node->right != prev) {
                node = node->right;
                continue;
            }

            visitor(node, delayedStack, post_order_tag{});
            delayedStack.pop_back();
            prev = node;
            node = nullptr;
        }

        return visitor;
    }
};
