"""
Umbelt class that represents the primary SDK interface.

The UmbeltBoard class offers abstractions for connecting and writing commands
to Umbelt boards.
"""
from adafruit_ble import BLERadio
from adafruit_ble.advertising.standard import ProvideServicesAdvertisement
from adafruit_ble.services.nordic import UARTService


class Umbelt:

    def __init__(self, name: str = "Umlaut") -> None:
        """
        Initializes an Umbelt board object.

        @param name : Name of umbelt board
        """

        self.name = name

        self.ble = BLERadio()
        self.connection = None  # BLE connection handle

    def connect(self, timeout: int = 20) -> bool:
        """
        Connects to an umbelt board.

        @param timeout : How long to wait (in seconds) before giving up.

        @return True if successfully connected to Umbelt, False otherwise.
        """

        # Don't do anything if we are already connected to this Umbelt.
        if self.connection and self.connection.connected:
            print("Already connected with Umbelt board {self.name}.")
            return True

        print(f"Trying to connect to Umbelt board {self.name}.")

        # Attempt to connect to board in timeout seconds.
        start_time = time.time()
        while (time.time() - start_time < timeout) and not self.connection:
            # Attempt to establish connection
            for adv in self.ble.start_scan():
                name = adv.complete_name
                if not name: continue

                # TODO: Establish a naming scheme that allows for recognizable
                # yet customizable umbelt device names.
                if "Umbelt" in name:
                    self.connection = self.ble.connect(adv)
                    break
            self.ble.stop_scan()

        if self.connection:
            print(f"Successfully connected with Umbelt board {self.name}.")
            return True
        else:
            print(f"Failed to connect to Umbelt board {self.name}.")
            return False


    def send_command(command_payload, verbose : bool = False) -> bool:
        """
        @param command_payload
        @param verbose Whether to print debug logs on what payload was sent.

        @return True on success, False on failure.
        """
        n_bytes = len(command_payload)

        # Prepend '!' and n_bytes + 2 as the packet header:
        #   - 1 byte for '!'
        #   - 1 byte for len itself
        cmd = '!' + chr(n_bytes + 2) + command_payload
        cmd_arr = [ord(c) for c in cmd]
        encoded = bytes(cmd_arr)

        if verbose:
            print_packet_debug(command_payload, n_bytes, cmd, encoded)

        # Send payload to device.
        try:
            uart_service.write(encoded)
            print(uart_service.readline().decode("ascii"))
        except:
            print('Error in sending command to Umbelt.')
            return False

        return True

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
