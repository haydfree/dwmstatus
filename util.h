#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define SUCCESS 0
#define ERROR -1

int trim_whitespace(char * const dst, const char * const src, const size_t len);
int execute_script(const char * const cmd, char * const dst, const size_t len);

#endif
