# 0-1 Knapsack Problem

This is a one of the most popular dynamic programming problems.
The problem general deffintion is the following.

We have **n** objects and we whant to choose part of them. In a vector form:
- we have a vector of values  **v = [v<sub>0</sub>, v<sub>1</sub>, ..., v<sub>n-1</sub>]**
- we have a vector of weigths **w = [w<sub>0</sub>, w<sub>1</sub>, ..., w<sub>n-1</sub>]**
- we have a vector of peeks   **x = [x<sub>0</sub>, x<sub>1</sub>, ..., x<sub>n-1</sub>]** 
    were each of $x_j \in \{0, 1\}$
- we have a maximum weigth **W** can carry

And we need to apply two conditions:
- **v * x -> max**
- **w * x <= W**

---
## Brute force solution

We can see that we have a peek vector $x$ of size $n$. 
It means we need to process all **2<sup>n</sup>** variants of fector x to unswer a faced problem.

Let's look at an example. The two parameters indicated in the following recursion tree are n  and W. The recursion tree is for following sample inputs. w[] = {1, 1, 1}, W = 2, val[] = {10, 20, 30}
```
                                K(n, W)
                                K(3, 2)  
                        /                        \ 
                /                                     \               
            K(2, 2)                                 K(2, 1)
          /       \                              /          \ 
        /           \                         /                \
       K(1, 2)      K(1, 1)               K(1, 1)              K(1, 0)
       /  \         /   \               /        \           /        \     
     /      \     /       \           /            \       /            \   
K(0, 2)  K(0, 1)  K(0, 1)  K(0, 0)  K(0, 1)     K(0, 0)  K(0, 0)     K(0, -1)
```
It's clear that part of equvalent nodes was visited several times /*nodes 1,1 with subnodes*/ and part of nodes is invalid /*0,0 and 0,-1*/ since their's parent nodes reached problem's conditions or became invalid. So, it means that we can to optimize problem's complexity with cashing of node visits' values.

---
## Table solution

Let's add an addtional asumption that all values and weigths are positive
$$
    v_j \ge 0 \\
    w_j \ge 0 \\
    W_0 \ge 0 \\
$$

The assumption allows to introduce a new entity — $m[w]$ — maximum value of a knapsac with a capacity $w \le W$. The value can be found with use of requrent 
```
                |                    W              |
/-----------x---x-----------------------------------|
|   v   w   | i |   0   1   2   3   4   5   6   7   |
|-----------x---x-----------------------------------|
|   -   -   | - |   0   0   0   0   0   0   0   0   |
|   2   3   | 0 |   0   0   0   2   2   2   2   2   |
|   2   1   | 1 |                                   |
|   4   3   | 2 |                                   |
|   5   4   | 3 |                                   |
|   3   2   | 4 |                                   |
\---------------------------------------------------/
```

On each iteration cell $C_{i,W}$ is being calculated as $C_{i,W} = max()$
