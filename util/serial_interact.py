import serial 

PORT = "/dev/cu.usbmodem1101"

def serial_write(serial_number):
    user_input = input()
    ser = serial.Serial(serial_number)
    # TODO Baudrate Test
    ser.write(user_input)
    ser.close()
    return None

def serial_read(serial_number, length):
    ser = serial.Serial(serial_number)
    #TODO Baudrate Test
    s = None
    if length > 0:
        s = ser.read(length)
    elif length < 0:
        s = ser.readline()
    ser.close()
    return s

if __name__ == '__main__':
    ser = serial.Serial(PORT, 9600)
    user_input = ""
    while user_input != "exit":
        user_input = input()
        ser.write(user_input.encode())
    ser.close()