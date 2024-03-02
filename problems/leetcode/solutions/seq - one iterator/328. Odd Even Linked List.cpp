// https://leetcode.com/problems/odd-even-linked-list/description/

#include <cstddef>


struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        ListNode evenData;
        ListNode oddData;

        ListNode* even = &evenData;
        ListNode* odd = &oddData;

        for (size_t idx = 1; head; ++idx) {
            auto* tmp = head->next;
            if (idx % 2) {
                even->next = head;
                even = head;
            }
            else {
                odd->next = head;
                odd = head;
            }
            head->next = nullptr;
            head = tmp;
        }

        even->next = oddData.next;
        return evenData.next;
    }
};
