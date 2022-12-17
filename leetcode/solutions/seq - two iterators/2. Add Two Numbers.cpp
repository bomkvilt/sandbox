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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* head  = nullptr;
        ListNode* front = nullptr;
        int rest = 0;

        for (; l1 && l2; l1 = l1->next, l2 = l2->next) {
            const auto summ = l1->val + l2->val + rest;
            const auto val = summ % 10;
            rest = summ / 10;

            auto node = new ListNode(val);
            if (front) {
                front->next = node;
            }
            else {
                head = node;
            }
            front = node;
        }
        
        const auto* lr = l1 ? l1 : l2;
        for (; lr; lr = lr->next) {
            const auto summ = lr->val + rest;
            const auto val = summ % 10;
            rest = summ / 10;
            
            auto node = new ListNode(val);
            if (front) {
                front->next = node;
            }
            else {
                head = node;
            }
            front = node;
        }
        
        if (rest) {
            auto node = new ListNode(rest);
            front->next = node;
            front = node;
        }
        return head;
    }
};