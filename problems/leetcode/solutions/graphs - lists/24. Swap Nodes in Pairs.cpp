// https://leetcode.com/problems/swap-nodes-in-pairs/description/

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        for (ListNode** pnode = &head; true; ) {
            ListNode* node = *pnode;
            if (!node || !node->next) {
                break;
            }

            auto* next = node->next;
            node->next = next->next;
            next->next = node;
            (*pnode) = next;

            pnode = &node->next;
        }
        return head;
    }
};
