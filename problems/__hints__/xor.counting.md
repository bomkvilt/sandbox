
x = x % 3;
```cpp
x1 ^= num & ~x2;  // remove bits that where used twice
x2 ^= num & ~x1;  // remove bits that where used once
```

x = x % 4;
```cpp
y1 ^= num & ~(y2 | y3);
y2 ^= num & ~(y1 | y3);
y3 ^= num & ~(y2 | y1);
```
