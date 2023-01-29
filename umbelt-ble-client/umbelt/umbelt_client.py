"""
Umbelt class that represents the primary SDK interface.

The Umbelt class offers abstractions for connecting and writing commands
to Umbelt boards.
"""

import os
from pathlib import Path
import time
import struct

from adafruit_ble import BLERadio
from adafruit_ble.advertising.standard import ProvideServicesAdvertisement
from adafruit_ble.services.nordic import UARTService

from .utils import generate_opcodes


class Umbelt:

    def __init__(self, name: str = "Umlaut") -> None:
        """
        Initializes an Umbelt board object.

        Args:
            name: Name of umbelt board
        """

        self.name = name

        self.ble = BLERadio()
        self.connection = None  # BLE connection handle

        opcode_path = os.path.join(
            Path(__file__).parent.parent.parent,
            "src/umbelt_opcodes.h"
        )
        self.opcodes = generate_opcodes(opcode_path)

    def connect(self, timeout: int = 20) -> bool:
        """
        Connects to an umbelt board.

        Args:
            timeout: How long to wait (in seconds) before giving up.

        Returns:
            True if successfully connected to Umbelt, False otherwise.
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
            print(f"Connected with Umbelt board {self.name}.")
            return True
        else:
            print(f"Failed to connect to Umbelt board {self.name}.")
            return False

    def disconnect(self):
        """
        Disconnects from the umbelt board, if connected.
        """

        if not self.connection:
            print("No active connection.")
            return

        try:
            self.connection.disconnect()
        except Exception as e:
            raise e  # TODO better handling

        print("Disconnected from Umbelt board {self.name}.")

    def is_connected(self):
        if not self.connection:
            return False

        return self.connection.connected

    def vibrate_motor(self, motor_idx: int, duration: int, intensity: float):
        """
        Vibrates the specified motor at the requested duration and intensity.

        Args:
            motor_idx: Index of motor to vibrate.
            duration: Duration to vibrate motor through (ms).
            intensity: The intensity to vibrate a motor at, as a percentage of
                the maximum power.
        """

        if intensity < 0 or intensity > 1.0:
            raise ValueError("Intensity must be in the range 0.0 - 1.0")

        payload = bytearray()
        payload.extend(
            bytearray(struct.pack("B", self.opcodes["UMBELT_OP_HAPTIC"])))
        payload.append(bytearray(struct.pack("B", motor_idx))[0])
        payload.extend(bytearray(struct.pack("I", duration)))
        payload.extend(bytearray(struct.pack("f", intensity)))

        print("opcode:", bytearray(struct.pack("B", self.opcodes["UMBELT_OP_HAPTIC"])))
        print(payload)

        self.send_command(payload, verbose=True)

    def send_command(self, command_payload, verbose : bool = False) -> bool:
        """
        Args:
            command_payload: A bytearray object to be sent.
            verbose: Whether to print debug logs on what payload was sent.

        Returns:
            True on success, False on failure.
        """
        n_bytes = len(command_payload)

        # Prepend '!' and n_bytes + 2 as the packet header:
        #   - 1 byte for '!'
        #   - 1 byte for len itself
        cmd = bytearray()
        cmd.extend(bytearray(struct.pack("c", "!".encode("utf-8"))))
        cmd.extend(bytearray(struct.pack("c", chr(n_bytes + 2).encode("utf-8"))))
        cmd.extend(command_payload)

        if verbose:
            self.print_packet_debug(command_payload, n_bytes, cmd)

        # Send payload to device.
        try:
            uart_service = self.connection[UARTService]
            uart_service.write(bytes(cmd))
            # uart_service.write(b'\n')
            # print(uart_service.readline().decode("utf-8"))
        except Exception as e:
            print('Error in sending command to Umbelt: ', e)
            return False

        return True

    def print_packet_debug(self, raw_payload, n_bytes, cmd):

        print('----- PACKET DEBUG -------')
        print('[Command payload]: ', raw_payload)
        print('[Command payload with header]: ', cmd)

        cmd_arr = [c for c in cmd]
        print('[Length of the command array:' + str(len(cmd_arr)))
        print('[Command array]:')
        print(cmd_arr)

        print()


def scan_umbelts(timeout: int = 5):
    """
    Scan for advertising umbelt boards over BLE.

    Args:
        timeout: Duration to scan over, in seconds.
    """

    ble = BLERadio()

    print('Umbelt scan results:')
    print('=' * 80)
    print(f'{"":3} {"name":16} {"rssi":4}')
    print('-' * 80)

    seen = {}  # Store the unique umbelts we scan for here.

    for adv in ble.start_scan(timeout=timeout):
        name = adv.complete_name
        if not name: continue

        # TODO: Establish a naming scheme that allows for recognizable
        # yet customizable umbelt device names.
        if "Umbelt" in name and adv.address not in seen:
            seen[adv.address] = adv

    ble.stop_scan()

    # Print the ble scan results
    for i, addr in enumerate(seen):
        adv = seen[addr]
        print(f'{i:3d} {adv.complete_name:16} {adv.rssi:4d}')

