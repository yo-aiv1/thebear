#ifndef LOAD_Y
#define LOAD_Y


void *GetDllAddress(unsigned long DllHash);
void *GetFuncAddress(void *DllAddress, unsigned long Func);


#endif