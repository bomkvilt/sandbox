#include <algorithm>
#include <vector>


class Solution {
public:
    std::vector<int> intersection(std::vector<int>& nums1, std::vector<int>& nums2) {
        std::sort(nums1.begin(), nums1.end());
        std::sort(nums2.begin(), nums2.end());

        auto pos1 = nums1.begin();
        auto pos2 = nums2.begin();
        
        static constexpr auto propagate = [](auto it, auto end) {
            const auto& v = *it;
            while (++it != end && *it == v) {
                // pass;
            }
            return it;
        };
        
        std::vector<int> result;
        while (pos1 != nums1.end() && pos2 != nums2.end()) {
            if (*pos1 < *pos2) {
                pos1 = propagate(pos1, nums1.end());
            }
            else if (*pos2 < *pos1 ) {
                pos2 = propagate(pos2, nums2.end());
            }
            else { // pos1 == pos2
                result.push_back(*pos1);
                pos1 = propagate(pos1, nums1.end());
                pos2 = propagate(pos2, nums2.end());
            }
        }

        return result;
    }
};
