#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "util.h"

char* getBat(void) {
    FILE* bat;
    char* batStatus;
    int perc;

    char* buf;

    batStatus = malloc(255 * sizeof(char));
    buf = malloc(255 * sizeof(char));

    bat = fopen("/sys/class/power_supply/BAT0/status", "r");
    fgets(batStatus, sizeof(batStatus), bat);
    fclose(bat);

    bat = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    fscanf(bat, "%d", &perc);
    fclose(bat);

    if (strcmp(batStatus, "Full\n") == 0) {
        batStatus = "=";
    } else if (strcmp(batStatus, "Charging\n") == 0) {
        batStatus = "+";
    } else if (strcmp(batStatus, "Discharging\n") == 0) {
        batStatus = "-";
    } else {
        batStatus = "???";
    }

    snprintf(buf, sizeof(batStatus), "%s %d%%", batStatus, perc); 

    return buf;
}

