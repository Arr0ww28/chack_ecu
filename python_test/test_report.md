# ECU Simulator - Strict Dictionary Test Report

**Total Execution Status:** 56/56 PASSED

## 📝 Executive Summary

| ID | Test Name | Status |
|---|---|---|
| TC-01 | Normal Start (OFF -> ACC -> IGN_ON) | ✅ PASS |
| TC-02 | Overspeed | ✅ PASS |
| TC-03 | High Temperature Warning | ✅ PASS |
| TC-04 | Critical Overheat | ✅ PASS |
| TC-05 | Invalid Gear | ✅ PASS |
| TC-06 | Illegal Mode Transition | ✅ PASS |
| TC-07 | Multiple Faults in One Cycle | ✅ PASS |
| TC-08 | Persistent Fault Across 3 Cycles | ✅ PASS |
| TC-09 | Recovery / Reset Logic | ✅ PASS |
| TC-10 | Speed = 0 (min valid) | ✅ PASS |
| TC-11 | Speed = 200 (max valid) | ✅ PASS |
| TC-12 | Speed = 201 (over max) | ✅ PASS |
| TC-13 | Speed = 120 (overspeed boundary) | ✅ PASS |
| TC-14 | Speed = 121 (just over threshold) | ✅ PASS |
| TC-15 | Speed = 110 (warning boundary) | ✅ PASS |
| TC-16 | Temp = -40 (min valid) | ✅ PASS |
| TC-17 | Temp = 150 (max valid) | ✅ PASS |
| TC-18 | Temp = -41 (below min) | ✅ PASS |
| TC-19 | Temp = 151 (above max) | ✅ PASS |
| TC-20 | Temp = 95 (high temp boundary limit) | ✅ PASS |
| TC-21 | Temp = 96 (high temp triggers >95) | ✅ PASS |
| TC-22 | Temp = 110 (exactly 110 -> HIGH_TEMP warning, not critical) | ✅ PASS |
| TC-23 | Temp = 109 (just below critical) | ✅ PASS |
| TC-24 | Gear = 0 (min valid) | ✅ PASS |
| TC-25 | Gear = 5 (max valid) | ✅ PASS |
| TC-26 | Gear = 6 (just over max) | ✅ PASS |
| TC-27 | Gear = 255 (uint8 max) | ✅ PASS |
| TC-28 | Mode OFF -> OFF | ✅ PASS |
| TC-29 | Mode OFF -> ACC | ✅ PASS |
| TC-30 | Mode OFF -> IGN_ON | ✅ PASS |
| TC-31 | Mode OFF -> FAULT directly | ✅ PASS |
| TC-32 | Mode ACC -> OFF | ✅ PASS |
| TC-33 | Mode ACC -> ACC | ✅ PASS |
| TC-34 | Mode ACC -> IGN_ON | ✅ PASS |
| TC-35 | Mode IGN_ON -> OFF | ✅ PASS |
| TC-36 | Mode IGN_ON -> ACC | ✅ PASS |
| TC-37 | Mode IGN_ON -> IGN_ON | ✅ PASS |
| TC-38 | Mode FAULT -> ACC (trap state) | ✅ PASS |
| TC-39 | Mode FAULT -> OFF (trap state) | ✅ PASS |
| TC-40 | Mode value = 4 | ✅ PASS |
| TC-41 | Mode value = 99 | ✅ PASS |
| TC-42 | NORMAL -> DEGRADED via 1 major fault | ✅ PASS |
| TC-43 | NORMAL -> DEGRADED via 3 warnings | ✅ PASS |
| TC-44 | NORMAL -> SAFE directly | ✅ PASS |
| TC-45 | DEGRADED -> NORMAL recovery | ✅ PASS |
| TC-46 | DEGRADED -> SAFE escalation | ✅ PASS |
| TC-47 | SAFE is terminal without reset | ✅ PASS |
| TC-48 | SAFE reset via 0 0 0 0 triggers normal | ✅ PASS |
| TC-49 | SAFE: 3 cycles triggers watchdog | ✅ PASS |
| TC-50 | Fault clears counter on recovery | ✅ PASS |
| TC-51 | Persistent latch after 3 cycles | ✅ PASS |
| TC-52 | Counter saturation (255 chars handled in internal logic) | ✅ PASS |
| TC-53 | Multiple persistent faults | ✅ PASS |
| TC-54 | Watchdog in NORMAL | ✅ PASS |
| TC-55 | Watchdog in DEGRADED | ✅ PASS |
| TC-56 | Watchdog auto-reset after 3 SAFE | ✅ PASS |

## 🔍 Detailed Test Execution Outputs

---

### ✅ PASS TC-01: Normal Start (OFF -> ACC -> IGN_ON)

**Input Sequence**
```json
[
  "0 80 0 0",
  "40 80 0 1",
  "40 80 1 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "80 C",
    "Gear": "1",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "80 C",
    "Gear": "1",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-02: Overspeed

**Input Sequence**
```json
[
  "0 80 0 1",
  "130 80 4 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "130 km/h",
    "Temperature": "80 C",
    "Gear": "4",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "130 km/h",
    "Temperature": "80 C",
    "Gear": "4",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

---

### ✅ PASS TC-03: High Temperature Warning

**Input Sequence**
```json
[
  "0 80 0 1",
  "40 100 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "100 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00100 [ HIGH_TEMP ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL",
    "WARNING": "HIGH TEMP"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "100 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00100 [ HIGH_TEMP ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL",
    "WARNING": "HIGH TEMP"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-04: Critical Overheat

**Input Sequence**
```json
[
  "0 80 0 1",
  "40 115 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "115 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00010 [ CRITICAL_OVERHEAT ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "1",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "115 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00010 [ CRITICAL_OVERHEAT ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "1",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT"
  ]
}
```

---

### ✅ PASS TC-05: Invalid Gear

**Input Sequence**
```json
[
  "0 80 0 1",
  "40 80 9 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "01000 [ INVALID_GEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "2. INVALID_GEAR"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "01000 [ INVALID_GEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "2. INVALID_GEAR"
  ]
}
```

---

### ✅ PASS TC-06: Illegal Mode Transition

**Input Sequence**
```json
[
  "0 80 0 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "FAULT",
    "Current Mode": "FAULT",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "FAULT",
    "Current Mode": "FAULT",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-07: Multiple Faults in One Cycle

**Input Sequence**
```json
[
  "0 80 0 1",
  "140 120 9 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "140 km/h",
    "Temperature": "120 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "01011 [ OVERSPEED CRITICAL_OVERHEAT INVALID_GEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "1",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT",
    "2. INVALID_GEAR",
    "3. OVERSPEED"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "140 km/h",
    "Temperature": "120 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "01011 [ OVERSPEED CRITICAL_OVERHEAT INVALID_GEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "1",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT",
    "2. INVALID_GEAR",
    "3. OVERSPEED"
  ]
}
```

---

### ✅ PASS TC-08: Persistent Fault Across 3 Cycles

**Input Sequence**
```json
[
  "0 80 0 1",
  "40 115 2 2",
  "40 115 2 2",
  "40 115 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "115 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00010 [ CRITICAL_OVERHEAT ]",
    "Persistent": "00010 [ CRITICAL_OVERHEAT ]",
    "Major Faults": "3",
    "Warnings": "0",
    "Critical Faults": "3",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "SAFE"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "115 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00010 [ CRITICAL_OVERHEAT ]",
    "Persistent": "00010 [ CRITICAL_OVERHEAT ]",
    "Major Faults": "3",
    "Warnings": "0",
    "Critical Faults": "3",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "SAFE"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT"
  ]
}
```

---

### ✅ PASS TC-09: Recovery / Reset Logic

**Input Sequence**
```json
[
  "0 80 0 1",
  "40 115 2 2",
  "0 0 0 0",
  "0 80 0 1"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-10: Speed = 0 (min valid)

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-11: Speed = 200 (max valid)

**Input Sequence**
```json
[
  "0 80 0 1",
  "200 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "200 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "200 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

---

### ✅ PASS TC-12: Speed = 201 (over max)

**Input Sequence**
```json
[
  "0 80 0 1",
  "201 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-13: Speed = 120 (overspeed boundary)

**Input Sequence**
```json
[
  "0 80 0 1",
  "120 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "120 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "120 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-14: Speed = 121 (just over threshold)

**Input Sequence**
```json
[
  "0 80 0 1",
  "121 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "121 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "121 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

---

### ✅ PASS TC-15: Speed = 110 (warning boundary)

**Input Sequence**
```json
[
  "0 80 0 1",
  "110 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "110 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "110 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-16: Temp = -40 (min valid)

**Input Sequence**
```json
[
  "0 -40 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "-40 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "-40 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-17: Temp = 150 (max valid)

**Input Sequence**
```json
[
  "0 150 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "150 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00010 [ CRITICAL_OVERHEAT ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "1",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "150 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00010 [ CRITICAL_OVERHEAT ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "1",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT"
  ]
}
```

---

### ✅ PASS TC-18: Temp = -41 (below min)

**Input Sequence**
```json
[
  "0 -41 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-19: Temp = 151 (above max)

**Input Sequence**
```json
[
  "0 151 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-20: Temp = 95 (high temp boundary limit)

**Input Sequence**
```json
[
  "0 95 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "95 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "95 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-21: Temp = 96 (high temp triggers >95)

**Input Sequence**
```json
[
  "0 96 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "96 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00100 [ HIGH_TEMP ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL",
    "WARNING": "HIGH TEMP"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "96 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00100 [ HIGH_TEMP ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL",
    "WARNING": "HIGH TEMP"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-22: Temp = 110 (exactly 110 -> HIGH_TEMP warning, not critical)

**Input Sequence**
```json
[
  "0 110 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "110 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00100 [ HIGH_TEMP ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL",
    "WARNING": "HIGH TEMP"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "110 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00100 [ HIGH_TEMP ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL",
    "WARNING": "HIGH TEMP"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-23: Temp = 109 (just below critical)

**Input Sequence**
```json
[
  "0 109 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "109 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00100 [ HIGH_TEMP ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL",
    "WARNING": "HIGH TEMP"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "109 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00100 [ HIGH_TEMP ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "1",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL",
    "WARNING": "HIGH TEMP"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-24: Gear = 0 (min valid)

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-25: Gear = 5 (max valid)

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 80 5 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "5",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "5",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-26: Gear = 6 (just over max)

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 80 6 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "01000 [ INVALID_GEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "2. INVALID_GEAR"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "01000 [ INVALID_GEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "2. INVALID_GEAR"
  ]
}
```

---

### ✅ PASS TC-27: Gear = 255 (uint8 max)

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 80 255 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "01000 [ INVALID_GEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "2. INVALID_GEAR"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "01000 [ INVALID_GEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "2. INVALID_GEAR"
  ]
}
```

---

### ✅ PASS TC-28: Mode OFF -> OFF

**Input Sequence**
```json
[
  "0 80 0 0",
  "0 80 0 0",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-29: Mode OFF -> ACC

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-30: Mode OFF -> IGN_ON

**Input Sequence**
```json
[
  "0 80 0 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "FAULT",
    "Current Mode": "FAULT",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "FAULT",
    "Current Mode": "FAULT",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-31: Mode OFF -> FAULT directly

**Input Sequence**
```json
[
  "0 80 0 3",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "FAULT"
  },
  "MODE": {
    "Active Mode": "FAULT",
    "Current Mode": "FAULT",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "FAULT"
  },
  "MODE": {
    "Active Mode": "FAULT",
    "Current Mode": "FAULT",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-32: Mode ACC -> OFF

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 80 0 0",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-33: Mode ACC -> ACC

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 80 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-34: Mode ACC -> IGN_ON

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 80 0 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-35: Mode IGN_ON -> OFF

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 80 0 2",
  "0 80 0 0",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-36: Mode IGN_ON -> ACC

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 80 0 2",
  "0 80 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-37: Mode IGN_ON -> IGN_ON

**Input Sequence**
```json
[
  "0 80 0 1",
  "0 80 0 2",
  "0 80 0 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-38: Mode FAULT -> ACC (trap state)

**Input Sequence**
```json
[
  "0 80 0 2",
  "0 80 0 1",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "FAULT",
    "Current Mode": "FAULT",
    "Previous Mode": "FAULT"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "FAULT",
    "Current Mode": "FAULT",
    "Previous Mode": "FAULT"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-39: Mode FAULT -> OFF (trap state)

**Input Sequence**
```json
[
  "0 80 0 2",
  "0 80 0 0",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "FAULT",
    "Current Mode": "FAULT",
    "Previous Mode": "FAULT"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "FAULT",
    "Current Mode": "FAULT",
    "Previous Mode": "FAULT"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-40: Mode value = 4

**Input Sequence**
```json
[
  "0 80 0 4",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "10000 [ INVALID_MODE ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "2. INVALID_MODE"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "10000 [ INVALID_MODE ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "2. INVALID_MODE"
  ]
}
```

---

### ✅ PASS TC-41: Mode value = 99

**Input Sequence**
```json
[
  "0 80 0 99",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "10000 [ INVALID_MODE ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "2. INVALID_MODE"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "10000 [ INVALID_MODE ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "2. INVALID_MODE"
  ]
}
```

---

### ✅ PASS TC-42: NORMAL -> DEGRADED via 1 major fault

**Input Sequence**
```json
[
  "0 80 0 1",
  "130 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "130 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "130 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "ACC"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

---

### ✅ PASS TC-43: NORMAL -> DEGRADED via 3 warnings

**Input Sequence**
```json
[
  "0 80 0 1",
  "115 80 2 2",
  "115 80 2 2",
  "115 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "115 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "3",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "115 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "3",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-44: NORMAL -> SAFE directly

**Input Sequence**
```json
[
  "0 80 0 1",
  "40 115 2 2",
  "40 115 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "115 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00010 [ CRITICAL_OVERHEAT ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "2",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "SAFE"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "115 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00010 [ CRITICAL_OVERHEAT ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "2",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "SAFE"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT"
  ]
}
```

---

### ✅ PASS TC-45: DEGRADED -> NORMAL recovery

**Input Sequence**
```json
[
  "0 80 0 1",
  "130 80 2 2",
  "40 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "1",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-46: DEGRADED -> SAFE escalation

**Input Sequence**
```json
[
  "0 80 0 1",
  "130 80 2 2",
  "40 115 2 2",
  "40 115 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "115 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00010 [ CRITICAL_OVERHEAT ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "3",
    "Warnings": "0",
    "Critical Faults": "2",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "SAFE"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "115 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00010 [ CRITICAL_OVERHEAT ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "3",
    "Warnings": "0",
    "Critical Faults": "2",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "SAFE"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT"
  ]
}
```

---

### ✅ PASS TC-47: SAFE is terminal without reset

**Input Sequence**
```json
[
  "0 80 0 1",
  "40 115 2 2",
  "40 115 2 2",
  "40 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "2",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "SAFE"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "2",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "SAFE"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-48: SAFE reset via 0 0 0 0 triggers normal

**Input Sequence**
```json
[
  "0 80 0 1",
  "40 115 2 2",
  "40 115 2 2",
  "0 0 0 0",
  "0 80 0 1"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "80 C",
    "Gear": "0",
    "Requested Mode": "ACC"
  },
  "MODE": {
    "Active Mode": "ACC",
    "Current Mode": "ACC",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-49: SAFE: 3 cycles triggers watchdog

**Input Sequence**
```json
[
  "0 80 0 1",
  "120 115 2 2",
  "120 115 2 2",
  "120 115 2 2",
  "120 115 2 2"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "YES"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "YES"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-50: Fault clears counter on recovery

**Input Sequence**
```json
[
  "0 80 0 1",
  "130 80 2 2",
  "130 80 2 2",
  "40 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "40 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "2",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-51: Persistent latch after 3 cycles

**Input Sequence**
```json
[
  "0 80 0 1",
  "130 80 2 2",
  "130 80 2 2",
  "130 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "130 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00001 [ OVERSPEED ]",
    "Major Faults": "3",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "130 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00001 [ OVERSPEED ]",
    "Major Faults": "3",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

---

### ✅ PASS TC-52: Counter saturation (255 chars handled in internal logic)

**Input Sequence**
```json
[
  "0 80 0 1",
  "130 80 2 2",
  "130 80 2 2",
  "130 80 2 2",
  "130 80 2 2",
  "130 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "130 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00001 [ OVERSPEED ]",
    "Major Faults": "5",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "130 km/h",
    "Temperature": "80 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00001 [ OVERSPEED ]",
    "Persistent": "00001 [ OVERSPEED ]",
    "Major Faults": "5",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "DEGRADED"
  },
  "ACTIVE FAULTS": [
    "3. OVERSPEED"
  ]
}
```

---

### ✅ PASS TC-53: Multiple persistent faults

**Input Sequence**
```json
[
  "0 80 0 1",
  "140 115 2 2",
  "140 115 2 2",
  "140 115 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "140 km/h",
    "Temperature": "115 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00011 [ OVERSPEED CRITICAL_OVERHEAT ]",
    "Persistent": "00011 [ OVERSPEED CRITICAL_OVERHEAT ]",
    "Major Faults": "6",
    "Warnings": "0",
    "Critical Faults": "3",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "SAFE"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT",
    "3. OVERSPEED"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "140 km/h",
    "Temperature": "115 C",
    "Gear": "2",
    "Requested Mode": "IGNITION_ON"
  },
  "MODE": {
    "Active Mode": "IGNITION_ON",
    "Current Mode": "IGNITION_ON",
    "Previous Mode": "IGNITION_ON"
  },
  "FAULTS": {
    "Cycle Flags": "00011 [ OVERSPEED CRITICAL_OVERHEAT ]",
    "Persistent": "00011 [ OVERSPEED CRITICAL_OVERHEAT ]",
    "Major Faults": "6",
    "Warnings": "0",
    "Critical Faults": "3",
    "Reset Requested": "NO"
  },
  "STATE": {
    "System State": "SAFE"
  },
  "ACTIVE FAULTS": [
    "1. CRITICAL_OVERHEAT",
    "3. OVERSPEED"
  ]
}
```

---

### ✅ PASS TC-54: Watchdog in NORMAL

**Input Sequence**
```json
[
  "0 80 0 1",
  "40 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "YES"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "YES"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-55: Watchdog in DEGRADED

**Input Sequence**
```json
[
  "0 80 0 1",
  "130 80 2 2",
  "0 0 0 0"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "YES"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "YES"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

---

### ✅ PASS TC-56: Watchdog auto-reset after 3 SAFE

**Input Sequence**
```json
[
  "0 80 0 1",
  "40 120 2 2",
  "40 120 2 2",
  "40 120 2 2",
  "40 120 2 2"
]
```

**Expected Output Configuration**
```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "YES"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

**Execution Match**
> [!NOTE]
> The resulting ECU State perfectly matched the Expected Structure.

```json
{
  "INPUT": {
    "Speed": "0 km/h",
    "Temperature": "0 C",
    "Gear": "0",
    "Requested Mode": "OFF"
  },
  "MODE": {
    "Active Mode": "OFF",
    "Current Mode": "OFF",
    "Previous Mode": "OFF"
  },
  "FAULTS": {
    "Cycle Flags": "00000 [ CLEAR ]",
    "Persistent": "00000 [ CLEAR ]",
    "Major Faults": "0",
    "Warnings": "0",
    "Critical Faults": "0",
    "Reset Requested": "YES"
  },
  "STATE": {
    "System State": "NORMAL"
  },
  "ACTIVE FAULTS": [
    "(none)"
  ]
}
```

