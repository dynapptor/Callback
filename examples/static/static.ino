// examples/static/static.ino
#include <Arduino.h>
#include <Callback.h>

void printSum(int a, int b) {
  Serial.print("[static] sum: ");
  Serial.println(a + b);
}

Callback<void, int, int> cb;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("\n[static.ino] Callback example with static function");

  cb = Callback<void, int, int>(&printSum);
  cb(10, 5);
}

void loop() {
  // no-op
}
