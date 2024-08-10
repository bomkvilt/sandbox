## Count Subranges

### Naive Solution

SubRange is defined by two indices [i, j], hene it's enogth to use 2 nested loops over the two indices
to brute force all the possible subranges.

```cpp
int func(...) {
    for (const auto i : std::views::iota(0ul, nums.size())) {
        window.clear();
        for (const auto j : std::views::iota(i, nums.size())) {
            window.insert(nums[j]);
            if (window.size() == power) {
                result += 1;
            }
        }
    }
}
```


### Coun Subranges with K distict elements

```cpp
/**
 * .prefix
 * |   .core
 * |   |     .suffix : increases result by 1 and |prefix|
 * vvv vvvvv vvvvv
 *    -------     -------
 * * *|* * *|* * *|* * *|* * * * * *
 *    |^ ^ ^|     |^ ^ ^|
 *   ^|^ ^ ^|^   ^|^ ^ ^|
 * ^ ^|^ ^ ^|^ ^ ^|^ ^ ^|
 * -------------
 *         2 2 2 2 3    prefix
 *         3 6 9 1 1    result[part]
 *               2 6    result[part]
 *     ^           ^ here we assume that the element are the same
 */
int func(const std::vector<int>& nums, const int k) {
    std::unordered_map<int, int> window;

    int result = 0;
    int prefix = 0;

    auto i = nums.begin();
    auto j = nums.begin();

    for (; j != nums.end(); ++j) {
        window[*j] += 1;

        // new element, the prefix must be invalidated
        for (; window.size() > k; ++i) {
            if ((window[*i] -= 1) == 0) {
                window.erase(*i);
            }
            prefix = 0;
        }

        // keep the core minimum, increase the prefix
        for (; window[*i] > 1; ++i) {
            window[*i] -= 1;
            prefix += 1;
        }

        if (window.size() == k) {
            result += prefix;
            result += 1;
        }
    }

    return result;
}
```
