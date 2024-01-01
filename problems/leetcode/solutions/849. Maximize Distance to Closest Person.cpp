class Solution 
{
public:
    
    int maxDistToClosest(const vector<int>& seats) 
    {
        int maxEmpties = 0;
        int personI = -1;
        for (int i = 0; i <= seats.size(); ++i)
        {
            const auto bInRange = i < seats.size();
            if (bInRange && seats[i] != 1)
            {
                continue;
            }
            
            const auto currEmpties = i - personI - 2;
            maxEmpties = std::max(maxEmpties
                , personI >= 0 && bInRange
                ? currEmpties / 2
                : currEmpties
            );
            personI = i;
        }
        
        return maxEmpties + 1;
    }
};
