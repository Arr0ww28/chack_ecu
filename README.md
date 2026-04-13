# Vehicle ECU Simulator

A modular, cyclic Electronic Control Unit (ECU) simulator written in C. This project simulates the core processing pipeline of a vehicle ECU, enforcing safety thresholds, processing sensor inputs, managing vehicle modes, and evaluating system states based on fault persistence.

## System Architecture

The simulator is built using a strict modular architecture, mimicking embedded system standards. The processing pipeline runs cyclically in `main.c`, stepping through the following modules:

1. **`input`**: Reads raw data (Speed, Temperature, Gear, Mode) and validates sensor bounds.
2. **`mode`**: Manages state-machine transitions (e.g., `OFF` → `ACC` → `IGN_ON`). It rejects illegal transitions and prevents unsafe jumps.
3. **`control`**: The business logic layer. Evaluates physical driving parameters against static safety thresholds (e.g., Overspeed, Critical Overheat).
4. **`fault`**: Tracks the persistence of errors. A warning must occur repeatedly across consecutive cycles before it is "latched" as a persistent fault.
5. **`state`**: The supreme safety evaluator. Escalates the overall vehicle state (`NORMAL` → `DEGRADED` → `SAFE`) based on accumulated fault counts.
6. **`log`**: Formats and prints a detailed diagnostic summary at the end of every cycle.

## Compilation & Execution

This project is written cleanly in ISO C99.

### Requirements
- GCC Compiler
- Linux/macOS Terminal or Windows Subsystem for Linux (WSL)

### Compiling
Use the following strict compiler flags to build the simulator:

```bash
gcc -Wall -Wextra -pedantic -std=c99 -Iinclude -o ecu_sim src/main.c src/input.c src/mode.c src/control.c src/fault.c src/state.c src/log.c
```

### Running
```bash
./ecu_sim
```
## CMake Guide

cmake -G "MinGW Makefiles" ..
cmake --build .

## Usage Guide

Once running, the simulator runs an infinite loop representing the ECU cycle. It will prompt you for physical inputs in the following format:

**Format:** `[Speed] [Temp] [Gear] [Mode]`
*(Modes are mapped as: `0=OFF`, `1=ACC`, `2=IGN_ON`, `3=FAULT`)*

### Example Walkthrough (The 6 Test Cases)

The system enforces smooth mode transitions. To start the vehicle safely, you must escalate modes one by one.

**1. Normal Start-Up**
You must go from OFF `0` to ACC `1` before turning the ignition on `2`.
* Cycle 1: `0 80 0 1` (Transition to ACC)
* Cycle 2: `40 80 1 2` (Ignition ON, driving parameters normal)

**2. Overspeed Condition**
* Cycle 3: `130 85 4 2`
* System detects overspeed, logs `OVERSPEED` fault bit.

**3. High Temperature Warning**
* Cycle 4: `40 100 4 2`
* System detects high temp, logs `HIGH_TEMP`, does not crash.

**4. Critical Overheat**
* Cycle 5: `40 115 4 2`
* System detects critical heat, logs `CRITICAL_OVERHEAT`, escalates System State.

**5. Invalid Gear**
* Cycle 6: `40 80 9 2`
* System identifies gear `9` as strictly invalid, logs `INVALID_GEAR`.

**6. Illegal Mode Transition**
* Restart the simulator (Ctrl+C, then run again).
* Try jumping from OFF immediately to IGN_ON: `0 0 0 2`
* System blocks the transition and forces active mode into `FAULT`.

## Repository Structure
```text
.
├── include/           # Header files exposing module APIs
│   ├── types.h        # Centralised structs, enums, and bitmasks
│   ├── input.h
│   ├── mode.h
│   ├── control.h
│   ├── fault.h
│   ├── state.h
│   └── log.h
├── src/               # Implementations
│   ├── main.c         # The cyclic execution pipeline
│   └── ...            # C implementations for each module
└── README.md
```
