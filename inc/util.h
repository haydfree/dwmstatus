#ifndef UTIL_H
#define UTIL_H

void strTrim(char* str);
char* readFile(char* base, char* file);
char* execScript(char* cmd);
char* smprintf(char* fmt, ...);

#endif
