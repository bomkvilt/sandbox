class Solution {
public:
    string largestTimeFromDigits(const vector<int>& arr) 
    {
        auto idx = vector<int>();
        auto ans = std::vector<std::array<int, 2>>();
        GenerateHours(arr, idx, ans);
        
        if (ans.size() == 0)
        {
            return "";
        }
        
        auto max = ans[0];
        for (int i = 1; i < ans.size(); ++i)
        {
            const auto& num = ans[i];
            const auto nn = num[0] * 100 + num[1];
            const auto mm = max[0] * 100 + max[1];
            if (nn > mm)
            {
                max = num;
            }
        }
        
        auto hh = std::to_string(max[0]);
        auto mm = std::to_string(max[1]);
           
        return (hh.size() < 2 ? '0' + hh : hh) + ":" 
            +  (mm.size() < 2 ? '0' + mm : mm);
    }
    
    void GenerateHours(const vector<int>& arr, vector<int>& idx, std::vector<std::array<int, 2>>& ans)
    {
        if (idx.size() == arr.size())
        {
            auto num = std::array<int, 4>{
                arr[idx[0]],
                arr[idx[1]], 
                arr[idx[2]], 
                arr[idx[3]]
            };
            int hh = num[0] * 10 + num[1];
            int mm = num[2] * 10 + num[3];
            if (hh < 24 && mm < 60)
            {
                ans.push_back({hh, mm});
            }
        }
        
        for (int i = 0; i < arr.size(); ++i)
        {
            if (std::find(idx.begin(), idx.end(), i) != idx.end())
            {
                continue;
            }
            
            idx.push_back(i);
            GenerateHours(arr, idx, ans);
            idx.pop_back();
        }
    }
};
