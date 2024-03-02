#include <vector>

namespace NTree {
    struct TreeNode final {
        int val;
        TreeNode* left;
        TreeNode* right;
    };

    struct in_order_tag {};
    struct pre_order_tag {};
    struct post_order_tag {};

    template<typename TVisitor>
    TVisitor DFS(const TreeNode* const root, TVisitor visitor) {
        if (!root) {
            return visitor;
        }

        std::vector<const TreeNode*> delayedStack;
        const TreeNode* prev = nullptr;
        const TreeNode* node = root;

        while (delayedStack.size() || node) {
            if (node) {
                delayedStack.push_back(node);
                visitor(node, delayedStack, pre_order_tag{}); // NOTE:
                node = node->left;
                continue;
            }

            node = delayedStack.back();
            const auto right = node->right && node->right != prev;

            if (!node->right || right) {
                visitor(node, delayedStack, in_order_tag{}); // NOTE:
            }

            if (right) {
                node = node->right;
                continue;
            }

            visitor(node, delayedStack, post_order_tag{}); // NOTE:
            delayedStack.pop_back();
            prev = node;
            node = nullptr;
        }

        return visitor;
    }
}
