/**
 * Проверить, содержится ли перестановка строки s а строке t
 */
bool IsContaisPermulation(const std::string& t, const std::string& s)
{
    if (t.size() < s.size())
    {
        return false;
    }
    if (s.size() == 0)
    {
        return true;
    }
    
    auto charBallances = std::unordered_map<char, int>();
    for (auto& c : s)
    {
        ++charBallances[c];
    }
    
    size_t i = 0; // window's beginning
    size_t j = 0; // window's ending
    for (; j < t.size(); ++j)
    {
        // add to scan window
        if (const auto c = s[j]; --charBallances[c] == 0)
        {
            charBallances.erase(c);
        }
        
        const auto windowSize = j - i + 1;
        
        // remove from scan window if it's nedded
        if (windowSize > s.size())
        {
            if (const auto c = s[i]; ++charBallances[c] == 0)
            {
                charBallances.erase(c);
            }
            ++i;
        }
        
        if (charBallances.size() == 0)
        {
            return true;
        }
    }
    return false;
}
