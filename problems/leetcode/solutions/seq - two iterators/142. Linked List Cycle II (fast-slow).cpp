// https://leetcode.com/problems/linked-list-cycle-ii/description/

/**
 *  l, L        c, C
 *  ---------->P------------
 *             ^            |
 *              ------------
 * =============================
 *  if x < L : l = x  , c = N/A
 *     else  : l = N/A, c = (x - L) % C
 * =============================
 *   (t - L) % C = (2t - L) % C
 *  t - L + n1 C = 2t - L + n2 C
 *             t = n C
 *  =========> P = t + l
 */

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};


class Solution {
public:
    ListNode* detectCycle(ListNode* const head) {
        auto* fast = head;
        auto* slow = head;

        while (fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;

            if (fast == slow) {
                slow = head;
                while (fast != slow) {
                    fast = fast->next;
                    slow = slow->next;
                }
                return slow;
            }
        }

        return nullptr;
    }
};
