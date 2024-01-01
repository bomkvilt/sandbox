class Solution 
{
public:
    
    bool checkRecord(const string& s) 
    {
        int absentCount = 0;
        int currentLateSeq = 0;
        
        for (auto c : s)
        {
            switch(c) {
            case 'A':
                if (++absentCount >= 2)
                {
                    return false;
                }
                break;
                    
            case 'L':
                if (++currentLateSeq >= 3)
                {
                    return false;
                }
                break;
            }
            
            if (c != 'L')
            {
                currentLateSeq = 0;
            }
        }
        return true;
    }
};
