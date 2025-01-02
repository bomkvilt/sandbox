# Binary search tree

***def.***  **Binary search tree** — BST — is a type of trees that applyes to specific conditions:
 -  all nodes *A* consists of a **Data** and two child nodes *L* and *R*; **Data** have a specific property 
    **Key** that should have to have defined operations '>', '<', and '=='
 -  all child nodes are BST with the same properties
 -  $\{key[A] > key[l] : l \in \{L\}\}$ — all elements from left  subtree *L* are *less*    than *A*
 -  $\{key[A] < key[r] : r \in \{R\}\}$ — all elements from rigth subtree *R* are *greater* than *A*

$$a < \pmb{B} < c < \pmb{D} < p < \pmb{e} < q < \pmb{F} < g$$
```
      d
    /   \
  b       f
 / \     / \
a   c   e   g
       / \
      p   q

1: b < d < f
2: a < b < c
3: e < f < g
...
m: {a, b, c} < d
m: {e, f, g} > d
```
*Fig. 1 — tree elemets' relation*

# Table of Contents
1. Operations
    - [Delete   ](#op-delete-common)
    - [Delete if](#op-delete-if)
    - [Delete if not betwean](#op-delete-if-not-betwean)

---
<div id='op-delete-common'/>

## Operation: Delete (Fig.1)

This operation finds a node with a complexity O(log(n)) and extracts a smallest node in a 
    rigth subtree of the found element with the same complexity O(log(n)). 
Total operation's complexity is ***O(log(n))***.
Deletion algorithms is:
- if $(b = nullptr)~||~(f = nullptr)$ : \
    $\Rightarrow d \ll b~?~b~:~f$

- if $e = nullptr$ : \
    $\Rightarrow f.L = d.L$ \
    $\Rightarrow d \ll f$

- else: \
    $\Rightarrow t = extractMin(e)$ \
    $\Rightarrow swap(d.L,~t.L$) \
    $\Rightarrow swap(d.R,~t.R$) \
    $\Rightarrow d \ll t$

Where 'a $\ll$ b' is a 'relink' operation that means that a node under the pointer $a$ is getting deleted
and the pointer start to reference a node $b$.

```cpp
Node* Delete(Node*& root, const Key& key)
{
    // try to find a node with the key
    if (!root) return root;
    if (key < root->val) return Delete(root->l, key);
    if (key > root->val) return Delete(root->r, key);
    
    // node is found
    auto* l = root->l;
    auto* r = root->r;
    if (!l || !r)
    {
        delete root;
        root = l ? l : r;
    }
    else if (!r->l)
    {
        r->l = root->l;
        delete root;
        root = r;
    }
    else
    {
        auto* t = ExtractMin(r->l); // O(log(n))
        std::swap(root->l, t->l);
        std::swap(root->r, t->r);
        delete root;
        root = t;
    }
    return root;
}
```

---
<div id='op-delete-if'/>

## Operation: Delete if (Fig.1)

This operation finds nodes that matches a check function. 
For that we need to visit all nodes and, if the node must be deletted, we need to start 
    a [key deletion algorithm](#op-delete-common) for the current node.
The combination complexity is close to be ***O(n log(n))*** with *O(log(n))* extra space.

```cpp
Node* Delete(Node*& root, std::function<bool(const Node*)> isOnDelete)
{
    if (!root) return root;
    // delete child nodes if required
    Delete(root->l, isOnDelete);
    Delete(root->r, isOnDelete);

    // delete the root if it's required
    if (isOnDelete(root))
    {
        auto* l = root->l;
        auto* r = root->r;
        if (!l || !r)
        {
            delete root;
            root = l ? l : r;
        }
        else if (!r->l)
        {
            r->l = root->l;
            delete root;
            root = r;
        }
        else
        {
            auto* t = ExtractMin(r->l); // O(log(n))
            std::swap(root->l, t->l);
            std::swap(root->r, t->r);
            delete root;
            root = t;
        }
    }
    return root;
}
```

Also, we can to cash all visited nodes in the min $\to$ max order and the nodes parents to be able to 
    detach the nodes with *O(1)* complexity. 
This variant of the algorithm will have a *O(n)* complexity and will use *O(n)* extra space to store
    (a) map of node's parents,
    (b) list of visited nodes,
    (c) DFS stack.

```cpp
using TreeNodeStack   = std::stack<TreeNode*>;
using TreeNodeParents = std::unordered_map<TreeNode*, TreeNode*>;

TreeNode* Delete(TreeNode* root, std::function<bool(const TreeNode*)> isOnDelete)
{
    auto DFS     = TreeNodeStack();
    auto DFSstep = std::stack<int>();
    auto visited = TreeNodeStack();
    auto parents = TreeNodeParents();

    auto pushNode = [&DFS, &DFSstep, &parents](TreeNode* node, TreeNode* parent)
    {
        if (node)
        {
            DFS.push(node);
            DFSstep.push(0);
            parents[node] = parent;
        }
    };

    auto anchor = TreeNode();
    anchor.right = root;
    pushNode(root, &anchor);

    while (DFS.size())
    {
        auto& node = DFS.top();
        auto& step = DFSstep.top();
        
        if (node)
        {
            switch(step++)
            {
            case 0: // visit right
                pushNode(node->right, node);
                continue;
            case 2: // visit left
                pushNode(node->left, node);
                continue;
            case 1: // visit node
                if (isOnDelete(node))
                {
                    auto* parent = parents[node];                    
                    auto*& slot  = parent->left == node ? parent->left : parent->right;                    
                    DeleteEntery(node, visited, parents);                    
                    slot = node;
                    --step;
                }
                else 
                {
                    visited.push(node);
                }
                continue;
            }
        }
        DFS.pop();
        DFSstep.pop();
    }
    return anchor.right;
}

TreeNode* DeleteEntery(TreeNode*& node, TreeNodeStack& visited, TreeNodeParents& parents) 
{
    if (!node) return node;

    auto* l = node->left;
    auto* r = node->right;
    if (!l || !r)
    {
        delete node;
        node = l ? l : r;
    }
    else
    {
        // we have already visited left subnode
        assert(visited.size() > 0);

        // detach the huge previous node (@lastNode)
        auto* lastNode = visited.top();
        auto* parent   = parents[lastNode];
        assert(!lastNode->left || !lastNode->right);
        assert(parent);

        auto* next = lastNode->left ? lastNode->left : lastNode->right;
        lastNode->left  = nullptr;
        lastNode->right = nullptr;

        assert(lastNode == parent->right);
        parent->right = next;

        // set @lastNode instead of @node
        std::swap(node->left , lastNode->left);
        std::swap(node->right, lastNode->right);
        delete node;
        node = lastNode;
    }
    return node;
}
```

---
<div id='op-delete-if-not-betwean'/>

## Operation: Delete if not between (Fig.1)

This operation should stay elements that are in range $x \in [a, b]$.
```cpp
TreeNode* trimBST(TreeNode* root, int L, int H) {
    if(!root) return root;
    root -> left = trimBST(root -> left, L, H);
    root -> right = trimBST(root -> right, L, H);
    if(root -> val < L) return root -> right;
    if(root -> val > H) return root -> left;
    return root;
}
```
