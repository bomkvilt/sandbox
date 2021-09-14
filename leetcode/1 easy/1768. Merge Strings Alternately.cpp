class Solution 
{
public:
    
    string mergeAlternately(const string& word1, const string& word2) 
    {
        auto itr1 = word1.begin();
        auto itr2 = word2.begin();
        auto end1 = word1.end();
        auto end2 = word2.end();
        
        auto outword = std::string();
        outword.reserve(word1.length() + word2.length() + 1);
        
        for (; itr1 != end1 && itr2 != end2; ++itr1, ++itr2)
        {
            outword += *itr1;
            outword += *itr2;
        }
        
        for (; itr1 != end1; ++itr1)
        {
            outword += *itr1;
        }
        
        for (; itr2 != end2; ++itr2)
        {
            outword += *itr2;
        }
        return outword;
    }
};
