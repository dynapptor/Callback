// examples/member_context/member_context.ino
#include <Arduino.h>
#include <Callback.h>

struct Config {
  int factor = 3;
};

struct Processor {
  void compute(Config* ctx, int a, int b) {
    Serial.print("[member+context] ctx->factor * (a + b) = ");
    Serial.println(ctx->factor * (a + b));
  }
};

Config cfg;
Processor proc;
Callback<void, int, int> cb;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("\n[member_context.ino] Member function with context");

  cb = Callback<void, int, int>(cfg, proc, &Processor::compute);
  cb(4, 5);
}

void loop() {
  // no-op
}

