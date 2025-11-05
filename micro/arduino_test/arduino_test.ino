#include "includes.h"

uint16_t sensor_1[MAX_DATA];
uint16_t sensor_2[MAX_DATA];
uint16_t sensor_3[MAX_DATA];
uint16_t sensor_4[MAX_DATA];
uint16_t sensor_5[MAX_DATA];

uint8_t counter_1 = 0;
uint8_t counter_2 = 0;
uint8_t counter_3 = 0;
uint8_t counter_4 = 0;
uint8_t counter_5 = 0;

void setup() {
  Serial.begin(BAUD_RATE);
  analogReadResolution(ANALOG_RES);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
}

void loop() {
  sensor_1[counter_1] = (uint16_t)analogRead(A0);
  counter_1++;
  if (counter_1 == MAX_DATA) {
    for (int i = 0; i < MAX_DATA; i++) {
      Serial.print("sensor1 ");
      Serial.print(sensor_1[i]);
      Serial.println();
      sensor_1[i] = (uint16_t)0;
    }
    counter_1 = 0;
  }
  sensor_2[counter_2] = (uint16_t)analogRead(A1);
  counter_2++;
  if (counter_2 == MAX_DATA) {
    for (int i = 0; i < MAX_DATA; i++) {
      Serial.print("sensor2 ");
      Serial.print(sensor_2[i]);
      Serial.println();
      sensor_2[i] = (uint16_t)0;
    }
    counter_2 = 0;
  }
  sensor_3[counter_3] = (uint16_t)analogRead(A2);
  counter_3++;
  if (counter_3 == MAX_DATA) {
    for (int i = 0; i < MAX_DATA; i++) {
      Serial.print("sensor3 ");
      Serial.print(sensor_3[i]);
      Serial.println();
      sensor_3[i] = (uint16_t)0;
    }
    counter_3 = 0;
  }
  sensor_4[counter_4] = (uint16_t)analogRead(A3);
  counter_4++;
  if (counter_4 == MAX_DATA) {
    for (int i = 0; i < MAX_DATA; i++) {
      Serial.print("sensor4 ");
      Serial.print(sensor_4[i]);
      Serial.println();
      sensor_4[i] = (uint16_t)0;
    }
    counter_4 = 0;
  }
  sensor_5[counter_5] = (uint16_t)analogRead(A4);
  counter_5++;
  if (counter_5 == MAX_DATA) {
    for (int i = 0; i < MAX_DATA; i++) {
      Serial.print("sensor5 ");
      Serial.print(sensor_5[i]);
      Serial.println();
      sensor_5[i] = (uint16_t)0;
    }
    counter_5 = 0;
  }
}
