# API Documentation: Callback

## Overview
`Callback<RET, ARGS...>` is a lightweight, flexible, heap-free C++ callback wrapper for embedded and resource-constrained systems.

Supports:
- Free/static functions
- Member functions
- Non-capturing lambdas
- Context-passing to functions or member methods

---

## Class Template
```cpp
template <typename RET, typename... ARGS>
class Callback;
```

### Typedefs
```cpp
using FunctionType = RET (*)(ARGS...);
```

---

## Constructors

### Free/static function
```cpp
explicit Callback(FunctionType fn);
```

### Static function with context
```cpp
template <typename TContext>
Callback(TContext* ctx, RET (*fn)(TContext*, ARGS...));
Callback(TContext& ctx, RET (*fn)(TContext*, ARGS...));
```

### Member function
```cpp
template <typename TObj>
Callback(TObj* obj, RET (TObj::*fn)(ARGS...));
Callback(TObj& obj, RET (TObj::*fn)(ARGS...));
```

### Member function with context
```cpp
template <typename TContext, typename TObj>
Callback(TContext* ctx, TObj* obj, RET (TObj::*fn)(TContext*, ARGS...));
Callback(TContext& ctx, TObj& obj, RET (TObj::*fn)(TContext*, ARGS...));
```

---

## Methods

### attach
Rebinds the callback target after construction.
```cpp
void attach(FunctionType fn);

// with context:
template <typename TContext>
void attach(TContext* ctx, RET (*fn)(TContext*, ARGS...));

// member:
template <typename TObj>
void attach(TObj* obj, RET (TObj::*fn)(ARGS...));

// member with context:
template <typename TContext, typename TObj>
void attach(TContext* ctx, TObj* obj, RET (TObj::*fn)(TContext*, ARGS...));
```

### invoke
Invokes the stored callback.
```cpp
RET invoke(ARGS... args);
```

### context
Retrieves context as specific type.
```cpp
template <typename TContext>
TContext* context() const;
```

### valid
Checks if the callback is valid.
```cpp
bool valid() const;
```

---

## Notes
- `RET()` is returned by default if no function is bound.
- No heap allocations or `std::function` used.
- Memory footprint: ~16–20 bytes per instance.

---

## License
MIT License

