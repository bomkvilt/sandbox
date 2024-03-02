// https://leetcode.com/problems/linked-list-cycle-ii/description/

#include <unordered_set>


struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};


class Solution {
public:
    ListNode* detectCycle(ListNode* head) {
        std::unordered_set<ListNode*> visited;

        for (; head; head = head->next) {
            if (visited.contains(head)) {
                return head;
            }
            visited.insert(head);
        }

        return nullptr;
    }
};
