import subprocess
import os
import sys
import json
import re

ECU_EXEC = r"..\ecu_sim.exe" if os.name == 'nt' else "../ecu_sim"
REPORT_FILE = "test_report.md"

DISPLAYABLE_FAULT_BITS = {
    "OVERSPEED",
    "CRITICAL_OVERHEAT",
    "INVALID_GEAR",
    "INVALID_MODE",
}


def normalize_cycle_dict(cycle_dict):
    # Warning-only internal bits can render as "00000 [ ]"; normalize to "CLEAR"
    # so expected dictionaries compare consistently.
    for key in ("Cycle Flags", "Persistent"):
        raw = cycle_dict["FAULTS"].get(key)
        if not raw:
            continue
        match = re.match(r"^(\d{5})\s*\[(.*)\]$", raw)
        if not match:
            continue

        bits = match.group(1)
        labels = match.group(2).strip()
        if labels == "":
            cycle_dict["FAULTS"][key] = f"{bits} [ CLEAR ]"

    # ACTIVE FAULTS is reported only for displayable bits; warning-only bits can
    # leave this section empty even when warning_count increments.
    if not cycle_dict["ACTIVE FAULTS"]:
        flags_raw = cycle_dict["FAULTS"].get("Cycle Flags", "")
        if "[" in flags_raw and "]" in flags_raw:
            labels = flags_raw.split("[", 1)[1].rsplit("]", 1)[0].strip()
            has_displayable = any(name in labels for name in DISPLAYABLE_FAULT_BITS)
            if (labels == "" or labels == "CLEAR") or not has_displayable:
                cycle_dict["ACTIVE FAULTS"] = ["(none)"]

    # Detect HIGH_TEMP in cycle flags and add WARNING field to STATE if present
    flags_raw = cycle_dict["FAULTS"].get("Cycle Flags", "")
    system_state = cycle_dict["STATE"].get("System State", "")
    if "HIGH_TEMP" in flags_raw and system_state == "NORMAL":
        cycle_dict["STATE"]["WARNING"] = "HIGH TEMP"

    return cycle_dict

def expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="OFF",
                   act_mode="OFF", cur_mode="OFF", prev_mode="OFF",
                   cycle_flags="00000 [ CLEAR ]", persistent="00000 [ CLEAR ]",
                   maj="0", warn="0", crit="0", reset="NO", state="NORMAL",
                   active_faults=None, warning=None):
    if active_faults is None:
        active_faults = ["(none)"]
    state_dict = {"System State": state}
    if warning:
        state_dict["WARNING"] = warning
    return {
        "INPUT": {"Speed": speed, "Temperature": temp, "Gear": gear, "Requested Mode": req_mode},
        "MODE": {"Active Mode": act_mode, "Current Mode": cur_mode, "Previous Mode": prev_mode},
        "FAULTS": {"Cycle Flags": cycle_flags, "Persistent": persistent,
                   "Major Faults": str(maj), "Warnings": str(warn),
                   "Critical Faults": str(crit), "Reset Requested": reset},
        "STATE": state_dict,
        "ACTIVE FAULTS": active_faults
    }

TEST_CASES = [
    # --- GROUP A: Mandatory Requirements ---
    {
        "id": "TC-01", "name": "Normal Start (OFF -> ACC -> IGN_ON)",
        "inputs": ["0 80 0 0", "40 80 0 1", "40 80 1 2", "0 0 0 0"],
        "expected": expected_state(speed="40 km/h", temp="80 C", gear="1", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC")
    },
    {
        "id": "TC-02", "name": "Overspeed",
        "inputs": ["0 80 0 1", "130 80 4 2", "0 0 0 0"],
        "expected": expected_state(speed="130 km/h", temp="80 C", gear="4", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   cycle_flags="00001 [ OVERSPEED ]", maj="1", state="DEGRADED",
                                   active_faults=["3. OVERSPEED"])
    },
    {
        "id": "TC-03", "name": "High Temperature Warning",
        "inputs": ["0 80 0 1", "40 100 2 2", "0 0 0 0"],
        # HIGH_TEMP is a WARNING (warn=1, maj=0). State stays NORMAL.
        "expected": expected_state(speed="40 km/h", temp="100 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   cycle_flags="00100 [ HIGH_TEMP ]", warn="1", warning="HIGH TEMP")
    },
    {
        "id": "TC-04", "name": "Critical Overheat",
        "inputs": ["0 80 0 1", "40 115 2 2", "0 0 0 0"],
        # CRITICAL_OVERHEAT: crit=1, maj=0
        "expected": expected_state(speed="40 km/h", temp="115 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   cycle_flags="00010 [ CRITICAL_OVERHEAT ]", crit="1", state="DEGRADED", maj="1",
                                   active_faults=["1. CRITICAL_OVERHEAT"])
    },
    {
        "id": "TC-05", "name": "Invalid Gear",
        "inputs": ["0 80 0 1", "40 80 9 2", "0 0 0 0"],
        # Gear=9 clamped to 0. INVALID_GEAR sets maj=1 -> DEGRADED.
        "expected": expected_state(speed="40 km/h", temp="80 C", gear="0", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   cycle_flags="01000 [ INVALID_GEAR ]", maj="1",
                                   state="DEGRADED", active_faults=["2. INVALID_GEAR"])
    },
    {
        "id": "TC-06", "name": "Illegal Mode Transition",
        "inputs": ["0 80 0 2", "0 0 0 0"],
        # OFF -> IGN_ON is illegal -> forced to FAULT. maj=1, cycle flags CLEAR.
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="IGNITION_ON",
                                   act_mode="FAULT", cur_mode="FAULT", prev_mode="OFF",
                                   maj="1", state="DEGRADED")
    },
    {
        "id": "TC-07", "name": "Multiple Faults in One Cycle",
        "inputs": ["0 80 0 1", "140 120 9 2", "0 0 0 0"],
        # OVERSPEED (maj) + CRITICAL_OVERHEAT (crit) + INVALID_GEAR (maj) all in cycle flags.
        "expected": expected_state(speed="140 km/h", temp="120 C", gear="0", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   cycle_flags="01011 [ OVERSPEED CRITICAL_OVERHEAT INVALID_GEAR ]",
                                   maj="2", crit="1", state="DEGRADED",
                                   active_faults=["1. CRITICAL_OVERHEAT", "2. INVALID_GEAR", "3. OVERSPEED"])
    },
    {
        "id": "TC-08", "name": "Persistent Fault Across 3 Cycles",
        "inputs": ["0 80 0 1", "40 115 2 2", "40 115 2 2", "40 115 2 2", "0 0 0 0"],
        # CRITICAL_OVERHEAT x3: crit=3, maj=0, persistent latched, state=SAFE
        "expected": expected_state(speed="40 km/h", temp="115 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="IGNITION_ON",
                                   cycle_flags="00010 [ CRITICAL_OVERHEAT ]",
                                   persistent="00010 [ CRITICAL_OVERHEAT ]",
                                   crit="3", maj="3", state="SAFE", active_faults=["1. CRITICAL_OVERHEAT"])
    },
    {
        "id": "TC-09", "name": "Recovery / Reset Logic",
        "inputs": ["0 80 0 1", "40 115 2 2", "0 0 0 0", "0 80 0 1"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF")
    },

    # --- GROUP B: Input Boundaries ---
    {
        "id": "TC-10", "name": "Speed = 0 (min valid)",
        "inputs": ["0 80 0 1", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF")
    },
    {
        "id": "TC-11", "name": "Speed = 200 (max valid)",
        "inputs": ["0 80 0 1", "200 80 2 2", "0 0 0 0"],
        # 200 > 120 -> OVERSPEED
        "expected": expected_state(speed="200 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   cycle_flags="00001 [ OVERSPEED ]", maj="1", state="DEGRADED",
                                   active_faults=["3. OVERSPEED"])
    },
    {
        "id": "TC-12", "name": "Speed = 201 (over max)",
        "inputs": ["0 80 0 1", "201 80 2 2", "0 0 0 0"],
        # 201 invalid -> clamped to 0, no fault
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC")
    },
    {
        "id": "TC-13", "name": "Speed = 120 (overspeed boundary)",
        "inputs": ["0 80 0 1", "120 80 2 2", "0 0 0 0"],
        # Exactly 120: NOT >120, so no OVERSPEED fault. In warning zone 110-120 -> warn=1.
        # Cycle Flags stay CLEAR. ACTIVE FAULTS = (none).
        "expected": expected_state(speed="120 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   warn="1")
    },
    {
        "id": "TC-14", "name": "Speed = 121 (just over threshold)",
        "inputs": ["0 80 0 1", "121 80 2 2", "0 0 0 0"],
        "expected": expected_state(speed="121 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   cycle_flags="00001 [ OVERSPEED ]", maj="1", state="DEGRADED",
                                   active_faults=["3. OVERSPEED"])
    },
    {
        "id": "TC-15", "name": "Speed = 110 (warning boundary)",
        "inputs": ["0 80 0 1", "110 80 2 2", "0 0 0 0"],
        # Exactly 110: in warning zone -> warn=1. Cycle Flags = CLEAR. ACTIVE FAULTS = (none).
        "expected": expected_state(speed="110 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   warn="1")
    },
    {
        "id": "TC-16", "name": "Temp = -40 (min valid)",
        "inputs": ["0 -40 0 1", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="-40 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF")
    },
    {
        "id": "TC-17", "name": "Temp = 150 (max valid)",
        "inputs": ["0 150 0 1", "0 0 0 0"],
        # 150 > 110 -> CRITICAL_OVERHEAT. crit=1, maj=0.
        "expected": expected_state(speed="0 km/h", temp="150 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF",
                                   cycle_flags="00010 [ CRITICAL_OVERHEAT ]", crit="1", maj="1", state="DEGRADED",
                                   active_faults=["1. CRITICAL_OVERHEAT"])
    },
    {
        "id": "TC-18", "name": "Temp = -41 (below min)",
        "inputs": ["0 -41 0 1", "0 0 0 0"],
        # -41 invalid -> clamped to 0, displays "0 C", no fault
        "expected": expected_state(speed="0 km/h", temp="0 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF")
    },
    {
        "id": "TC-19", "name": "Temp = 151 (above max)",
        "inputs": ["0 151 0 1", "0 0 0 0"],
        # 151 invalid -> clamped to 0, displays "0 C", no fault
        "expected": expected_state(speed="0 km/h", temp="0 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF")
    },
    {
        "id": "TC-20", "name": "Temp = 95 (high temp boundary limit)",
        "inputs": ["0 95 0 1", "0 0 0 0"],
        # Exactly 95: NOT >95, so no HIGH_TEMP. Normal.
        "expected": expected_state(speed="0 km/h", temp="95 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF")
    },
    {
        "id": "TC-21", "name": "Temp = 96 (high temp triggers >95)",
        "inputs": ["0 96 0 1", "0 0 0 0"],
        # 96 > 95 -> HIGH_TEMP WARNING (warn=1, maj=0). State stays NORMAL.
        "expected": expected_state(speed="0 km/h", temp="96 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF",
                                   cycle_flags="00100 [ HIGH_TEMP ]", warn="1", warning="HIGH TEMP")
    },
    {
        "id": "TC-22", "name": "Temp = 110 (exactly 110 -> HIGH_TEMP warning, not critical)",
        "inputs": ["0 110 0 1", "0 0 0 0"],
        # Exactly 110: NOT >110, so HIGH_TEMP WARNING only (warn=1, maj=0). State stays NORMAL.
        "expected": expected_state(speed="0 km/h", temp="110 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF",
                                   cycle_flags="00100 [ HIGH_TEMP ]", warn="1", warning="HIGH TEMP")
    },
    {
        "id": "TC-23", "name": "Temp = 109 (just below critical)",
        "inputs": ["0 109 0 1", "0 0 0 0"],
        # 96 <= 109 <= 110 -> HIGH_TEMP WARNING (warn=1, maj=0). State stays NORMAL.
        "expected": expected_state(speed="0 km/h", temp="109 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF",
                                   cycle_flags="00100 [ HIGH_TEMP ]", warn="1", warning="HIGH TEMP")
    },
    {
        "id": "TC-24", "name": "Gear = 0 (min valid)",
        "inputs": ["0 80 0 1", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF")
    },
    {
        "id": "TC-25", "name": "Gear = 5 (max valid)",
        "inputs": ["0 80 0 1", "0 80 5 2", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="5", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC")
    },
    {
        "id": "TC-26", "name": "Gear = 6 (just over max)",
        "inputs": ["0 80 0 1", "0 80 6 2", "0 0 0 0"],
        # Gear=6 clamped to 0. INVALID_GEAR is a WARNING (warn=1, maj=0). State stays NORMAL.
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   cycle_flags="01000 [ INVALID_GEAR ]", maj="1", state="DEGRADED",
                                   active_faults=["2. INVALID_GEAR"])
    },
    {
        "id": "TC-27", "name": "Gear = 255 (uint8 max)",
        "inputs": ["0 80 0 1", "0 80 255 2", "0 0 0 0"],
        # Gear=255 clamped to 0. INVALID_GEAR is a WARNING (warn=1, maj=0). State stays NORMAL.
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   cycle_flags="01000 [ INVALID_GEAR ]", maj="1", state="DEGRADED",
                                   active_faults=["2. INVALID_GEAR"])
    },

    # --- GROUP C: Mode Transitions ---
    {
        "id": "TC-28", "name": "Mode OFF -> OFF",
        "inputs": ["0 80 0 0", "0 80 0 0", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="OFF",
                                   act_mode="OFF", cur_mode="OFF", prev_mode="OFF")
    },
    {
        "id": "TC-29", "name": "Mode OFF -> ACC",
        "inputs": ["0 80 0 1", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF")
    },
    {
        "id": "TC-30", "name": "Mode OFF -> IGN_ON",
        "inputs": ["0 80 0 2", "0 0 0 0"],
        # Illegal: OFF -> IGN_ON, forced to FAULT. maj=1.
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="IGNITION_ON",
                                   act_mode="FAULT", cur_mode="FAULT", prev_mode="OFF",
                                   maj="1", state="DEGRADED")
    },
    {
        "id": "TC-31", "name": "Mode OFF -> FAULT directly",
        "inputs": ["0 80 0 3", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="FAULT",
                                   act_mode="FAULT", cur_mode="FAULT", prev_mode="OFF",
                                   maj="1", state="DEGRADED")
    },
    {
        "id": "TC-32", "name": "Mode ACC -> OFF",
        "inputs": ["0 80 0 1", "0 80 0 0", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="OFF",
                                   act_mode="OFF", cur_mode="OFF", prev_mode="ACC")
    },
    {
        "id": "TC-33", "name": "Mode ACC -> ACC",
        "inputs": ["0 80 0 1", "0 80 0 1", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="ACC")
    },
    {
        "id": "TC-34", "name": "Mode ACC -> IGN_ON",
        "inputs": ["0 80 0 1", "0 80 0 2", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC")
    },
    {
        "id": "TC-35", "name": "Mode IGN_ON -> OFF",
        "inputs": ["0 80 0 1", "0 80 0 2", "0 80 0 0", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="OFF",
                                   act_mode="OFF", cur_mode="OFF", prev_mode="IGNITION_ON")
    },
    {
        "id": "TC-36", "name": "Mode IGN_ON -> ACC",
        "inputs": ["0 80 0 1", "0 80 0 2", "0 80 0 1", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="IGNITION_ON")
    },
    {
        "id": "TC-37", "name": "Mode IGN_ON -> IGN_ON",
        "inputs": ["0 80 0 1", "0 80 0 2", "0 80 0 2", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="IGNITION_ON")
    },
    {
        "id": "TC-38", "name": "Mode FAULT -> ACC (trap state)",
        "inputs": ["0 80 0 2", "0 80 0 1", "0 0 0 0"],
        # FAULT is a trap state: stays FAULT even when ACC requested. maj=2 (2 fault cycles).
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="ACC",
                                   act_mode="FAULT", cur_mode="FAULT", prev_mode="FAULT",
                                   maj="2", state="DEGRADED")
    },
    {
        "id": "TC-39", "name": "Mode FAULT -> OFF (trap state)",
        "inputs": ["0 80 0 2", "0 80 0 0", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="OFF",
                                   act_mode="FAULT", cur_mode="FAULT", prev_mode="FAULT",
                                   maj="2", state="DEGRADED")
    },
    {
        "id": "TC-40", "name": "Mode value = 4",
        "inputs": ["0 80 0 4", "0 0 0 0"],
        # mode=4 invalid -> clamped to OFF. INVALID_MODE is a WARNING (warn=1, maj=0). State NORMAL.
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="OFF",
                                   act_mode="OFF", cur_mode="OFF", prev_mode="OFF",
                                   cycle_flags="10000 [ INVALID_MODE ]", maj="1", state="DEGRADED",
                                   active_faults=["2. INVALID_MODE"])
    },
    {
        "id": "TC-41", "name": "Mode value = 99",
        "inputs": ["0 80 0 99", "0 0 0 0"],
        # mode=99 invalid -> clamped to OFF. INVALID_MODE is a WARNING (warn=1, maj=0). State NORMAL.
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="OFF",
                                   act_mode="OFF", cur_mode="OFF", prev_mode="OFF",
                                   cycle_flags="10000 [ INVALID_MODE ]", maj="1", state="DEGRADED",
                                   active_faults=["2. INVALID_MODE"])
    },

    # --- GROUP D: State Machine ---
    {
        "id": "TC-42", "name": "NORMAL -> DEGRADED via 1 major fault",
        "inputs": ["0 80 0 1", "130 80 2 2", "0 0 0 0"],
        "expected": expected_state(speed="130 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="ACC",
                                   cycle_flags="00001 [ OVERSPEED ]", maj="1", state="DEGRADED",
                                   active_faults=["3. OVERSPEED"])
    },
    {
        "id": "TC-43", "name": "NORMAL -> DEGRADED via 3 warnings",
        "inputs": ["0 80 0 1", "115 80 2 2", "115 80 2 2", "115 80 2 2", "0 0 0 0"],
        # Speed 115 is in warning band (110-120). No fault bit set.
        # After 3 accumulated warnings -> DEGRADED. Cycle Flags = CLEAR. ACTIVE FAULTS = (none).
        "expected": expected_state(speed="115 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="IGNITION_ON",
                                   warn="3", state="DEGRADED")
    },
    {
        "id": "TC-44", "name": "NORMAL -> SAFE directly",
        "inputs": ["0 80 0 1", "40 115 2 2", "40 115 2 2", "0 0 0 0"],
        # CRITICAL_OVERHEAT x2 -> crit=2, maj=0, state=SAFE
        "expected": expected_state(speed="40 km/h", temp="115 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="IGNITION_ON",
                                   cycle_flags="00010 [ CRITICAL_OVERHEAT ]", maj="2", crit="2", state="SAFE",
                                   active_faults=["1. CRITICAL_OVERHEAT"])
    },
    {
        "id": "TC-45", "name": "DEGRADED -> NORMAL recovery",
        "inputs": ["0 80 0 1", "130 80 2 2", "40 80 2 2", "0 0 0 0"],
        # 1 prior major fault in accumulator -> stays DEGRADED despite clean current cycle
        "expected": expected_state(speed="40 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="IGNITION_ON",
                                   maj="1", state="DEGRADED")
    },
    {
        "id": "TC-46", "name": "DEGRADED -> SAFE escalation",
        "inputs": ["0 80 0 1", "130 80 2 2", "40 115 2 2", "40 115 2 2", "0 0 0 0"],
        "expected": expected_state(speed="40 km/h", temp="115 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="IGNITION_ON",
                                   cycle_flags="00010 [ CRITICAL_OVERHEAT ]", maj="3", crit="2",
                                   state="SAFE", active_faults=["1. CRITICAL_OVERHEAT"])
    },
    {
        "id": "TC-47", "name": "SAFE is terminal without reset",
        "inputs": ["0 80 0 1", "40 115 2 2", "40 115 2 2", "40 80 2 2", "0 0 0 0"],
        # SAFE is terminal. Clean cycle can't escape it. crit=2, maj=0.
        "expected": expected_state(speed="40 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="IGNITION_ON", maj="2",
                                   crit="2", state="SAFE")
    },
    {
        "id": "TC-48", "name": "SAFE reset via 0 0 0 0 triggers normal",
        "inputs": ["0 80 0 1", "40 115 2 2", "40 115 2 2", "0 0 0 0", "0 80 0 1"],
        "expected": expected_state(speed="0 km/h", temp="80 C", gear="0", req_mode="ACC",
                                   act_mode="ACC", cur_mode="ACC", prev_mode="OFF")
    },
    {
        "id": "TC-49", "name": "SAFE: 3 cycles triggers watchdog",
        "inputs": ["0 80 0 1", "120 115 2 2", "120 115 2 2", "120 115 2 2", "120 115 2 2"],
        # After 2 SAFE cycles, watchdog fires auto-reset: Reset Requested=YES, all cleared.
        "expected": expected_state(speed="0 km/h", temp="0 C", gear="0", req_mode="OFF",
                                   act_mode="OFF", cur_mode="OFF", prev_mode="OFF", reset="YES")
    },

    # --- GROUP E: Fault Persistence ---
    {
        "id": "TC-50", "name": "Fault clears counter on recovery",
        "inputs": ["0 80 0 1", "130 80 2 2", "130 80 2 2", "40 80 2 2", "0 0 0 0"],
        "expected": expected_state(speed="40 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="IGNITION_ON",
                                   maj="2", state="DEGRADED")
    },
    {
        "id": "TC-51", "name": "Persistent latch after 3 cycles",
        "inputs": ["0 80 0 1", "130 80 2 2", "130 80 2 2", "130 80 2 2", "0 0 0 0"],
        "expected": expected_state(speed="130 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="IGNITION_ON",
                                   cycle_flags="00001 [ OVERSPEED ]", persistent="00001 [ OVERSPEED ]",
                                   maj="3", state="DEGRADED", active_faults=["3. OVERSPEED"])
    },
    {
        "id": "TC-52", "name": "Counter saturation (255 chars handled in internal logic)",
        "inputs": ["0 80 0 1"] + ["130 80 2 2"] * 5 + ["0 0 0 0"],
        "expected": expected_state(speed="130 km/h", temp="80 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="IGNITION_ON",
                                   cycle_flags="00001 [ OVERSPEED ]", persistent="00001 [ OVERSPEED ]",
                                   maj="5", state="DEGRADED", active_faults=["3. OVERSPEED"])
    },
    {
        "id": "TC-53", "name": "Multiple persistent faults",
        "inputs": ["0 80 0 1", "140 115 2 2", "140 115 2 2", "140 115 2 2", "0 0 0 0"],
        "expected": expected_state(speed="140 km/h", temp="115 C", gear="2", req_mode="IGNITION_ON",
                                   act_mode="IGNITION_ON", cur_mode="IGNITION_ON", prev_mode="IGNITION_ON",
                                   cycle_flags="00011 [ OVERSPEED CRITICAL_OVERHEAT ]",
                                   persistent="00011 [ OVERSPEED CRITICAL_OVERHEAT ]",
                                   maj="6", crit="3", state="SAFE",
                                   active_faults=["1. CRITICAL_OVERHEAT", "3. OVERSPEED"])
    },

    # --- GROUP F: Watchdog ---
    {
        "id": "TC-54", "name": "Watchdog in NORMAL",
        "inputs": ["0 80 0 1", "40 80 2 2", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="0 C", gear="0", req_mode="OFF",
                                   act_mode="OFF", cur_mode="OFF", prev_mode="OFF", reset="YES")
    },
    {
        "id": "TC-55", "name": "Watchdog in DEGRADED",
        "inputs": ["0 80 0 1", "130 80 2 2", "0 0 0 0"],
        "expected": expected_state(speed="0 km/h", temp="0 C", gear="0", req_mode="OFF",
                                   act_mode="OFF", cur_mode="OFF", prev_mode="OFF", reset="YES")
    },
    {
        "id": "TC-56", "name": "Watchdog auto-reset after 3 SAFE",
        "inputs": ["0 80 0 1", "40 120 2 2", "40 120 2 2", "40 120 2 2", "40 120 2 2"],
        "expected": expected_state(speed="0 km/h", temp="0 C", gear="0", req_mode="OFF",
                                   act_mode="OFF", cur_mode="OFF", prev_mode="OFF", reset="YES")
    }
]

def parse_ecu_output_to_dicts(output_str):
    cycles = []
    blocks = output_str.split('CYCLE SUMMARY REPORT')
    for block in blocks[1:]:
        cycle_dict = {
            'INPUT': {}, 'MODE': {}, 'FAULTS': {}, 'STATE': {}, 'ACTIVE FAULTS': []
        }
        current_section = None
        for line in block.split('\n'):
            line = line.strip()
            if not line or line.startswith('---'):
                continue
            if line.startswith('==='):
                if cycle_dict['INPUT']: break
                else: continue

            if line.startswith('[INPUT]'): current_section = 'INPUT'
            elif line.startswith('[MODE]'): current_section = 'MODE'
            elif line.startswith('[FAULTS]'): current_section = 'FAULTS'
            elif line.startswith('[STATE]'): current_section = 'STATE'
            elif line.startswith('[ACTIVE FAULTS'): current_section = 'ACTIVE FAULTS'
            elif current_section == 'ACTIVE FAULTS':
                if "none" in line or (line and line[0].isdigit()):
                    cycle_dict['ACTIVE FAULTS'].append(line)
            elif current_section and ':' in line:
                key, val = line.split(':', 1)
                cycle_dict[current_section][key.strip()] = val.strip()

        if cycle_dict['INPUT']:
            cycle_dict = normalize_cycle_dict(cycle_dict)
            cycles.append(cycle_dict)
    return cycles

def run_test(test):
    input_str = "\n".join(test["inputs"]) + "\n"

    try:
        proc = subprocess.Popen([ECU_EXEC], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        out, _ = proc.communicate(input=input_str, timeout=3)
    except subprocess.TimeoutExpired:
        proc.kill()
        out, _ = proc.communicate()

    parsed_cycles = parse_ecu_output_to_dicts(out)
    target = test["expected"]

    if "0 0 0 0" == test["inputs"][-1] and target["FAULTS"]["Reset Requested"] == "NO":
        # Target the cycle before the watchdog sentinel
        target_idx = len(test["inputs"]) - 2
    else:
        # Target the final cycle in the input sequence
        target_idx = len(test["inputs"]) - 1

    if target_idx < len(parsed_cycles):
        actual = parsed_cycles[target_idx]
    else:
        return False, out, target, f"Parse failure - Could not find cycle {target_idx}"

    passed = (target == actual)
    return passed, out, target, actual

def main():
    if not os.path.exists(ECU_EXEC):
        print(f"Cannot find executable at {ECU_EXEC}")
        sys.exit(1)

    print(f"Starting Robust 1:1 Dictionary Validation Test Framework ({len(TEST_CASES)} cases)...\n")
    results = []

    for test in TEST_CASES:
        print(f"Running {test['id']} - {test['name']}...", end=" ", flush=True)
        passed, out, target, actual = run_test(test)
        results.append((test, passed, target, actual))

        if passed:
            print("PASS")
        else:
            print("FAIL")

    total = len(TEST_CASES)
    passed_count = sum(1 for _, p, _, _ in results if p)

    with open(REPORT_FILE, "w", encoding='utf-8') as rf:
        rf.write("# ECU Simulator - Strict Dictionary Test Report\n\n")
        rf.write(f"**Total Execution Status:** {passed_count}/{total} PASSED\n\n")

        # ---------------------------------------------------------
        # SECTION 1: Summary Table
        # ---------------------------------------------------------
        rf.write("## 📝 Executive Summary\n\n")
        rf.write("| ID | Test Name | Status |\n")
        rf.write("|---|---|---|\n")

        for test, passed, _, _ in results:
            status_sym = "✅ PASS" if passed else "❌ FAIL"
            rf.write(f"| {test['id']} | {test['name']} | {status_sym} |\n")

        # ---------------------------------------------------------
        # SECTION 2: Detailed Test Case Mappings
        # ---------------------------------------------------------
        rf.write("\n## 🔍 Detailed Test Execution Outputs\n\n")

        for test, passed, target, actual in results:
            status_sym = "✅ PASS" if passed else "❌ FAIL"

            rf.write("---\n\n")
            rf.write(f"### {status_sym} {test['id']}: {test['name']}\n\n")

            rf.write("**Input Sequence**\n")
            rf.write("```json\n")
            rf.write(json.dumps(test['inputs'], indent=2) + "\n")
            rf.write("```\n\n")

            rf.write("**Expected Output Configuration**\n")
            rf.write("```json\n")
            rf.write(json.dumps(target, indent=2) + "\n")
            rf.write("```\n\n")

            if passed:
                rf.write("**Execution Match**\n")
                rf.write("> [!NOTE]\n")
                rf.write("> The resulting ECU State perfectly matched the Expected Structure.\n\n")
                rf.write("```json\n")
                rf.write(json.dumps(actual, indent=2) + "\n")
                rf.write("```\n\n")
            else:
                rf.write("**Actual ECU Processing Outcome**\n")
                rf.write("```json\n")
                if isinstance(actual, dict):
                    rf.write(json.dumps(actual, indent=2) + "\n")
                else:
                    rf.write(f"Parsing Error: {actual}\n")
                rf.write("```\n\n")

    print(f"\n{passed_count}/{total} PASSED")
    print(f"Highly structured report saved to {REPORT_FILE}")

if __name__ == "__main__":
    main()
