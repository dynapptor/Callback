// Callback.tpp
#ifndef CALLBACK_TPP
#define CALLBACK_TPP

#include "Callback.h"

template <typename RET, typename... ARGS>
Callback<RET, ARGS...>::Callback() {
  clear();
}

template <typename RET, typename... ARGS>
Callback<RET, ARGS...>::Callback(FunctionType fn) {
  attach(fn);
}

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

template <typename RET, typename... ARGS>
void Callback<RET, ARGS...>::attach(FunctionType fn) {
  _p.function = fn;
  _membercaller = nullptr;
  _contextFn = nullptr;
  _contextedMemberCaller = nullptr;
  _context = nullptr;
}

template <typename RET, typename... ARGS>
template <typename TContext>
void Callback<RET, ARGS...>::attach(TContext* ctx, RET (*fn)(TContext*, ARGS...)) {
  _context = static_cast<void*>(ctx);
  _contextFn = reinterpret_cast<RET (*)(void*, ARGS...)>(fn);
  _membercaller = nullptr;
  _contextedMemberCaller = nullptr;
}

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

template <typename RET, typename... ARGS>
RET Callback<RET, ARGS...>::invoke(ARGS... args) const {
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
RET Callback<RET, ARGS...>::operator()(ARGS... args) const {
  return invoke(args...);
}

template <typename RET, typename... ARGS>
template <typename TObj>
RET Callback<RET, ARGS...>::membercaller(void* object, const uintptr_t* member, ARGS... args) {
  TObj* o = static_cast<TObj*>(object);
  // RET (TObj::* * m)(ARGS...) = reinterpret_cast<RET (TObj::**)(ARGS...)>(member);
  // return (o->**m)(args...);
  using MemberFnPtr = RET (TObj::*)(ARGS...);
  const MemberFnPtr* m = reinterpret_cast<const MemberFnPtr*>(member);
  return (o->**m)(args...);
}

template <typename RET, typename... ARGS>
template <typename TContext, typename TObj>
RET Callback<RET, ARGS...>::contextedMemberCaller(void* object, void* context, const uintptr_t* member, ARGS... args) {
  TObj* o = static_cast<TObj*>(object);
  TContext* c = static_cast<TContext*>(context);
  // RET (TObj::* * m)(TContext*, ARGS...) = reinterpret_cast<RET (TObj::**)(TContext*, ARGS...)>(member);
  // return (o->**m)(c, args...);
  using MemberFnPtr = RET (TObj::*)(TContext*, ARGS...);                // Define the member function pointer type
  const MemberFnPtr* m = reinterpret_cast<const MemberFnPtr*>(member);  // Preserve const
  return (o->**m)(c, args...);
}

template <typename RET, typename... ARGS>
template <typename TContext>
TContext* Callback<RET, ARGS...>::context() const {
  return static_cast<TContext*>(_context);
}

template <typename RET, typename... ARGS>
bool Callback<RET, ARGS...>::valid() const {
  return _contextedMemberCaller || _contextFn || _membercaller || _p.function;
}

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