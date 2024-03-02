// https://leetcode.com/problems/rotate-list/description/

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
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head) {
            return nullptr;
        }

        size_t count = 1;
        auto* tail = head;
        for (; tail->next; tail = tail->next, ++count) {
            // pass
        }

        k = k % count;
        if (k == 0) {
            return head;
        }

        tail->next = head;

        for (size_t i = 0; i < count - k; ++i) {
            tail = head;
            head = head->next;
        }

        tail->next = nullptr;
        return head;
    }
};
