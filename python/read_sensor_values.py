import serial
import pandas as pd
import time

BAUD_RATE = 115200


def main() -> None:
    port = "COM11"
    time_to_run = 2
    arduino = serial.Serial(port, BAUD_RATE, timeout=15)

    data = {"sensor_0": [], "sensor_1": [], "sensor_2": []}

    start_time = time.time()
    while time.time() - start_time < time_to_run:
        received_data: str = arduino.readline().decode()
        if received_data:
            sensor_type, measured_data = tuple(received_data.split(" "))
            data[sensor_type].append(measured_data)

    min_len = 2000000000
    for key in data:
        if len(data[key]) < min_len:
            min_len = len(data[key])

    for key in data:
        data[key] = data[key][:min_len]

    data_pd = pd.DataFrame(data)
    data_pd.to_csv("data/open_hand_9.csv", index=False)


if __name__ == "__main__":
    main()
