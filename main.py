import serial
import time

ser = serial.Serial(port="COM3",baudrate=115200)
time.sleep(2)
ser.reset_input_buffer()

while True:
    if (ser.in_waiting != 0):
        rawBytes = ser.readline()
        serialString = rawBytes.decode("utf-8",errors="ignore")
        serialString.strip()
        serialStringArray = serialString.split(",")
        Ax = serialStringArray[0]
        Ay = serialStringArray[1]
        Az = serialStringArray[2]
        print(f"Acceleration x: {Ax}. Acceleration y: {Ay}. Acceleration z: {Az}")