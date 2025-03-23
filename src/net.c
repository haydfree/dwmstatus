#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define BUFSIZE 255

char* getNetSsid(void) {
    FILE* fp;
    char* ssid;

    ssid = malloc(BUFSIZE * sizeof(char));

    if (ssid == NULL) {
        fprintf(stderr, "%s:%d: Error: allocate ssid buffer failed\n", __FILE__, __LINE__);
        exit(1);
    }

    fp = popen("nmcli -t -f active,ssid dev wifi | grep ^yes | cut -d: -f2", "r");
    if (fp == NULL) {
        fprintf(stderr, "%s:%d: Error: open nmcli command failed\n", __FILE__, __LINE__);
        exit(1);
    }

    if (fscanf(fp, "%s", ssid) <= 0) {
        fprintf(stderr, "%s:%d: Error: read ssid into ssid buffer failed\n", __FILE__, __LINE__);
        exit(1);
    }

    strTrim(ssid);
    fclose(fp);

    return ssid;
}

char* getNetStatus(void) {
    FILE* fp;
    char* status;

    status = malloc(BUFSIZE * sizeof(char));
    if (status == NULL) {
        fprintf(stderr, "%s:%d: Error: allocate return buffer failed\n", __FILE__, __LINE__);
        exit(1);
    }

    fp = fopen("/sys/class/net/wlan0/operstate", "r");
    if (fp == NULL) {
        fprintf(stderr, "%s:%d: Error: open net interface status failed\n", __FILE__, __LINE__);
        exit(1);
    }

    if (fscanf(fp, "%s", status) <= 0) {
        fprintf(stderr, "%s:%d: Error: read status into status buffer failed\n", __FILE__, __LINE__);
        exit(1);
    }

    strTrim(status);
    fclose(fp);

    return status;
}
