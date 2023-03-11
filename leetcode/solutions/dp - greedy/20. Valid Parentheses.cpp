// https://leetcode.com/problems/valid-parentheses/description/

class Solution {
public:
    bool isValid(string s) {
        auto stack = std::stack<char>();
        for (const char c : s) {
            char expected = 0;
            switch (c) {
                case '{': case '[': case '(':
                    stack.push(c);
                    continue;
                case '}': expected = '{'; break;
                case ']': expected = '['; break;
                case ')': expected = '('; break;
                default: continue;
            }
            
            if (stack.empty()) {
                return false;
            }
            
            const auto top = stack.top();
            stack.pop();
            if (top != expected) {
                return false;
            }
        }

        return stack.empty();
    }
};
