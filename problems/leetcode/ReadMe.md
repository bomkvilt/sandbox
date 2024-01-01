# LeetCode problem's solutions

## Problem classes
- Knapsack problem
    - 0-1 problem
        - [Last Stone Weight II](<2 medium/1049. Last Stone Weight II.cpp>)
- Graphs
    - [Tarjan's strongly connected components](<../algoritms/graphs/misc/Tarjan's connections.md>)
        - [Critical Connections in a Network](<solutions/graphs - connections/1192. Critical Connections in a Network.cpp>)
- Trees
    - Traverses: BFS
        - [BFS with level control](<solutions/graphs - trees - BFS & DFS/662. Maximum Width of Binary Tree.cpp>)
        - [BFS with time priority](<solutions/graphs - trees - BFS & DFS/743. Network Delay Time.cpp>)
        - [Delete if not betwean ](<solutions/graphs - trees - BFS & DFS/669. Trim a Binary Search Tree.cpp>)
    - Segment Tree
        - [Mutable sum of index range](<solutions/graphs - trees - Index trees/307. Range Sum Query - Mutable.cpp>)
- Enumerations
    - Ways to get permulations:
        - [v1](<solutions/math - enumerations/949. Largest Time for Given Digits.cpp>)


## Tricks:
Way to optimize code with use of a compiler
```
#pragma GCC optimize("O3")

static auto _f = [](){
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    return nullptr;
}();
```
