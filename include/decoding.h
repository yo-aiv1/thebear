#ifndef DECODE_YS
#define DECODE_YS

void DecodeString(unsigned char *str);
void DecodeStringW(unsigned short *str);
unsigned int DecodeBase64(const char *Base64String, unsigned int StringLength, unsigned char *DecodedString);

#endif