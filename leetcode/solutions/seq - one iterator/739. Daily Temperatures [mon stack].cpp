// https://leetcode.com/problems/daily-temperatures/description/


class Solution {
public:
    std::vector<int> dailyTemperatures(std::vector<int>& temperatures) {
        const int n = temperatures.size();
        if (n == 0) {
            return {};
        }

        std::stack<int> index;

        auto answer = std::vector<int>(n, 0);
        for (int i = n - 1; i >= 0; --i) {
            const auto Ti = temperatures[i];
            while (index.size() && temperatures[index.top()] <= Ti) {
                index.pop();
            }
            if (index.size()) {
                answer[i] = index.top() - i;
            }
            index.push(i);
        }
        return std::move(answer);
    }
};
