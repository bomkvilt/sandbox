
class Solution {
public:
    int numWaterBottles(int numBottles, int numExchange) {
        int numEmpty = 0;
        int drunk = 0;

        while (numBottles > 0) {
            drunk += numBottles;
            numEmpty += numBottles;

            numBottles = numEmpty / numExchange;
            numEmpty = numEmpty % numExchange;
        }

        return drunk;
    }
};
