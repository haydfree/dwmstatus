#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 255


char* getVol(void) {
    FILE* fp;
    char* buf;
    int vol;
    char mute[4];

    buf = malloc(BUF_SIZE * sizeof(char));
    if (buf == NULL) {
        fprintf(stderr, "%s:%d Error: buf allocation failed\n", __FILE__, __LINE__);
        exit(1);
    }

    fp = popen("amixer get Master", "r");
    if (fp == NULL) { 
        fprintf(stderr, "%s:%d Error: get volume command failed\n", __FILE__, __LINE__);
        exit(1);
    }

    while (fgets(buf, BUF_SIZE, fp)) {
        if (strstr(buf, "Mono:")) {
            if (!sscanf(buf, "%*s %*s %*d [%d%%] [%*fdB] [%3[^]]", &vol, mute)) {
                continue;
            }
        }
    }

    pclose(fp);

    if (strcmp(mute, "on") == 0) {
        if (snprintf(buf, BUF_SIZE, "+%d%%", vol) < 0) {
            fprintf(stderr, "%s:%d Error: failed to write string to buf\n", __FILE__, __LINE__);
            exit(1);
        }
    } else if (strcmp(mute, "off") == 0) {
        if (snprintf(buf, BUF_SIZE, "-%d%%", vol) < 0) {
            fprintf(stderr, "%s:%d Error: failed to write string to buf\n", __FILE__, __LINE__);
            exit(1);
        }
    } else {
        fprintf(stderr, "%s:%d Error: mute not expected format\n", __FILE__, __LINE__);
        exit(1);
    }

    return buf;
}


