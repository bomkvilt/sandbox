// https://leetcode.com/problems/populating-next-right-pointers-in-each-node/

/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};
*/

class Solution {
public:
    Node* connect(Node* root) {
        std::queue<Node*> bfs({root});
        
        while (bfs.size()) {
            Node* prev = nullptr;
            for (int levelNodes = bfs.size(); levelNodes > 0; --levelNodes, bfs.pop()) {
                auto* node = bfs.front();
                if (!node) {
                    continue;
                }

                if (prev) {
                    prev->next = node;
                }

                bfs.push(node->left);
                bfs.push(node->right);
                prev = node;
            }
        }
        return root;
    }
};
