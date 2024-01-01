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
    vector<vector<int>> levelOrder(TreeNode* root) {
        if (!root) {
            return {};
        }
        
        std::queue<TreeNode*> dfs;
        dfs.push(root);
        
        std::vector<std::vector<int>> result;
        while (dfs.size()) {
            std::vector<int> levelValues;
            
            const size_t N = dfs.size();
            for (size_t i = 0; i < N; ++i) {
                const auto* node = dfs.front();
                dfs.pop();

                levelValues.push_back(node->val);
                if (node->left) {
                    dfs.push(node->left);
                }
                
                if (node->right) {
                    dfs.push(node->right);
                }
            }
            result.push_back(std::move(levelValues));
        }
        return result;
    }
};
