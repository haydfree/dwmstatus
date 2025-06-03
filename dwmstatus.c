#include "dwmstatus.h"

static Display *dpy;

int
get_net(char * const dst, const size_t len)
{
    int ret = ERROR;
	size_t cur_len = 0;
	char net[BUF_SIZE] = {0}, net_id[SMALL_BUF_SIZE] = {0}, net_st[SMALL_BUF_SIZE] = {0}, *line = NULL;

    if (!dst) { goto cleanup; }
    if (len <= 0) { goto cleanup; }

		
	execute_script("nmcli", net, BUF_SIZE);
	sscanf(net, "wlo1: %s to %s\n", net_st, net_id);
	if (strlen(net_id) <= 0 || strlen(net_st) <= 0) { goto cleanup; }
	trim_whitespace(net_st, net_st, SMALL_BUF_SIZE);
	trim_whitespace(net_id, net_id, SMALL_BUF_SIZE);

	if (strcmp(net_st, "connected") == 0) { strncpy(net_st, "+", SMALL_BUF_SIZE); }
	else if (strcmp(net_st, "disconnected") == 0) { strncpy(net_st, "-", SMALL_BUF_SIZE); }
	else { /* leave status unchanged */ }
	if (snprintf(dst, len, "%s%s", net_st, net_id) < 0) { goto cleanup; }

    ret = SUCCESS;
cleanup:
    return ret;
}

int
get_bat(char * const dst, const size_t len)
{
    int ret = ERROR;
	size_t cur_len = 0;
	char bat_per[SMALL_BUF_SIZE] = {0}, bat_st[SMALL_BUF_SIZE] = {0};

    if (!dst) { goto cleanup; }
    if (len <= 0) { goto cleanup; }

	execute_script("apm -l", bat_per, SMALL_BUF_SIZE);	
	trim_whitespace(bat_per, bat_per, SMALL_BUF_SIZE);
	cur_len = strlen(bat_per);
	if (cur_len <= 0 || cur_len+2 >= len) { goto cleanup; }
	execute_script("apm -a", bat_st, SMALL_BUF_SIZE);
	trim_whitespace(bat_st, bat_st, SMALL_BUF_SIZE);
	cur_len += strlen(bat_st);
	if (cur_len+1 >= len) { goto cleanup; }

	if (strcmp(bat_st, "1") == 0) { strncpy(bat_st, "+", SMALL_BUF_SIZE); }
	else if (strcmp(bat_st, "0") == 0) { strncpy(bat_st, "-", SMALL_BUF_SIZE); }
	else { /* leave status unchanged */ }
	if (snprintf(dst, BUF_SIZE, "%s%s%%", bat_st, bat_per) < 0) { goto cleanup; }

    ret = SUCCESS;
cleanup:
    return ret;
}

int
get_cpu(char * const dst, const size_t len)
{    
	int ret = ERROR;
	size_t cur_len = 0;
	char cpu[BUF_SIZE] = {0}, temp[SMALL_BUF_SIZE] = {0}, *line = NULL;

    if (!dst) { goto cleanup; }
    if (len <= 0) { goto cleanup; }

	execute_script("sysctl hw.sensors", cpu, BUF_SIZE);	
	trim_whitespace(cpu, cpu, BUF_SIZE);
	cur_len = strlen(cpu);
	if (cur_len <= 0) { goto cleanup; }

	line = strtok(cpu, "\n");
	while (line)
	{
		if (strstr(line, "temp0"))
		{
			sscanf(line, "hw.sensors.cpu0.temp0=%s", temp);
		}
		
		line = strtok(NULL, "\n");
	}
	trim_whitespace(temp, temp, SMALL_BUF_SIZE);
	if (temp[2] == '.') { temp[2] = '\0'; }
	else if (temp[3] == '.') { temp[3] = '\0'; }
	if (snprintf(dst, BUF_SIZE, "%s°C", temp) < 0) { goto cleanup; }
	
    ret = SUCCESS;
cleanup:
    return ret;
}

int
get_mem(char * const dst, const size_t len)
{
	int ret = ERROR;
	size_t cur_len = 0;
	char mem[BUF_SIZE] = {0}, temp[SMALL_BUF_SIZE] = {0}, *found = NULL;

    if (!dst) { goto cleanup; }
    if (len <= 0) { goto cleanup; }

	execute_script("top -dl | grep Memory", mem, BUF_SIZE);
	trim_whitespace(mem, mem, BUF_SIZE);
	cur_len = strlen(mem);
	if (cur_len <= 0) { goto cleanup; }

	found = strstr(mem, "Real: ");
	if (found) { found += strlen("Real: "); }
	sscanf(found, "%s", temp);
	trim_whitespace(temp, temp, SMALL_BUF_SIZE);
	cur_len = strlen(temp);
	if (cur_len <= 0 || cur_len+1 >= len) { goto cleanup; }

	if (snprintf(dst, len, "%s", temp) < 0) { goto cleanup; }
	
    ret = SUCCESS;
cleanup:
    return ret;
}

int
get_vol(char * const dst, const size_t len)
{
	int ret = ERROR, li = 0;
	size_t cur_len = 0;
	char vol[BUF_SIZE] = {0}, level[SMALL_BUF_SIZE] = {0}, mute[SMALL_BUF_SIZE] = {0}, *line = NULL, *end = NULL;
	float lf = 0.0;

    if (!dst) { goto cleanup; }
    if (len <= 0) { goto cleanup; }

	execute_script("sndioctl", vol, BUF_SIZE);
	trim_whitespace(vol, vol, BUF_SIZE);
	cur_len = strlen(vol);
	if (cur_len <= 0) { goto cleanup; }

	line = strtok(vol, "\n");
	while (line)
	{
		if (strstr(line, "output.level=")) 
		{ 
			trim_whitespace(level, line, SMALL_BUF_SIZE); 
			cur_len = strlen(level);
			if (cur_len <= 0) { goto cleanup; }
			sscanf(level, "output.level=%s", level);
		} else if (strstr(line, "output.mute="))
		{
			trim_whitespace(mute, line, SMALL_BUF_SIZE); 
			cur_len = strlen(mute);
			if (cur_len <= 0) { goto cleanup; }
			sscanf(mute, "output.mute=%s", mute);
		}
		line = strtok(NULL, "\n");
	}
	cur_len = strlen(level) + strlen(mute);
	if (cur_len+1 >= len) { goto cleanup; }
	lf = strtof(level, &end);
	if (end - level <= 0) { goto cleanup; }
	li = (int) (lf * 100);

	if (strcmp(mute, "0") == 0) { strncpy(mute, "+", SMALL_BUF_SIZE); }
	else if (strcmp(mute, "1") == 0) { strncpy(mute, "-", SMALL_BUF_SIZE); }
	if (snprintf(dst, len, "%s%d%%", mute, li) < 0) { goto cleanup; }
	
    ret = SUCCESS;
cleanup:
    return ret;
}

int
get_time(char * const dst, const size_t len)
{
	int ret = ERROR;
	size_t cur_len = 0;
	char temp[SMALL_BUF_SIZE] = {0};
	time_t raw_time = 0;
	struct tm *time_info = {0};

    if (!dst) { goto cleanup; }
    if (len <= 0) { goto cleanup; }
	
    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(temp, SMALL_BUF_SIZE, "%a %Y-%m-%d %H:%M:%S", time_info);
	trim_whitespace(temp, temp, SMALL_BUF_SIZE);
	cur_len = strlen(temp);
	if (cur_len <= 0 || cur_len+1 >= len) { goto cleanup; }
	if (snprintf(dst, len, "%s", temp) < 0) { goto cleanup; }
	
    ret = SUCCESS;
cleanup:
    return ret;
}

static int
reset_status(char * const dst, const size_t len, char * const net, char * const bat, char * const cpu, char * const mem, char * const vol, char * const time)
{
	int ret = ERROR; 

    if (!dst || !net || !bat || !cpu || !mem || !vol || !time) { goto cleanup; }
	if (len <= 0) { goto cleanup; }
	
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

static int
create_status(char * const dst, const size_t len, 
				const char * const net, const char * const bat, const char * const cpu, const char * const mem, const char * const vol, const char * const time)
{
	int ret = ERROR; 

    if (!dst || !net || !bat || !cpu || !mem || !vol || !time) { goto cleanup; }
	if (len <= 0) { goto cleanup; }

    if (snprintf(dst, len, " net: %s | bat: %s | cpu: %s | mem: %s | vol: %s | time: %s ", net, bat, cpu, mem, vol, time) < 0) { goto cleanup; }

    return SUCCESS;

cleanup:
	return ret;
}

static int
set_status(const char * const src, const size_t len)
{
    int ret = ERROR;

	if (!src) { goto cleanup; }
	if (len <= 0) { goto cleanup; }

	XStoreName(dpy, DefaultRootWindow(dpy), src);
	XSync(dpy, False);

    ret = SUCCESS;
cleanup:
    return ret;
}

int 
status_loop(void) 
{
    int ret = ERROR;
    char status[BUF_SIZE] = {0}, net[BUF_SIZE] = {0}, bat[BUF_SIZE] = {0}, cpu[BUF_SIZE] = {0}, mem[BUF_SIZE] = {0}, vol[BUF_SIZE] = {0}, cur_time[BUF_SIZE] = {0}; 

	if (!(dpy = XOpenDisplay(NULL))) { goto cleanup; }

	for (;;sleep(REFRESH_RATE)) 
    {
		if (reset_status(status, BUF_SIZE, net, bat, cpu, mem, vol, cur_time)) { goto cleanup; }	

		if (get_net(net, BUF_SIZE)) { goto cleanup; }
		/*if (get_bat(bat, BUF_SIZE)) { goto cleanup; }
		if (get_cpu(cpu, BUF_SIZE)) { goto cleanup; }
		if (get_mem(mem, BUF_SIZE)) { goto cleanup; }
		if (get_vol(vol, BUF_SIZE)) { goto cleanup; }
		if (get_time(cur_time, BUF_SIZE)) { goto cleanup; }*/
		

		if (create_status(status, BUF_SIZE, net, bat, cpu, mem, vol, cur_time)) { goto cleanup; }	
		if (set_status(status, BUF_SIZE)) { goto cleanup; }
	}

    ret = SUCCESS;

cleanup:
	if (dpy) { XCloseDisplay(dpy); }
	return ret;
}
