// https://leetcode.com/problems/multiply-strings/


class Solution {
public:
    string multiply(string num1, string num2) {
        if (num1 == "0" || num2 == "0") {
            return "0";
        }

        std::string num(num1.size() + num2.size(), '0');
        for (int i = num1.size() - 1; i >= 0; --i) {
            for (int j = num2.size() - 1; j >= 0; --j) {
                const int idxC = i + j + 1;
                const int idxN = i + j;

                const int summ = (num[idxC] - '0') + (num1[i] - '0') * (num2[j] - '0');
                num[idxC] = summ % 10 + '0';
                num[idxN] += summ / 10;
            }
        }
        return (num[0] == '0') ? num.substr(1) : num;
    }
};
