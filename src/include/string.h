
#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

int memcmp(const void* s1, const void* s2, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
void* memset(void* dest, int v, size_t n);
char* strcat(char* dest, const char* src);
const char* strchr(const char* s, int v);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
char* strdup(const char* s);
size_t strlen(const char* s);
char* strncat(char* dest, const char* src, size_t n);
int strncmp(const char* s1, const char* s2, size_t n);
char* strncpy(char* dest, const char* src, size_t n);

#endif // #ifndef _STRING_H
