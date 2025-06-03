// examples/dispatcher/event_dispatcher.ino
#include <Arduino.h>
#include <Callback.h>

// Dispatcher that can register and invoke multiple callbacks
class EventDispatcher {
public:
  static constexpr size_t MAX_CALLBACKS = 4;

  void add(const Callback<void, int>& cb) {
    if (count < MAX_CALLBACKS) {
      callbacks[count++] = cb;
    }
  }

  void dispatch(int value) {
    Serial.print("[Dispatcher] Dispatching to ");
    Serial.print(count);
    Serial.println(" callbacks:");
    for (size_t i = 0; i < count; ++i) {
      if (callbacks[i].valid()) {
        callbacks[i](value);
      }
    }
  }

private:
  Callback<void, int> callbacks[MAX_CALLBACKS];
  size_t count = 0;
};

// Example listener classes
struct ListenerA {
  void onEvent(int v) {
    Serial.print("[A] Value = ");
    Serial.println(v);
  }
};

struct ListenerB {
  void onEvent(int v) {
    Serial.print("[B] Value * 2 = ");
    Serial.println(v * 2);
  }
};

EventDispatcher dispatcher;
ListenerA a;
ListenerB b;

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("\n[event_dispatcher.ino] Basic dispatcher demo");

  dispatcher.add(Callback<void, int>(a, &ListenerA::onEvent));
  dispatcher.add(Callback<void, int>(b, &ListenerB::onEvent));

  dispatcher.dispatch(10);
}

void loop() {
  // no-op
}

