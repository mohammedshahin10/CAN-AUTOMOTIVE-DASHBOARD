# Project Progress

This file is chronological. Historical entries are retained even if the implementation later changes.

## 2025-12-01

### Initial producer firmware stages

- `e1.X` source comments identify work created on 1 December 2025.
- Implemented ADC initialization and channel 4 sampling.
- Implemented CAN initialization and standard-ID transmit/receive helpers.
- Implemented RPM payload transmission using `RPM_MSG_ID` (`0x30`).
- Implemented indicator input scanning on RC0–RC3 and indicator payload transmission using `INDICATOR_MSG_ID` (`0x40`).
- Hardware test result: not recorded.

### Gear and speed producer firmware

- `e2.X` source comments identify work created on 1 December 2025.
- Implemented ADC channel 4 sampling and speed payload transmission using `SPEED_MSG_ID` (`0x10`).
- Implemented gear input handling on RC0–RC2 with press/release flags.
- Implemented gear payload transmission using `GEAR_MSG_ID` (`0x20`).
- Hardware test result: not recorded.

### Dashboard receiver firmware

- Implemented `e3.X` CAN receive polling.
- Implemented LCD initialization and 8-bit parallel output.
- Implemented display formatting for speed, RPM, gear, and indicator messages.
- Added fixed LCD header text `RPM  IND  SPD GR`.
- Hardware test result: not recorded.

## 2026-09-06

### MPLAB project configuration and generated artifacts

- All three stages are configured for PIC18F4580.
- All three stages use Microchip XC8 3.10 in their project configuration.
- The project configuration records `PIC18Fxxxx_DFP` version 1.4.151.
- Generated `build`/`dist` artifacts are present for the three stages, but this documentation pass did not independently reproduce or validate those builds.

## 2026-09-06

### Documentation baseline

- Created the repository documentation set: `README.md`, `REQUIREMENTS.md`, `AGENT.md`, `PENDING_TASKS.md`, and `PROGRESS.md`.
- Documented the three firmware stages, known logical signal assignments, CAN message IDs, LCD behavior, toolchain configuration, and known uncertainties.
- Recorded hardware definition, peripheral verification, integration, and acceptance work as pending because no confirmed results were present in the repository.
