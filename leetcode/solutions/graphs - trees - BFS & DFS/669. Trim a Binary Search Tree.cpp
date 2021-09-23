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
class Solution 
{
public:
    
    TreeNode* trimBST(TreeNode* root, int L, int H) {
        if(!root) return root;
        root -> left = trimBST(root -> left, L, H);
        root -> right = trimBST(root -> right, L, H);
        if(root -> val < L) return root -> right;
        if(root -> val > H) return root -> left;
        return root;
    }
};
