#ifndef DWMSTATUS_H
#define DWMSTATUS_H

#include "util.h"
#include <X11/Xlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define REFRESH_RATE 1
#define BUF_SIZE 256
#define SMALL_BUF_SIZE 32

int get_net(char *const dst, const size_t len);
int get_bat(char *const dst, const size_t len);
int get_cpu(char *const dst, const size_t len);
int get_mem(char *const dst, const size_t len);
int get_vol(char *const dst, const size_t len);
int get_time(char *const dst, const size_t len);
int status_loop(void);

#endif
