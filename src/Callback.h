// Callback.h
#ifndef CALLBACK_H
#define CALLBACK_H

#include <stdint.h>
#include <string.h>

// MIT License
// Copyright (c) 2025 YourName
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// A flexible, type-safe and heap-free callback mechanism for embedded and low-resource C++ environments.
// Supports:
// - Free functions
// - Lambdas (non-capturing)
// - Member functions
// - Context-passing to static or member functions

template <typename RET, typename... ARGS>
class Callback {
 public:
  using FunctionType = RET (*)(ARGS...);

  Callback();

  // Constructor: free/static function
  explicit Callback(FunctionType fn);

  // Constructor: static function with external context
  template <typename TContext>
  Callback(TContext* ctx, RET (*fn)(TContext*, ARGS...));

  template <typename TContext>
  Callback(TContext& ctx, RET (*fn)(TContext*, ARGS...));

  // Constructor: member function
  template <typename TObj>
  Callback(TObj* obj, RET (TObj::*fn)(ARGS...));

  template <typename TObj>
  Callback(TObj& obj, RET (TObj::*fn)(ARGS...));

  // Constructor: member function with context passed to method
  template <typename TContext, typename TObj>
  Callback(TContext* ctx, TObj* obj, RET (TObj::*fn)(TContext*, ARGS...));

  template <typename TContext, typename TObj>
  Callback(TContext& ctx, TObj& obj, RET (TObj::*fn)(TContext*, ARGS...));

  // Attach overloads
  void attach(FunctionType fn);

  template <typename TContext>
  void attach(TContext* ctx, RET (*fn)(TContext*, ARGS...));

  template <typename TObj>
  void attach(TObj* obj, RET (TObj::*fn)(ARGS...));

  template <typename TContext, typename TObj>
  void attach(TContext* ctx, TObj* obj, RET (TObj::*fn)(TContext*, ARGS...));

  // Invoke the stored callback, if valid
  RET invoke(ARGS... args);

  RET operator()(ARGS... args);
  
  // Retrieve context as specific type
  template <typename TContext>
  TContext* context() const;

  // Check if callback is assigned and valid
  bool valid() const;

  // Set empty state
  void clear();

 private:
  // Member dispatcher helper
  template <typename TObj>
  static RET membercaller(void* object, uintptr_t* member, ARGS... args);

  // Member dispatcher helper with context
  template <typename TContext, typename TObj>
  static RET contextedMemberCaller(void* object, void* context, uintptr_t* member, ARGS... args);

  union {
    FunctionType function;
    void* object;
  } _p;

  uintptr_t _member[2];
  RET (*_membercaller)(void*, uintptr_t*, ARGS...) = nullptr;
  RET (*_contextFn)(void*, ARGS...) = nullptr;
  RET (*_contextedMemberCaller)(void*, void*, uintptr_t*, ARGS...) = nullptr;
  void* _context = nullptr;
};

#include "Callback.tpp"

#endif  // CALLBACK_H
