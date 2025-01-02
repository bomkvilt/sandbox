# strings.search

## hashing

```
s: [0][1][2][3][4][5][6][7][8][9]...[n]
p: [a][b][a]
```

We can use an [additive string hasing](<hashes.additive_hashing.md>) to
- calc a hash of the pattern string `p`
- slide over the search string `s` by a window `w` of size `p.size()`
- if the hash(`w`) == hash(`p`), we **can** check that the substrings matches

The search algorithm's coplecity:

- deterenistic modification (with candidates checking)

  |       | avg      | worst |
  |:---   |:---      |:---   |
  | space | O(1)     | O(1)  |
  | time  | O(n + m) | O(n + nm) -> O(nm) |

- non-detrmenistic modification (with no checking; can lead false matches)

  |       | avg      | worst |
  |:---   |:---      |:---   |
  | space | O(1)     | O(1)  |
  | time  | O(n + m) | O(n)  |


## Knuth–Morris–Pratt algorithm

```
s: [0][1][2][3][4][5][6][7][8][9]...[n]
p: [a][b][a]
t: [0][0][a]
```

def. pref(s) — is the maximum substring x that's both the prefix of the `s` and the suffix of the `s`.

$ pref(x) = max X: x = prefix(s)\ and\ x = suffix(s)\ and\ x != s $

```
s: [a][b][c][ ]...[ ][a][b][c]
   ^^^^^^^^^         ^^^^^^^^^
   x                 x
```
- **NOTE**: $ pref("abaaba") = "aba" $
- **NOTE**: $ pref("abba") = "a" $
- **NOTE**: $ pref("aabb") = "" $
- **NOTE**: $ pref("") = N/A $


def. longest prefix suffix (LPS) — is an array of the longest prefixes $t[i] = p(i + 1) = pref(s[0..i]).size()$.
```
 s: t:
[a][0] pref( a ) = 0
[b][0] pref( ab ) = 0
[a][1] pref(a b a) = 1
[c][0] pref( abac ) = 0
[a][1] pref(a bac a) = 1
[b][2] pref(ab ac ab) = 2
[a][3] pref(aba c aba) = 3
```

**NOTE**: `p(i)` should be interpreted as the "prefix of the substring of length `i`".


Let's assume that the part `t[0...i-1]` the array is known.
It means that we know all the `p(k): k = [1, i]`.

```
i:  0  1  2  3  4 ...i-1 i i+1...n-1 n
s: [X][X][X][X][X]...[X][X][X]...[X][X]
t: [X][X][X][X][X]...[X][ ][ ]...[ ][ ]
   ^^^^^^^^^^^^^^^^^^^^^
   the part is known
```

When we want to find the `t[i] = pref(s[0...i]) = p(i + 1)`.

```
i:  0     2     4    i-2    i    ...    n
       1     3    ...   i-1   i+1   n-1
s: [X][X][X][X][X]...[X][X][X][X]...[X][X]
s: [       ][a]   ...[       ][b]
   ^^^^^^^^^         ^^^^^^^^^
   p(i)              p(i)

if (a == b):
  # our bet payed off
  p(i+1) = p(i) + 1  |  t[i] = t[i-1] + 1

else:
  # we need to try to use a shorter substring

s: [p(i)        ][a]...[p(i)        ][x] - first approach (did not matched)
   ^ pick a prefix    pick a suffix ^
   * the rows are the same
   * hence, we need to pick a p(p(i))
   * repeat untill the prefix is not ''

```

The prefix function leads a substring search properties.
- ```
  [w][w][w][w] # [s][s][s][s][s][s][s][s][s][s]
  ^ word         ^ string
  ```
- we can start building a prefix function over the joint word + string
- if the p(i) == word.size(), we have found the substring

The search algorithm could be modified to:
- find all the (non-intersecting) matches
- find the [longest repeating substring](<../../problems/leetcode/solutions/dp - cashing & prefix summ/search/1668. Maximum Repeating Substring (KMP - longest prefix suffix).cpp>)
