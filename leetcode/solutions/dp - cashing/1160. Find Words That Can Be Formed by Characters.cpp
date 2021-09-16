class Solution {
public:
    int countCharacters(const vector<string>& words, const string& chars) 
    {
        auto charDB = std::unordered_map<char, int>();
        for (auto& c : chars)
        {
            charDB[c] += 1;
        }
        
        int counter = 0;
        
        auto wordDB = std::unordered_map<char, int>();
        for (auto& word : words)
        {
            wordDB = charDB;
            
            auto bWrong = false;
            for (auto& c : word)
            {
                if (--wordDB[c] < 0)
                {
                    bWrong = true;
                    break;
                }
            }
            
            if (!bWrong)
            {
                counter += word.length();
            }
        }
        return counter;
    }
};
