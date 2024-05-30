import serial

ser = serial.Serial(port='/dev/ttyS0', baudrate=115200, timeout=1)
print("Serial open")
while True:
    x = ser.readline()
    if len(x) > 0:
        try:
            num = int(x)
            print(f"Received: {num}")
            ser.write((str(num) + '\n').encode())
        except ValueError:
            print("Received non-integer data")