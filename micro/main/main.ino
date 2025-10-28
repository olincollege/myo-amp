#include "includes.h"

void setup() {
    Serial.begin(BAUD_RATE);
}

void loop() {
    Serial.println("Hello, world!");
}