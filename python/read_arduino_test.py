import serial
import pandas as pd
import time

BAUD_RATE = 115200


def main() -> None:
    port = input("What port is your Arduino in?: ")
    time_to_run = 30
    arduino = serial.Serial(port, BAUD_RATE, timeout=15)

    data = {
        "sensor_1": [],
        "sensor_2": [],
        "sensor_3": [],
        "sensor_4": [],
        "sensor_5": [],
    }

    start_time = time.time()
    while time.time() - start_time < time_to_run:
        received_data: str = arduino.readline().decode()
        if received_data:
            sensor_type, measured_data = tuple(received_data.split(" "))
            data[sensor_type].append(measured_data)

    data_pd = pd.DataFrame(data)
    data_pd.to_csv("data/arduino_test_data.csv")


if __name__ == "__main__":
    main()
