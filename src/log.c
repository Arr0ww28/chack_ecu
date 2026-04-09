#include "log.h"
#include "types.h"
#include <stdio.h>
#include <time.h>

/* Maps Mode enum values to human-readable strings */
static const char *mode_to_string(Mode m)
{
    switch (m)
    {
        case MODE_OFF:         return "OFF";
        case MODE_ACC:         return "ACC";
        case MODE_IGNITION_ON: return "IGNITION_ON";
        case MODE_FAULT:       return "FAULT";
        case MODE_INVALID:     return "INVALID";
        default:               return "UNKNOWN";
    }
}

/* Maps SystemState enum values to human-readable strings */
static const char *state_to_string(SystemState s)
{
    switch (s)
    {
        case NORMAL:   return "NORMAL";
        case DEGRADED: return "DEGRADED";
        case SAFE:     return "SAFE";
        default:       return "UNKNOWN";
    }
}

/* Maps FaultPriority enum values to human-readable strings */
static const char *priority_to_string(FaultPriority p)
{
    switch (p)
    {
        case PRIORITY_NONE:              return "NONE";
        case PRIORITY_HIGH_TEMP:         return "HIGH_TEMP";
        case PRIORITY_OVERSPEED:         return "OVERSPEED";
        case PRIORITY_INVALID_GEAR_MODE: return "INVALID_GEAR_MODE";
        case PRIORITY_CRITICAL_OVERHEAT: return "CRITICAL_OVERHEAT";
        default:                         return "UNKNOWN";
    }
}

/* Logs individual fault flag status from a bitmask */
static void log_fault_flags(uint16_t flags, const char *label)
{
    printf("  %s: 0x%04X [ ", label, flags);

    if (flags & FAULT_BIT_OVERSPEED)
    {
        printf("OVERSPEED ");
    }
    if (flags & FAULT_BIT_CRITICAL_OVERHEAT)
    {
        printf("CRITICAL_OVERHEAT ");
    }
    if (flags & FAULT_BIT_HIGH_TEMP)
    {
        printf("HIGH_TEMP ");
    }
    if (flags & FAULT_BIT_INVALID_GEAR)
    {
        printf("INVALID_GEAR ");
    }
    if (flags & FAULT_BIT_INVALID_MODE)
    {
        printf("INVALID_MODE ");
    }
    if (flags == 0U)
    {
        printf("CLEAR ");
    }

    printf("]\n");
}

/* Logs individual fault flag status from a bitmask to a file */
static void fprint_fault_flags(FILE *fp, uint16_t flags, const char *label)
{
    if (!fp) return;
    fprintf(fp, "  %s: 0x%04X [ ", label, flags);

    if (flags & FAULT_BIT_OVERSPEED)
        fprintf(fp, "OVERSPEED ");
    if (flags & FAULT_BIT_CRITICAL_OVERHEAT)
        fprintf(fp, "CRITICAL_OVERHEAT ");
    if (flags & FAULT_BIT_HIGH_TEMP)
        fprintf(fp, "HIGH_TEMP ");
    if (flags & FAULT_BIT_INVALID_GEAR)
        fprintf(fp, "INVALID_GEAR ");
    if (flags & FAULT_BIT_INVALID_MODE)
        fprintf(fp, "INVALID_MODE ");
    if (flags == 0U)
        fprintf(fp, "CLEAR ");

    fprintf(fp, "]\n");
}

/* Prints a full cycle summary covering inputs, mode, faults, and system state */
void log_cycle_summary(const VehicleInput *input, const VehicleStatus *status, const FaultStatus *faults)
{
    if ((input == NULL) || (status == NULL) || (faults == NULL))
    {
        printf("[LOG] ERROR: NULL pointer passed to log_cycle_summary\n");
        return;
    }

    printf("========================================\n");
    printf("         CYCLE SUMMARY REPORT           \n");
    printf("========================================\n");

    /* ---- Input Snapshot ---- */
    printf("[INPUT]\n");
    printf("  Speed       : %d km/h\n", input->speed);
    printf("  Temperature : %d C\n", input->temperature);
    printf("  Gear        : %u\n", (unsigned int)input->gear);
    printf("  Requested Mode : %s\n", mode_to_string(input->mode));

    FILE *fin = fopen("valid_input.txt", "a");
    if (fin)
    {
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char time_buf[64];
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

        fprintf(fin, "[INPUT] - %s\n", time_buf);
        fprintf(fin, "  Speed       : %d km/h\n", input->speed);
        fprintf(fin, "  Temperature : %d C\n", input->temperature);
        fprintf(fin, "  Gear        : %u\n", (unsigned int)input->gear);
        fprintf(fin, "  Requested Mode : %s\n", mode_to_string(input->mode));
        fprintf(fin, "----------------------------------------\n");
        fclose(fin);
    }

    /* ---- Mode Status ---- */
    printf("[MODE]\n");
    printf("  Active Mode   : %s\n", mode_to_string(status->active_mode));
    printf("  Current Mode  : %s\n", mode_to_string(status->current_mode));
    printf("  Previous Mode : %s\n", mode_to_string(status->previous_mode));

    /* ---- Fault Status ---- */
    printf("[FAULTS]\n");
    log_fault_flags(faults->current_cycle_flags, "Cycle Flags ");
    log_fault_flags(faults->persistent_flags,    "Persistent  ");
    printf("  Major Faults    : %u\n", (unsigned int)faults->major_fault_count);
    printf("  Warnings        : %u\n", (unsigned int)faults->warning_count);
    printf("  Critical Faults : %u\n", (unsigned int)faults->critical_fault_count);
    printf("  Reset Requested : %s\n", faults->reset_requested ? "YES" : "NO");

    FILE *ffault = fopen("faults_errors.txt", "a");
    if (ffault)
    {
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        char time_buf[64];
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

        fprintf(ffault, "[FAULTS] - %s\n", time_buf);
        fprint_fault_flags(ffault, faults->current_cycle_flags, "Cycle Flags ");
        fprint_fault_flags(ffault, faults->persistent_flags,    "Persistent  ");
        fprintf(ffault, "  Major Faults    : %u\n", (unsigned int)faults->major_fault_count);
        fprintf(ffault, "  Warnings        : %u\n", (unsigned int)faults->warning_count);
        fprintf(ffault, "  Critical Faults : %u\n", (unsigned int)faults->critical_fault_count);
        fprintf(ffault, "  Reset Requested : %s\n", faults->reset_requested ? "YES" : "NO");
        fprintf(ffault, "----------------------------------------\n");
        fclose(ffault);
    }

    /* ---- System State ---- */
    printf("[STATE]\n");
    printf("  System State      : %s\n", state_to_string(status->system_state));
    printf("  Highest Priority  : %s\n", priority_to_string(status->highest_priority_issue));

    printf("========================================\n\n");
}
