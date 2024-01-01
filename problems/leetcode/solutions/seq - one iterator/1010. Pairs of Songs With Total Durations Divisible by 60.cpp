// https://leetcode.com/problems/pairs-of-songs-with-total-durations-divisible-by-60/


class Solution {
public:
    int numPairsDivisibleBy60(vector<int>& time) {
        static constexpr int BASE = 60;
        
        auto reminders = std::array<int, BASE>{};
        reminders.fill(0);

        int cases = 0;
        for (int i = 0; i < time.size(); ++i) {
            const int base = time[i] % BASE;
            const int rest = (BASE - base) % BASE;

            cases += reminders[rest];
            reminders[base] += 1;
        }

        return cases;
    }
};
