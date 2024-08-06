// https://leetcode.com/problems/minimum-hours-of-training-to-win-a-competition

#include <algorithm>
#include <ranges>
#include <vector>


class Solution {
public:
    int minNumberOfHours(
        const int initialEnergy,
        const int initialExperience,
        const std::vector<int>& energy,
        const std::vector<int>& experience
    ) {
        int e = initialEnergy;
        int x = initialExperience;

        int de = 0;
        int dx = 0;

        for (const auto i : std::views::iota(0ul, energy.size())) {
            const auto ei = e - energy[i];
            const auto xi = x - experience[i];

            de = std::max(de, std::max(0, 1 - ei));
            dx = std::max(dx, std::max(0, 1 - xi));

            e -= energy[i];
            x += experience[i];
        }

        return de + dx;
    }
};
