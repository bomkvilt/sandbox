// https://leetcode.com/problems/angle-between-hands-of-a-clock/

class Solution {
public:
    double angleClock(int hour, int minutes) {
        double stateM = (minutes % 60) / 60.;
        double stateH = (hour % 12) / 12. + stateM / 12.;
        
        auto delta = std::abs(stateH - stateM);
        if (delta > 0.5) {
            delta = 1 - delta;
        }
        
        return delta * 360;
    }
};
