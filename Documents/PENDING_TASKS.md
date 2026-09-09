# Pending Tasks

Only unfinished or unverified work is listed here.

## Hardware Definition

- [ ] Identify the development board(s) used for `e1.X`, `e2.X`, and `e3.X`.
- [ ] Document the final/custom PCB, MCU package, external CAN transceiver, power rails, and programmer/debugger.
- [ ] Map logical MCU signals to physical package pin numbers and board net names.
- [ ] Confirm the oscillator implementation and reconcile the 20 MHz firmware definition with the CAN timing configuration.
- [ ] Document the ADC sensor, input voltage range, calibration, and engineering-unit conversion.

## Peripheral and Firmware Verification

- [ ] Verify ADC channel 4 sampling and scaling on target hardware.
- [ ] Verify gear input polarity, pull configuration, debouncing, and valid gear range.
- [ ] Verify indicator input mapping and expected payload values 1–4.
- [ ] Verify CAN bus bitrate, standard IDs, payload lengths, transceiver wiring, and termination.
- [ ] Add or verify CAN transmit error, receive overflow, invalid-length, and missing-message handling.
- [ ] Verify LCD wiring, initialization, busy polling, field positions, and displayed values.
- [ ] Confirm whether receiver-side indicator GPIO output macros are required and implement/test them if so.

## Integration and Acceptance

- [ ] Run and record isolated ADC, gear, indicator, CAN, and LCD tests.
- [ ] Run a full `e1.X` + `e2.X` + `e3.X` integration test on the target hardware.
- [ ] Perform a long-duration and communication-error test.
- [ ] Confirm the acceptance criteria in `REQUIREMENTS.md` and record results in `PROGRESS.md`.

