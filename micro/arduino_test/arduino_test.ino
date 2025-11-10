#include "includes.h"

uint16_t sensor_0[MAX_DATA];
uint16_t sensor_1[MAX_DATA];
uint16_t sensor_2[MAX_DATA];

uint8_t counter_0 = 0;
uint8_t counter_1 = 0;
uint8_t counter_2 = 0;

void setup() {
    Serial.begin(BAUD_RATE);
    analogReadResolution(ANALOG_RES);
    pinMode(ANALOG0, INPUT);
    pinMode(ANALOG1, INPUT);
    pinMode(ANALOG2, INPUT);
}

void loop() {
    sensor_0[counter_0] = (uint16_t)analogRead(ANALOG0);
    counter_0++;
    if (counter_0 == MAX_DATA) {
        for (int i = 0; i < MAX_DATA; i++) {
            Serial.print("sensor_0 ");
            Serial.print(sensor_0[i]);
            Serial.println();
            sensor_0[i] = (uint16_t)0;
            // delay(10);
        }
        counter_0 = 0;
    }
    sensor_1[counter_1] = (uint16_t)analogRead(ANALOG1);
    counter_1++;
    if (counter_1 == MAX_DATA) {
        for (int i = 0; i < MAX_DATA; i++) {
            Serial.print("sensor_1 ");
            Serial.print(sensor_1[i]);
            Serial.println();
            sensor_1[i] = (uint16_t)0;
            // delay(10);
        }
        counter_1 = 0;
    }
    sensor_2[counter_2] = (uint16_t)analogRead(ANALOG2);
    counter_2++;
    if (counter_2 == MAX_DATA) {
        for (int i = 0; i < MAX_DATA; i++) {
            Serial.print("sensor_2 ");
            Serial.print(sensor_2[i]);
            Serial.println();
            sensor_2[i] = (uint16_t)0;
            // delay(10);
        }
        counter_2 = 0;
    }
}

void setup1() {}

void loop1() {}
