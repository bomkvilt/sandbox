// https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/


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
    TreeNode* sortedArrayToBST(std::vector<int>& nums) {
        return BuildSubtree(nums, 0, nums.size());
    }
    
private:
    TreeNode* BuildSubtree(const std::vector<int>& nums, int i, int j) {
        if (j - i < 1) {
            return nullptr;
        }

        const auto m = i + (j - i) / 2;
        return new TreeNode(
            nums[m],
            BuildSubtree(nums, i, m),
            BuildSubtree(nums, m + 1, j)
        );
    }
};
