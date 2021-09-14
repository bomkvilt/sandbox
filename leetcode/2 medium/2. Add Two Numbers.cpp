/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */

class Solution 
{
public:

    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) 
    {
        if(!l1) return l2;
        if(!l2) return l1;
        
        ListNode  head(-1);
        ListNode* tail = &head;
        const auto addNode = [&tail](int val)
        {
            auto newNode = new ListNode(val);
            tail->next = newNode;
            tail = newNode;
        };
        
        auto itr1 = l1;
        auto itr2 = l2;
        auto shift = 0;
        while (itr1 && itr2)
        {
            const auto sum = itr1->val + itr2->val + shift;
            const auto val = sum % 10;
            shift = sum / 10;
            addNode(val);
            
            itr1 = itr1->next;
            itr2 = itr2->next;
        }
        
        auto itr = itr1 ? itr1 : itr2;
        for (; itr; itr = itr->next)
        {
            const auto sum = itr->val + shift;
            const auto val = sum % 10;
            shift = sum / 10;
            addNode(val);
        }
        
        if (shift)
        {
            addNode(shift);
        }
        
        return head.next;
    }
};
