2 задача

Написать класс, который после первичной инициализации 
сможет отдавать сумму элементов на заданном уровне

// ------------------------

struct Node {
    int value;
    Node* l;
    Node* r;
};

class Handler 
{
public:

    void init(const Node* root)
    {
        if (!root)
        {
            return;
        }
        
        auto queue = std::queue<const Node*>();
        queue.push(root);
        
        int level = 0;
        for(; queue.size(); ++level)
        {
            const auto count = queue.size();
            
            sums.push_back(0);
            auto& sum = sums[level];
            for (int i = 0; i < count; ++i)
            {
                auto node = queue.front();
                queue.pop();
                
                sums += node->value;
                
                if (node->l)
                {
                    queue.push(node->l);
                }
                if (node->r)
                {
                    queue.push(node->r);
                }
            }
        }
    }
    
    int sum(size_t level)
    {
        return level < sums.size() ? sums[level] : 0;
    }
    
private:

    std::vector<int> sums;
};
