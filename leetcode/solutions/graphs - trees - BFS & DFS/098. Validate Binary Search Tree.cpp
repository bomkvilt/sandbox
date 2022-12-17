/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return isValidBST_v1(root);
    }

private:
    bool isValidBST_V1(TreeNode* root) {
        if (!root) {
            return true;
        }

        // validity check
        const TreeNode* prev = nullptr;
        const auto visitor = [&prev](const TreeNode* node) -> bool {
            if (!prev || node->val > prev->val) {
                prev = node;
                return true;
            }
            return false;
        };

        // in-order dfs
        std::vector<const TreeNode*> dfs;
        for (const TreeNode* node = root; dfs.size() || node; ) {
            if (node) {
                dfs.push_back(node);
                node = node->left;
            }
            else if (node = dfs.back(); visitor(node)) {
                dfs.pop_back();
                node = node->right;
            }
            else {
                return false;
            }
        }
        return true;
    }

    bool isValidBST_v2(TreeNode* root) {
        if (!root) {
            return true;
        }

        // validity check
        const TreeNode* prev = nullptr;
        const auto visitor = [&prev](const TreeNode* node) -> bool {
            if (!prev || node->val > prev->val) {
                prev = node;
                return true;
            }
            return false;
        };

        // in-order DFS
        std::vector<const TreeNode*> dfs({nullptr});
        for (const TreeNode* node = root; dfs.size() && node; ) {
            if (node->left && node != dfs.back()) {
                dfs.push_back(node);
                node = node->left;
                continue;
            }
            if (!visitor(node)) {
                return false;
            }
            if (node == dfs.back()) {
                dfs.pop_back();
            }
            if (node->right) {
                node = node->right;
                continue;
            }
            node = dfs.back();
        }
        return true;
    }
};
