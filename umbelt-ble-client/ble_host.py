"""
Python script for pairing host device with an umbelt as the peripheral over BLE.
Once connected, this program allows you to input various commands that will be
sent to the umbelt.
"""
from adafruit_ble import BLERadio
from adafruit_ble.advertising.standard import ProvideServicesAdvertisement
from adafruit_ble.services.nordic import UARTService

import umbelt_ble_actions as ub

ble = BLERadio()

uart_connection = None

def input_command():
    """
    Handles user input for command selection.
    Outputs:
        - The umbelt command as a string (without any UART-protocol specific
          parts such as the checksum)
    """
    commands = {
        'T': ub.current_time,
        'X': ub.custom_command,
        'MU': ub.motor_up,
        'MD': ub.motor_down,
        'ML': ub.motor_left,
        'MR': ub.motor_right,
    }

    func = None
    while func is None:
        choice = input("Select a command: ")
        func = commands.get(choice)

        if func is None:
            print("Invalid command. Use 'X' to type a custom command.\n")

    command_str = func()
    return command_str

def print_packet_debug(command_str, n_bytes, cmd, encoded):

    print('----- PACKET DEBUG -------')
    print('[Raw Command string]:' + command_str)
    print('[Command string with header]:' + cmd)

    cmd_arr = [c for c in cmd]
    print('[Length of the command array:' + str(len(cmd_arr)))
    print('[Command array]:')
    print(cmd_arr)
    cmd_arr_hx = [hex(ord(c)) for c in cmd_arr]
    print('[Command array (hex)]:')
    print(cmd_arr_hx)

    print('[Encoded command]:' + str(encoded))
    print('[Length of encoded command]:' + str(len(encoded)))

    print()

def encode_str(cmd):

    cmd_arr = [ord(c) for c in cmd]
    cmd_bytes = bytes(cmd_arr)

    return cmd_bytes

def main():
    global uart_connection
    while True:
        # Establish Conenction
        if not uart_connection:
            print("Trying to connect...")
            for adv in ble.start_scan():
                name = adv.complete_name
                if not name: continue
                print(name)
                # if UARTService in adv.services:
                if "Umbelt" in name:
                    uart_connection = ble.connect(adv)
                    print("Connected")
                    break
            ble.stop_scan()

        if uart_connection and uart_connection.connected:
            uart_service = uart_connection[UARTService]
            while uart_connection.connected:
                # Get input command from user
                command_str = input_command()

                # Compute length of the raw command str
                n_bytes = len(command_str)

                # Prepend '!' and command len + 3 as the packet header:
                #   - 1 byte for '!'
                #   - 1 byte for len itself
                #   - 1 byte for the checksum
                cmd = '!' + chr(n_bytes + 3) + command_str

                encoded = encode_str(cmd)
                print_packet_debug(command_str, n_bytes, cmd, xsum, cmd,
                        encoded)

                # Send to device
                uart_service.write(encoded)
                print(uart_service.readline().decode("ascii"))

if __name__ == "__main__":
    main()
