#ifndef UTILS_C
#define UTILS_C

int lenW(const unsigned short *string);
int len(const unsigned char *string);
void ConcatStringW(unsigned short *dest, unsigned short *src, int DestIdx);
void ConcatString(unsigned char *dest, unsigned char *src, int DestIdx);
int IntLen(int num);
void IntToString(char *str, int num);
int TotalBufferLength(unsigned short *FileName, int FileSize);
void WideToNormal(char *OutBuffer, unsigned short *String);

#endif