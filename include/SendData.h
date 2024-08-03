#ifndef SENDDATA_YS
#define SENDDATA_YS

#include <windows.h>

void SendData(HANDLE FileHandle, char *name, int DataSize, char *DataSizeAsString, unsigned char *buffer, unsigned char *data, int IsFile);
int InitConnection();

#endif