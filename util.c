#include "util.h"

int 
trim_whitespace(char * const dst, const char * const src, const size_t len) 
{
    int ret = ERROR;
    char copy[len + 1], *start = NULL, *end = NULL;
	memset(copy, 0, len+1);

    if (!dst || !src) { goto cleanup; }
    if (len <= 0) { goto cleanup; }

    memcpy(copy, src, len);
    copy[len] = '\0';

    start = copy;
    end = start + len - 1;

    while (isspace((int)*start) || *start == '\0') { start++; }
    while (isspace((int)*end) || *end == '\0') { end--; }

    *(end + 1) = '\0';
    strncpy(dst, start, len);

    ret = SUCCESS;

cleanup:
    return ret;
}

int
execute_script(const char * const cmd, char * const dst, const size_t len)
{
    int ret = ERROR;
	FILE *fp = NULL;
	size_t read_len = 0;
	
	if (!cmd || !dst) { goto cleanup; }
	if (len <= 0) { goto cleanup; }
	fp = popen(cmd, "r");
	if (!fp) { goto cleanup; }

	while (fgets(dst+read_len, len-read_len, fp))
	{
		read_len = strlen(dst);	
		if (read_len >= len - 1) { break; }
	} 	

    ret = SUCCESS;
cleanup:
	if (fp) { pclose(fp); }
    return ret;
}
