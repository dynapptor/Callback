// Callback.tpp
#ifndef CALLBACK_TPP
#define CALLBACK_TPP

#include "Callback.h"
// Default constructor
template <typename RET, typename... ARGS>
Callback<RET, ARGS...>::Callback() {
  clear();
}

// Constructor: static function
template <typename RET, typename... ARGS>
Callback<RET, ARGS...>::Callback(FunctionType fn) {
  attach(fn);
}

// Constructor: static function with context
template <typename RET, typename... ARGS>
template <typename TContext>
Callback<RET, ARGS...>::Callback(TContext* ctx, RET (*fn)(TContext*, ARGS...)) {
  attach(ctx, fn);
}

template <typename RET, typename... ARGS>
template <typename TContext>
Callback<RET, ARGS...>::Callback(TContext& ctx, RET (*fn)(TContext*, ARGS...)) {
  attach(&ctx, fn);
}

// Constructor: member function
template <typename RET, typename... ARGS>
template <typename TObj>
Callback<RET, ARGS...>::Callback(TObj* obj, RET (TObj::*fn)(ARGS...)) {
  attach(obj, fn);
}

template <typename RET, typename... ARGS>
template <typename TObj>
Callback<RET, ARGS...>::Callback(TObj& obj, RET (TObj::*fn)(ARGS...)) {
  attach(&obj, fn);
}

// Constructor: member function with context
template <typename RET, typename... ARGS>
template <typename TContext, typename TObj>
Callback<RET, ARGS...>::Callback(TContext* ctx, TObj* obj, RET (TObj::*fn)(TContext*, ARGS...)) {
  attach(ctx, obj, fn);
}

template <typename RET, typename... ARGS>
template <typename TContext, typename TObj>
Callback<RET, ARGS...>::Callback(TContext& ctx, TObj& obj, RET (TObj::*fn)(TContext*, ARGS...)) {
  attach(&ctx, &obj, fn);
}

// Static function attach
template <typename RET, typename... ARGS>
void Callback<RET, ARGS...>::attach(FunctionType fn) {
  _p.function = fn;
  _membercaller = nullptr;
  _contextFn = nullptr;
  _contextedMemberCaller = nullptr;
  _context = nullptr;
}

// Static function with context attach
template <typename RET, typename... ARGS>
template <typename TContext>
void Callback<RET, ARGS...>::attach(TContext* ctx, RET (*fn)(TContext*, ARGS...)) {
  _context = static_cast<void*>(ctx);
  _contextFn = reinterpret_cast<RET (*)(void*, ARGS...)>(fn);
  _membercaller = nullptr;
  _contextedMemberCaller = nullptr;
}

// Member function attach
template <typename RET, typename... ARGS>
template <typename TObj>
void Callback<RET, ARGS...>::attach(TObj* obj, RET (TObj::*fn)(ARGS...)) {
  _p.object = static_cast<void*>(obj);
  *reinterpret_cast<RET (TObj::**)(ARGS...)>(_member) = fn;
  _membercaller = &Callback::membercaller<TObj>;
  _contextFn = nullptr;
  _contextedMemberCaller = nullptr;
  _context = nullptr;
}

// Member function with context attach
template <typename RET, typename... ARGS>
template <typename TContext, typename TObj>
void Callback<RET, ARGS...>::attach(TContext* ctx, TObj* obj, RET (TObj::*fn)(TContext*, ARGS...)) {
  _context = static_cast<void*>(ctx);
  _p.object = static_cast<void*>(obj);
  *reinterpret_cast<RET (TObj::**)(TContext*, ARGS...)>(_member) = fn;
  _contextedMemberCaller = &Callback::contextedMemberCaller<TContext, TObj>;
  _membercaller = nullptr;
  _contextFn = nullptr;
}

// Invoke dispatcher
template <typename RET, typename... ARGS>
RET Callback<RET, ARGS...>::invoke(ARGS... args) {
  if (_contextedMemberCaller && _context && _p.object)
    return _contextedMemberCaller(_p.object, _context, _member, args...);
  if (_contextFn && _context)
    return _contextFn(_context, args...);
  if (_membercaller && _p.object)
    return _membercaller(_p.object, _member, args...);
  if (_p.function)
    return _p.function(args...);
  return RET();
}

template <typename RET, typename... ARGS>
RET Callback<RET, ARGS...>::operator()(ARGS... args) {
  return invoke(args...);
}

// Member dispatcher
template <typename RET, typename... ARGS>
template <typename TObj>
RET Callback<RET, ARGS...>::membercaller(void* object, uintptr_t* member, ARGS... args) {
  TObj* o = static_cast<TObj*>(object);
  RET (TObj::* * m)(ARGS...) = reinterpret_cast<RET (TObj::**)(ARGS...)>(member);
  return (o->**m)(args...);
}

// Contexted member dispatcher
template <typename RET, typename... ARGS>
template <typename TContext, typename TObj>
RET Callback<RET, ARGS...>::contextedMemberCaller(void* object, void* context, uintptr_t* member, ARGS... args) {
  TObj* o = static_cast<TObj*>(object);
  TContext* c = static_cast<TContext*>(context);
  RET (TObj::* * m)(TContext*, ARGS...) = reinterpret_cast<RET (TObj::**)(TContext*, ARGS...)>(member);
  return (o->**m)(c, args...);
}

// Retrieve context pointer
template <typename RET, typename... ARGS>
template <typename TContext>
TContext* Callback<RET, ARGS...>::context() const {
  return static_cast<TContext*>(_context);
}

// Check for validity
template <typename RET, typename... ARGS>
bool Callback<RET, ARGS...>::valid() const {
  return _contextedMemberCaller || _contextFn || _membercaller || _p.function;
}

// set empty stae
template <typename RET, typename... ARGS>
void Callback<RET, ARGS...>::clear() {
  memset(_member, 0, sizeof(_member));
  _p.function = nullptr;
  _membercaller = nullptr;
  _contextFn = nullptr;
  _contextedMemberCaller = nullptr;
  _context = nullptr;
}

#endif