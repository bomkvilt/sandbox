# comprehension for macro

The sketch was inspired by the [video](<https://www.youtube.com/watch?v=SMCRQj9Hbx8&t=2016s>).
Reference Comprehensions' gramary can be found [here](<https://cpython3.readthedocs.io/en/latest/reference/grammar.html>).

```txt
# Comprehensions & Generators
# ---------------------------

for_if_clauses:
    | for_if_clause+ 

for_if_clause:
    | ASYNC 'for' star_targets 'in' ~ disjunction ('if' disjunction )* 
    | 'for' star_targets 'in' ~ disjunction ('if' disjunction )* 
listcomp:
    | '[' named_expression for_if_clauses ']' 
setcomp:
    | '{' named_expression for_if_clauses '}' 
genexp:
    | '(' ( assigment_expression | expression !':=') for_if_clauses ')' 
dictcomp:
    | '{' kvpair for_if_clauses '}' 
```

## Summary

+ procedural functional macros are studied
+ rust-flavored iterators are tasted on practice; C++ iterators seem more useful, Python iterators are more obvious and useful
