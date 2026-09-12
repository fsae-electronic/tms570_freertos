#!/usr/bin/env python3
"""
Decode the Main ECU's bench-test telemetry stream on /dev/ttyUSB0.

Frame layout (64 bytes, big-endian -- this MCU is a big-endian ARM):
    [0:4]   float32  rpm
    [4:8]   float32  battery_voltage
    [8:12]  float32  battery_current
    [12:16] float32  wheel_speed_fl
    [16:20] float32  wheel_speed_fr
    [20:64] zero padding (unused in this bench-test build)

No sync marker exists in this format -- it's just 64-byte frames sent
back-to-back. If the board was already mid-stream when this script
started, every decode below will be shifted and look wrong. Power-cycle
or JTAG-reset the board right after starting this script to line up
with frame 0.

Usage:
    pip install pyserial   # if not already installed
    python3 decode_telemetry.py [/dev/ttyUSB0] [baud]
"""
import sys
import struct
import serial

FRAME_SIZE = 64
FIELD_NAMES = ["rpm", "battery_voltage", "battery_current",
               "wheel_speed_fl", "wheel_speed_fr"]

def main():
    port = sys.argv[1] if len(sys.argv) > 1 else "/dev/ttyUSB0"
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 115200

    ser = serial.Serial(port, baud, timeout=2)
    print(f"Listening on {port} @ {baud} baud. Ctrl-C to stop.")
    print(f"{'frame':>6}  " + "  ".join(f"{n:>16}" for n in FIELD_NAMES))

    frame_num = 0
    try:
        while True:
            frame = ser.read(FRAME_SIZE)
            if len(frame) < FRAME_SIZE:
                continue  # timeout with a partial read; just keep trying
            values = struct.unpack(">5f", frame[:20])
            print(f"{frame_num:>6}  " + "  ".join(f"{v:>16.2f}" for v in values))
            frame_num += 1
    except KeyboardInterrupt:
        pass
    finally:
        ser.close()

if __name__ == "__main__":
    main()
