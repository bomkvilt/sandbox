## Case study: c++20 (+) coroutines && c++20 modules

Good lectures:
- https://www.youtube.com/watch?v=mDajl0pIUjQ
- https://www.youtube.com/watch?v=WZhxMwKaXmw

Articles:
- how to build an effitient generator: https://habr.com/ru/articles/754314/

-----

Function public inteface decomposition:
- `call`   = `SUSPEND`   + `ACTIVATE` (by caller)
- `return` = `TERMINATE` + `YIELD`    (by callee)

The component can produce two additional combinations that make sense:
- `pause`  = `SUSPEND` + `YIELD` (by caller)
- `resume` = `SUSPEND` + `YIELD` (by callee)

Couroutines can be classified as:
- by suspend mode:
    - `symmetric`: TODO:
    - `asymmetric`: TODO:
- by data localisation:
    - `stackful`: new coroutine per each coroutine
    - `stackless`: coroutine saves it's state in a struct-like frame

C++ objects must realize the methods: (TODO: is there the formalized c++ concept?)
- requred:
    - `auto get_return_object()`:
    - `auto initial_suspend()`:
    - `auto final_suspend() noexcept`:
    - `void unhandled_exception()`:
- generators:
    - `auto yield_value(T value)`:
    - `void return_void()`:
- TODO:
    - `return_value()`:
