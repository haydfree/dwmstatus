#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char* getVol(void) {
    int vol;
    char* mute;
    char* buf;
    int bufSize = 255;
    FILE* fp;

    buf = malloc(bufSize * sizeof(char));
    mute = malloc(bufSize * sizeof(char));

    if (buf == NULL) {
        fprintf(stderr, "%s:%d Error: buffer not allocated successfully\n", __FILE__, __LINE__);
        exit(1);
    }

    if (mute == NULL) {
        fprintf(stderr, "%s:%d Error: mute buffer not allocated successfully\n", __FILE__, __LINE__);
        exit(1);
    }

    fp = popen("pactl get-sink-volume @DEFAULT_SINK@", "r");
    if (fp == NULL) { 
        fprintf(stderr, "%s:%d Error: get volume command failed\n", __FILE__, __LINE__);
        exit(1);
    }

    /* ignore chars until / found, then start scanning */
    if (fscanf(fp, "%*[^/]/%d", &vol) != 1) {
        fprintf(stderr, "%s:%d Error: failed to find volume when scanning\n", __FILE__, __LINE__);
        exit(1);
    }

    pclose(fp);

    fp = popen("pactl get-sink-mute @DEFAULT_SINK@", "r");
    if (fp == NULL) { 
        fprintf(stderr, "%s:%d Error: get mute command failed\n", __FILE__, __LINE__);
        exit(1);
    }

    if (fscanf(fp, "Mute: %s", mute) != 1) {
        fprintf(stderr, "%s:%d Error: failed to find mute when scanning\n", __FILE__, __LINE__);
        exit(1);
    }

    pclose(fp);

    if (strcmp(mute, "no") == 0) {
        if (snprintf(buf, bufSize, "unmuted %d", vol) < 0) {
            fprintf(stderr, "%s:%d Error: failed to write string to buf\n", __FILE__, __LINE__);
            exit(1);
        }
    } else if (strcmp(mute, "yes") == 0) {
        if (snprintf(buf, bufSize, "muted %d", vol) < 0) {
            fprintf(stderr, "%s:%d Error: failed to write string to buf\n", __FILE__, __LINE__);
            exit(1);
        }
    } else {
        fprintf(stderr, "%s:%d Error: mute not 1 or 0\n", __FILE__, __LINE__);
        exit(1);
    }

    return buf;
}


