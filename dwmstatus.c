#include "dwmstatus.h"

static Display *dpy;

int get_net(char *const dst, const size_t len) {
  int ret = ERROR;
  size_t cur_len = 0;
  char net[1024] = {0}, net_id[SMALL_BUF_SIZE] = {0},
       net_st[SMALL_BUF_SIZE] = {0}, if_name[SMALL_BUF_SIZE] = {0},
       *line = NULL;

  if (!dst) {
    goto cleanup;
  }
  if (len <= 0) {
    goto cleanup;
  }

  execute_script("nmcli", net, sizeof net);
  char *c = net;
  while (*c++ != '\n') {
  }
  *c = '\0';
  int n = 0;
  if (sscanf(net, "%31[^:]: %31s%n", if_name, net_st, &n) >= 2) {
    if (sscanf(net + n, " to %31s", net_id) == 1) {
      snprintf(dst, c - net + n, "%s%s:%s", "+", if_name, net_id);
    } else {
      snprintf(dst, c - net + n, "%s%s", "-", if_name);
    }
  } else {
    goto cleanup;
  }

  ret = SUCCESS;
cleanup:
  return ret;
}

int get_bat(char *const dst, const size_t len) {
  int ret = ERROR;
  size_t cur_len = 0;
  char bat_per[SMALL_BUF_SIZE] = {0}, bat_st[SMALL_BUF_SIZE] = {0};
  char *ptr = dst;

  bat_per[0] = '1';
  bat_per[1] = '0';
  bat_per[2] = '0';
  bat_st[0] = '+';

  if (!dst) {
    goto cleanup;
  }
  if (len <= 0) {
    goto cleanup;
  }
  /* if desktop / no power supply */
  if (execute_script("ls /sys/class/power_supply", dst, len)) {
    goto cleanup;
  }
  if (*dst == 0) {
    snprintf(dst, BUF_SIZE, "%s%s%%", bat_st, bat_per);
    ret = SUCCESS;
    goto cleanup;
  }

  memset(dst, 0, len);
  execute_script("cat /sys/class/power_supply/BAT0/capacity", bat_per,
                 SMALL_BUF_SIZE);
  trim_whitespace(bat_per, bat_per, SMALL_BUF_SIZE);
  cur_len = strlen(bat_per);
  if (cur_len <= 0 || cur_len + 2 >= len) {
    goto cleanup;
  }
  execute_script("cat /sys/class/power_supply/BAT0/status", bat_st,
                 SMALL_BUF_SIZE);
  trim_whitespace(bat_st, bat_st, SMALL_BUF_SIZE);
  cur_len += strlen(bat_st);
  if (cur_len + 1 >= len) {
    goto cleanup;
  }

  if (strcmp(bat_st, "Charging") == 0 || strcmp(bat_st, "Full") == 0) {
    strncpy(bat_st, "+", SMALL_BUF_SIZE);
  } else if (strcmp(bat_st, "Discharging") == 0 ||
             strcmp(bat_st, "Not charging") == 0) {
    strncpy(bat_st, "-", SMALL_BUF_SIZE);
  } else { /* leave status unchanged */
  }
  if (snprintf(dst, BUF_SIZE, "%s%s%%", bat_st, bat_per) < 0) {
    goto cleanup;
  }

  ret = SUCCESS;
cleanup:
  return ret;
}

int get_cpu(char *const dst, const size_t len) {
  int ret = ERROR;
  size_t cur_len = 0;
  char cpu[BUF_SIZE] = {0};

  if (!dst) {
    goto cleanup;
  }
  if (len <= 0) {
    goto cleanup;
  }

  execute_script("cat /sys/class/thermal/thermal_zone0/temp", cpu, BUF_SIZE);
  trim_whitespace(cpu, cpu, BUF_SIZE);
  cur_len = strlen(cpu);
  if (cur_len <= 0) {
    goto cleanup;
  }

  cpu[2] = '\0';
  if (snprintf(dst, BUF_SIZE, "%s°C", cpu) < 0) {
    goto cleanup;
  }

  ret = SUCCESS;
cleanup:
  return ret;
}

int get_mem(char *const dst, const size_t len) {
  int ret = ERROR;
  size_t cur_len = 0;
  char mem_total[BUF_SIZE] = {0}, mem_used[SMALL_BUF_SIZE] = {0}, *temp = NULL;

  if (!dst) {
    goto cleanup;
  }
  if (len <= 0) {
    goto cleanup;
  }

  execute_script("free -m", mem_total, BUF_SIZE);
  cur_len = strlen(mem_total);
  if (cur_len <= 0) {
    goto cleanup;
  }

  temp = strchr(mem_total, '\n') + 1;
  sscanf(temp, "Mem: %s %s", mem_total, mem_used);
  cur_len = strlen(temp);
  if (cur_len <= 0 || cur_len + 1 >= len) {
    goto cleanup;
  }
  if (snprintf(dst, len, "%sMiB/%sMiB", mem_used, mem_total) < 0) {
    goto cleanup;
  }

  ret = SUCCESS;
cleanup:
  return ret;
}

int get_vol(char *const dst, const size_t len) {
  int ret = ERROR, li = 0;
  size_t cur_len = 0;
  char vol[BUF_SIZE] = {0}, level[SMALL_BUF_SIZE] = {0},
       mute[SMALL_BUF_SIZE] = {0}, *temp = NULL;
  float lf = 0.0;

  if (!dst) {
    goto cleanup;
  }
  if (len <= 0) {
    goto cleanup;
  }

  execute_script("pactl get-sink-volume @DEFAULT_SINK@", vol, BUF_SIZE);
  execute_script("pactl get-sink-mute @DEFAULT_SINK@", mute, BUF_SIZE);
  cur_len = strlen(vol);
  if (cur_len <= 0) {
    goto cleanup;
  }
  cur_len = strlen(mute);
  if (cur_len <= 0) {
    goto cleanup;
  }
  cur_len = strlen(vol) + strlen(mute);
  if (cur_len + 1 >= len) {
    goto cleanup;
  }

  sscanf(vol, "%*[^/]/ %s%%", vol);
  sscanf(mute, "%*[^:]: %3s", mute);
  trim_whitespace(level, vol, SMALL_BUF_SIZE);
  trim_whitespace(mute, mute, SMALL_BUF_SIZE);
  temp = strchr(level, '%');
  *temp = '\0';

  if (strcmp(mute, "0") == 0 || strcmp(mute, "no") == 0) {
    strncpy(mute, "+", SMALL_BUF_SIZE);
  } else if (strcmp(mute, "1") == 0 || strcmp(mute, "yes") == 0) {
    strncpy(mute, "-", SMALL_BUF_SIZE);
  }
  if (snprintf(dst, len, "%s%s%%", mute, level) < 0) {
    goto cleanup;
  }

  ret = SUCCESS;
cleanup:
  return ret;
}

int get_time(char *const dst, const size_t len) {
  int ret = ERROR;
  size_t cur_len = 0;
  char temp[SMALL_BUF_SIZE] = {0};
  time_t raw_time = 0;
  struct tm *time_info = {0};

  if (!dst) {
    goto cleanup;
  }
  if (len <= 0) {
    goto cleanup;
  }

  time(&raw_time);
  time_info = localtime(&raw_time);
  strftime(temp, SMALL_BUF_SIZE, "%a %Y-%m-%d %H:%M:%S", time_info);
  trim_whitespace(temp, temp, SMALL_BUF_SIZE);
  cur_len = strlen(temp);
  if (cur_len <= 0 || cur_len + 1 >= len) {
    goto cleanup;
  }
  if (snprintf(dst, len, "%s", temp) < 0) {
    goto cleanup;
  }

  ret = SUCCESS;
cleanup:
  return ret;
}

static int reset_status(char *const dst, const size_t len, char *const net,
                        char *const bat, char *const cpu, char *const mem,
                        char *const vol, char *const time) {
  int ret = ERROR;

  if (!dst || !net || !bat || !cpu || !mem || !vol || !time) {
    goto cleanup;
  }
  if (len <= 0) {
    goto cleanup;
  }

  memset(dst, 0, len);
  memset(net, 0, len);
  memset(bat, 0, len);
  memset(cpu, 0, len);
  memset(mem, 0, len);
  memset(vol, 0, len);
  memset(time, 0, len);

  return SUCCESS;

cleanup:
  return ret;
}

static int create_status(char *const dst, const size_t len,
                         const char *const net, const char *const bat,
                         const char *const cpu, const char *const mem,
                         const char *const vol, const char *const time) {
  int ret = ERROR;

  if (!dst || !net || !bat || !cpu || !mem || !vol || !time) {
    goto cleanup;
  }
  if (len <= 0) {
    goto cleanup;
  }

  if (snprintf(dst, len,
               " net: %s | bat: %s | cpu: %s | mem: %s | vol: %s | time: %s ",
               net, bat, cpu, mem, vol, time) < 0) {
    goto cleanup;
  }

  return SUCCESS;

cleanup:
  return ret;
}

static int set_status(const char *const src, const size_t len) {
  int ret = ERROR;

  if (!src) {
    goto cleanup;
  }
  if (len <= 0) {
    goto cleanup;
  }

  XStoreName(dpy, DefaultRootWindow(dpy), src);
  XSync(dpy, False);

  ret = SUCCESS;
cleanup:
  return ret;
}

int status_loop(void) {
  int ret = ERROR;
  char status[BUF_SIZE] = {0}, net[BUF_SIZE] = {0}, bat[BUF_SIZE] = {0},
       cpu[BUF_SIZE] = {0}, mem[BUF_SIZE] = {0}, vol[BUF_SIZE] = {0},
       cur_time[BUF_SIZE] = {0};

  if (!(dpy = XOpenDisplay(NULL))) {
    goto cleanup;
  }

  for (;; sleep(REFRESH_RATE)) {
    if (reset_status(status, BUF_SIZE, net, bat, cpu, mem, vol, cur_time)) {
      goto cleanup;
    }

    if (get_net(net, BUF_SIZE)) {
      goto cleanup;
    }
    if (get_bat(bat, BUF_SIZE)) {
      goto cleanup;
    }
    if (get_cpu(cpu, BUF_SIZE)) {
      goto cleanup;
    }
    if (get_mem(mem, BUF_SIZE)) {
      goto cleanup;
    }
    if (get_vol(vol, BUF_SIZE)) {
      goto cleanup;
    }
    if (get_time(cur_time, BUF_SIZE)) {
      goto cleanup;
    }

    if (create_status(status, BUF_SIZE, net, bat, cpu, mem, vol, cur_time)) {
      goto cleanup;
    }
    if (set_status(status, BUF_SIZE)) {
      goto cleanup;
    }
  }

  ret = SUCCESS;
cleanup:
  if (dpy) {
    XCloseDisplay(dpy);
  }
  return ret;
}
