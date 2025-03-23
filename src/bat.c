#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "util.h"

char* getBat(void) {
    FILE* fp;
    char* buf;
    char batStatus[255];
    char* symbol;
    int perc;
    int bufSize;
    size_t len;

    bufSize = 255;

    buf = malloc(bufSize * sizeof(char));
    if (buf == NULL) {
        fprintf(stderr, "%s:%d Error: return buffer not allocated successfully\n", __FILE__, __LINE__);
        exit(1);
    }

    fp = fopen("/sys/class/power_supply/BAT0/status", "r");
    if (fp == NULL) {
        fprintf(stderr, "%s:%d Error: open BAT0/status failed\n", __FILE__, __LINE__);
        exit(1);
    }

    if (fgets(batStatus, bufSize, fp) == NULL) {
        fprintf(stderr, "%s:%d Error: reading str to batStatus failed\n", __FILE__, __LINE__);
        exit(1);
    }

    len = strlen(batStatus);
    if (len > 0 && batStatus[len-1] == '\n') {
        batStatus[len-1] = '\0';
    }
    
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

    if (snprintf(buf, bufSize, "%s %d%%", symbol, perc) < 0) {
        fprintf(stderr, "%s:%d Error: failed to write str to return buffer\n", __FILE__, __LINE__);
        exit(1);
    }

    return buf;
}

