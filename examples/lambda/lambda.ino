// examples/lambda/lambda.ino
#include <Arduino.h>
#include <Callback.h>

Callback<void, int, int> cb;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("\n[lambda.ino] Non-capturing lambda as callback");

  cb = Callback<void, int, int>(+[](int a, int b) {
    Serial.print("[lambda] a - b = ");
    Serial.println(a - b);
  });

  cb(8, 3);
}

void loop() {
  // no-op
}
