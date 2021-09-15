class Solution 
{
private:
    
    struct Transaction
    {
        std::string name;
        std::string city;
        std::string raw;
        int time;
        int amount;
        
        bool bInvalid = false;
    };
    
public:
    
    vector<string> invalidTransactions(const vector<string>& transactions) 
    {
        auto transes = std::vector<Transaction>();
        transes.reserve(transactions.size());
        for (auto& transaction : transactions)
        {
            auto trans = ParseTransaction(transaction);
            transes.push_back(trans);
        }
        std::sort(transes.begin(), transes.end(), [](const auto& t1, const auto& t2)
        {
            return t1.time < t2.time;
        });
        
        auto invalid = vector<string>();
        
        auto backLog = std::unordered_multimap<std::string, Transaction>();
        
        for (auto& trans : transes)
        {
            if (trans.amount > 1000)
            {
                AddToInvalid(invalid, trans);
            }
            
            auto range = backLog.equal_range(trans.name);
            auto itr = range.first;
            auto end = range.second;
            for (; itr != end; ++itr)
            if (itr != backLog.end())
            {
                auto& prev = itr->second;
                if (trans.city != prev.city)
                if (trans.time -  prev.time <= 60)
                {
                    AddToInvalid(invalid, prev);
                    AddToInvalid(invalid, trans);
                }
            }
            
            backLog.insert({trans.name, trans});
        }
        return invalid;
    }
    
private:
    
    void AddToInvalid(vector<string>& invalid, Transaction& trans)
    {
        if (trans.bInvalid)
        {
            return;
        }
        
        trans.bInvalid = true;
        invalid.push_back(trans.raw);
    }
    
    Transaction ParseTransaction(const std::string& str)
    {
        auto itr = str.begin();
        auto end = str.end();
        
        auto ans = Transaction();
        ans.raw  = str;
        for (int i = 0; i < 4; ++i)
        {
            auto pos = std::find(itr, end, ',');
            auto fld = std::string(itr, pos);
            
            switch(i) {
            case 0: ans.name   = fld; break;
            case 1: ans.time   = std::stoi(fld); break;
            case 2: ans.amount = std::stoi(fld); break;
            case 3: ans.city   = fld; break;
            }
            itr = ++pos;
        }
        return ans;
    }
};
