#include "log.h"
#include "types.h"
#include <stdio.h>
#include <time.h>

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

static void log_fault_flags(uint16_t flags, const char *label)
{
    printf("  %s: %d%d%d%d%d [ ", label,
           (flags & FAULT_BIT_INVALID_MODE) ? 1 : 0,
           (flags & FAULT_BIT_INVALID_GEAR) ? 1 : 0,
           (flags & FAULT_BIT_HIGH_TEMP) ? 1 : 0,
           (flags & FAULT_BIT_CRITICAL_OVERHEAT) ? 1 : 0,
           (flags & FAULT_BIT_OVERSPEED) ? 1 : 0);

    if (flags & FAULT_BIT_OVERSPEED)
    {
        printf(ANSI_COLOR_RED "OVERSPEED " ANSI_COLOR_RESET);
    }
    if (flags & FAULT_BIT_CRITICAL_OVERHEAT)
    {
        printf(ANSI_COLOR_RED "CRITICAL_OVERHEAT " ANSI_COLOR_RESET);
    }
    if (flags & FAULT_BIT_HIGH_TEMP)
    {
        printf(ANSI_COLOR_YELLOW "HIGH_TEMP " ANSI_COLOR_RESET);
    }
    if (flags & FAULT_BIT_INVALID_GEAR)
    {
        printf(ANSI_COLOR_YELLOW "INVALID_GEAR " ANSI_COLOR_RESET);
    }
    if (flags & FAULT_BIT_INVALID_MODE)
    {
        printf(ANSI_COLOR_YELLOW "INVALID_MODE " ANSI_COLOR_RESET);
    }
    if (flags == 0U)
    {
        printf(ANSI_COLOR_GREEN "CLEAR " ANSI_COLOR_RESET);
    }

    printf("]\n");
}

static void fprint_fault_flags(FILE *fp, uint16_t flags, const char *label)
{
    if (!fp) return;
    fprintf(fp, "  %s: %d%d%d%d%d [ ", label,
           (flags & FAULT_BIT_INVALID_MODE) ? 1 : 0,
           (flags & FAULT_BIT_INVALID_GEAR) ? 1 : 0,
           (flags & FAULT_BIT_HIGH_TEMP) ? 1 : 0,
           (flags & FAULT_BIT_CRITICAL_OVERHEAT) ? 1 : 0,
           (flags & FAULT_BIT_OVERSPEED) ? 1 : 0);

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
void log_cycle_summary(const VehicleInput *input, const VehicleStatus *status, const FaultStatus *faults)
{
    if ((input == NULL) || (status == NULL) || (faults == NULL))
    {
        printf("[LOG] ERROR: NULL pointer passed to log_cycle_summary\n");
        return;
    }
    printf("\n\033[2J\033[H"); // Clear screen and move to top
    printf("\n");
    printf(ANSI_COLOR_CYAN "---- CYCLE SUMMARY REPORT ---" ANSI_COLOR_RESET "\n");
    printf("\n");

    //inputs
    printf(ANSI_COLOR_CYAN "[INPUT]" ANSI_COLOR_RESET "\n");
    printf("  Speed       : %d km/h\n", input->speed);
    printf("  Temperature : %d C\n", input->temperature);
    printf("  Gear        : %u\n", (unsigned int)input->gear);
    printf("  Req. Mode   : %s\n", mode_to_string(input->mode));

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
        fprintf(fin, "\n");
        fclose(fin);
    }
    printf("\n");
    
    //mode status
    printf(ANSI_COLOR_CYAN "[MODE]" ANSI_COLOR_RESET "\n");
    printf("  Active Mode   : %s\n", mode_to_string(status->active_mode));
    printf("  Current Mode  : %s\n", mode_to_string(status->current_mode));
    printf("  Previous Mode : %s\n", mode_to_string(status->previous_mode));

    printf("\n");
    //fault status
    printf(ANSI_COLOR_CYAN "[FAULTS]" ANSI_COLOR_RESET "\n");
    log_fault_flags(faults->current_cycle_flags, "Cycle Flags ");
    log_fault_flags(faults->persistent_flags,    "Persistent  ");
    printf("  Major Faults    : %u\n", (unsigned int)faults->major_fault_count);
    printf("  Warnings        : %u\n", (unsigned int)faults->warning_count);
    printf("  Critical Faults : %u\n", (unsigned int)faults->critical_fault_count);
    printf("  Reset Requested : %s\n", faults->reset_requested ? (ANSI_COLOR_RED "YES" ANSI_COLOR_RESET) : (ANSI_COLOR_GREEN "NO" ANSI_COLOR_RESET));

    printf("\n");
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
        fprintf(ffault, "\n");
        fclose(ffault);
    }
    printf("\n");
    
    //system state
    printf(ANSI_COLOR_CYAN "[STATE]" ANSI_COLOR_RESET "\n");
    if (status->system_state == NORMAL) {
        printf("  System State : " ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "\n", state_to_string(status->system_state));
    } else {
        printf("  System State : " ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", state_to_string(status->system_state));
    }
    printf("\n");
    printf(ANSI_COLOR_YELLOW "[WARNINGS]" ANSI_COLOR_RESET "\n");
    //active faults in priority order
    if (faults->current_cycle_flags & FAULT_BIT_HIGH_TEMP)
    {
        printf(ANSI_COLOR_YELLOW "  WARNING: HIGH TEMP\n" ANSI_COLOR_RESET);
    }
    printf("\n");
    printf(ANSI_COLOR_RED "[ACTIVE FAULTS - Priority Order]" ANSI_COLOR_RESET "\n");
    
    if (faults->current_cycle_flags & FAULT_BIT_CRITICAL_OVERHEAT)
    {
        printf(ANSI_COLOR_RED "  1. CRITICAL_OVERHEAT\n" ANSI_COLOR_RESET);
    }
    
    if (faults->current_cycle_flags & FAULT_BIT_INVALID_MODE)
    {
        printf(ANSI_COLOR_YELLOW "  2. INVALID_MODE\n" ANSI_COLOR_RESET);
    }
    if (faults->current_cycle_flags & FAULT_BIT_INVALID_GEAR)
    {
        printf(ANSI_COLOR_YELLOW "  2. INVALID_GEAR\n" ANSI_COLOR_RESET);
    }

    if (faults->current_cycle_flags & FAULT_BIT_OVERSPEED)
    {
        printf(ANSI_COLOR_RED "  3. OVERSPEED\n" ANSI_COLOR_RESET);
    }


    
    if ((faults->current_cycle_flags & 0x0FFF) == 0U)
    {
        printf(ANSI_COLOR_GREEN "  (none)\n" ANSI_COLOR_RESET);
    }

    printf("\n\n");
}
