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
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
        if (!root1) return root2;
        if (!root2) return root1;
        
        struct TState {
            TreeNode* node1;
            TreeNode* node2;
        };
        auto dfs = std::vector<TState>();
        dfs.push_back({root1, root2});
        
        while (dfs.size()) {
            const auto [node1, node2] = dfs.back();
            dfs.pop_back();
            
            node1->val += node2->val;

            if (node1->right && node2->right) {
                dfs.push_back({node1->right, node2->right});
            }
            else if (!node1->right) {
                node1->right = node2->right;
                node2->right = nullptr;
            }

            if (node1->left && node2->left) {
                dfs.push_back({node1->left, node2->left});
            }
            else if (!node1->left) {
                node1->left = node2->left;
                node2->left = nullptr;
            }
        }
        return root1;
    }
};
