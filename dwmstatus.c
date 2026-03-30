#include "dwmstatus.h"

#define BUFSIZE 64

static Display *dpy;

int get_date(char *const date) {
    int ret = -1;

    FILE *fp;
    #ifdef __OpenBSD__
    fp = popen("date +\"%a %y/%m/%d %l:%M:%S %p\"", "r");
    #else
    fp = popen("date +\"%a %y/%m/%d %I:%M:%S %p\"", "r");
    #endif
    if (!fp) {
        fprintf(stderr, "get_date: got null fp.\n");
        goto cleanup;
    }
    if (!fgets(date, BUFSIZE, fp)) {
        fprintf(stderr, "get_date: cannot read fp to str.\n");
        goto cleanup;
    }
    date[strcspn(date, "\n")] = '\0';

    ret = 0;
cleanup:
    if (fp)
        pclose(fp);
    return ret;
}

int get_tmp(char *tmp) {
    int ret = -1;

    FILE *fp;
    #ifdef __OpenBSD__
    fp = popen("sysctl hw.sensors | awk -F'[ =]' 'NR==1 {print $2}'",
        "r");
    if (!fp) {
        fprintf(stderr, "get_tmp: got null fp.\n");
        goto cleanup;
    }
    fgets(tmp, BUFSIZE, fp);
    tmp[strcspn(tmp, ".")] = '\0';
    #else
    fp = popen("cat /sys/class/thermal/thermal_zone0/temp", "r");
    if (!fp) {
        fprintf(stderr, "get_tmp: got null fp.\n");
        goto cleanup;
    }
    fgets(tmp, BUFSIZE, fp);
    int a = atoi(tmp)/1000;
    snprintf(tmp, BUFSIZE, "%d", a);
    #endif

    ret = 0;
cleanup:
    if (fp)
        pclose(fp);
    return ret;
}

int get_mem(char *mem) {
    int ret = -1;

    FILE *fp;
    #ifdef __OpenBSD__
    fp = popen("vmstat | awk 'NR==3 {print $3}'", "r");
    if (!fp) {
        fprintf(stderr, "get_mem: got null fp.\n");
        goto cleanup;
    }
    fgets(mem, BUFSIZE, fp);
    mem[strcspn(mem, "\n")] = '\0';
    #else
    fp = popen("cat /proc/meminfo | awk 'NR==1 || NR==3 {print $2}'", "r");
    if (!fp) {
        fprintf(stderr, "get_mem: got null fp.\n");
        goto cleanup;
    }
    int total, available, used;
    char str_total[BUFSIZE], str_available[BUFSIZE];
    fgets(str_total, BUFSIZE, fp);
    fgets(str_available, BUFSIZE, fp);
    total = atoi(str_total);
    available = atoi(str_available);
    used = (total - available) / 1024;
    snprintf(mem, BUFSIZE, "%dM", used);
    #endif

    ret = 0;
cleanup:
    if (fp)
        pclose(fp);
    return ret;
}

int get_dsk(char *dsk) {
    int ret = -1;

    FILE *fp = popen("df -k | awk 'NR > 1 {sum += $3} END {print sum/1024}'",
        "r");
    if (!fp) {
        fprintf(stderr, "get_dsk: got null fp.\n");
        goto cleanup;
    }
    fgets(dsk, BUFSIZE, fp);
    dsk[strcspn(dsk, "\n")] = 'M';

    ret = 0;
cleanup:
    if (fp)
        pclose(fp);
    return ret;
}

int get_bat(char *bat) {
    int ret = -1;

    char status[BUFSIZE], capacity[BUFSIZE];
    FILE *fp;
    #ifdef __OpenBSD__
    fp = popen("apm -v | awk 'NR==1 {print $4} NR==2 {print $4}'",
        "r");
    if (!fp) {
        fprintf(stderr, "get_bat: got null fp.\n");
        goto cleanup;
    }
    fscanf(fp, "%s %s", capacity, status);
    if (!strncmp(status, "connected", 9))
        strncpy(status, "+", BUFSIZE);
    else
        strncpy(status, "-", BUFSIZE);
    snprintf(bat, BUFSIZE, "%s%s", status, capacity);
    #else
    fp = popen("cat /sys/class/power_supply/BAT0/capacity && cat /sys/class/power_supply/BAT0/status", "r");
    if (!fp) {
        fprintf(stderr, "get_bat: got null fp.\n");
        goto cleanup;
    }
    fgets(capacity, BUFSIZE, fp);
    capacity[strcspn(capacity, "\n")] = '\0';
    fgets(status, BUFSIZE, fp);
    if (!strncmp(status, "Charging", 8))
        strncpy(status, "+", BUFSIZE);
    else
        strncpy(status, "-", BUFSIZE);
    snprintf(bat, BUFSIZE, "%s%s%%", status, capacity);
    #endif

    ret = 0;
cleanup:
    if (fp)
        pclose(fp);
    return ret;
}

int get_net(char *net, char *netint) {
    int ret = -1;

    char cmd[BUFSIZE], status[BUFSIZE], ssid[BUFSIZE];
    FILE *fp;
    #ifdef __OpenBSD__
    snprintf(cmd, BUFSIZE,
        "ifconfig %s | awk 'NR==6 {v=$2} NR==7 {print v, $3}'",
        netint);
    fp = popen(cmd, "r");
    fscanf(fp, "%s %s", status, ssid);
    if (!strncmp(status, "active", 6))
        strncpy(status, "+", BUFSIZE);
    else
        strncpy(status, "-", BUFSIZE);
    #else
    snprintf(cmd, BUFSIZE, "iw dev %s link | awk 'NR==1 {v=$1} NR==2 {print v, $2}'", netint);
    fp = popen(cmd, "r");
    fscanf(fp, "%s %s", status, ssid);
    if (!strncmp(status, "Connected", 9))
        strncpy(status, "+", BUFSIZE);
    else {
        strncpy(status, "-", BUFSIZE);
        strncpy(ssid, "None", BUFSIZE);
    }
    snprintf(net, BUFSIZE, "%s%s", status, ssid);
    #endif

    ret = 0;
cleanup:
    if (fp)
        pclose(fp);
    return ret;
}

int get_vol(char *vol) {
    int ret = -1;

    char status[BUFSIZE], level[BUFSIZE];
    FILE *fp;
    fp = popen(
        "pactl get-sink-volume @DEFAULT_SINK@ | awk '{print $5}'", "r");
    fgets(level, BUFSIZE, fp);
    pclose(fp);
    fp = popen("pactl get-sink-mute @DEFAULT_SINK@ | awk '{print $2}'", "r");
    fgets(status, BUFSIZE, fp);
    if (!strncmp(status, "no", 2))
        strncpy(status, "+", BUFSIZE);
    else
        strncpy(status, "-", BUFSIZE);
    snprintf(vol, BUFSIZE, "%s%s", status, level);
    vol[strcspn(vol, "\n")] = '\0';

    ret = 0;
cleanup:
    if (fp)
        pclose(fp);
    return ret;
}

int main(int argc, char **argv) {
    int ret = -1;

    if (!(dpy = XOpenDisplay(NULL))) {
        fprintf(stderr, "main: cannot open display.\n");
        goto cleanup;
    }

    if (argc != 2) {
        fprintf(stderr, "main: wrong # args.\n");
        goto cleanup;
    }

    char netint[BUFSIZE];
    strncpy(netint, argv[1], BUFSIZE);

    while (1) {
        char date[BUFSIZE];
        if (get_date(date)) {
            fprintf(stderr, "main: cannot get date.\n");
            goto cleanup;
        }

        char tmp[BUFSIZE];
        if (get_tmp(tmp)) {
            fprintf(stderr, "main: cannot get cpu temp.\n");
            goto cleanup;
        }

        char mem[BUFSIZE];
        if (get_mem(mem)) {
            fprintf(stderr, "main: cannot get mem.\n");
            goto cleanup;
        }

        char dsk[BUFSIZE];
        if (get_dsk(dsk)) {
            fprintf(stderr, "main: cannot get dsk.\n");
            goto cleanup;
        }

        char bat[BUFSIZE];
        if (get_bat(bat)) {
            fprintf(stderr, "main: cannot get bat.\n");
            goto cleanup;
        }

        char net[BUFSIZE];
        if (get_net(net, netint)) {
            fprintf(stderr, "main: cannot get net.\n");
            goto cleanup;
        }

        char vol[BUFSIZE];
        if (get_vol(vol)) {
            fprintf(stderr, "main: cannot get vol.\n");
            goto cleanup;
        }

        char status[BUFSIZE*10];
        snprintf(status, BUFSIZE*10,
            " vol: %s | net: %s | bat: %s | dsk: %s | mem: %s | tmp: %sC | %s ",
            vol, net, bat, dsk, mem, tmp, date);
        XStoreName(dpy, DefaultRootWindow(dpy), status);
        XSync(dpy, False);
        sleep(1);
    }

    ret = 0;
cleanup:
    return ret;
}
