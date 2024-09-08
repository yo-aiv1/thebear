#ifndef LOAD_Y
#define LOAD_Y

#include "../include/global.h"

void *GetDllAddress(HashInfo DllHashInfo);
void *GetFuncAddress(void *DllAddress, HashInfo FunctionHashInfo);


#endif