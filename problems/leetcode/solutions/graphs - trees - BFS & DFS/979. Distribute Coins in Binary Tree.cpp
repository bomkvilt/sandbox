// https://leetcode.com/problems/distribute-coins-in-binary-tree/


class Solution {
public:
    int distributeCoins(TreeNode* root) {
        int total_moves = 0;
        rec_contribution(root, total_moves);
        return total_moves;
    }
    
private:
    int rec_contribution(const TreeNode* root, int& total_moves) {
        if (!root) {
            return 0;
        }

        const auto contribL = rec_contribution(root->left, total_moves);
        const auto contribR = rec_contribution(root->right, total_moves);

        total_moves += std::abs(contribL) + std::abs(contribR);
        const auto ballance = contribL + contribR + (root->val - 1);
        return ballance;
    }
};
