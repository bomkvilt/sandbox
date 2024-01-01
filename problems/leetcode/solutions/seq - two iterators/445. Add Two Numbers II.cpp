/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(const ListNode* l1, const ListNode* l2) 
    {
        auto s1 = std::stack<const ListNode*>();
        auto s2 = std::stack<const ListNode*>();
        
        for (auto node = l1; node; node = node->next)
        {
            s1.push(node);
        }
        for (auto node = l2; node; node = node->next)
        {
            s2.push(node);
        }
        
        int shift = 0;
        ListNode* root = nullptr;
        
        auto addDigit = [&root, &shift](const int val, const bool bLast = false)
        {
            const auto sum = val + shift;
            if (!bLast || bLast && sum)
            {
                const auto dgt = sum % 10;
                shift = sum / 10;
                root  = new ListNode(dgt, root);
            }
        };
        
        for (; s1.size() && s2.size(); s1.pop(), s2.pop())
        {
            addDigit(s1.top()->val + s2.top()->val);
        }
        for (; s1.size(); s1.pop())
        {
            addDigit(s1.top()->val);
        }
        for (; s2.size(); s2.pop())
        {
            addDigit(s2.top()->val);
        }
        // flush shift
        addDigit(0, true);
        
        return root;
    }
};
