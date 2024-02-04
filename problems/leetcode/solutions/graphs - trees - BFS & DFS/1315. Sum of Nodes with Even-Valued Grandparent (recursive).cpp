// https://leetcode.com/problems/sum-of-nodes-with-even-valued-grandparent/description/


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
    int sumEvenGrandparent(const TreeNode* const root) {
        return Inner(root, nullptr, nullptr);
    }

private:
    /**
     * NOTE: c++ has limitations over a stack size (e.g. 1...8 MiB);
     * in this case each function call frame is ~24B;
     * hence, the recursive algorithm can handle 4e4...3e5 or even 6e6 nodes in depth https://godbolt.org/z/GoWbP9Wfs
     */
    static int Inner(const TreeNode* const node, const TreeNode* const parent, const TreeNode* const grand) {
        if (!node) {
            return 0;
        }
        
        int diff = 0;
        if (grand && grand->val % 2 == 0) {
            diff += node->val;
        }

        return
            Inner(node->left, node, parent) +
            Inner(node->right, node, parent) +
            diff
        ;
    }
};
