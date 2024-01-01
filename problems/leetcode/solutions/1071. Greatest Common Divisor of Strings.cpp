class Solution 
{
public:

    string gcdOfStrings(string str1, string str2) 
    {
        const auto LCD = GetLCD(str1.length(), str2.length());
        if (LCD < 1)
        {
            return "";
        }
        
        const auto sub = str1.substr(0, LCD);
        std::cout << sub << ".";
        
        const auto b1 = IsMadeFrom(str1, sub);
        const auto b2 = IsMadeFrom(str2, sub);
        return b1 && b2 ? sub : "";
    }
    
    int GetLCD(int a, int b)
    {
        if (a < b) 
        {
            swap(a, b);
        }
        while (a % b != 0) 
        {
            a = a % b;
            swap(a, b);
        }
        return b;
    }
    
    bool IsMadeFrom(const string& str1, const string& str2)
    {
        const auto step = str2.length();
        
        for (auto pos = 0; pos < str1.length(); pos += step)
        {
            if (str1.find(str2, pos) != pos)
            {
                return false;
            }
        }
        return true;
    }
};
