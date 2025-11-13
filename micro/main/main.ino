#include "includes.h"
// controller: request 1 byte asking if ready
// controller: receives either 0 or 1 - not ready or ready
// controller: when receive 1 - next loop request amount of bytes for data
// controller: receive bytes for data
// controller: receive data byte by byte

uint8_t typeOfData = NONE;

uint16_t all_data[5][255] = { {}, {}, {}, {}, {} };

int counter1 = 0, counter2 = 0, counter3 = 0, counter4 = 0, counter5 = 0;

void setup() {
    Serial.begin(BAUD_RATE);
    analogReadResolution(ANALOG_RES);
    pinMode(ANALOG_0, INPUT);
    pinMode(ANALOG_1, INPUT);
    Wire.setSDA(4);
    Wire.setSCL(5);
    Wire.begin();
    Wire.setClock(I2C_CLOCK);
    delay(100);
}

void writeData(const char *data, size_t size) {
    Wire.beginTransmission(PERIPHERAL_ADDRESS);
    Wire.write(data, size);
    Wire.endTransmission();
}

void loop() {
    Serial.print("typeOfData: ");
    Serial.println(typeOfData);
    if (typeOfData == NONE) {
        Serial.println("Gets here");
        Wire.requestFrom(PERIPHERAL_ADDRESS, 1);
        Serial.println("sent request? ig?");
        if (Wire.available()) {
            Serial.println("this is changing!");
            typeOfData = Wire.read();
        }
    } else {
        Wire.requestFrom(PERIPHERAL_ADDRESS, 2);
        Serial.print("Data: ");
        uint16_t data;
        uint8_t byte_arr[2];
        for (int i = 0; i < 2 && Wire.available(); i++) {
            byte_arr[i] = (uint8_t)Wire.read();
        }

        data = ((uint16_t)byte_arr[0]) | (((uint16_t)byte_arr[1]) << 8);

        Serial.printf("Type of data: %d\n", typeOfData);
        Serial.println(data);

        switch (typeOfData) {
            case SENSOR_1:
                if (counter3 == 255) counter3 = 0;
                all_data[SENSOR_1 + 2][counter3] = data;
                counter3++;
                break;
            case SENSOR_2:
                if (counter4 == 255) counter4 = 0;
                all_data[SENSOR_2 + 2][counter4] = data;
                counter4++;
                break;
            case SENSOR_3:
                if (counter5 == 255) counter5 = 0;
                all_data[SENSOR_3 + 2][counter5] = data;
                counter5++;
                break;
        }

        typeOfData = NONE;

        writeData("sent", 4);
        delay(50);
    }
}

void setup1() {
}

void loop1() {
    if (counter1 == 255) counter1 = 0;
    all_data[0][counter1] = analogRead(ANALOG_0);
    counter1++;
    delay(100);
    if (counter2 == 255) counter2 = 0;
    all_data[1][counter2] = analogRead(ANALOG_1);
    counter2++;
    delay(100);
    Serial.println("things are changing i thinks");
}
