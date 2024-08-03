#ifndef DIRECTORYOPS_YS
#define DIRECTORYOPS_YS

#include <windows.h>
#include "../include/ntdll.h"

int ReadFolder(unsigned char *OutBuffer, unsigned short *path);
int OpenFolder(PHANDLE FileHandle, unsigned short *path);


#endif