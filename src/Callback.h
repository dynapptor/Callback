// Callback.h
#ifndef CALLBACK_H
#define CALLBACK_H

#include <stdint.h>
#include <string.h>

/**
 * @file Callback.h
 * @brief A flexible, type-safe, heap-free callback mechanism for embedded and low-resource C++ environments.
 *
 * The Callback class provides a generic, type-safe way to store and invoke callbacks, supporting:
 * - Free functions
 * - Non-capturing lambdas
 * - Member functions
 * - Context-passing to static or member functions
 *
 * This implementation is designed for embedded systems, avoiding dynamic memory allocation and ensuring minimal overhead.
 */

/**
 * @class Callback
 * @brief A template class for managing type-safe callbacks with various function types.
 *
 * @tparam RET The return type of the callback function.
 * @tparam ARGS The parameter types of the callback function.
 *
 * The Callback class supports multiple callback types, including free functions, static functions with context,
 * member functions, and member functions with context. It provides methods to attach, invoke, and manage callbacks.
 */
template <typename RET, typename... ARGS>
class Callback {
 public:
  /// @brief Type alias for a free function pointer with the specified return and argument types.
  using FunctionType = RET (*)(ARGS...);

  /**
   * @brief Default constructor. Initializes an empty callback.
   */
  Callback();

  /**
   * @brief Constructs a callback from a free/static function.
   * @param fn Pointer to the free/static function.
   */
  explicit Callback(FunctionType fn);

  /**
   * @brief Constructs a callback from a static function with an external context.
   * @tparam TContext The type of the context.
   * @param ctx Pointer to the context object.
   * @param fn Pointer to the static function that takes a context and arguments.
   */
  template <typename TContext>
  Callback(TContext* ctx, RET (*fn)(TContext*, ARGS...));

  /**
   * @brief Constructs a callback from a static function with an external context (reference version).
   * @tparam TContext The type of the context.
   * @param ctx Reference to the context object.
   * @param fn Pointer to the static function that takes a context and arguments.
   */
  template <typename TContext>
  Callback(TContext& ctx, RET (*fn)(TContext*, ARGS...));

  /**
   * @brief Constructs a callback from a member function.
   * @tparam TObj The type of the object containing the member function.
   * @param obj Pointer to the object instance.
   * @param fn Pointer to the member function.
   */
  template <typename TObj>
  Callback(TObj* obj, RET (TObj::*fn)(ARGS...));

  /**
   * @brief Constructs a callback from a member function (reference version).
   * @tparam TObj The type of the object containing the member function.
   * @param obj Reference to the object instance.
   * @param fn Pointer to the member function.
   */
  template <typename TObj>
  Callback(TObj& obj, RET (TObj::*fn)(ARGS...));

  /**
   * @brief Constructs a callback from a member function with context.
   * @tparam TContext The type of the context.
   * @tparam TObj The type of the object containing the member function.
   * @param ctx Pointer to the context object.
   * @param obj Pointer to the object instance.
   * @param fn Pointer to the member function that takes a context and arguments.
   */
  template <typename TContext, typename TObj>
  Callback(TContext* ctx, TObj* obj, RET (TObj::*fn)(TContext*, ARGS...));

  /**
   * @brief Constructs a callback from a member function with context (reference version).
   * @tparam TContext The type of the context.
   * @tparam TObj The type of the object containing the member function.
   * @param ctx Reference to the context object.
   * @param obj Reference to the object instance.
   * @param fn Pointer to the member function that takes a context and arguments.
   */
  template <typename TContext, typename TObj>
  Callback(TContext& ctx, TObj& obj, RET (TObj::*fn)(TContext*, ARGS...));

  /**
   * @brief Attaches a free/static function to the callback.
   * @param fn Pointer to the free/static function.
   */
  void attach(FunctionType fn);

  /**
   * @brief Attaches a static function with an external context to the callback.
   * @tparam TContext The type of the context.
   * @param ctx Pointer to the context object.
   * @param fn Pointer to the static function that takes a context and arguments.
   */
  template <typename TContext>
  void attach(TContext* ctx, RET (*fn)(TContext*, ARGS...));

  /**
   * @brief Attaches a member function to the callback.
   * @tparam TObj The type of the object containing the member function.
   * @param obj Pointer to the object instance.
   * @param fn Pointer to the member function.
   */
  template <typename TObj>
  void attach(TObj* obj, RET (TObj::*fn)(ARGS...));

  /**
   * @brief Attaches a member function with context to the callback.
   * @tparam TContext The type of the context.
   * @tparam TObj The type of the object containing the member function.
   * @param ctx Pointer to the context object.
   * @param obj Pointer to the object instance.
   * @param fn Pointer to the member function that takes a context and arguments.
   */
  template <typename TContext, typename TObj>
  void attach(TContext* ctx, TObj* obj, RET (TObj::*fn)(TContext*, ARGS...));

  /**
   * @brief Invokes the stored callback with the provided arguments.
   * @param args Arguments to pass to the callback function.
   * @return The return value of the callback, or default-constructed RET if no valid callback is set.
   */
  RET invoke(ARGS... args) const;

  /**
   * @brief Function call operator to invoke the callback.
   * @param args Arguments to pass to the callback function.
   * @return The return value of the callback, or default-constructed RET if no valid callback is set.
   */
  RET operator()(ARGS... args) const;

  /**
   * @brief Retrieves the stored context as a specific type.
   * @tparam TContext The type of the context.
   * @return Pointer to the context object, or nullptr if no context is set or type is incorrect.
   */
  template <typename TContext>
  TContext* context() const;

  /**
   * @brief Checks if the callback is assigned and valid.
   * @return True if a valid callback is set, false otherwise.
   */
  bool valid() const;

  /**
   * @brief Clears the callback, resetting it to an empty state.
   */
  void clear();

 private:
  /**
   * @brief Helper function to dispatch calls to member functions.
   * @tparam TObj The type of the object containing the member function.
   * @param object Pointer to the object instance.
   * @param member Pointer to the member function.
   * @param args Arguments to pass to the member function.
   * @return The return value of the member function.
   */
  template <typename TObj>
  static RET membercaller(void* object, const uintptr_t* member, ARGS... args);

  /**
   * @brief Helper function to dispatch calls to member functions with context.
   * @tparam TContext The type of the context.
   * @tparam TObj The type of the object containing the member function.
   * @param object Pointer to the object instance.
   * @param context Pointer to the context object.
   * @param member Pointer to the member function.
   * @param args Arguments to pass to the member function.
   * @return The return value of the member function.
   */
  template <typename TContext, typename TObj>
  static RET contextedMemberCaller(void* object, void* context, const uintptr_t* member, ARGS... args);

  /// @brief Union to store either a function pointer or an object pointer.
  union {
    FunctionType function;  ///< Pointer to a free/static function.
    void* object;           ///< Pointer to an object instance for member functions.
  } _p;

  uintptr_t _member[2];                                                    ///< Storage for member function pointer.
  RET (*_membercaller)(void*, const uintptr_t*, ARGS...);                  ///< Dispatcher for member functions.
  RET (*_contextFn)(void*, ARGS...);                                       ///< Dispatcher for static functions with context.
  RET (*_contextedMemberCaller)(void*, void*, const uintptr_t*, ARGS...);  ///< Dispatcher for member functions with context.
  void* _context;                                                          ///< Pointer to the context object.
};

#include "Callback.tpp"

#endif  // CALLBACK_H