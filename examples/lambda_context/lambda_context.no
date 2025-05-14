// examples/lambda_context/lambda_context.ino
#include <Arduino.h>
#include <Callback.h>

struct Env {
  int multiplier = 2;
};

Env env;
Callback<void, int, int> cb;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("\n[lambda_context.ino] Lambda with external context");

  cb = Callback<void, int, int>(&env, +[](Env* ctx, int a, int b) {
    Serial.print("[lambda+context] (a + b) * ctx->multiplier = ");
    Serial.println((a + b) * ctx->multiplier);
  });

  cb(2, 3);
}

void loop() {
  // no-op
}
