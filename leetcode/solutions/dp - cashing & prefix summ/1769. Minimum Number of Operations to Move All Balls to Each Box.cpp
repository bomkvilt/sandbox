// https://leetcode.com/problems/minimum-number-of-operations-to-move-all-balls-to-each-box


class Solution {
private:
    struct TCacheInfo {
        int Balls = 0;
        int Moves = 0;
    };
    
public:
    vector<int> minOperations(string boxes) {
        if (boxes.size() == 0) {
            return {};
        }
        
        const int lastIdx = boxes.size() - 1;
        
        auto csumm = std::vector<TCacheInfo>(boxes.size());        
        for (int i = lastIdx; i >= 0; --i) {
            csumm[i].Balls = boxes[i] == '1' ? 1 : 0;
            if (i != lastIdx) {
                csumm[i].Balls += csumm[i + 1].Balls;
                csumm[i].Moves += csumm[i + 1].Balls + csumm[i + 1].Moves;
            }
        }
        
        auto result = std::vector<int>(boxes.size(), 0);
        
        TCacheInfo state;
        for (int i = 0; i <= lastIdx; ++i) {
            state.Moves += state.Balls;
            state.Balls += boxes[i] == '1' ? 1 : 0;
            
            result[i] = state.Moves + csumm[i].Moves;
        }
        
        return result;
    }
};
