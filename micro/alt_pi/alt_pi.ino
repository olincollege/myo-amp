#include "includes.h"

int mostRecentData = NONE;
volatile int dataReadyToSend = NONE;

uint8_t split_data[2];

uint16_t prevVal_Sensor1 = 0;
volatile uint16_t currVal_Sensor1 = 0;
uint16_t prevVal_Sensor2 = 0;
volatile uint16_t currVal_Sensor2 = 0;
uint16_t prevVal_Sensor3 = 0;
volatile uint16_t currVal_Sensor3 = 0;


// peripheral: gather data, always return 0
// peripheral: once data ready, return 1
// peripheral: send size
// peripheral: send data as a whole

void setup() {
    Serial.begin(BAUD_RATE);
    analogReadResolution(ANALOG_RES);
    pinMode(ANALOG_0, INPUT);
    pinMode(ANALOG_1, INPUT);
    pinMode(ANALOG_2, INPUT);
    Wire.setSDA(4);
    Wire.setSCL(5);
    Wire.begin(PERIPHERAL_ADDRESS);
    Wire.setClock(I2C_CLOCK);
    Wire.onRequest(I2C_communication_handler);
    Wire.onReceive(event);
    delay(100);
}

void event(int numBytes) {
    Serial.print("Controller: ");
    for (int i = 0; i < numBytes && Wire.available(); i++) {
        Serial.print((char)Wire.read());
    }

    Serial.println();
}

void I2C_communication_handler() {
    Serial.println("receiving a request!");
    if (dataReadyToSend == NONE) {
        Serial.println("Does it get here?");
        switch (mostRecentData) {
            case SENSOR_1:
                dataReadyToSend = SENSOR_1;
                Wire.write(dataReadyToSend);
                break;
            case SENSOR_2:
                dataReadyToSend = SENSOR_2;
                Wire.write(dataReadyToSend);
                break;
            case SENSOR_3:
                dataReadyToSend = SENSOR_3;
                Wire.write(dataReadyToSend);
                break;
            default:
                Serial.println("there is SOMETHING wrong here...");
                break;
        }
    } else {
        Serial.println("are we getting...here?");
        switch (dataReadyToSend) {
            case SENSOR_1:
                split_data[0] = ((uint8_t)(currVal_Sensor1));
                split_data[1] = (uint8_t)(((currVal_Sensor1)) >> 8);
                Wire.write(split_data, 2);
                break;
            case SENSOR_2:
                split_data[0] = ((uint8_t)(currVal_Sensor2));
                split_data[1] = (uint8_t)(((currVal_Sensor2)) >> 8);
                Wire.write(split_data, 2);
                break;
            case SENSOR_3:
                split_data[0] = ((uint8_t)(currVal_Sensor3));
                split_data[1] = (uint8_t)(((currVal_Sensor3)) >> 8);
                Wire.write(split_data, 2);
                break;
            default:
                Serial.println("Something went wrong.");
                break;
        }
        split_data[0] = 0;
        split_data[1] = 0;
        dataReadyToSend = NONE;
    }
}

void loop() {
    Serial.print("mostRecentData: ");
    Serial.println(mostRecentData);
    Serial.print("dataReadyToSend: ");
    Serial.println(dataReadyToSend);
    if (currVal_Sensor1 != prevVal_Sensor1) {
        mostRecentData = SENSOR_1;
    } else if (currVal_Sensor2 != prevVal_Sensor2) {
        mostRecentData = SENSOR_2;
    } else if (currVal_Sensor3 != prevVal_Sensor3) {
        mostRecentData = SENSOR_3;
    } else {
        mostRecentData = NONE;
    }

    prevVal_Sensor1 = currVal_Sensor1;
    prevVal_Sensor2 = currVal_Sensor2;
    prevVal_Sensor3 = currVal_Sensor3;
    delay(100);
}

void setup1() {
}

void loop1() {
    currVal_Sensor1 = analogRead(ANALOG_0);
    delay(100);
    currVal_Sensor2 = analogRead(ANALOG_1);
    delay(100);
    currVal_Sensor3 = analogRead(ANALOG_2);
    delay(100);
}
