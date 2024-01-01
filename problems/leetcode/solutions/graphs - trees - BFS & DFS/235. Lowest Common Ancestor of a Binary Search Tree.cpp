// https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/description/


class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        for (auto* node = root; node; ) {
            if (node == p || node == q) {
                return node;
            }
            if (node->val > p->val && node->val > q->val) {
                node = node->left;
                continue;
            }
            if (node->val < p->val && node->val < q->val) {
                node = node->right;
                continue;
            }
            return node;
        }
        throw std::runtime_error("");
    }
};
