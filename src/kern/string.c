
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

void* memcpy(void* dest, const void* src, size_t n) {
    for(unsigned int i = 0; i < n; i++) {
        ((uint8_t*)dest)[i] = ((const uint8_t*)src)[i];
    }

    return dest;
}

void* memset(void* s, int v, size_t n) {
    for(unsigned int i = 0; i < n; i++) {
        ((uint8_t*)s)[i] = (uint8_t) v;
    }

    return s;
}

const char* strchr(const char* s, int v) {
    int i = 0;
    while(true) {
        if(s[i] == v) {
            return (s + i);
        } else if(s[i] == '\0') {
            return NULL;
        } else {
            i++;
        }
    }
}

int strcmp(const char* s1, const char* s2) {
    int i = 0;
    while(s1[i] == s2[i] && s1[i] != '\0') {
        i++;
    }
    return s1[i] - s2[i];
}

char* strcpy(char* dest, const char* src) {
    int i = 0;
    while(true) {
        dest[i] = src[i];
        if(src[i] == '\0') {
            return dest;
        }
    }
}

size_t strlen(const char* s) {
    size_t i = 0;

    while(s[i] != '\0') {
        i++;
    }

    return i;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    unsigned int i = 0;
    while(i < n) {
        if(s1[i] != s2[i] || s1[i] == '\0') {
            return s1[i] - s2[i];
        }
    }
    return 0;
}

char* strncpy(char* dest, const char* src, size_t n) {
    unsigned int i = 0;
    while(i < n) {
        dest[i] = src[i];
        if(src[i] == '\0') {
            return dest;
        }
        i++;
    }
    dest[n-1] = '\0';
    return dest;
}
