#include <unordered_map>
#include <stack>

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
    bool findTarget(TreeNode* root, int k) 
    {
        auto numStat = unordered_map<int, int>();
        CollectStatistics(root, numStat);
        
        for (const auto& [num, count] : numStat)
        {
            const auto left = k - num;
            
            auto itr = numStat.find(left);
            auto end = numStat.end();
            if (itr == end)
            {
                continue;
            }
            
            const auto num2 = itr->first;
            if (num == num2 && count < 2)
            {
                continue;
            }
            return true;
        }
        return false;
    }
    
private:
    
    void CollectStatistics(const TreeNode* root, unordered_map<int, int>& numStat)
    {
        enum EState 
        {
            eLeft  = 0,
            eRigth = 1,
            eNode  = 2
        };
        
        if (!root)
        {
            return;
        }
        
        auto path  = std::stack<int>();
        auto nodes = std::stack<const TreeNode*>();
        path .push(eLeft);
        nodes.push(root);
        
        while (path.size())
        {
            auto& state = path.top();
            auto& node  = nodes.top();
            
            switch (state) {
            case eLeft :                    
                path.top() += 1;                    
                if (node->left)
                {
                    path .push(eLeft);
                    nodes.push(node->left);
                }                
                break;
                    
            case eRigth:                    
                path.top() += 1;                    
                if (node->right)
                {
                    path .push(eLeft);
                    nodes.push(node->right);
                }                
                break;
            
            case eNode :                    
                numStat[node->val] += 1;
                path .pop();
                nodes.pop();                
                break;
            }
        }
    }
};
