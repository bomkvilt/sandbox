# strings.hashes.additive_hashing

String hashes can have additive properties.

E.g. we can define the following hash function:<p>
$ hash(s[0..n]) = (s_0 \cdot x^{n-1} + s_1 \cdot x^{n-2} + ... + s_{n-1} \cdot x^0) \% M $

Where:
(1) $x$ is an arbitrary number $\in [1, M - 1]$,
(2) $M$ is an arbitrary large prime number.

Hence,
- we can add a new element to the hash sum (*building a prefix hash vector*):<p>
  $ hash(s[0..j+1]) = (hash(s[0..j]) \cdot x + s_{j}) \% M $

- we can find a cash of an arbitrary substring (*culculating the exact hash*):<p>
  $ hash(s[i..j]) = (M + hash(s[0..j]) - hash(s[0..i-1]) \cdot x^{j - i + 1} ) \% M $

The hashes can clash if the selected x is a root of the cache polynomial:
- $ (s_0 \cdot x^{n-1} + s_1 \cdot x^{n-2} + ... + s_{n-1} \cdot x^0) \% M = 0 $
- $ s_0 \cdot x^{n-1} + s_1 \cdot x^{n-2} + ... + s_{n-1} \cdot x^0 = k \cdot M $, $ k \in Z $
