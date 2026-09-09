# Development Rules for Coding Agents

This file governs changes to the CAR Automotive Dashboard repository. Preserve the existing three-stage structure unless a change explicitly requires architectural work.

## Repository Structure

- `e1.X`: RPM and indicator producer firmware.
- `e2.X`: speed and gear producer firmware.
- `e3.X`: CAN receiver and LCD dashboard firmware.
- Each `.X` directory is an MPLAB X make project targeting PIC18F4580 with XC8.

## Required Practices

- Read the relevant stage's source and project configuration before changing firmware.
- Keep hardware definitions centralized in headers or dedicated driver modules.
- Verify port direction, alternate functions, oscillator assumptions, and CAN timing before changing peripheral code.
- Use meaningful names, consistent indentation, and short comments that explain intent or hardware constraints.
- Keep application logic separate from register-level access where practical.
- Make the smallest change that solves the requested problem.
- Preserve working behavior and avoid unrelated refactors.
- Keep message IDs centralized in `msg_id.h` and synchronized across all stages.
- Use wrapper functions for project-local or third-party peripheral code where that reduces coupling.
- Record new libraries, toolchain requirements, protocol changes, and hardware assumptions in `REQUIREMENTS.md`.
- Update pin tables whenever a signal assignment changes.
- Update `PENDING_TASKS.md` when work starts, pauses, or completes.
- Add completed work to `PROGRESS.md` in chronological order.
- Keep all five documentation files synchronized with source behavior.

## Do Not

- Do not invent physical pin numbers, hardware part numbers, test results, versions, or compatibility claims.
- Do not scatter pin definitions or configuration constants through application code.
- Do not hard-code values when a named macro or configuration constant is clearer.
- Do not introduce unused symbols, libraries, or abstractions.
- Do not duplicate functionality already provided by a project-local module or library.
- Do not modify third-party or generated files unless the reason is explicit and documented.
- Do not silently change CAN IDs, payload formats, LCD positions, input polarity, or gear/indicator semantics.
- Do not remove working behavior without recording the reason and affected documentation.
- Do not ignore compiler warnings without justification.

## Embedded/Firmware Rules

- Keep interrupt handlers short if interrupts are added.
- Avoid blocking operations in interrupts and avoid unnecessary dynamic allocation.
- Review RAM, stack, flash, and timing impact for every change.
- Avoid long blocking delays when a timer or event-based approach is suitable.
- Protect data shared between interrupt and main contexts.
- Validate CAN receive availability and payload length before reading data.
- Bound CAN transmit length to the hardware data capacity.
- Confirm oscillator frequency before calculating ADC timing, CAN bitrate, UART baud, timer periods, or PWM output.
- Treat the ADC scaling formula as a hardware-dependent calibration rule, not a universal conversion.
- Treat RC0–RC2 as stage-specific assignments: LCD control in `e3.X`, digital inputs in `e1.X`/`e2.X`.
- Keep LCD register access inside the CLCD interface in `e3.X`.

## Library Rules

1. Prefer a project-level wrapper around external libraries.
2. Keep application code independent of library-specific implementation where practical.
3. Do not modify library source unless necessary.
4. If modification is necessary, document the reason, exact change, affected module, and verification result in `REQUIREMENTS.md` and `PROGRESS.md`.
5. Do not add a new dependency when existing project functionality can reasonably satisfy the requirement.

## Verification Before Handoff

- Build every affected `.X` project with the configured XC8 toolchain.
- Inspect warnings and generated artifacts.
- Check changed pin assignments against the source and device configuration.
- Check CAN IDs and payload lengths at both producer and receiver.
- Test hardware behavior when hardware is available; otherwise label results `TBD` or `PENDING`.
- Review all five Markdown files for contradictions before completing the task.

