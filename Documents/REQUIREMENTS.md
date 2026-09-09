# Requirements and Technical Reference

## Project Overview

The project implements a distributed automotive dashboard demonstration using three PIC18F4580 firmware images. Two nodes generate dashboard values and publish them over CAN; a display node receives the values and renders them on a parallel character LCD.

The source is the authoritative basis for the details below. Items not present in source or project configuration are marked `TBD`.

## Scope

### Included

- PIC18F4580 firmware projects `e1.X`, `e2.X`, and `e3.X`.
- ECAN initialization, standard identifier encoding, one-byte transmit payloads, and receive-buffer polling.
- ADC channel 4 sampling in `e1.X` and `e2.X`.
- Digital input handling for indicator and gear controls.
- 8-bit character LCD initialization and text output in `e3.X`.
- Message identifiers and dashboard field formatting.

### Not yet defined or confirmed

- Physical development board and final PCB.
- MCU package pin numbers and board net names.
- CAN transceiver, bus termination, bitrate requirement, and wiring.
- Sensor type, analog voltage range, calibration, and engineering-unit conversion.
- Product-level diagnostics, fault handling, power design, and low-power behavior.
- Verified hardware test results.

## Hardware Requirements

| Component | Requirement/source evidence | Status |
|---|---|---|
| MCU | PIC18F4580 target in all MPLAB project configurations | Identified |
| Clock | `_XTAL_FREQ 20000000` in `e1.X` and `e2.X`; `e3.X` does not define it locally | Requires system verification |
| CAN controller | Internal ECAN peripheral; RB2 TX and RB3 RX in source | Implemented in source |
| CAN transceiver | Required for physical CAN bus operation | `TBD` |
| LCD | Parallel 8-bit character LCD using PORTD data and RC0–RC2 control | Implemented in `e3.X` source |
| Analog input | ADC channel 4 used in `e1.X` and `e2.X` | Sensor and physical pin `TBD` |
| Indicator inputs | RC0, RC1, RC2, RC3 read in `e1.X` | External switches/circuit `TBD` |
| Gear inputs | RC0, RC1, RC2 read in `e2.X` | External switches/circuit `TBD` |
| Power | Voltage rails, regulation, protection, and current budget | `TBD` |
| Final PCB/package | No production hardware information in repository | `TBD` |

## Pin Requirements

Physical package pin numbers are intentionally not populated because they are not declared in the repository.

| Pin No. | MCU pin | Peripheral | Signal name | Direction | Description | Active level | Notes |
|---|---|---|---|---|---|---|---|
| TBD | RB2 | ECAN | CAN_TX | Output | CAN transmit signal | `TBD` | Set by `TRISB2 = 0` |
| TBD | RB3 | ECAN | CAN_RX | Input | CAN receive signal | `TBD` | Set by `TRISB3 = 1` |
| TBD | RC0 | LCD/input | RW or input 1 | Output in LCD stage; input in producer stages | LCD read/write control or digital input | LCD-specific | Assignment differs by firmware stage |
| TBD | RC1 | LCD/input | RS or input 2 | Output in LCD stage; input in producer stages | LCD register select or digital input | LCD-specific | Assignment differs by firmware stage |
| TBD | RC2 | LCD/input | EN or input 3 | Output in LCD stage; input in producer stages | LCD enable or digital input | LCD-specific | Assignment differs by firmware stage |
| TBD | RC3 | GPIO | Indicator input 4 | `TBD` | Read by `e1.X` | Active-low in source comparison | Direction must be verified |
| TBD | PORTD[7:0] | LCD | LCD_DATA[7:0] | Bidirectional during busy polling | LCD data bus | `TBD` | RD7 is switched to input to read busy flag |
| TBD | ADC channel 4 | ADC | ADC4 | Input | Analog source for RPM/speed producer | `TBD` | Physical pin and scaling are unknown |
| TBD | PORTB[1:0] | GPIO | LEFT_IND | Output macro | Left indicator bit mask | `TBD` | Declared in `message_handler.h`; not driven by `e3.X` |
| TBD | PORTB[7:6] | GPIO | RIGHT_IND | Output macro | Right indicator bit mask | `TBD` | Declared in `message_handler.h`; not driven by `e3.X` |

## Peripheral Requirements

### CAN

- Purpose: exchange speed, gear, RPM, and indicator values between firmware stages.
- Instance: PIC18F4580 ECAN peripheral; exact module naming is MCU-register based.
- Signals: RB2 (`CAN_TX`), RB3 (`CAN_RX`).
- Identifier type: standard CAN IDs encoded by `set_msg_id_std()` and decoded by `get_msg_id_std()`.
- Payload length: producer calls use length `1`; the generic API accepts up to the CAN data-buffer capacity but does not enforce a maximum.
- Configuration: `BRGCON1 = 0xE1`, `BRGCON2 = 0x1B`, `BRGCON3 = 0x03`; source comments describe this as an 8 MHz CAN timing setup.
- Receive behavior: polling of RXB0; the implementation clears the full and interrupt flags after reading.
- Error handling: no explicit transmit-completion, bus-off, overflow, or invalid-length handling is implemented.
- Requirement: confirm oscillator, CAN bitrate, transceiver, termination, and payload contract on hardware.

### ADC

- Purpose: produce the source value for RPM (`e1.X`) or speed (`e2.X`).
- Channel: `CHANNEL4`, value `0x04`.
- Configuration: right-justified result, acquisition setting `ACQT = 4 Tad`, conversion clock bits set to `Fosc/32`, VSS/VDD voltage references, ADC enabled.
- Conversion: blocking wait on `GO` completion.
- Scaling: `val = var / 10.23`; the engineering-unit meaning is not documented.
- Requirement: define sensor range, calibration, expected resolution, and physical ADC pin.

### GPIO / Inputs

- `e1.X` reads RC0–RC3 and maps the most recently detected low input to indicator payloads 1–4.
- `e2.X` reads RC0–RC2, uses release flags (`sw1`–`sw3`) for one-step-per-press behavior, and maps gear values 0–7.
- The source compares inputs against `0`, implying active-low behavior.
- Pull-up/pull-down configuration and debouncing duration are not defined.

### LCD / Display

- Interface: 8-bit parallel data bus on PORTD with RC0 (`RW`), RC1 (`RS`), and RC2 (`EN`).
- Initialization: two-line, 5x8 matrix, cursor home, display on/cursor off, and clear display commands.
- Busy polling: RD7 is temporarily configured as an input and read as `CLCD_BUSY`.
- Header: `RPM  IND  SPD GR` is repeatedly written to line 1.
- Data fields: RPM line 2 column 0; indicator field around columns 5–8; speed line 2 column 10; gear line 2 column 14.
- Requirement: confirm LCD controller/module, voltage levels, contrast circuit, and physical pin mapping.

## Software Requirements

- Each stage must retain a clear initialization function followed by a main loop.
- CAN identifiers must remain centralized in `msg_id.h` and synchronized across stages.
- CAN receive code must validate availability and payload length before processing.
- Sensor conversion must be documented when calibration is defined.
- LCD access should remain behind the `clcd` interface in the receiver stage.
- Interrupts are not used by the visible application; if introduced, shared data and timing must be reviewed.
- Blocking loops and delays must be reviewed for their effect on CAN servicing and display refresh.
- Invalid message IDs and invalid gear/indicator payloads should be handled deterministically.

## Libraries Used

| Library | Purpose | Version/source | Used by | Notes |
|---|---|---|---|---|
| XC8 compiler headers (`xc.h`) | MCU register definitions and compiler support | XC8 3.10 configured in project files | All stages | Toolchain dependency, not a project-local library |
| C standard headers (`stdint.h`, `stdio.h`, `stdlib.h`, `string.h`) | Integer types, formatting, and general C support | Version not separately specified | Various source files | `sprintf` is used by `e3.X`; review memory/code-size impact |
| Project-local CAN module | CAN initialization and transmit/receive API | Repository source in each stage | All stages | Duplicated stage-local implementation |
| Project-local CLCD module | LCD initialization and output | Repository source in `e3.X` | `e3.X` | Similar LCD code also exists in `e1.X`/`e2.X` but is not initialized in their main flow |

## Testing Requirements

| Test | Objective/setup | Expected result | Actual result | Status |
|---|---|---|---|---|
| Build `e1.X` | Build with MPLAB/XC8 project configuration | Hex/ELF generated without unresolved errors | Generated artifacts are present; execution was not independently verified | `TBD` |
| Build `e2.X` | Build with MPLAB/XC8 project configuration | Hex/ELF generated without unresolved errors | Generated artifacts are present; execution was not independently verified | `TBD` |
| Build `e3.X` | Build with MPLAB/XC8 project configuration | Hex/ELF generated without unresolved errors | Generated artifacts are present; execution was not independently verified | `TBD` |
| ADC producer test | Apply known input to ADC channel 4 | Correctly scaled one-byte CAN payload | Not documented | `PENDING` |
| CAN transmit/receive test | Connect two nodes through confirmed transceivers and analyzer/peer | IDs `0x10`, `0x20`, `0x30`, `0x40` received with expected payloads | Not documented | `PENDING` |
| Gear input test | Exercise RC0–RC2 controls | Gear changes once per press and follows defined range | Not documented | `PENDING` |
| Indicator input test | Exercise RC0–RC3 controls | Correct indicator payload and display symbol | Not documented | `PENDING` |
| LCD test | Power and connect the LCD using verified wiring | Header and received fields render correctly | Not documented | `PENDING` |
| Integration test | Run producer and receiver stages together | Dashboard fields update without loss or unacceptable latency | Not documented | `PENDING` |
| Long-duration/error test | Run bus and display continuously | No lockup, buffer fault, or unacceptable drift | Not documented | `PENDING` |

## Dependencies

- Microchip MPLAB X project environment: exact IDE version `TBD`.
- Microchip XC8 compiler 3.10, as configured by all three projects.
- PIC18F4580 device support pack `PIC18Fxxxx_DFP` version 1.4.151, as recorded in project configuration.
- PIC18F4580 hardware, CAN transceiver/bus hardware, LCD, analog source, and digital controls: exact parts `TBD`.
- Programmer/debugger: `TBD`.

## Constraints

- Physical pin numbers and board-level electrical constraints are unknown.
- CAN timing must be reconciled with the actual oscillator; source comments and clock definitions are not sufficient proof of a valid bitrate.
- Current CAN routines lack explicit bounds and error handling.
- LCD and producer stages assign different functions to RC0–RC2; they are separate firmware images, but final system wiring must be checked.
- ADC conversion scaling is not tied to a documented sensor characteristic.
- The receiver uses a single-byte local `data` variable while the generic receive routine copies `*len` bytes; payload length validation is required before relying on multi-byte frames.

## Acceptance Criteria

- All three projects build reproducibly with the documented device and toolchain.
- Physical pin map, clock source, CAN transceiver, bitrate, and bus termination are documented and verified.
- ADC, gear, indicator, CAN, and LCD tests have recorded results.
- Every message ID and payload has a tested producer/consumer contract.
- Receiver behavior is correct for valid, invalid, missing, and oversized/unexpected CAN payloads.
- Full integration and an appropriate duration/error test pass on the target hardware.
- Documentation and source contain no contradictory pin, protocol, or status information.

