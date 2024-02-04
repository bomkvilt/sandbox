// https://leetcode.com/problems/smallest-subtree-with-all-the-deepest-nodes/description/

#include <stack>


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
    struct TPreOrderTag {};
    struct TPostOrderTag {};

    struct TState {
        size_t MaxDepth = 0;
        size_t Hits = 0;
    };

public:
    TreeNode* subtreeWithAllDeepest(TreeNode* const root) {
        if (!root) {
            return nullptr;
        }

        std::stack<TState> stateStack;
        TreeNode* target = nullptr;
        int maxDepth = 0;

        DoDFSPostOrder(root, [&]<typename TStage>(TreeNode* const node, TStage) -> void {
            // >> pre-order pass: figer out our current depth
            if constexpr (std::is_same_v<TStage, TPreOrderTag>) {
                stateStack.push({.MaxDepth = stateStack.size()});
                return;
            }

            // >> post-order pass: process a child-given state and propagate the state to a parent
            auto state = stateStack.top();
            stateStack.pop();

            // update max state
            if (state.Hits != 1 && state.MaxDepth >= maxDepth) {
                maxDepth = state.MaxDepth;
                target = node;
            }

            // update parent state
            if (stateStack.size()) {
                auto& parentState = stateStack.top();
                if (parentState.MaxDepth < state.MaxDepth) {
                    parentState.MaxDepth = state.MaxDepth;
                    parentState.Hits = 1;
                }
                else if (parentState.MaxDepth == state.MaxDepth) {
                    parentState.Hits += 1;
                }
            }
        });

        return target ? target : root;
    }

private:
    template <typename TVisitor>
    static TVisitor DoDFSPostOrder(TreeNode* const root, TVisitor visitor) {
        if (!root) {
            return visitor;
        }

        std::stack<TreeNode*> delayedStack;
        TreeNode* prev = nullptr;
        TreeNode* node = root;
        int maxDepth = 0;

        while (node || delayedStack.size()) {
            if (node) {
                visitor(node, TPreOrderTag{});
                delayedStack.push(node);
                node = node->left;
                continue;
            }

            node = delayedStack.top();
            if (node->right && node->right != prev) {
                node = node->right;
                continue;
            }

            visitor(node, TPostOrderTag{});
            delayedStack.pop();
            prev = node;
            node = nullptr;
        }

        return visitor;
    }
};
