// examples/context/context.ino
#include <Arduino.h>
#include <Callback.h>

struct Ctx {
  int base = 10;
};

void printWithBase(Ctx* ctx, int a, int b) {
  Serial.print("[context] base + a + b = ");
  Serial.println(ctx->base + a + b);
}

Ctx context;
Callback<void, int, int> cb;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("\n[context.ino] Callback with external context");

  cb = Callback<void, int, int>(context, &printWithBase);
  cb(3, 7);
}

void loop() {
  // no-op
}
