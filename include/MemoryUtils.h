#ifndef MEMUTILS_YS
#define MEMUTILS_YS

#include <windows.h>

void *AllocMemory(SIZE_T size);
int FreeMemory(void *buffer);
void SetMemory(void *src, int c, int size);
void MovMemory(unsigned char *src, unsigned char *dest, int size);

#endif