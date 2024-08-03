#include "../include/decoding.h"
#include "../include/macros.h"


void DecodeString(unsigned char *str) {
    int TempKey = ENKEY;

    while (*str != 0x00) {
        if (TempKey < 0) {
            TempKey = TempKey * -1;
        } else if (TempKey == 0) {
            TempKey = ENKEY;
        }

        *str = *str - TempKey;
        TempKey = *str - TempKey;
        str++;
    }
}

void DecodeStringW(unsigned short *str) {
    int TempKey = ENKEY;

    while (*str != 0x0000) {
        if (TempKey < 0) {
            TempKey = TempKey * -1;
        } else if (TempKey == 0) {
            TempKey = ENKEY;
        }

        *str = *str - TempKey;
        TempKey = *str - TempKey;
        str++;
    }
}

unsigned int DecodeBase64(const char *Base64String, unsigned int StringLength, unsigned char *DecodedString) {
	static const unsigned char base64de[] = {
		/* nul, soh, stx, etx, eot, enq, ack, bel, */
		255, 255, 255, 255, 255, 255, 255, 255,

		/*  bs,  ht,  nl,  vt,  np,  cr,  so,  si, */
		255, 255, 255, 255, 255, 255, 255, 255,

		/* dle, dc1, dc2, dc3, dc4, nak, syn, etb, */
		255, 255, 255, 255, 255, 255, 255, 255,

		/* can,  em, sub, esc,  fs,  gs,  rs,  us, */
		255, 255, 255, 255, 255, 255, 255, 255,

		/*  sp, '!', '"', '#', '$', '%', '&', ''', */
		255, 255, 255, 255, 255, 255, 255, 255,

		/* '(', ')', '*', '+', ',', '-', '.', '/', */
		255, 255, 255,  62, 255, 255, 255,  63,

		/* '0', '1', '2', '3', '4', '5', '6', '7', */
			52,  53,  54,  55,  56,  57,  58,  59,

		/* '8', '9', ':', ';', '<', '=', '>', '?', */
			60,  61, 255, 255, 255, 255, 255, 255,

		/* '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', */
		255,   0,   1,  2,   3,   4,   5,    6,

		/* 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', */
			7,   8,   9,  10,  11,  12,  13,  14,

		/* 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', */
			15,  16,  17,  18,  19,  20,  21,  22,

		/* 'X', 'Y', 'Z', '[', '\', ']', '^', '_', */
			23,  24,  25, 255, 255, 255, 255, 255,

		/* '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', */
		255,  26,  27,  28,  29,  30,  31,  32,

		/* 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', */
			33,  34,  35,  36,  37,  38,  39,  40,

		/* 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', */
			41,  42,  43,  44,  45,  46,  47,  48,

		/* 'x', 'y', 'z', '{', '|', '}', '~', del, */
			49,  50,  51, 255, 255, 255, 255, 255
	};
	unsigned int i;
	unsigned int j;
	unsigned char c;

	if (StringLength & 0x3) {
		return 0;
	}

	for (i = j = 0; i < StringLength; i++) {
		if (Base64String[i] == '=') {
			break;
		}
		if (Base64String[i] < '+' || Base64String[i] > 'z') {
			return 0;
		}

		c = base64de[(unsigned char)Base64String[i]];
		if (c == 255) {
			return 0;
		}

		switch (i & 0x3) {
		case 0:
			DecodedString[j] = (c << 2) & 0xFF;
			break;
		case 1:
			DecodedString[j++] |= (c >> 4) & 0x3;
			DecodedString[j] = (c & 0xF) << 4; 
			break;
		case 2:
			DecodedString[j++] |= (c >> 2) & 0xF;
			DecodedString[j] = (c & 0x3) << 6;
			break;
		case 3:
			DecodedString[j++] |= c;
			break;
		}
	}

	return j;
}