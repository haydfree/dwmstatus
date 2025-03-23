/*
 * Copy me if you can.
 * by 20h
 */

#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <X11/Xlib.h>

#include "util.h"
#include "vol.h"
#include "bat.h"
#include "cpu.h"
#include "tm.h"
#include "mem.h"
#include "net.h"

#define REFRESH_RATE 1

static Display *dpy;

void setStatus(char *str) {
	XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}

int main(void) {
    char* netSsid;
    char* netStatus;
    char* bat;
    char* cpu;
    char* mem;
    char* vol;
    char* time;
    char* status;

	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "dwmstatus: cannot open display.\n");
		return 1;
	}

    netSsid = getNetSsid(); 

	for (;;sleep(REFRESH_RATE)) {
        netStatus = getNetStatus();
		bat = getBat();
		cpu = getCpu("/sys/devices/virtual/thermal/thermal_zone0", "temp");
        mem = getMem();
        vol = getVol();
		time = getTime("%a %Y-%m-%d %H:%M:%S", "America/Chicago");

        status = smprintf(" net: %s %s | bat: %s | cpu: %s | mem: %s | vol: %s | time: %s ", netSsid, netStatus, bat, cpu, mem, vol, time);
		setStatus(status);

        free(netStatus);
        free(bat);
        free(cpu);
        free(mem);
        free(vol);
        free(time);
        free(status);
	}

    free(netSsid);

	XCloseDisplay(dpy);
	return 0;
}

