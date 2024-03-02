// https://leetcode.com/problems/split-linked-list-in-parts/description/

#include <vector>


struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


class Solution {
public:
    std::vector<ListNode*> splitListToParts(ListNode* head, int k) {
        constexpr auto listLength = [](const ListNode* node) -> size_t {
            for (size_t size = 0; true; node = node->next, ++size) {
                if (!node) {
                    return size;
                }
            }
        };

        const auto size = listLength(head);
        const auto b = size / k;
        const auto r = size % k;

        std::vector<ListNode*> result;
        result.reserve(k);

        while (head) {
            const auto i = result.size();
            const auto l = b + (i < r);

            result.push_back(head);

            // skip l - 1 elements
            auto* node = head;
            for (size_t k = 1; k < l; ++k) {
                node = node->next;
            }

            // cut off the part
            auto* tmp = node->next;
            node->next = nullptr;
            head = tmp;
        }

        for (size_t i = result.size(); i < k; ++i) {
            result.push_back(nullptr);
        }

        return result;
    }
};
