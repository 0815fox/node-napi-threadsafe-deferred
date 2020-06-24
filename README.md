# node-napi-threadsafe-deferred
A thread-safe variant of a Napi-Promise which can be resolved from any thread.

In case you stumble across the issue, that you tried to resolve or reject a promise from a thread different from the main javascript thread, this module is for you. It provides you the class `ThreadSafeDeferred` which is a replacement class for `Napi::Promise::Deferred`. It uses `Napi::ThreadSafeFunction`'s release callback (which is called from the main thread) to resolve or reject the promise from there.

## Installation

1. Install
```
npm i napi-threadsafe-deferred
```
2. Then add it to your `binding.gyp`
```
    ...
    "include_dirs": [
        ...
        "<!@(node -p \"require('napi-threadsafe-deferred').include\")"
        ...
    ],
    "dependencies": [
        ...
        "<!(node -p \"require('napi-threadsafe-deferred').gyp\")"
        ...
    ]
    ...
```

## Usage

1. Include `ThreadSafeDeferred.hpp`.
2. In a function called from JavaScript create an instance by calling the constructor and save a reference or pointer to it, e.g.:
```
Napi::Value myFunctionBoundToJS(const Napi::CallbackInfo & info) {
    auto myDeferred = new ThreadSafeDeferred(Env());
    // -> store a pointer or reference to the deferred somewhere
    return myDeferred->Promise();
}
```
3. Call `Resolve` or `Reject` from any thread you like.

### Promise resolution

You can resolve a promise without a value:
```
void functionCalledFromAnotherThread() {
    ...
    myDeferred.Reslove();
}
```

...with a value:
```
void functionCalledFromAnotherThread() {
    ...
    myDeferred.Reslove(THREADSAFE_DEFERRED_RESOLVER(Napi::Number::New(env, 42)));
}
```

Resolving with a value is done by passing a lambda call back which will be called from the main thread during promise resolution. `THREADSAFE_DEFERRED_RESOLVER` makes the symbols available as copy (`[=]`)

You can manually pass a lambda, if you need more control over the symbols or if you want to use the callback to release memory:

```
void functionCalledFromAnotherThread() {
    ...
    myDeferred.Reslove([&something]{
        delete something;
        return Napi::Number::New(env, 42);
    });
}
```

### Promise rejection

You can reject a promise using the `Reject` function without parameter or with a string, which is used as reason for the exception on which the rejection is based:
```
void functionCalledFromAnotherThread() {
    ...
    myDeferred.Reject("Gime gime gime - you tried to invoke man after midnight.");
    ...
}
```

## Features and Caveats

As the resolution of the `ThreadSafeDeferred` is handled asynchronously - the object will - in case it resides on heap - delete itself after the underlying `Napi::Promise::Deferred` has been resolved. As the object is still needed after your call to `Resolve` or `Reject` you must not delete it yourself.


