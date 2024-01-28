#include <tuple>


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    TreeNode* subtreeWithAllDeepest(TreeNode* root) {
        auto [subtree, _] = RecoursiveSubtree(root, 0);
        return subtree;
    }

private:
    static auto RecoursiveSubtree(TreeNode* root, const int depth) -> std::tuple<TreeNode*, int> {
        if (!root) {
            return {root, depth - 1};
        }

        auto [subtree_l, depth_l] = RecoursiveSubtree(root->left, depth + 1);
        auto [subtree_r, depth_r] = RecoursiveSubtree(root->right, depth + 1);

        if (depth_l == depth_r) {
            return {root, depth_l};
        }
        else if (depth_l > depth_r) {
            return {subtree_l, depth_l};
        }
        else {
            return {subtree_r, depth_r};
        }
    }
};
