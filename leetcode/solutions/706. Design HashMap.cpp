#include <unordered_map>



class MyHashMap 
{
public:
    /** Initialize your data structure here. */
    MyHashMap() 
    {
    }
    
    /** value will always be non-negative. */
    void put(int key, int value) 
    {
        hash[key] = value;
    }
    
    /** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
    int get(int key) 
    {
        auto itr = hash.find(key);
        if (itr != hash.end())
        {
            return itr -> second;
        }
        return -1;
    }
    
    /** Removes the mapping of the specified value key if this map contains a mapping for the key */
    void remove(int key) 
    {
        hash.erase(key);
    }
    
private:
    
    std::unordered_map<int, int> hash;
};
