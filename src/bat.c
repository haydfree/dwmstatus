#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "util.h"

#define BUFSIZE 255

char* getBat(void) {
    FILE* fp;
    char* buf;
    char* symbol;
    char batStatus[255];
    int perc;

    buf = malloc(BUFSIZE * sizeof(char));
    if (buf == NULL) {
        fprintf(stderr, "%s:%d Error: return buffer not allocated successfully\n", __FILE__, __LINE__);
        exit(1);
    }

    fp = fopen("/sys/class/power_supply/BAT0/status", "r");
    if (fp == NULL) {
        fprintf(stderr, "%s:%d Error: open BAT0/status failed\n", __FILE__, __LINE__);
        exit(1);
    }

    if (fgets(batStatus, BUFSIZE, fp) == NULL) {
        fprintf(stderr, "%s:%d Error: reading str to batStatus failed\n", __FILE__, __LINE__);
        exit(1);
    }

    strTrim(batStatus);
    fclose(fp);

    fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    if (fp == NULL) {
        fprintf(stderr, "%s:%d Error: open BAT0/capacity failed\n", __FILE__, __LINE__);
        exit(1);
    }

    if (fscanf(fp, "%d", &perc) != 1) {
        fprintf(stderr, "%s:%d Error: reading int into bat perc failed \n", __FILE__, __LINE__);
        exit(1);
    }

    fclose(fp);
    
    if (strcmp(batStatus, "Full") == 0) {
        symbol = "=";
    } else if (strcmp(batStatus, "Charging") == 0) {
        symbol = "+";
    } else if (strcmp(batStatus, "Not charging") == 0) {
        symbol = "-";
    } else {
        symbol = "?";
    }

    if (snprintf(buf, BUFSIZE, "%s %d%%", symbol, perc) < 0) {
        fprintf(stderr, "%s:%d Error: failed to write str to return buffer\n", __FILE__, __LINE__);
        exit(1);
    }

    return buf;
}

