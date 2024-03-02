// https://leetcode.com/problems/n-ary-tree-postorder-traversal/description/

#include <vector>
#include <stack>


class Node {
public:
    int val;
    std::vector<Node*> children;

    Node() {}

    Node(int _val) {
        val = _val;
    }

    Node(int _val, std::vector<Node*> _children) {
        val = _val;
        children = _children;
    }
};


class Solution {
public:
    std::vector<int> postorder(Node* root) {
        std::vector<int> result;
        DFS(root, [&](const Node* const node) -> void {
            result.push_back(node->val);
        });
        return result;
    }

private:
    template <typename TVisitor>
    static TVisitor DFS(Node* const root, TVisitor visitor) {
        std::stack<Node*> delayedStack;
        std::stack<decltype(root->children.begin())> states;

        Node* node = root;

        while (node || delayedStack.size()) {
            if (node) {
                delayedStack.push(node);
                states.push(node->children.begin());
            }

            node = delayedStack.top();

            if (auto& pos = states.top(); pos != node->children.end()) {
                node = *(pos++);
                continue;
            }

            visitor(node);
            
            delayedStack.pop();
            states.pop();

            node = nullptr;
        }

        return visitor;
    }
};
