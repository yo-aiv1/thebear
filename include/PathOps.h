#ifndef PATH_OPS
#define PATH_OPS

#include "../include/ntdll.h"

void InitPathObj(unsigned short *SourceString, POBJECT_ATTRIBUTES ObjectAttributes, PUNICODE_STRING PathInfo);
int InitPath(unsigned short *FilePath, unsigned short *OutBuffer);


#endif