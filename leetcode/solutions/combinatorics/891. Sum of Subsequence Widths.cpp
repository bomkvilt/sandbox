// https://leetcode.com/problems/sum-of-subsequence-widths/

// https://www.geeksforgeeks.org/sum-of-width-max-and-min-diff-of-all-subsequences/

// final solution

class Solution {
public:
    int sumSubseqWidths(vector<int>& nums) {
        constexpr int modulo = 1000 * 1000 * 1000 + 7;
        
        std::sort(nums.begin(), nums.end());
        
        const auto n = nums.size();
        
        int result0 = 0;
        int result1 = 0;
        for (size_t a = 1, i = 0; i < n; ++i, a = (a * 2) % modulo) {
            result0 = (result0 + a * nums[i]) % modulo;
            result1 = (result1 + a * nums[n - 1 - i]) % modulo;
        }
        std::cout << result0 << " " << result1 << std::endl;
        return (result0 + modulo - result1) % modulo;
    }
};

// slow solution

class Solution {
private:
    struct TSubrange {
        int min = 0;
        int max = 0;
        size_t i = 0;
        size_t j = 0;
        
    public:
        void InMerge(const TSubrange& r) {
            min = std::min(min, r.min);
            max = std::max(max, r.max);
            i = std::min(i, r.i);
            j = std::max(j, r.j);
        }
        
        int GetWidth() const {
            return max - min;
        }
    };
    
public:
    int sumSubseqWidths(vector<int>& nums) {
        // make basic ranges
        std::vector<TSubrange> atomicNodes;
        atomicNodes.reserve(nums.size());
        for (size_t i = 0; i < nums.size(); ++i) {
            const auto num = nums[i];
            atomicNodes.push_back({num, num, i, i});
        }
        
        // generate sequences
        int result = 0;
        const auto addToResult = [&result](const auto delta) {
            constexpr int modulo = 1000 * 1000 * 1000 + 7;
            result = (result + delta) % modulo;
        };
        
        std::queue<TSubrange> ranges;
        for (const auto& node : atomicNodes) {
            ranges.push(node);
        }
        
        while (ranges.size()) {
            const auto range = ranges.front();
            ranges.pop();
            
            for (size_t j = range.j + 1; j < atomicNodes.size(); ++j) {
                auto newRange = range;
                newRange.InMerge(atomicNodes[j]);
                addToResult(newRange.GetWidth());
                
                if (newRange.j != atomicNodes.size()) {
                    ranges.push(newRange);
                }
            }
        }
        
        return result;
    }
};
