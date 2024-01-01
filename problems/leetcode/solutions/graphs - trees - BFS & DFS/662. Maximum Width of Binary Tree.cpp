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
       
    int widthOfBinaryTree(TreeNode* root) 
    {
        if(!root) 
        {
            return 0;
        }
        
        int maxWidth = 0;        
        auto q = queue<pair<TreeNode*, int>>();
        q.push({root, 1}); 
        
        while(!q.empty())
        {
            auto levelSize     = q.size();
            auto startPosition = q.front().second;
            auto endPosition   = q.back ().second;
             
            maxWidth = std::max(maxWidth, endPosition - startPosition + 1);
            
            for(int i = 0; i < levelSize; ++i)
            {
                auto p = q.front();
                q.pop();
                
                auto node = p.first;
                auto position = p.second;
                auto offsettedPosition = position - startPosition;
                 
                if(node->left) 
                {
                    q.push({node->left, 2 * offsettedPosition}); // or 2 * idx + 1
                }
                if(node->right)
                {
                    q.push({node->right, 2 * offsettedPosition+1}); // or 2* idx + 2
                }
            }
           
        }
        return maxWidth;
    }
};
