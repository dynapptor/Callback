# Callback

A lightweight, heap-free, type-safe C++ callback system for embedded and resource-constrained environments (e.g. Arduino Mega, ATtiny85).

Supports:
- Free/static functions
- Non-capturing lambdas
- Member functions
- Context-passing to static/member functions

## 🔧 Features
- No `std::function`, no heap allocation
- Works with static/lambdas/member methods
- Easily integrated in bare-metal environments
- Compile-time type safety with runtime flexibility

## 📦 Installation

```bash
git clone https://github.com/youruser/callback.git
```

Place `src/Callback.h` and `src/Callback.tpp` in your `lib/` folder or install via PlatformIO once available in the registry.

## 💡 Usage Examples

### Free Function
```cpp
Callback<void, int> cb(&myFreeFunction);
cb.invoke(42);
```

### Member Function
```cpp
MyClass obj;
Callback<void, int> cb(obj, &MyClass::myMethod);
cb.invoke(7);
```

### Member + Context
```cpp
Context ctx;
MyClass obj;
Callback<void, int> cb(ctx, obj, &MyClass::methodWithContext);
cb.invoke(3);
```

## 📁 Directory Structure

```
.
├── src/
│   ├── Callback.h
│   └── Callback.tpp
├── examples/
│   ├── member/
│   ├── static/
│   ├── context/
│   ├── member_context/
│   ├── lambda/
│   ├── lambda_context/
│   ├── dispatcher/
│   └── holder/
├── doc/
│   ├── readme.md
│   └── api.md
├── LICENSE.txt
├── README.adoc
├── library.properties
└── keywords.txt
```

## 📝 License
MIT License - see `LICENSE.txt`
