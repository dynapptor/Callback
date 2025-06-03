#include <Arduino.h>
#include <Callback.h>

struct Device {
  void toggle(int a, int b) {
    Serial.print("[Device] toggle: ");
    Serial.println(a + b);
  }
};

Device dev;
Callback<void, int, int> cb;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("\n[member.ino] Callback example with member function");

  cb = Callback<void, int, int>(dev, &Device::toggle);
  cb(2, 3); // using operator()
}

void loop() {
  // no-op
}

