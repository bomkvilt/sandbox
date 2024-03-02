
Source graph:
```
Z -> A  -> C
     . <-> B
```

Algorithm:
```cpp
void DFS(TState& state, std::string airport) {
    while (!state.Adj[airport].empty()) {
        auto& adj = state.Adj[airport];

        auto pos = adj.begin();
        auto next = std::move(*pos);
        adj.erase(pos);

        DFS(state, next);
    }
    state.Itinerary.push_back(airport);
}
```

Traversal:
```
->                    | ZA AB AC BA   | VOID
->  Z -> A            |    AB AC BA   | VOID
->    A -> B          |       AC BA   | VOID
->      B -> A        |       AC      | VOID
->        A -> C      | VOID          | VOID
->          C -> 0    | VOID          | VOID
<-          C (ADD)   | VOID          | C 
<-        A (ADD)     | VOID          | CA
<-      B (ADD)       | VOID          | CAB
<-    A (ADD)         | VOID          | CABA
<-  Z (ADD)           | VOID          | CABAZ
```
