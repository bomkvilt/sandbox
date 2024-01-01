// https://leetcode.com/problems/min-stack/

class MinStack {
private:
    struct TEntry {
        int CurrMin;
        int Value;
    };

public:
    MinStack() {
        // void
    }
    
    void push(int val) {
        Stack.push({
            .CurrMin = Stack.size() ? std::min(val, getMin()) : val,
            .Value = val,
        });
    }
    
    void pop() {
        Stack.pop();
    }
    
    int top() {
        return Stack.top().Value;
    }
    
    int getMin() {
        return Stack.top().CurrMin;
    }
    
private:
    std::stack<TEntry> Stack;
};
