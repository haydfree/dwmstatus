#include <stdio.h>
#include <stdlib.h>

char* getMem(void) {
    FILE *fp;
    long totalMem, freeMem, availableMem, usedMem;
    char* memBuf = malloc(32);

    fp = fopen("/proc/meminfo", "r");
    if (!fp) return NULL;

    fscanf(fp, "MemTotal: %ld kB\n", &totalMem);
    fscanf(fp, "MemFree: %ld kB\n", &freeMem);
    fscanf(fp, "MemAvailable: %ld kB\n", &availableMem);
    fclose(fp);

    usedMem = totalMem - availableMem;
    totalMem/=1000;
    freeMem/=1000;
    availableMem/=1000;
    usedMem/=1000;
    sprintf(memBuf, "%ld MB / %ld MB", usedMem, totalMem);

    return memBuf;
}
