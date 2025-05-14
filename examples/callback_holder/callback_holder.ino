// examples/holder/callback_holder.ino
#include <Arduino.h>
#include <Callback.h>

// A generic handler class that stores a callback
class Handler {
public:
  Handler() = default;

  // Accept a copy of the callback instance
  void setCallback(Callback<void, int, int> &cb) {
    _cb = cb;
  }

  void run(int a, int b) {
    if (_cb.valid()) {
      Serial.println("[Handler] invoking stored callback...");
      _cb(a, b);
    }
  }

private:
  Callback<void, int, int> _cb;
};

// Example: external context + callback passed into handler
struct Ctx {
  int offset = 5;
};

void callbackFn(Ctx* ctx, int a, int b) {
  Serial.print("[CallbackFn] result = ");
  Serial.println(ctx->offset + a + b);
}

Ctx ctx;
Handler handler;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("\n[callback_holder.ino] Passing callback into handler class");

  Callback<void, int, int> cb(&ctx, &callbackFn);
  handler.setCallback(cb);
  handler.run(2, 3);
}

void loop() {
  // no-op
}

