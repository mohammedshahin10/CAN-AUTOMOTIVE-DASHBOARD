# CAR Automotive Dashboard

## Short Description

CAR Automotive Dashboard is an embedded dashboard firmware project for a PIC18F4580-based system. The repository contains three MPLAB X/XC8 firmware stages:

- `e1.X` reads an ADC input and four indicator inputs, then transmits RPM and indicator values over CAN.
- `e2.X` reads an ADC input and gear controls, then transmits speed and gear values over CAN.
- `e3.X` receives the CAN messages and displays RPM, indicator state, speed, and gear on an 8-bit character LCD.

The final application topology, development-board names, physical MCU package, CAN transceiver, and production PCB details are not specified in the repository and are `TBD`.

## Hardware Description

| Role | Hardware identified from the repository | Status/notes |
|---|---|---|
| MCU | Microchip PIC18F4580 | Declared as the target device in all three MPLAB projects |
| Development board | `TBD` | No board name is present in the project files |
| Final/custom PCB | `TBD` | Not described in the repository |
| CAN interface | PIC18F4580 ECAN peripheral; external transceiver `TBD` | CAN TX/RX are assigned in firmware to RB2/RB3 |
| Display | Parallel 8-bit character LCD | Connected through PORTD and RC0–RC2 in `e3.X` |
| Inputs | ADC channel 4 and digital inputs on PORTC | Exact external sensors/switches are `TBD` |
| Storage, RTC, keypad | Not identified | `TBD` if required by the product |
| Clock | Firmware defines `_XTAL_FREQ` as 20 MHz in `e1.X` and `e2.X`; `e3.X` has no equivalent definition | Hardware oscillator implementation is `TBD` |

## Pin Configuration

The firmware exposes logical port assignments. Physical package pin numbers and external net names are not present and remain `TBD`.

| Pin No. | MCU pin | Peripheral/function | Pin description | Usage/notes |
|---|---|---|---|---|
| TBD | RB2 | ECAN TX | CAN transmit | Configured as output by `init_can()` |
| TBD | RB3 | ECAN RX | CAN receive | Configured as input by `init_can()` |
| TBD | RC0 | LCD RW in LCD code; digital input in `e1.X`/`e2.X` | Shared logical assignment in different stages | Verify board-level muxing before integration |
| TBD | RC1 | LCD RS in LCD code; digital input in `e1.X`/`e2.X` | Shared logical assignment in different stages | Verify board-level muxing before integration |
| TBD | RC2 | LCD EN in LCD code; digital input in `e1.X`/`e2.X` | Shared logical assignment in different stages | Verify board-level muxing before integration |
| TBD | RC3 | Digital input | Indicator input in `e1.X` | Direction and external circuit require verification |
| TBD | PORTD[7:0] | LCD data bus | 8-bit LCD data interface | Used by `e3.X`; RD7 is also sampled as LCD busy flag |
| TBD | ADC channel 4 | Analog input | Source for RPM/speed value generation | Physical analog pin and sensor scaling are `TBD` |
| TBD | PORTB[1:0] | Indicator output macros | Left-indicator bit mask in `message_handler.h` | Receiver output behavior is declared but not implemented in `e3.X` |
| TBD | PORTB[7:6] | Indicator output macros | Right-indicator bit mask in `message_handler.h` | Receiver output behavior is declared but not implemented in `e3.X` |

## Project Flow

1. Each firmware stage initializes its required GPIO, ADC, CAN, and/or LCD modules.
2. `e1.X` repeatedly samples ADC channel 4, converts the reading to an 8-bit-style value, transmits `RPM_MSG_ID`, reads RC0–RC3, and transmits `INDICATOR_MSG_ID`.
3. `e2.X` repeatedly samples ADC channel 4, transmits `SPEED_MSG_ID`, scans RC0–RC2 with simple edge/release flags, and transmits `GEAR_MSG_ID`.
4. `e3.X` initializes CAN and the LCD, writes the fixed header `RPM  IND  SPD GR`, polls the CAN receive buffer, and updates LCD fields according to the received standard CAN message ID.
5. No interrupt-driven application flow, shutdown behavior, or explicit CAN error handling is implemented in the visible source.

## Logic / Working Principle

The two producer stages publish one-byte payloads using standard CAN identifiers:

| Message ID | Symbol | Producer | Consumer behavior |
|---|---|---|---|
| `0x10` | `SPEED_MSG_ID` | `e2.X` | Formats and displays speed at LCD line 2, column 10 |
| `0x20` | `GEAR_MSG_ID` | `e2.X` | Displays numeric gears, `N`, `R`, or `_c` according to payload |
| `0x30` | `RPM_MSG_ID` | `e1.X` | Multiplies payload by 60 and displays RPM at LCD line 2, column 0 |
| `0x40` | `INDICATOR_MSG_ID` | `e1.X` | Displays left, right, both, or blank indicator symbols |

Payload scaling, sensor calibration, CAN bitrate, debounce timing, and the meaning of the physical inputs require hardware-level confirmation. The source comments identify an 8 MHz CAN timing setup, while the firmware clock definitions in `e1.X` and `e2.X` use 20 MHz; this relationship must be verified before production use.

## Current Status

Development/integration stage. The repository contains source, MPLAB project configuration, and generated build artifacts for all three stages. The source-level CAN producer/receiver flow and LCD formatting are present, but hardware validation, physical pin mapping, integration status, and acceptance testing are not documented.

