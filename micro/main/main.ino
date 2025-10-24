#include "includes.h"

void setup() {
    Serial.begin(BAUD_RATE);
    delay(100);
    Serial.println("Start!");
}

void loop() {
    Serial.println("Hello, world!");
}