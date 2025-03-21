#include <stddef.h>
#include <stdlib.h>

#include "util.h"

char* getCpu(char *base, char *sensor) {
	char *co;

	co = readFile(base, sensor);
	if (co == NULL)
		return smprintf("");
	return smprintf("%02.0f°C", atof(co) / 1000);
}
